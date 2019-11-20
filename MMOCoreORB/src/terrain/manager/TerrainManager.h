/*
 * TerrainManager.h
 *
 *  Created on: 31/01/2010
 *      Author: victor
 */

#ifndef TERRAINMANAGER_H_
#define TERRAINMANAGER_H_

#include "terrain/TerrainAppearance.h"
#ifdef COMPILE_CORE3_TESTS
#include "gmock/gmock.h"
#endif
#include "TerrainCache.h"

class ProceduralTerrainAppearance;

class TerrainManager : public Logger, public Object {
	Reference<TerrainAppearance*> terrainData;

	TerrainCache* heightCache;

	float min, max;

protected:
	void clearCache(TerrainGenerator* generator);

public:
	TerrainManager();
	~TerrainManager();

	bool initialize(const String& terrainFile);

	inline bool getWaterHeight(float x, float y, float& waterHeight) const {
		return terrainData->getWater(x, y, waterHeight);
	}

	/**
	 *  	|--------------- | x1,y1
	 *  	|----------------| <- stepping
	 *	|----------------|
	 *  	|----------------|
	 *x0,y0 |----------------|
	 */
	float getHighestHeight(float x0, float y0, float x1, float y1, int stepping = 1);
	float getLowestHeight(float x0, float y0, float x1, float y1, int stepping = 1);
	float getHighestHeightDifference(float x0, float y0, float x1, float y1, int stepping = 1);

	void addTerrainModification(float x, float y, const String& terrainModificationFilename, uint64 objectid);
	void removeTerrainModification(uint64 objectid);

	ProceduralTerrainAppearance* getProceduralTerrainAppearance();

	float getCachedHeight(float x, float y);
	float getUnCachedHeight(float x, float y) const;

	virtual float getHeight(float x, float y);

	float getMin() const {
		if (terrainData) {
			return terrainData->getSize() / 2 * -1;
		} else {
			return -256;
		}
	}

	float getMax() const {
		if (terrainData) {
			return terrainData->getSize() / 2;
		} else {
			return 256;
		}
	}

	float getSize() const {
		return terrainData->getSize();
	}

	int getCacheHitCount() const {
		return heightCache->getHitCount();
	}

	int getCacheMissCount() const {
		return heightCache->getMissCount();
	}

	int getCacheClearCount() const {
		return heightCache->getClearCount();
	}

	int getCacheClearHeightsCount() const {
		return heightCache->getClearHeightsCount();
	}

	int getCachedValuesCount() const {
		return heightCache->getSize();
	}

	int getCacheEvictCount() const {
		return heightCache->getEvictCount();
	}
};

#ifdef COMPILE_CORE3_TESTS
class MockTerrainManager : public TerrainManager {
public:
	MOCK_METHOD2(getHeight,float(float x, float y));
};
#endif

#endif /* TERRAINMANAGER_H_ */
