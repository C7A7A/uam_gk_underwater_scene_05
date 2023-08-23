#pragma once
#include "objload.h"
#include <glew.h>
#include <freeglut.h>
#include <ext.hpp>
#include "../helpers/Random.h"

class Bubbles {

public:
	obj::Model bubbleModel = obj::loadModelFromFile("models/sphere.obj");

	glm::vec3 bubblesColor = glm::vec3(0.58f, 0.90f, 0.93f);

	glm::vec3 bubblesPos = glm::vec3(0);

	glm::mat4 bubblesMatrix = glm::mat4(0);


private:

	float initY = 0, epsilon = 3;
	int maxBubbleHeigt = 0;
	float bubbleScale = 0.0f;

public:
	Bubbles();
	void initBubbles();
	void createBubblesMatrix();


private:
		
	void setBubblesInitialPos();

};

