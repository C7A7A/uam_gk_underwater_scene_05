#pragma once

#include <vector>
#include <glew.h>
#include <ext.hpp>
#include "terrainTexture.h"

class Terrain
{
public:
	Terrain();

	void prepareTerrain(terrainTexture* heightMap);
	void heightMapDataSetup(terrainTexture* heightMap);
	void drawTerrain(GLuint program, terrainTexture *heightMap, terrainTexture *groundTexture, glm::mat4 perspectiveMatrix, glm::mat4 cameraMatrix);

	glm::vec3 getVerticesPosition(int pointer);
	glm::vec3 getMapPosition(int pointer);
	void checkCollision(glm::vec3 shipPos, glm::vec3 &cameraPos);

private:
	void verticesTextureCoordsSetup();
	void indicesSetup();
	void generateBuffers();

public:
	unsigned int terrainVAO = 0, positionVBO = 0, terrainVBO = 0, terrainIBO = 0;

	std::vector<float> vertices;
	std::vector<float> textureCoords;
	std::vector<unsigned int> indices;

	int verticesCount = 0;

	const int TERRAIN_SIZE = 1000;
	int mapScale = 2;
	int mapScale2 = 4;
	const float AMPLITUDE = 25.0f;

private:
	int vertexPointer = 0;

	int mapWidth = 0, mapHeight = 0;
	int nrComponents = 0;
	unsigned char* data;

	float collisionEpsilon = 2.0f;
};

