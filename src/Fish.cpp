#include "Fish.h"

Fish::Fish()
{
}

void Fish::prepareFish(int terrainSize) {
	
	maxPositiveX = 0.8 * terrainSize / 2;
	maxNegativeX = -maxPositiveX;

	maxPositiveZ = 0.8 * terrainSize / 2;
	maxNegativeZ = -maxPositiveZ;

	maxPositiveY = 0;
	maxNegativeY = -180;

	setFishInitialPos();
	setFishModel();
	//setFishColor();
	setFishTexture();
	setCatmullKeyPoints();
}

void Fish::createFishMatrix() {
	float time = (glutGet(GLUT_ELAPSED_TIME) / 7000.0f);

	int timeRounded = floorf(time);
	float timeFraction = time - timeRounded;

	glm::vec3 catmullInterpolation = glm::catmullRom(
		keyPoints[(timeRounded - 1) % num_catmull_points],
		keyPoints[timeRounded % num_catmull_points],
		keyPoints[(timeRounded + 1) % num_catmull_points],
		keyPoints[(timeRounded + 2) % num_catmull_points],
		timeFraction
	);

	glm::vec3 staticVector = glm::normalize(
		glm::catmullRom(
			keyPoints[(timeRounded - 1) % num_catmull_points],
			keyPoints[timeRounded % num_catmull_points],
			keyPoints[(timeRounded + 1) % num_catmull_points],
			keyPoints[(timeRounded + 2) % num_catmull_points],
			timeFraction + 0.001
		) -
		glm::catmullRom(
			keyPoints[(timeRounded - 1) % num_catmull_points],
			keyPoints[timeRounded % num_catmull_points],
			keyPoints[(timeRounded + 1) % num_catmull_points],
			keyPoints[(timeRounded + 2) % num_catmull_points],
			timeFraction - 0.001
		)
	);

	fishDir = staticVector;
	fishAngle = atan2f(fishDir.z, fishDir.x);

	// fishUp = glm::vec3(0, 1, 0);
	fishPos = catmullInterpolation;
	// fishSide = glm::cross(fishDir, fishUp);

	glm::mat4 fishTranslation;
	fishTranslation[3] = glm::vec4(-fishPos, 1.0f);

	fishMatrix = fishTranslation;
}

void Fish::setFishInitialPos() {
	fishPos.x = randNumber(maxNegativeX, maxPositiveX);
	fishPos.z = randNumber(maxNegativeZ, maxPositiveZ); 
	fishPos.y = randNumber(maxNegativeY, maxPositiveY); // ???
}

void Fish::setFishModel() {
	const int fishModelPathIndex = randNumber(0, num_models - 1);
	fishModel = obj::loadModelFromFile(fishModelsPath[fishModelPathIndex]);
}

void Fish::setFishTexture() {
	const int fishModelPathIndex = randNumber(0, num_textures - 1);
	fishTexture = Core::LoadTexture(fishTexturesPath[fishModelPathIndex].data());
}

void Fish::setFishColor() {
	const int fishColorIndex = randNumber(0, num_colors - 1);
	fishColor = colors[fishColorIndex];
}

void Fish::setCatmullKeyPoints() {
	const int fishCatmullPathIndex = randNumber(0, NUM_CATMULL_PATHS - 1);

	setCatmullPath(keyPoints, fishCatmullPathIndex, fishPos.x, fishPos.y, fishPos.z);
}
