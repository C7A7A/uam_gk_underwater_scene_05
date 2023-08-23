#pragma once
#include "objload.h"
#include "Camera.h"
#include <glew.h>
#include <random>
#include <freeglut.h>
#include <ext.hpp>
#include "../helpers/Random.h"

class Coin
{
public:
	obj::Model coinModel = obj::loadModelFromFile("models/coin.obj");

	glm::mat4 coinMatrix = glm::mat4(0);

	glm::vec3 coinPos = glm::vec3(0);

	glm::vec3 coinColor = glm::vec3(0.96f, 0.67f, 0.07f);

	bool isCollected = false;

private:
	float maxPositiveX = 0;
	float maxNegativeX = 0;
	float maxPositiveZ = 0;
	float maxNegativeZ = 0;
	float maxPositiveY = 0;
	float maxNegativeY = 0;

public:
	Coin();
	void prepareCoins(int terrainSize);
	void createCoinMatrix();

private:
	void setCoinInitialPos();
	//void applyShader(GLuint shader, Camera camera);
};

