//
// Copyright (c) 2009-2010 Mikko Mononen memon@inside.org
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.
//

#ifndef RECASTSAMPLETILEMESH_H
#define RECASTSAMPLETILEMESH_H
#include "engine/engine.h"
#include "pathfinding/recast/DetourNavMesh.h"
#include "pathfinding/recast/Recast.h"
#include "terrain/layer/boundaries/BoundaryPolygon.h"
#include "RecastPolygon.h"
#include "server/zone/Zone.h"


class MeshData;
class dtNavMeshQuery;
class BoundaryPolygon;
class TerrainManager;

class RecastNavMeshBuilder : public Logger
{
protected:
	String name;
	float chunkSize;
	float distanceBetweenHeights;
	bool m_keepInterResults;
	bool m_buildAll;
	float m_totalBuildTimeMs;
	float m_cellHeight;
	float m_agentHeight;
	float m_agentRadius;
	float m_agentMaxClimb;
	float m_agentMaxSlope;
	float m_regionMinSize;
	float m_regionMergeSize;
	float m_edgeMaxLen;
	float m_edgeMaxError;
	float m_vertsPerPoly;
	float m_detailSampleDist;
	float m_detailSampleMaxError;
	int m_partitionType;
	rcContext* m_ctx;
	
	unsigned char* m_triareas;
	rcHeightfield* m_solid;
	rcCompactHeightfield* m_chf;
	rcContourSet* m_cset;
	rcPolyMesh* m_pmesh;
	rcPolyMeshDetail* m_dmesh;
	rcConfig m_cfg;
	dtNavMesh *m_navMesh;
	
	int m_maxTiles;
	int m_maxPolysPerTile;
	float m_tileSize;
	float m_cellSize;
	Reference<MeshData*> m_geom;
	AABB bounds;
	AABB lastTileBounds;
	AABB m_meshBounds;
	int m_tileTriCount;
	Zone *m_currentZone;
	Vector<Reference<MeshData*> > m_initialMeshData;
	unsigned char* buildTileMesh(const int tx, const int ty, int& dataSize);
	
	void cleanup();
	Vector<Reference<RecastPolygon*> > water;
	
	float waterTableHeight;
public:
	
	void setTileSize(float tileSize) {
		m_tileSize = tileSize;
	}
	
	void setChunkSize(float newChunkSize) {
		chunkSize = newChunkSize;
	}
	
	void setDistanceBetweenHeights(float newDistBetweenHeights) {
		distanceBetweenHeights = newDistBetweenHeights;
	}
	
	void setCellHeight(float height) {
		m_cellHeight = height;
	}
	
	void setCellSize(float size) {
		m_cellSize = size;
	}
	
	void setMaxError(float maxError) {
		m_detailSampleMaxError = maxError;
	}
	
	void initialize(AABB bounds);
	void saveAll(const String& file);
	RecastNavMeshBuilder(AABB meshBounds, Vector<Reference<MeshData*> > meshData, Zone* zone, String name);
	RecastNavMeshBuilder(AABB meshBounds, Vector<Reference<MeshData*> > meshData, Zone* zone, String name, dtNavMesh *mesh);
	virtual ~RecastNavMeshBuilder();

	virtual void changeMesh(MeshData* geom);
	inline void addWater(Reference<RecastPolygon*> waterBoundary) {
		water.add(waterBoundary);
	}
	virtual bool build(AABB bounds);
	//virtual void collectSettings(struct BuildSettings& settings);
	
	void getTilePos(const Vector3& pos, int& tx, int& ty);
	
	void buildTile(const Vector3& pos);
	void removeTile(const Vector3& pos);
	
	void removeAllTiles();
	
	void buildAllTiles(AABB area);
	
	static Reference<MeshData*> getTerrainMesh(Vector3& position, float terrainSize, TerrainManager* terrainManager, float chunkSize = 32, float distanceBetweenHeights=2);
	
	static Reference<MeshData*> flattenMeshData(Vector<Reference<MeshData*> >& data);
	
	static void dumpOBJ(String filename, Vector<Reference<MeshData*> > data);
	
private:
	// Explicitly disabled copy constructor and copy assignment operator.
	RecastNavMeshBuilder(const RecastNavMeshBuilder&);
	RecastNavMeshBuilder& operator=(const RecastNavMeshBuilder&);
};


#endif // RECASTSAMPLETILEMESH_H
