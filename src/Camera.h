#pragma once

#include "glm.hpp"
#include "ext.hpp"
#include "map"
#include "string"

class Camera {

public:
	glm::vec3 cameraPos = glm::vec3(0, 50, 0); // init position
	glm::vec3 cameraDir = glm::vec3(0, 0, 0); // forward vector
	glm::vec3 cameraSide = glm::vec3(0, 0, 0); // up vector

	glm::mat4 cameraMatrix;
	glm::mat4 perspectiveMatrix;

	glm::quat rotation = glm::quat(-1, 0, 0, 0);

private:
	float angleSpeed = 0.2f;
	float moveSpeed = 1.0f; // 1.0f git jest

	float autoRotateYaw = 0.03f;
	float autoRotatePitch = 0.03f;

	float reduceAngle = 0.003f;

public:
	Camera();

	void setCameraMatrixQuat(glm::vec2 &differenceMousePosition, std::map<std::string, bool> mouseEdgeflags);
	void setPerspectiveMatrix();
	void keyboard(unsigned char key, int x, int y);

private:
	glm::mat4 createPerspectiveMatrix(float zNear = 0.1f, float zFar = 1500.0f);
	glm::mat4 createViewMatrixQuat(glm::vec3 position, glm::quat rotation);
};
