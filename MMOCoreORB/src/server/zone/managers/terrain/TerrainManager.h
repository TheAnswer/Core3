/*
 * TerrainManager.h
 *
 *  Created on: 31/01/2010
 *      Author: victor
 */

#ifndef TERRAINMANAGER_H_
#define TERRAINMANAGER_H_


#include "engine/engine.h"
#include "server/zone/objects/terrain/ProceduralTerrainAppearance.h"
#include "server/zone/objects/scene/SceneObject.h"
#include "server/zone/objects/creature/CreatureObject.h"

namespace server {
 namespace zone {
  class Zone;
 }
}

using namespace server::zone;

class TerrainManager : public Logger, public Object {
	ProceduralTerrainAppearance terrainData;

	Zone* zone;

public:
	TerrainManager(Zone* planet);
	TerrainManager(ManagedWeakReference<Zone*> planet);

	bool initialize(const String& terrainFile);

	inline bool getWaterHeight(float x, float y, float& waterHeight) {
		return terrainData.getWater(x, y, waterHeight);
	}

	/**
	 *  	|--------------- | x1,y1
	 *  	|----------------| <- stepping
	 *	    |----------------|
	 *  	|----------------|
	 *x0,y0 |----------------|
	 */
	float getHighestHeight(float x0, float y0, float x1, float y1, int stepping = 1);

	int notifyPositionUpdate(CreatureObject* object);

	void addTerrainModification(float x, float y, const String& terrainModificationFilename, uint64 objectid);
	void removeTerrainModification(uint64 objectid);

	ProceduralTerrainAppearance* getProceduralTerrainAppearance() {
		return &terrainData;
	}

	float getHeight(float x, float y) {
		return terrainData.getHeight(x, y);
	}

	float getMin() {
		return terrainData.getSize() / 2 * -1;
	}

	float getMax() {
		return terrainData.getSize() / 2;
	}

	float getSize() {
		return terrainData.getSize();
	}
};


#endif /* TERRAINMANAGER_H_ */
