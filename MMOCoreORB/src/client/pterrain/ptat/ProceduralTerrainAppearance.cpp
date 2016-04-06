/*
 * ProceduralTerrainAppearance.cpp
 *
 *  Created on: 31/01/2010
 *      Author: victor
 */

#include "ProceduralTerrainAppearance.h"
#include "TerrainGenerator.h"
#include "TerrainMaps.h"
#include "layer/boundaries/Boundary.h"
#include "PerlinNoise.h"
#include "TerrainChunk.h"

ProceduralTerrainAppearance::ProceduralTerrainAppearance(TerrainGenerator* terrainGenerator) : Logger("ProceduralTerrainAppearance") {
	if (terrainGenerator == NULL)
		terrainGenerator = new TerrainGenerator(this);

	this->terrainGenerator = terrainGenerator;
	terrainMaps = new TerrainMaps();

	useGlobalWaterTable = 0;
}

ProceduralTerrainAppearance::~ProceduralTerrainAppearance() {
	delete terrainGenerator;
	terrainGenerator = NULL;

	delete terrainMaps;
	terrainMaps = NULL;
}

bool ProceduralTerrainAppearance::load(IffStream* iffStream) {
	waterBoundaries.removeAll();

	delete terrainGenerator;
	delete terrainMaps;

	terrainGenerator = new TerrainGenerator(this);
	terrainMaps = new TerrainMaps();

	readObject(iffStream);

	terrainGenerator->processLayers();

	return true;
}

void ProceduralTerrainAppearance::parseFromIffStream(engine::util::IffStream* iffStream) {
	uint32 version = iffStream->getNextFormType();

	iffStream->openForm(version);

	switch (version) {
	case '0014':
		parseFromIffStream(iffStream, Version<'0014'>());
		break;
	default:
		error("unknown version in ProceduralTerrainAppearance::parseFromIffStream " + String::valueOf(version));
		break;
	}

	iffStream->closeForm(version);
}

void ProceduralTerrainAppearance::parseFromIffStream(engine::util::IffStream* iffStream, Version<'0014'>) {
	iffStream->openChunk('DATA');

	iffStream->getString(terrainFile);

	//info(terrainFile);

	size = iffStream->getFloat();
	chunkSize = iffStream->getFloat();
	tilesPerChunk = iffStream->getUnsignedInt();
	useGlobalWaterTable = iffStream->getUnsignedInt();
	globalWaterTableHeight = iffStream->getFloat();
	globalWaterTableShaderSize = iffStream->getFloat();

	iffStream->getString(globalWaterTableShader);

	timeCycle = iffStream->getFloat(); // i have docs on how this works somewhere..

	floraCollidableMinDistance = iffStream->getUnsignedInt();
	floraCollidableMaxDistance = iffStream->getFloat();
	floraCollidableTileSize = iffStream->getFloat();
	floraCollidableTileBorder = iffStream->getFloat();
	floraCollidableSeed = iffStream->getUnsignedInt();

	floraNonCollidableMinDistance = iffStream->getUnsignedInt();
	floraNonCollidableMaxDistance = iffStream->getFloat();
	floraNonCollidableTileSize = iffStream->getFloat();
	floraNonCollidableTileBorder = iffStream->getFloat();
	floraNonCollidableSeed = iffStream->getUnsignedInt();

	radialNearMinDistance = iffStream->getUnsignedInt();
	radialNearMaxDistance = iffStream->getFloat();
	radialNearTileSize = iffStream->getFloat();
	radialNearTileBorder = iffStream->getFloat();
	radialNearSeed = iffStream->getUnsignedInt();

	radialFarMinDistance = iffStream->getFloat();
	radialFarMaxDistance = iffStream->getFloat();
	radialFarTileSize = iffStream->getFloat();
	radialFarTileBorder = iffStream->getFloat();
	radialFarSeed = iffStream->getUnsignedInt();

	iffStream->closeForm('DATA');

	terrainGenerator->readObject(iffStream);

	terrainMaps->readObject(iffStream);
}

bool ProceduralTerrainAppearance::getWater(float x, float y, float& waterHeight) {
	for (int i = 0; i < waterBoundaries.size(); ++i) {
		Boundary* boundary = waterBoundaries.get(i);

		if (boundary->containsPoint(x, y)) {
			waterHeight = boundary->getLocalWaterTableHeight();
			return true;
		}
	}

	if (useGlobalWaterTable != 0) {
		waterHeight = globalWaterTableHeight;
		return true;
	}

	return false;
}

