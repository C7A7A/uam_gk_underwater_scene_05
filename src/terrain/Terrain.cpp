#include "Terrain.h"

Terrain::Terrain()
{
	
}

void Terrain::prepareTerrain(terrainTexture* heightMap)
{
	heightMapDataSetup(heightMap);

	vertices.resize(mapHeight * mapWidth * 3);
	textureCoords.resize(mapWidth * mapHeight * 2);
	indices.resize(6 * (mapWidth - 1) * (mapHeight - 1));

	verticesTextureCoordsSetup();
	indicesSetup();
	generateBuffers();
}

void Terrain::heightMapDataSetup(terrainTexture* heightMap) {
	data = heightMap->data;
	nrComponents = heightMap->nrComponents;
	mapWidth = heightMap->width;
	mapHeight = heightMap->height;
	mapScale = TERRAIN_SIZE / mapHeight;
	mapScale2 = TERRAIN_SIZE / (mapHeight / 2);
}

void Terrain::drawTerrain(GLuint program, terrainTexture *heightMap, terrainTexture *groundTexture, glm::mat4 perspectiveMatrix, glm::mat4 cameraMatrix) {
	glUseProgram(program);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	// Heightmap texture
	glActiveTexture(GL_TEXTURE1);
	heightMap->bind();
	glUniform1i(glGetUniformLocation(program, "heightMapTexture"), 1);

	// Setup Shader
	glUniform1f(glGetUniformLocation(program, "AMPLITUDE"), AMPLITUDE);
	//glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, glm::value_ptr(perspectiveMatrix));
	//glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, glm::value_ptr(glm::translate(glm::mat4(), glm::vec3(0, -AMPLITUDE, 0))));
	//glUniform3fv(glGetUniformLocation(program, "lightPosition"), 1, glm::value_ptr(glm::vec3(20000, 20000, 20000)));
	//glUniform3fv(glGetUniformLocation(program, "lightColor"), 1, glm::value_ptr(glm::vec3(0.5, 0.5, 0.5)));
	//glUniform1f(glGetUniformLocation(program, "shineDamper"), 1);
	//glUniform1f(glGetUniformLocation(program, "reflectivity"), 0);

	// Ground texture
	glActiveTexture(GL_TEXTURE0);
	groundTexture->bind();
	glUniform1i(glGetUniformLocation(program, "tex"), 0);

	// Draw the vertices/indices (send them to the graphics card to be processed)
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBindVertexArray(terrainVAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	heightMap->unbind();
	groundTexture->unbind();

	glUseProgram(0);
}

void Terrain::verticesTextureCoordsSetup()
{
	unsigned bytePerPixel = nrComponents;

	// setup vertices and textureCoords
	for (int i = 0; i < mapHeight; i++) {
		for (int j = 0; j < mapWidth; j++) {
			unsigned char* pixelOffset = data + (j + mapWidth * i) * bytePerPixel;
			unsigned char y = pixelOffset[0]; // get color from png

			float height = (float)y / 255.0f; // convert from (0 - 255) -> (0 - 1)
			height = height + pow(2, -height);
			height = height * AMPLITUDE; // calculate height just like in terrain shader

			//std::cout << height << std::endl;

			vertices[vertexPointer * 3] = int((float)j / ((float)mapHeight) * TERRAIN_SIZE - TERRAIN_SIZE / 2); // v.x
			vertices[vertexPointer * 3 + 1] = int(height); // v.y
			vertices[vertexPointer * 3 + 2] = int((float)i / ((float)mapWidth) * TERRAIN_SIZE - TERRAIN_SIZE / 2); // v.z
			
			//std::cout << "x: " << vertices[vertexPointer * 3] << " y: " << vertices[vertexPointer * 3 + 1] << " z: " << vertices[vertexPointer * 3 + 2] << std::endl;

			textureCoords[vertexPointer * 2] = (float)j / ((float)mapHeight); // texCoord.x
			textureCoords[vertexPointer * 2 + 1] = (float)i / ((float)mapWidth); // texCoord.z

			vertexPointer++;
		}
	}

	this->verticesCount = vertices.size() / 3;
	std::cout << "Loaded " << verticesCount << " vertices" << std::endl;
	stbi_image_free(data); // free pointer to png data
}

void Terrain::indicesSetup()
{
	// setup indices
	int pointer = 0;
	for (int gz = 0; gz < mapHeight - 1; gz++) {
		for (int gx = 0; gx < mapWidth - 1; gx++) {
			int topLeft = (gz * mapHeight) + gx;
			int topRight = topLeft + 1;
			int bottomLeft = ((gz + 1) * mapHeight) + gx;
			int bottomRight = bottomLeft + 1;

			indices[pointer++] = topLeft;
			indices[pointer++] = bottomLeft;
			indices[pointer++] = topRight;

			indices[pointer++] = topRight;
			indices[pointer++] = bottomLeft;
			indices[pointer++] = bottomRight;
		}
	}

	std::cout << "Loaded " << indices.size() << " indices" << std::endl;
}

void Terrain::generateBuffers()
{
	// terrainVAO
	glGenVertexArrays(1, &terrainVAO);
	glBindVertexArray(terrainVAO);

	// terrain indices
	glGenBuffers(1, &terrainIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrainIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// terrain positions
	glGenBuffers(1, &positionVBO);
	glBindBuffer(GL_ARRAY_BUFFER, positionVBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// terrain tex coords
	glGenBuffers(1, &terrainVBO);
	glBindBuffer(GL_ARRAY_BUFFER, terrainVBO);
	glBufferData(GL_ARRAY_BUFFER, textureCoords.size() * sizeof(float), &textureCoords[0], GL_STATIC_DRAW);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

glm::vec3 Terrain::getVerticesPosition(int pointer) {
	glm::vec3 terrainPosition = glm::vec3(
		vertices[pointer * 3],
		vertices[pointer * 3 + 1] - AMPLITUDE,
		vertices[pointer * 3 + 2]
	);

	return terrainPosition;
}

glm::vec3 Terrain::getMapPosition(int pointer) {
	if (pointer < 0 || pointer > verticesCount) return glm::vec3(0, -500, 0);

	glm::vec3 terrainPosition = glm::vec3(
		vertices[pointer * 3],
		vertices[pointer * 3 + 1] - AMPLITUDE,
		vertices[pointer * 3 + 2]
	);

	return terrainPosition;
}

void Terrain::checkCollision(glm::vec3 shipPos, glm::vec3 &cameraPos) {
	// get pointer to vertices  
	int verticesPointer = (int)
		round( (shipPos.x / mapScale) + (float)TERRAIN_SIZE / mapScale2) +
		mapHeight * (round( (shipPos.z / mapScale) + (float)TERRAIN_SIZE / mapScale2));

	glm::vec3 positions = getMapPosition(verticesPointer); // get terrain (x, y, z)

	//std::cout << "cam: " << round(shipPos.x) << " " << round(shipPos.y) << " " << round(shipPos.z) << std::endl;
	//std::cout << "ter: " << positions.x << " " << positions.y << " " << positions.z << std::endl;
	//std::cout << "pointer: " << verticesPointer << std::endl;

	// check for collision with epsilon
	if (shipPos.y - collisionEpsilon <= positions.y) {
		std::cout << "Collision, respawn at start position (0, 50, 0)" << std::endl;
		//std::cout << "COLLISION! " << shipPos.y << " " << positions.y << std::endl;
		cameraPos = glm::vec3(0, 50, 0); // respawn at start position
	}
}