#include "Bubbles.h"

Bubbles::Bubbles() {

}

void Bubbles::initBubbles() {
	setBubblesInitialPos();
	maxBubbleHeigt = randNumber(30, 60);
	bubbleScale = randNumberDouble(0.25, 0.75);
}

void Bubbles::createBubblesMatrix() {

	float time = (glutGet(GLUT_ELAPSED_TIME) / 1000.0f);

	float velocity = 0.20 - (1.5 * bubbleScale / 1000 * time);

	bubblesMatrix = glm::translate(bubblesPos);

	if (bubblesPos.y - initY <= maxBubbleHeigt)
		bubblesPos.y += velocity;
	else if (bubblesPos.y - initY > maxBubbleHeigt)
		bubblesPos.y = initY;

	bubblesMatrix = bubblesMatrix * glm::scale(glm::vec3(bubbleScale));

}

void Bubbles::setBubblesInitialPos() {
	initY = bubblesPos.y;

	bubblesPos.x = randNumber(bubblesPos.x - epsilon, bubblesPos.x + epsilon);
	bubblesPos.y = randNumber(bubblesPos.y - 5 * epsilon, bubblesPos.y + 5 * epsilon);
	bubblesPos.z = randNumber(bubblesPos.z - epsilon, bubblesPos.z + epsilon);
}