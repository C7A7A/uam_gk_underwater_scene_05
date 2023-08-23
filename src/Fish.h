#pragma once
#include "objload.h"
#include "Texture.h"
#include <string>
#include <glm.hpp>
#include <random>
#include <freeglut.h>
#include <ext.hpp>
#include "../helpers/Colors.h"
#include "../helpers/helpersFish.h"
#include "../helpers/Random.h"

class Fish
{
public:
	obj::Model fishModel;
	
	glm::mat4 fishMatrix = glm::mat4(0);
	
	glm::vec3 fishPos = glm::vec3(0);

	glm::vec3 fishDir = glm::vec3(0);
	glm::vec3 fishSide = glm::vec3(0);
	glm::vec3 fishUp = glm::vec3(0);

	const int num_models = NUM_FISH_MODELS;

	const int num_catmull_points = NUM_CATMULL_POINTS;
	glm::vec3 keyPoints[NUM_CATMULL_POINTS];

	const int num_colors = NUM_COLORS;
	glm::vec3 fishColor = glm::vec3(0);

	const int num_textures = NUM_FISH_TEXTURES;
	GLuint fishTexture = 0;

	float fishAngle = 0;

private:
	float maxPositiveX = 0;
	float maxNegativeX = 0;
	float maxPositiveZ = 0;
	float maxNegativeZ = 0;
	float maxPositiveY = 0;
	float maxNegativeY = 0;

public:
	Fish();
	void prepareFish(int terrainSize);
	void createFishMatrix();

private:
	void setFishInitialPos();
	void setFishColor();
	void setFishTexture();
	void setFishModel();
	void setCatmullKeyPoints();
};

