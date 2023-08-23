#include "helpersFish.h"

std::string fishModelsPath[] = {
	   "models/fishes/anglerfish.obj",
	   "models/fishes/armoredcatfish.obj",
	   "models/fishes/manta.obj",
	   "models/fishes/piranha.obj",
	   "models/fishes/puffer.obj",
	   "models/fishes/turbot.obj",
	   "models/fishes/fish1.obj",
	   "models/fishes/fish2.obj",
	   "models/fishes/dolphin.obj",
	   "models/fishes/shark.obj",
	   "models/fishes/whale.obj"
};

std::string fishTexturesPath[] = {
	   "textures/abstract1.png",
	   "textures/abstract2.png",
	   "textures/abstract3.png",
	   "textures/abstract4.png",
	   "textures/abstract5.png",
	   "textures/abstract6.png",
	   "textures/abstract7.png",
	   "textures/abstract8.png",
	   "textures/abstract9.png",
	   "textures/abstract10.png"
};

void setCatmullPath(glm::vec3 keyPoints[NUM_CATMULL_POINTS], int pathIndex, float x, float y, float z) {
	switch (pathIndex) {
	case 0:
		keyPoints[0] = glm::vec3(x, y, z);
		keyPoints[1] = glm::vec3(x - 30, y, z - 25);
		keyPoints[2] = glm::vec3(x - 48, y, z - 60);
		keyPoints[3] = glm::vec3(x - 48, y, z - 100);
		keyPoints[4] = glm::vec3(x - 34, y, z - 135);
		keyPoints[5] = glm::vec3(x - 6, y, z - 162);
		keyPoints[6] = glm::vec3(x + 30, y, z - 176);
		keyPoints[7] = glm::vec3(x + 70, y, z - 173);
		keyPoints[8] = glm::vec3(x + 103, y, z - 155);
		keyPoints[9] = glm::vec3(x + 128, y, z - 124);
		keyPoints[10] = glm::vec3(x + 137, y, z - 86);
		keyPoints[11] = glm::vec3(x + 131, y, z - 48);
		keyPoints[12] = glm::vec3(x + 109, y, z - 15);
		keyPoints[13] = glm::vec3(x + 76, y, z + 6);
		keyPoints[14] = glm::vec3(x + 37, y, z + 10);
		break;
	case 1:
		keyPoints[0] = glm::vec3(x, y, z);
		keyPoints[1] = glm::vec3(x + 37, y, z + 10);
		keyPoints[2] = glm::vec3(x + 76, y, z + 6);
		keyPoints[3] = glm::vec3(x + 109, y, z - 15);
		keyPoints[4] = glm::vec3(x + 131, y, z - 48);
		keyPoints[5] = glm::vec3(x + 137, y, z - 86);
		keyPoints[6] = glm::vec3(x + 128, y, z - 124);
		keyPoints[7] = glm::vec3(x + 103, y, z - 155);
		keyPoints[8] = glm::vec3(x + 70, y, z - 173);
		keyPoints[9] = glm::vec3(x + 30, y, z - 176);
		keyPoints[10] = glm::vec3(x - 6, y, z - 162);
		keyPoints[11] = glm::vec3(x - 34, y, z - 135);
		keyPoints[12] = glm::vec3(x - 48, y, z - 100);
		keyPoints[13] = glm::vec3(x - 48, y, z - 60);
		keyPoints[14] = glm::vec3(x - 30, y, z - 25);
		break;
	case 2:
		keyPoints[0] = glm::vec3(x, y, z);
		keyPoints[1] = glm::vec3(x + 20, y, z + 32);
		keyPoints[2] = glm::vec3(x + 53, y, z + 58);
		keyPoints[3] = glm::vec3(x + 66, y, z + 24);
		keyPoints[4] = glm::vec3(x + 48, y, z - 6);
		keyPoints[5] = glm::vec3(x + 80, y, z - 15);
		keyPoints[6] = glm::vec3(x + 101, y, z - 42);
		keyPoints[7] = glm::vec3(x + 71, y, z - 54);
		keyPoints[8] = glm::vec3(x + 53, y, z - 30);
		keyPoints[9] = glm::vec3(x + 30, y, z - 51);
		keyPoints[10] = glm::vec3(x + 6, y, z - 70);
		keyPoints[11] = glm::vec3(x - 32, y, z - 59);
		keyPoints[12] = glm::vec3(x - 59, y, z - 28);
		keyPoints[13] = glm::vec3(x - 67, y, z - 65);
		keyPoints[14] = glm::vec3(x - 17, y, z - 29);
		break;
	case 3:
		keyPoints[0] = glm::vec3(x, y, z);
		keyPoints[1] = glm::vec3(x - 17, y, z - 29);
		keyPoints[2] = glm::vec3(x - 67, y, z - 65);
		keyPoints[3] = glm::vec3(x - 59, y, z - 28);
		keyPoints[4] = glm::vec3(x - 32, y, z - 59);
		keyPoints[5] = glm::vec3(x + 6, y, z - 70);
		keyPoints[6] = glm::vec3(x + 30, y, z - 51);
		keyPoints[7] = glm::vec3(x + 53, y, z - 30);
		keyPoints[8] = glm::vec3(x + 71, y, z - 54);
		keyPoints[9] = glm::vec3(x + 101, y, z - 42);
		keyPoints[10] = glm::vec3(x + 80, y, z - 15);
		keyPoints[11] = glm::vec3(x + 48, y, z - 6);
		keyPoints[12] = glm::vec3(x + 66, y, z + 24);
		keyPoints[13] = glm::vec3(x + 53, y, z + 58);
		keyPoints[14] = glm::vec3(x + 20, y, z + 32);
		break;
	case 4:
		keyPoints[0] = glm::vec3(x, y, z);
		keyPoints[1] = glm::vec3(x + 5, y, z - 62);
		keyPoints[2] = glm::vec3(x - 18, y, z - 137);
		keyPoints[3] = glm::vec3(x - 61, y, z - 46);
		keyPoints[4] = glm::vec3(x + 20, y, z - 60);
		keyPoints[5] = glm::vec3(x + 40, y, z - 120);
		keyPoints[6] = glm::vec3(x + 90, y, z - 158);
		keyPoints[7] = glm::vec3(x + 111, y, z - 91);
		keyPoints[8] = glm::vec3(x + 145, y, z - 175);
		keyPoints[9] = glm::vec3(x + 153, y, z - 90);
		keyPoints[10] = glm::vec3(x + 116, y, z - 47);
		keyPoints[11] = glm::vec3(x + 67, y, z - 86);
		keyPoints[12] = glm::vec3(x + 105, y, z - 9);
		keyPoints[13] = glm::vec3(x + 44, y, z - 36);
		keyPoints[14] = glm::vec3(x + 50, y, z + 37);
		break;
	case 5:
		keyPoints[0] = glm::vec3(x, y, z);
		keyPoints[1] = glm::vec3(x + 50, y, z + 37);
		keyPoints[2] = glm::vec3(x + 44, y, z - 36);
		keyPoints[3] = glm::vec3(x + 105, y, z - 9);
		keyPoints[4] = glm::vec3(x + 67, y, z - 86);
		keyPoints[5] = glm::vec3(x + 116, y, z - 47);
		keyPoints[6] = glm::vec3(x + 153, y, z - 90);
		keyPoints[7] = glm::vec3(x + 145, y, z - 175);
		keyPoints[8] = glm::vec3(x + 111, y, z - 91);
		keyPoints[9] = glm::vec3(x + 90, y, z - 158);
		keyPoints[10] = glm::vec3(x + 40, y, z - 120);
		keyPoints[11] = glm::vec3(x + 20, y, z - 60);
		keyPoints[12] = glm::vec3(x - 61, y, z - 46);
		keyPoints[13] = glm::vec3(x - 18, y, z - 137);
		keyPoints[14] = glm::vec3(x + 5, y, z - 62);
		break;
	}
}
