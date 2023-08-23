#pragma once
#include <string>
#include "glm.hpp"
#include <iostream>

static const int NUM_FISH_MODELS = 11;
static const int NUM_FISH_TEXTURES = 10;
static const int NUM_CATMULL_PATHS = 6;
static const int NUM_CATMULL_POINTS = 15;

extern std::string fishModelsPath[NUM_FISH_MODELS];
extern std::string fishTexturesPath[NUM_FISH_TEXTURES];
extern void setCatmullPath(glm::vec3 keyPoints[NUM_CATMULL_POINTS], int pathIndex, float x, float y, float z);
