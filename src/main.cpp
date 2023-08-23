#include "glew.h"
#include "freeglut.h"
#include "glm.hpp"
#include "ext.hpp"
#include <iostream>
#include <cmath>
#include <vector>

#include "Shader_Loader.h"
#include "Render_Utils.h"
#include "Texture.h"
#include "Camera.h"
#include "Mouse.h"
#include "Skybox.h"
#include "Fish.h"
#include "Coin.h"
#include "GroundEntity.h"
#include "Bubbles.h"
#include "ParticleGenerator.h"

#include "terrain/terrainTexture.h"
#include "terrain/mapGenerator.h"
#include "terrain/Terrain.h"

using namespace std;

constexpr auto WINDOW_WIDTH = 1200;
constexpr auto WINDOW_HEIGHT = 1000;

GLuint programColor;
GLuint programTexture;
GLuint programSkybox;
GLuint programSpotlightTexture;
GLuint programSpotlightColor;
GLuint programCoins;
GLuint programTerrain;
GLuint programParticles;

obj::Model submarineModel;

terrainTexture* underwaterGroundTexture;
terrainTexture* heightMapTexture;

Core::Shader_Loader shaderLoader;
Camera camera;
Mouse mouse(WINDOW_WIDTH, WINDOW_HEIGHT);
Skybox skybox;
Terrain terrain;


glm::vec3 lightDir = glm::normalize(glm::vec3(0.1f, -1.0f, 0.1f));

float appLoadingTime, lastTime, gameplayTime;

static const int NUM_FISH = 700;
Fish fishes[NUM_FISH];

static const int NUM_GROUND_ENT = 200;
GroundEntity groundEnts[NUM_GROUND_ENT];

static const int NUM_MAXPOINTS = 5;
Coin coins[NUM_MAXPOINTS];
int NUM_POINTS = 0;
string scoreText, timerText;

ParticleGenerator particleGenerator;

static const int NUM_BUBBLES_PER_GROUP = 10;
static const int NUM_BUBBLES = 125;
Bubbles bubbles[NUM_BUBBLES][NUM_BUBBLES_PER_GROUP];

static void callKeyboard(unsigned char key, int x, int y) {
	camera.keyboard(key, x, y);
}

void callMouse(int x, int y) {
	mouse.setMousePosition(x, y);
}

bool isShipNearCoin(glm::vec3 shipPos, glm::vec3 coinPos, float epsilon) {
	float shipX = shipPos.x;
	float shipY = shipPos.y;
	float shipZ = shipPos.z;

	float coinX = coinPos.x;
	float coinY = coinPos.y;
	float coinZ = coinPos.z;
	if ((abs(shipX - coinX) <= epsilon) && (abs(shipY - coinY) <= epsilon) && (abs(shipZ - coinZ) <= epsilon))
		return true;
	return false;
}

void displayText(int x, int y, float r, float g, float b, const char* string)
{
	glColor3f(r, g, b);
	glWindowPos2f(x, y);
	int len, i;
	len = (int)strlen(string);
	for (i = 0; i < len; i++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
	}
}

void endingScreenDisplay() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glm::vec3 colorBlue = colors[8];

	glClearColor(colorBlue.r, colorBlue.g, colorBlue.b, 1.0f);
	
	std::string tmp = to_string(roundFloat(gameplayTime - appLoadingTime));
	std::string endText = "Thanks for playing!";
	displayText(WINDOW_WIDTH / 6, WINDOW_HEIGHT / 2, 22.f, 202.f, 49.f, endText.data());

	endText = "You collected " + to_string(NUM_POINTS) + " coins in " + tmp.substr(0, tmp.size() - 4) + " seconds";
	displayText(WINDOW_WIDTH / 6, WINDOW_HEIGHT / 2 - 50, 22.f, 202.f, 49.f, endText.data());


	glutSwapBuffers();

}

