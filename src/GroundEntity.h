#pragma once
#include "objload.h"
#include <glm.hpp>
#include "../helpers/Colors.h"
#include "../helpers/Random.h"
#include "../helpers/helpersGroundEnts.h"

class GroundEntity
{
public:
	obj::Model modelPath;

	float rescaleSize = 10.0f;

	int num_colors = NUM_COLORS;
	glm::vec3 color = glm::vec3(0);

	glm::vec3 position = glm::vec3(0);

private:

public:
	GroundEntity();
	void prepareGroundEnts();


private:
	//void setColor();
	//void setModel();
};

