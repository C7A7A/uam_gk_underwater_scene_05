#include "Coin.h"

Coin::Coin()
{
}

void Coin::prepareCoins(int terrainSize) {
	maxPositiveX = 0.5 * terrainSize/2;
	maxNegativeX = -maxPositiveX;
	
	maxPositiveZ = 0.5 * terrainSize/2;
	maxNegativeZ = -maxPositiveZ;

	maxPositiveY = 65;
	maxNegativeY = 35;

	setCoinInitialPos();
}

void Coin::createCoinMatrix() {
	float time = (glutGet(GLUT_ELAPSED_TIME) / 1000.0f);

	coinMatrix = glm::translate(coinPos) * glm::scale(glm::vec3(1.2f));

	coinMatrix = glm::rotate(coinMatrix, glm::radians(time * 180.f), glm::vec3(0.f, 1.f, 0.f));
}

void Coin::setCoinInitialPos() {

	coinPos.x = randNumber(maxNegativeX, maxPositiveX);
	coinPos.z = randNumber(maxNegativeZ, maxPositiveZ);
	coinPos.y = randNumber(maxNegativeY, maxPositiveY);
}