void spotlight(GLuint program, glm::mat4 cameraMatrix, glm::mat4 perspectiveMatrix) {
	glUseProgram(program);

	// Set material properties
	glUniform1f(glGetUniformLocation(program, "material.shininess"), 32.0f);
	glUniform1i(glGetUniformLocation(program, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(program, "material.specular"), 1);

	// Set light properties: direction position etc
	glUniform3f(glGetUniformLocation(program, "light.position"), camera.cameraPos.x, camera.cameraPos.y, camera.cameraPos.z);
	glUniform3f(glGetUniformLocation(program, "light.direction"), camera.cameraDir.x, camera.cameraDir.y, camera.cameraDir.z);
	glUniform1f(glGetUniformLocation(program, "light.cutOff"), glm::cos(glm::radians(12.5f)));
	glUniform1f(glGetUniformLocation(program, "light.outerCutOff"), glm::cos(glm::radians(17.5f)));
	glUniform3f(glGetUniformLocation(program, "viewPos"), camera.cameraPos.x, camera.cameraPos.y, camera.cameraPos.z);

	// Set light properties: ambient, diffuse, specular; and variables needed for spotlight
	glUniform3f(glGetUniformLocation(program, "light.ambient"), 0.075f, 0.075f, 0.075f);
	glUniform3f(glGetUniformLocation(program, "light.diffuse"), 0.4f, 0.4f, 0.4f);
	glUniform1f(glGetUniformLocation(program, "light.constant"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "light.linear"), 0.014f);
	glUniform1f(glGetUniformLocation(program, "light.quadratic"), 0.0007f);
	
	// Set view and perspective matrices
	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, (float*)&cameraMatrix);
	glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, (float*)&perspectiveMatrix);

}

void drawCoin(obj::Model* model, glm::mat4 modelMatrix, glm::vec3 color, glm::vec3 coinPos, GLuint program)
{
	glUseProgram(program);

	// Set material properties
	glUniform3f(glGetUniformLocation(program, "material.ambient"), 0.24725f, 0.1995f, 0.0745f);
	glUniform3f(glGetUniformLocation(program, "material.diffuse"), 0.75164f, 0.60648f, 0.22648f);
	glUniform3f(glGetUniformLocation(program, "material.specular"), 0.628281f, 0.555802f, 0.366065f);
	glUniform1f(glGetUniformLocation(program, "material.shininess"), 0.4f);

	// Set light properties: direction position etc
	glUniform3f(glGetUniformLocation(program, "light.position"), coinPos.x, coinPos.y, coinPos.z);
	glUniform3f(glGetUniformLocation(program, "viewPos"), camera.cameraPos.x, camera.cameraPos.y, camera.cameraPos.z);

	// Set light properties: ambient, diffuse, specular; and variables needed for spotlight
	glUniform3f(glGetUniformLocation(program, "light.ambient"), 1.f, 1.f, 1.f);
	glUniform3f(glGetUniformLocation(program, "light.diffuse"), 0.5f, 0.5f, 0.5f);
	glUniform3f(glGetUniformLocation(program, "light.specular"), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(program, "light.constant"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "light.linear"), 0.0014f);
	glUniform1f(glGetUniformLocation(program, "light.quadratic"), 0.000007f);

	// Set view and perspective matrices
	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, (float*)&camera.cameraMatrix);
	glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, (float*)&camera.perspectiveMatrix);

	glUniform3f(glGetUniformLocation(program, "objectColor"), color.x, color.y, color.z);
	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, (float*)&modelMatrix);

	Core::DrawModel(model);

	glUseProgram(0);
}


void drawObjectColor(obj::Model* model, glm::mat4 modelMatrix, glm::vec3 color, GLuint program)
{
	glUseProgram(program);

	 if (program == programColor) {
		glUniform3f(glGetUniformLocation(program, "objectColor"), color.x, color.y, color.z);
		glUniform3f(glGetUniformLocation(program, "lightDir"), lightDir.x, lightDir.y, lightDir.z);

		glm::mat4 transformation = camera.perspectiveMatrix * camera.cameraMatrix * modelMatrix;
		glUniformMatrix4fv(glGetUniformLocation(program, "modelViewProjectionMatrix"), 1, GL_FALSE, (float*)&transformation);
		glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_FALSE, (float*)&modelMatrix);
	}
	else {
		// Set color to model
		glUniform3f(glGetUniformLocation(program, "objectColor"), color.x, color.y, color.z);
		// Set model matrix to spotlight shader
		glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, (float*)&modelMatrix);
	}


	Core::DrawModel(model);

	glUseProgram(0);
}

void drawObjectTexture(obj::Model* model, glm::mat4 modelMatrix, GLuint texture_grid, GLuint program) {

	// Set texture to model
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_grid);

	glUseProgram(program);

	// Set model matrix to spotlight shader
	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, (float*)&modelMatrix);

	Core::DrawModel(model);

	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}

