#pragma once

#include <string>
#include <vector>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <glew.h>
#include <iostream>
#include "../stb_image.h"

class terrainTexture {
public:
	int textureID = 0;
	int width = 0, height = 0;
	int nrComponents = 0;
	unsigned char* data;
	std::string& filePath;

private:

public:
	terrainTexture(std::string fileName);

	void bind();
	void unbind();

private:
	unsigned int load();
};