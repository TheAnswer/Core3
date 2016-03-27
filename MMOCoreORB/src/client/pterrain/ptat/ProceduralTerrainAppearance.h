/*
 * ProceduralTerrainTemplate.h
 *
 *  Created on: 31/01/2010
 *      Author: victor
 */

#ifndef PROCEDURALTERRAINAPPEARANCE_H_
#define PROCEDURALTERRAINAPPEARANCE_H_

#include "TemplateVariable.h"

class TerrainGenerator;
class Boundary;
class TerrainMaps;

class Boundary;
class ShaderFamily;
class Layer;
class TerrainChunk;
class FilterProceduralRule;

class ProceduralTerrainAppearance : public TemplateVariable<'PTAT'>, public Logger {
	TerrainGenerator* terrainGenerator;

	Vector<Boundary*> waterBoundaries;

	TerrainMaps* terrainMaps;

	//float defaultHeight;

	String terrainFile;
	float size;
	float chunkSize;
	uint32 tilesPerChunk;

	uint32 useGlobalWaterTable;
	float globalWaterTableHeight;
	float globalWaterTableShaderSize;
	String globalWaterTableShader;

	float timeCycle;

	uint32 floraCollidableMinDistance;
	float floraCollidableMaxDistance;
	float floraCollidableTileSize;
	float floraCollidableTileBorder;
	float floraCollidableSeed;

	uint32 floraNonCollidableMinDistance;
	float floraNonCollidableMaxDistance;
	float floraNonCollidableTileSize;
	float floraNonCollidableTileBorder;
	float floraNonCollidableSeed;

	uint32 radialNearMinDistance;
	float radialNearMaxDistance;
	float radialNearTileSize;
	float radialNearTileBorder;
	float radialNearSeed;

	float radialFarMinDistance;
	float radialFarMaxDistance;
	float radialFarTileSize;
	float radialFarTileBorder;
	float radialFarSeed;

protected:
	float calculateFeathering(float value, int featheringType);
	float processTerrain(Layer* layer, float x, float y, float& baseValue, float affectorTransformValue, int affectorType, TerrainChunk* chunk, int row, int column);
	Layer* getLayerRecursive(float x, float y, Layer* rootParent);
	Layer* getLayer(float x, float y);

	float processBoundaries(Vector<Boundary*>* boundaries, float x, float y);
	float processFilters(Vector<FilterProceduralRule*>* filters, float x, float y, float& transformValue, float& baseValue, TerrainChunk* chunk, int row, int column);


public:
	ProceduralTerrainAppearance(TerrainGenerator* terrainGenerator);
	~ProceduralTerrainAppearance();

	bool load(engine::util::IffStream* iffStream);

	void parseFromIffStream(engine::util::IffStream* iffStream);
	void parseFromIffStream(engine::util::IffStream* iffStream, Version<'0014'>);

	void insertWaterBoundary(Boundary* boundary) {
		waterBoundaries.add(boundary);
	}

	bool hasGlobalWaterTableOption() {
		return useGlobalWaterTable;
	}

	float getGlobalWaterTableHeight() {
		return globalWaterTableHeight;
	}

	float getGlobalWaterTableShaderSize() {
		return globalWaterTableShaderSize;
	}

	String getGlobalWaterShader() {
		return globalWaterTableShader;
	}

	Vector<Boundary*>* getWaterBoundaries() {
		return &waterBoundaries;
	}

	Vector<TerrainChunk*>* generateTerrainChunks(float minX, float minY, float size, float distanceBetweenHeights, int oneChunkNumRows, int oneChunkNumColumns, float chunkSize);

	/**
	 * Returns the size of the terrain.
	 * @return float The size of the terrain.
	 */
	inline float getSize() {
		return size;
	}

	bool getWater(float x, float y, float& waterHeight);
	float getHeight(float x, float y);
	int getEnvironmentID(float x, float y);
	ShaderFamily* getShaderFamily(float x, float y);
	ShaderFamily* getShaderFamily(int shaderFamilyId);


};


#endif /* PROCEDURALTERRAINAPPEARANCE_H_ */
