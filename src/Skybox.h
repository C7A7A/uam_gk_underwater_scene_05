#pragma once

#include "glew.h"
#include "glm.hpp"
#include <ext.hpp>
#include <string>
#include <iostream>
#include "Camera.h"
#include "stb_image.h"

class Skybox
{
private:
	unsigned int skyboxVAO = 0, skyboxVBO = 0, skyboxEBO = 0;
	unsigned int cubemapTexture = 0;

	float skyboxVertices[24] = {
		//   Coordinates
		-1.0f, -1.0f,  1.0f,//        7--------6
		 1.0f, -1.0f,  1.0f,//       /|       /|
		 1.0f, -1.0f, -1.0f,//      4--------5 |
		-1.0f, -1.0f, -1.0f,//      | |      | |
		-1.0f,  1.0f,  1.0f,//      | 3------|-2
		 1.0f,  1.0f,  1.0f,//      |/       |/
		 1.0f,  1.0f, -1.0f,//      0--------1
		-1.0f,  1.0f, -1.0f
	};

	unsigned int skyboxIndices[36] = {
		// Right
		1, 2, 6,
		6, 5, 1,
		// Left
		0, 4, 7,
		7, 3, 0,
		// Top
		4, 5, 6,
		6, 7, 4,
		// Bottom
		0, 3, 2,
		2, 1, 0,
		// Back
		0, 1, 5,
		5, 4, 0,
		// Front
		3, 7, 6,
		6, 2, 3
	};

	// All sides of the cubemap
	std::string cubemapSides[6] = {
		"./textures/skybox/right.jpg",
		"./textures/skybox/left.jpg",
		"./textures/skybox/top.jpg",
		"./textures/skybox/bottom.jpg",
		"./textures/skybox/front.jpg",
		"./textures/skybox/back.jpg"
	};

public:
	Skybox();

	void prepareSkybox();
	void drawSkybox(Camera camera, GLuint programSkybox);

private:
	void prepareSkyboxData();
	void createSkyboxTexture();
	void attachTextureToCubemap();
};

