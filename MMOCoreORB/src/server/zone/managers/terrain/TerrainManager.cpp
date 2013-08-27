/*
 * TerrainManager.cpp
 *
 *  Created on: 18/04/2010
 *      Author: victor
 */

#include "TerrainManager.h"
#include "server/zone/Zone.h"
#include "server/zone/managers/templates/TemplateManager.h"

TerrainManager::TerrainManager(Zone* planet) : Logger("TerrainManager") {
	zone = planet;
}

TerrainManager::TerrainManager(ManagedWeakReference<Zone*> planet) : Logger("TerrainManager") {
	zone = planet.get();
}

bool TerrainManager::initialize(const String& terrainFile) {
	IffStream* iffStream = TemplateManager::instance()->openIffFile(terrainFile);

	if (iffStream == NULL)
		return false;

	if (iffStream->getNextFormType() != 'PTAT')
		return false;

	return terrainData.load(iffStream);
}

/**
 *  	|----------------| x1,y1
 *  	|----------------| <- stepping
 *	    |----------------|
 *  	|----------------|
 *x0,y0 |----------------|
 */
float TerrainManager::getHighestHeight(float x0, float y0, float x1, float y1, int stepping) {
	/*StringBuffer msg2;
	msg2 << "getHeights for (x0, y0) = (" << x0 << ", " << y0 << ") (x1, y1) = (" << x1 << ", " << y1 << ")";
	info(msg2.toString(), true);*/

	int deltaX = (int)fabs(x1 - x0);
	int deltaY = (int)fabs(y1 - y0);

	float maxHeight = -16000.f;

	for (int i = (int)y0; i < (int)y0 + deltaY; i += stepping) {
		for (int j = (int)x0; j < (int)x0 + deltaX; j += stepping) {
			float height = zone->getHeight(j, i);

			/*StringBuffer msg;
			msg << "checking x:" << j << " y:" << i << " height:" << height;
			info(msg.toString(), true);*/

			if (height > maxHeight)
				maxHeight = height;
		}
	}

	return maxHeight;
}

int TerrainManager::notifyPositionUpdate(CreatureObject* object) {
	CreatureObject* creature = cast<CreatureObject*>( object);

	float waterHeight;

	if (creature->getParent() == NULL && getWaterHeight(creature->getPositionX(), creature->getPositionY(), waterHeight)) {

		if (creature->getPositionZ() + creature->getSwimHeight()- waterHeight < 0.2) {

			if (creature->hasState(CreatureState::ONFIRE))
				creature->healDot(CreatureState::ONFIRE, 100);
		}
	}

	return 0;
}

void TerrainManager::addTerrainModification(float x, float y, const String& terrainModificationFilename, uint64 objectid) {
	IffStream* stream = TemplateManager::instance()->openIffFile(terrainModificationFilename);

	if (stream == NULL) {
		error("could not find custom terrain file: " + terrainModificationFilename);
		return;
	}

	if (terrainData.addTerrainModification(stream, x, y, objectid) == NULL) {
		error("could not add custom terrain file: " + terrainModificationFilename);
	}

	delete stream;
}

void TerrainManager::removeTerrainModification(uint64 objectid) {
	terrainData.removeTerrainModification(objectid);
}
