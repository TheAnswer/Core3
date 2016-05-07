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
#include "recast/DetourNavMesh.h"
#include "recast/Recast.h"
#include "terrain/layer/boundaries/BoundaryPolygon.h"

class MeshData;
class dtNavMeshQuery;
class BoundaryPolygon;



enum SamplePolyAreas
{
	SAMPLE_POLYAREA_GROUND,
	SAMPLE_POLYAREA_WATER,
	SAMPLE_POLYAREA_ROAD,
	SAMPLE_POLYAREA_DOOR,
	SAMPLE_POLYAREA_GRASS,
	SAMPLE_POLYAREA_JUMP,
};
enum SamplePolyFlags
{
	SAMPLE_POLYFLAGS_WALK		= 0x01,		// Ability to walk (ground, grass, road)
	SAMPLE_POLYFLAGS_SWIM		= 0x02,		// Ability to swim (water).
	SAMPLE_POLYFLAGS_DOOR		= 0x04,		// Ability to move through doors.
	SAMPLE_POLYFLAGS_JUMP		= 0x08,		// Ability to jump.
	SAMPLE_POLYFLAGS_DISABLED	= 0x10,		// Disabled polygon
	SAMPLE_POLYFLAGS_ALL		= 0xffff	// All abilities.
};

class RecastPolygon : public Object {
public:
	RecastPolygon(int numVerts) {
		verts = new float[numVerts*3];
		this->numVerts = numVerts;
	}
	~RecastPolygon() {
		delete[] verts;
	}
	RecastPolygon(const RecastPolygon& rhs) {
		*this = rhs;
	}
	
	RecastPolygon& operator=(const RecastPolygon& rhs) {
		verts = new float[rhs.numVerts*3];
		memcpy(verts, rhs.verts, sizeof(float)*rhs.numVerts);
		type = rhs.type;
		numVerts = rhs.numVerts;
		hmin = rhs.hmin;
		hmax = rhs.hmax;
	}
	
	SamplePolyAreas type;
	float *verts;
	int numVerts;
	float hmin, hmax;
};
class RecastNavMeshBuilder : public Logger
{
protected:
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
	dtNavMeshQuery* m_navQuery;
	int m_tileTriCount;
	
	unsigned char* buildTileMesh(const int tx, const int ty, int& dataSize);
	
	void cleanup();
	Vector<Reference<RecastPolygon*> > water;
	
	float waterTableHeight;
public:
	
	void saveAll(const String& file);
	RecastNavMeshBuilder(float waterTableHeight);
	virtual ~RecastNavMeshBuilder();
//	
//	virtual void handleSettings();
//	virtual void handleTools();
//	virtual void handleDebugMode();
//	virtual void handleRender();
//	virtual void handleRenderOverlay(double* proj, double* model, int* view);
	virtual void changeMesh(MeshData* geom);
	inline void addWater(Reference<RecastPolygon*> waterBoundary) {
		water.add(waterBoundary);
	}
	virtual bool build();
	//virtual void collectSettings(struct BuildSettings& settings);
	
	void getTilePos(const Vector3& pos, int& tx, int& ty);
	
	void buildTile(const Vector3& pos);
	void removeTile(const Vector3& pos);
	
	void removeAllTiles();
	
	void buildAllTiles();
	
private:
	// Explicitly disabled copy constructor and copy assignment operator.
	RecastNavMeshBuilder(const RecastNavMeshBuilder&);
	RecastNavMeshBuilder& operator=(const RecastNavMeshBuilder&);
};


#endif // RECASTSAMPLETILEMESH_H
