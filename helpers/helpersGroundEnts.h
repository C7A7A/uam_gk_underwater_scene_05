#pragma once

#include <string>
#include "map"
#include "glm.hpp"
#include "Colors.h"

struct groundEntData {
	std::string modelPath;
	glm::vec3 color;
	float scaleFrom;
	float scaleTo;
};

static const int NUM_GROUND_ENTS_MODELS = 11;

extern std::map<int, groundEntData> groundEntsData;
