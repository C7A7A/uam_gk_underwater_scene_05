#include "Camera.h"

Camera::Camera() {}

void Camera::setPerspectiveMatrix()
{
	perspectiveMatrix = createPerspectiveMatrix();
}

void Camera::setCameraMatrixQuat(glm::vec2 &differenceMousePosition, std::map<std::string, bool> mouseEdgeflags) {
	float angleYaw = differenceMousePosition.x * reduceAngle;
	float anglePitch = differenceMousePosition.y * reduceAngle;
	
	// check for auto rotation
	if (mouseEdgeflags["upperEdge"]) {
		anglePitch = -autoRotatePitch;
	}
	else if (mouseEdgeflags["bottomEdge"]) {
		anglePitch = autoRotatePitch;
	}
	else if (mouseEdgeflags["leftEdge"]) {
		angleYaw = -autoRotateYaw;
	}
	else if (mouseEdgeflags["rightEdge"]) {
		angleYaw = autoRotateYaw;
	}

	glm::quat rotationYaw = glm::quat(glm::angleAxis(angleYaw, glm::vec3(0, 1, 0))); // rotate around Y axi
	glm::quat rotationPitch = glm::quat(glm::angleAxis(anglePitch, glm::vec3(1, 0, 0))); // rotate around X axi

	differenceMousePosition.x = 0;
	differenceMousePosition.y = 0;

	rotation = glm::normalize(rotationPitch * rotation * rotationYaw);
	glm::quat inverseRotation = glm::quat(glm::inverse(rotation));

	cameraDir = inverseRotation * glm::vec3(0, 0, -1);
	cameraSide = inverseRotation * glm::vec3(1, 0, 0);

	cameraMatrix = createViewMatrixQuat(cameraPos, rotation);
}

glm::mat4 Camera::createPerspectiveMatrix(float zNear, float zFar)
{
	const float frustumScale = 1.1f;
	glm::mat4 perspective;
	perspective[0][0] = frustumScale;
	perspective[1][1] = frustumScale;
	perspective[2][2] = (zFar + zNear) / (zNear - zFar);
	perspective[3][2] = (2 * zFar * zNear) / (zNear - zFar);
	perspective[2][3] = -1;
	perspective[3][3] = 0;

	return perspective;
}

glm::mat4 Camera::createViewMatrixQuat(glm::vec3 position, glm::quat rotation)
{
	glm::mat4 cameraTranslation;
	cameraTranslation[3] = glm::vec4(-position, 1.0f);

	return glm::mat4_cast(rotation) * cameraTranslation;
}

void Camera::keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'w':
			cameraPos += cameraDir * moveSpeed;
			break;
		case 's':
			cameraPos -= cameraDir * moveSpeed;
			break;
		case 'd':
			cameraPos += cameraSide * moveSpeed;
			break;
		case 'a':
			cameraPos -= cameraSide * moveSpeed;
			break;
		case 'z':
			cameraPos -= angleSpeed;
			break;
		case 'x':
			cameraPos += angleSpeed;
			break;
		default:
			break;
	}
}