Layer* ProceduralTerrainAppearance::getLayerRecursive(float x, float y, Layer* rootParent) {
	Layer* returnLayer = NULL;

	Vector<Boundary*>* boundaries = rootParent->getBoundaries();

	for (int i = 0; i < boundaries->size(); ++i) {
		float result = boundaries->get(i)->process(x, y);

		if (result != 0)
			return rootParent;
	}

	Vector<Layer*>* children = rootParent->getChildren();

	for (int i = 0; i < children->size(); ++i) {
		Layer* layer = children->get(i);

		returnLayer = getLayerRecursive(x, y, layer);

		if (returnLayer != NULL)
			return returnLayer;
	}

	return returnLayer;
}

Layer* ProceduralTerrainAppearance::getLayer(float x, float y) {
	Layer* returnLayer = NULL;

	LayersGroup* layersGroup = terrainGenerator->getLayersGroup();

	Vector<Layer*>* layers = layersGroup->getLayers();

	for (int i = 0; i < layers->size(); ++i) {
		Layer* layer = layers->get(i);

		if (!layer->isEnabled())
			continue;

		returnLayer = getLayerRecursive(x, y, layer);

		if (returnLayer != NULL)
			return returnLayer;
		//Vector<TerrainRule*>* rules = layer->getRules();
	}

	return returnLayer;
}

float ProceduralTerrainAppearance::calculateFeathering(float value, int featheringType) {
	/* 1: x^2
	 * 2: sqrt(x)
	 * 3: x^2 * (3 - 2x)
	 */


	float result = value;

	switch (featheringType) {
	case 1:
		result = result * result;
		break;
	case 2:
		result = sqrt(result);
		break;
	case 3:
		result = result * result * (3 - 2 * result);
		break;
	case 0:
		//result = result;
		break;
	default:
		result = 0;
		break;
	}

	return result;
}

Vector<TerrainChunk*>* ProceduralTerrainAppearance::generateTerrainChunks(float minX, float minY, float size, float distanceBetweenHeights, int oneChunkNumRows, int oneChunkNumColumns, float chunkSize) {
	Vector<TerrainChunk*>* chunks = new Vector<TerrainChunk*>();

	//for (float y = minY; y < maxY; 

	LayersGroup* layersGroup = terrainGenerator->getLayersGroup();

	Vector<Layer*>* layers = layersGroup->getLayers();

	int numRows = size / chunkSize;
	int numColumns = numRows;

	float originX = minX;
	float originY = minY;

	for (int row = 0; row < numRows; ++row) {
        float currentY = originY + (chunkSize * row);

		for (int col = 0; col < numColumns; ++col) {
            float currentX = originX + ( chunkSize * col );

			TerrainChunk* chunk = new TerrainChunk(currentX, currentY, oneChunkNumRows, oneChunkNumColumns);

			for (int i = 0; i < oneChunkNumRows; ++i) {
				for (int j = 0; j < oneChunkNumColumns; ++j) {
					float workX = currentX + (i * distanceBetweenHeights);
					float workY = currentY + (j * distanceBetweenHeights);

					float affectorTransform = 1.0;

					float transformValue = 0;
					float fullTraverse = 0;

					for (int l = 0; l < layers->size(); ++l) {
						Layer* layer = layers->get(l);
	
						if (layer->isEnabled()) {
							transformValue = processTerrain(layer, workX, workY, fullTraverse, affectorTransform, AffectorProceduralRule::ALL, chunk, i, j);

							float inChunkHeight = chunk->getHeight(i, j);

							if (fullTraverse != inChunkHeight) {
								transformValue--;
							}
						}

					}
				}
			}

			chunks->add(chunk);
		}
	}

	return chunks;
}

float ProceduralTerrainAppearance::processBoundaries(Vector<Boundary*>* boundaries, float x, float y) {
	float transformValue = 0;
	
	bool hasBoundaries = false;

	for (int i = 0; i < boundaries->size(); ++i) {
		Boundary* boundary = boundaries->get(i);

		if (!boundary->isEnabled()) {
			continue;
		} else
			hasBoundaries = true;

		float result = boundary->process(x, y);

		int featheringType = boundary->getFeatheringType();

		result = calculateFeathering(result, featheringType);

		if (result > transformValue)
			transformValue = result;

		if (transformValue >= 1) {
			break;
		}
	}

	if (!hasBoundaries)
		transformValue = 1.0;

	return transformValue;
}

float ProceduralTerrainAppearance::processFilters(Vector<FilterProceduralRule*>* filters, float x, float y, float& transformValue, float& baseValue, TerrainChunk* chunk, int row, int column) {
	for (int i = 0; i < filters->size(); ++i) {
		FilterProceduralRule* filter = filters->get(i);

		if (!filter->isEnabled())
			continue;

		/*if (!(filter->getFilterType() & affectorType))
			continue;*/

		float result = filter->process(x, y, transformValue, baseValue, terrainGenerator, chunk, row, column);

		int featheringType = filter->getFeatheringType();

		result = calculateFeathering(result, featheringType);

		if (transformValue > result)
			transformValue = result;

		if (transformValue == 0)
			break;
	}

	return transformValue;
}