void renderScene()
{
	float currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
	float delta = currentTime - lastTime;
	lastTime = currentTime;

	scoreText = "POINTS: " + to_string(NUM_POINTS) + " / " + to_string(NUM_MAXPOINTS);
	timerText = "TIME: " + to_string(roundFloat(gameplayTime - appLoadingTime));

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.02f, 0.05f, 0.28f, 1.0f);

	// Display score and time
	displayText(WINDOW_WIDTH - (WINDOW_WIDTH / 6), WINDOW_HEIGHT - (WINDOW_HEIGHT / 10), 255.f, 223.f, 0.f, scoreText.data());
	displayText(WINDOW_WIDTH - (WINDOW_WIDTH / 6), WINDOW_HEIGHT - (WINDOW_HEIGHT / 8), 255.f, 0.f, 0.f, (timerText.substr(0, timerText.size() - 4) + " s").data());

	// Update of camera and perspective matrices
	camera.setCameraMatrixQuat(mouse.differenceMousePosition, mouse.mouseEdgeflags);
	camera.setPerspectiveMatrix();
	//std::cout << camera.cameraPos.x << " " << camera.cameraPos.y << " " << camera.cameraPos.z << std::endl;
	//std::cout << camera.cameraDir.x << " " << camera.cameraDir.y << " " << camera.cameraDir.z << std::endl;

	spotlight(programTerrain, camera.cameraMatrix, camera.perspectiveMatrix);
	// render terrain
	terrain.drawTerrain(programTerrain, heightMapTexture, underwaterGroundTexture, camera.perspectiveMatrix, camera.cameraMatrix);

	// render skybox
	//skybox.drawSkybox(camera, programSkybox);


	// MODELS NOT AFFECTED WITH SPOTLIGHT

	// Submarine
	glm::mat4 shipRotation = glm::mat4(glm::mat4_cast(glm::quat(glm::inverse(camera.rotation))));
	glm::mat4 shipInitialTransformation = 
		glm::translate(glm::vec3(-0.0f, -1.5f, -4.0f)) * 
		glm::rotate(glm::radians(180.0f), glm::vec3(0, 1, 0)) * 
		glm::scale(glm::vec3(0.1f));
	glm::vec3 shipPos = camera.cameraPos + camera.cameraDir * 1.5f;
	glm::mat4 shipModelMatrix = glm::translate(shipPos) * shipRotation * shipInitialTransformation;

	drawObjectColor(&submarineModel, shipModelMatrix, glm::vec3(0.08f, 0.05f, 0.05f), programColor);

	terrain.checkCollision(shipPos, camera.cameraPos);

	//Coins
	for (int i = 0; i < NUM_MAXPOINTS; i++) {
		coins[i].createCoinMatrix();
		if (!coins[i].isCollected) {
			// draw coin
			drawCoin(
				&coins[i].coinModel,
				coins[i].coinMatrix,
				coins[i].coinColor,
				coins[i].coinPos,
				programCoins
			);

			// draw coin marker
			particleGenerator.drawGenerator(coins[i].coinPos, delta, camera, programParticles);
		}
	}
	

	// APPLY LIGHTING SHADER TO MODELS WITH TEXTURE
	GLuint program = programSpotlightTexture;
	spotlight(program, camera.cameraMatrix, camera.perspectiveMatrix);
	

	// Fishes
	for (int i = 0; i < NUM_FISH; i++) {
		fishes[i].createFishMatrix();
		drawObjectTexture(
			&fishes[i].fishModel,
			fishes[i].fishMatrix * glm::rotate(glm::radians(-90.0f), glm::vec3(0,1,0)) * glm::rotate(-fishes[i].fishAngle, glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(0.5f)),
			fishes[i].fishTexture,
			program
		);

		/*drawObjectColor(
			&fishes[i].fishModel,
			fishes[i].fishMatrix * glm::rotate(glm::radians(-90.0f), glm::vec3(0, 1, 0)) * glm::rotate(-fishes[i].fishAngle, glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(0.5f)),
			fishes[i].fishColor,
			program
		);*/
	}

	// APPLY LIGHTING SHADER TO MODELS WITH COLOR
	program = programSpotlightColor;
	spotlight(program, camera.cameraMatrix, camera.perspectiveMatrix);

	// Ground Ents
	for (int i = 0; i < NUM_GROUND_ENT; i++) {
		drawObjectColor(
			&groundEnts[i].modelPath,
			glm::translate(groundEnts[i].position) * glm::scale(glm::vec3(groundEnts[i].rescaleSize)),
			groundEnts[i].color,
			program
		);
	}

	// Bubbles
	for (int i = 0; i < NUM_BUBBLES; i++) {
		for (int j = 0; j < NUM_BUBBLES_PER_GROUP; j++) {
			bubbles[i][j].createBubblesMatrix();
			drawObjectColor(
				&bubbles[i][j].bubbleModel,
				bubbles[i][j].bubblesMatrix,
				bubbles[i][j].bubblesColor,
				program
			);
		}
	}

	// GAME MECHANICS

	// Checking if game has ended
	if (NUM_POINTS == NUM_MAXPOINTS) {
		glutDisplayFunc(endingScreenDisplay);
	}

	// Checking contact with coins
	for (int i = 0; i < NUM_MAXPOINTS; i++) {
		if (isShipNearCoin(coins[i].coinPos, shipPos, 4.0f)) {
			if (!coins[i].isCollected)
				NUM_POINTS += 1;
			coins[i].isCollected = true;
		}
	}


	gameplayTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;

	glutSwapBuffers();
}

