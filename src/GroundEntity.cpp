#include "GroundEntity.h"

GroundEntity::GroundEntity() {
}

void GroundEntity::prepareGroundEnts() {
	const int modelIndex = randNumber(0, NUM_GROUND_ENTS_MODELS - 1);
	
	groundEntData data = groundEntsData[modelIndex];
	modelPath = obj::loadModelFromFile(data.modelPath);
	color = data.color;
	rescaleSize = randNumberDouble(data.scaleFrom, data.scaleTo);
}
