#pragma once

#include "glm.hpp"
#include "map"
#include "string"

class Mouse {

public:
	glm::vec2 prevMousePosition = glm::vec2(0, 0);
	glm::vec2 differenceMousePosition = glm::vec2(0, 0);
	std::map<std::string, bool> mouseEdgeflags = {
		{ "upperEdge", false },
		{ "rightEdge", false },
		{ "bottomEdge", false },
		{ "leftEdge", false }
	};

private:
	int windowWidth = 800;
	int windowHeight = 800;

public:
	Mouse();
	Mouse(int width, int height);
	
	void setMousePosition(int x, int y);
private:

};