void modelInit() {
	// Submarine
	submarineModel = obj::loadModelFromFile("models/submarine2.obj");
}

void init()
{
	glEnable(GL_DEPTH_TEST);

	// Create the height map
	createHeightMap(500, 500, randNumber(3, 8), randNumber(3, 8));

	// Setup skybox
	skybox.prepareSkybox();
	
	// Setup terrain
	underwaterGroundTexture = new terrainTexture("textures/ground/tex1.jpg");
	heightMapTexture = new terrainTexture("textures/heightmaps/heightmap.bmp");
	terrain.prepareTerrain(heightMapTexture);



	// Shaders
	programColor = shaderLoader.CreateProgram("shaders/shader_color.vert", "shaders/shader_color.frag");
	programTexture = shaderLoader.CreateProgram("shaders/shader_tex.vert", "shaders/shader_tex.frag");
	programSkybox = shaderLoader.CreateProgram("shaders/skybox.vert", "shaders/skybox.frag");
	programSpotlightTexture = shaderLoader.CreateProgram("shaders/lighting_tex.vert", "shaders/lighting_tex.frag");
	programSpotlightColor = shaderLoader.CreateProgram("shaders/lighting_color.vert", "shaders/lighting_color.frag");
	programCoins = shaderLoader.CreateProgram("shaders/coin.vert", "shaders/coin.frag");
	programTerrain = shaderLoader.CreateProgram("shaders/terrain.vert", "shaders/terrain.frag");
	programParticles = shaderLoader.CreateProgram("shaders/particles.vert", "shaders/particles.frag");

	modelInit();

	// Setup fish
	for (int i = 0; i < NUM_FISH; i++) {
		fishes[i].prepareFish(terrain.TERRAIN_SIZE);
	}

	// Setup ground ents
	for (int i = 0; i < NUM_GROUND_ENT; i++) {
		groundEnts[i].prepareGroundEnts();

		int terrainVerticesPointer = randNumber(0, terrain.verticesCount);
		groundEnts[i].position = terrain.getVerticesPosition(terrainVerticesPointer);
	}

	// Setup coins
	for (int i = 0; i < NUM_MAXPOINTS; i++) {
		coins[i].prepareCoins(terrain.TERRAIN_SIZE);
	}

	// Setup bubbles 
	for (int i = 0; i < NUM_BUBBLES; i++) {
		int terrainVerticesPointer = randNumber(0, terrain.verticesCount);

		for (int j = 0; j < NUM_BUBBLES_PER_GROUP; j++) {
			bubbles[i][j].bubblesPos = terrain.getVerticesPosition(terrainVerticesPointer);
			bubbles[i][j].initBubbles();
		}
	}

	// Setup particle generator
	particleGenerator.initGenerator();


	appLoadingTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
	lastTime = appLoadingTime;
}

void shutdown()
{
	shaderLoader.DeleteProgram(programColor);
	shaderLoader.DeleteProgram(programTexture);
	shaderLoader.DeleteProgram(programSkybox);
	shaderLoader.DeleteProgram(programSpotlightTexture);
	shaderLoader.DeleteProgram(programSpotlightColor);
	shaderLoader.DeleteProgram(programCoins);
	shaderLoader.DeleteProgram(programTerrain);
	shaderLoader.DeleteProgram(programParticles);

	particleGenerator.clearGenerator();
}

void idle()
{
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(400, 0);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("Underwater scene");
	glewInit();

	init();
	modelInit();

	glutKeyboardFunc(callKeyboard);
	glutPassiveMotionFunc(callMouse);

	glutDisplayFunc(renderScene);
	glutIdleFunc(idle);

	glutMainLoop();

	shutdown();

	return 0;
}