float ProceduralTerrainAppearance::processTerrain(Layer* layer, float x, float y, float& baseValue, float affectorTransformValue, int affectorType, TerrainChunk* chunk, int row, int column) {
	Vector<Boundary*>* boundaries = layer->getBoundaries();
	Vector<AffectorProceduralRule*>* affectors = layer->getAffectors();
	Vector<FilterProceduralRule*>* filters = layer->getFilters();

	float transformValue = processBoundaries(boundaries, x, y);

	if (layer->invertBoundaries())
		transformValue = 1.0 - transformValue;

	if (transformValue != 0) {
		processFilters(filters, x, y, transformValue, baseValue, chunk, row, column);

		if (layer->invertFilters())
			transformValue = 1.0 - transformValue;

		if (transformValue != 0) {
			for (int i = 0; i < affectors->size(); ++i) {
				AffectorProceduralRule* affector = affectors->get(i);

				if (affector->isEnabled() && (affector->getAffectorType() & affectorType))
					affector->process(x, y, transformValue * affectorTransformValue, baseValue, terrainGenerator, chunk, row, column);
			}


			Vector<Layer*>* children = layer->getChildren();

			for (int i = 0; i < children->size(); ++i) {
				Layer* layer = children->get(i);

				if (layer->isEnabled()) {
					processTerrain(layer, x, y, baseValue, affectorTransformValue * transformValue, affectorType, chunk, row, column);
				}
			}

		}

	}

	return transformValue;
}

int ProceduralTerrainAppearance::getEnvironmentID(float x, float y) {
	LayersGroup* layersGroup = terrainGenerator->getLayersGroup();

	Vector<Layer*>* layers = layersGroup->getLayers();

	float affectorTransform = 1.0;

	float transformValue = 0;
	float fullTraverse = 0;

	for (int i = 0; i < layers->size(); ++i) {
		Layer* layer = layers->get(i);

		if (layer->isEnabled())
			transformValue = processTerrain(layer, x, y, fullTraverse, affectorTransform, AffectorProceduralRule::ENVIRONMENT, NULL, 0, 0);
	}

	//info("full traverse height ... is " + String::valueOf(fullTraverse) + " in mili:" + String::valueOf(start.miliDifference()), true);

	return fullTraverse;
}

ShaderFamily* ProceduralTerrainAppearance::getShaderFamily(int shaderFamilyId) {
	ShadersGroup* group = terrainGenerator->getShadersGroup();

	Vector<ShaderFamily*>* data = group->getShaderFamilies();

	//String fileName

	for (int i = 0; i < data->size(); ++i) {
		ShaderFamily* fam = data->get(i);

		if (fam->getID() == shaderFamilyId) {
			return fam;
		}
	}

	return NULL;
}

ShaderFamily* ProceduralTerrainAppearance::getShaderFamily(float x, float y) {
	LayersGroup* layersGroup = terrainGenerator->getLayersGroup();

	Vector<Layer*>* layers = layersGroup->getLayers();

	float affectorTransform = 1.0;

	float transformValue = 0;
	float fullTraverse = 0;

	for (int i = 0; i < layers->size(); ++i) {
		Layer* layer = layers->get(i);

		if (layer->isEnabled())
			transformValue = processTerrain(layer, x, y, fullTraverse, affectorTransform, AffectorProceduralRule::SHADER, NULL, 0, 0);
	}

	//info("full traverse height ... is " + String::valueOf(fullTraverse) + " in mili:" + String::valueOf(start.miliDifference()), true);

	return getShaderFamily((int)fullTraverse);
}

float ProceduralTerrainAppearance::getHeight(float x, float y) {
	LayersGroup* layersGroup = terrainGenerator->getLayersGroup();

	Vector<Layer*>* layers = layersGroup->getLayers();

	float affectorTransform = 1.0;

	float transformValue = 0;
	float fullTraverse = 0;

	//Time start;

	for (int i = 0; i < layers->size(); ++i) {
		Layer* layer = layers->get(i);

		if (layer->isEnabled())
			transformValue = processTerrain(layer, x, y, fullTraverse, affectorTransform, AffectorProceduralRule::HEIGHTTYPE, NULL, 0, 0);
	}

	//info("full traverse height ... is " + String::valueOf(fullTraverse) + " in mili:" + String::valueOf(start.miliDifference()), true);

	return fullTraverse;
}
