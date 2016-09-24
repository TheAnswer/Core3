/*
 * RecastNavMesh.h
 *
 *  Created on: April 29, 2016
 *      Author: gslomin
 */

#ifndef RECASTNAVMESH_H_
#define RECASTNAVMESH_H_

#include "engine/engine.h"
#include "pathfinding/recast/DetourNavMesh.h"


static const int NAVMESHSET_MAGIC = 'M'<<24 | 'S'<<16 | 'E'<<8 | 'T'; //'MSET';
static const int NAVMESHSET_VERSION = 1;

struct NavMeshSetHeader
{
	int magic;
	int version;
	int numTiles;
	dtNavMeshParams params;
};

struct NavMeshTileHeader
{
	dtTileRef tileRef;
	int dataSize;
};


class RecastNavMesh : public Object, Logger {
	void loadAll(String filename);
	dtNavMesh *navMesh;
	Vector3 position;
	NavMeshSetHeader header;
public:
	RecastNavMesh(String filename);
	
	void setPosition(Vector3& newPos) {
		position = newPos;
	}
	
	const Vector3& getPosition() {
		return position;
	}
	bool isLoaded() {
		return navMesh != NULL;
	}
	
	const NavMeshSetHeader& getMeshHeader() {
		return header;
	}
	
	dtNavMesh* getNavMesh() {
		return navMesh;
	}
};


#endif /* RECASTNAVMESH_H_ */
