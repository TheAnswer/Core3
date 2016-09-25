/*
 * RecastNavMesh.cpp
 *
 *  Created on: April 29, 2016
 *      Author: gslomin
 */

#include "RecastNavMesh.h"
#include "pathfinding/recast/Recast.h"

#include "pathfinding/recast/DetourNavMesh.h"
#include "pathfinding/recast/DetourNavMeshBuilder.h"



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

RecastNavMesh::RecastNavMesh(String filename) : Logger("RecastNavMesh") {
	navMesh = NULL;
}

void RecastNavMesh::loadAll(String filename)
{
	File file(filename);
	
	if (!file.exists())
		return;
	
	FileInputStream stream(&file);
	
	// Read header.
	NavMeshSetHeader header;
	int size = sizeof(NavMeshSetHeader);
	
	if(stream.read((byte*)&header, size) != size) {
		error("Error reading RecastNavMesh " + filename);
		file.close();
		return;
	}
	
	if (header.magic != NAVMESHSET_MAGIC)
	{
		error("Attempting to read invalid RecastNavMesh " + filename);
		file.close();
		return;
	}
	if (header.version != NAVMESHSET_VERSION)
	{
		error("RecastNavMesh header version is out of date");
		file.close();
		return;
	}
	
	dtNavMesh* mesh = dtAllocNavMesh();
	if (!mesh)
	{
		error("Failed to allocate RecastNavMesh " + filename);
		file.close();
		return;
	}
	dtStatus status = mesh->init(&header.params);
	if (dtStatusFailed(status))
	{
		error("Invalid Detour status for RecastNavMesh " + filename);
		file.close();
		return;
	}
	
	// Read tiles.
	for (int i = 0; i < header.numTiles; ++i)
	{
		NavMeshTileHeader tileHeader;
		int headerSize = sizeof(tileHeader);
		
		if (stream.read((byte*)&tileHeader, headerSize) != headerSize)
		{
			error("Failed to read tileHeader for tile :" + String::valueOf(i) + " in RecastNavMesh " + filename);
			file.close();
			return;
		}
		
		if (!tileHeader.tileRef || !tileHeader.dataSize) {
			error("Invalid tileHeader tileRef or dataSize in " + filename);
			file.close();
			return;
		}
		
		byte* data = (byte*)dtAlloc(tileHeader.dataSize, DT_ALLOC_PERM);
		if (!data) {
			error("Failed to buffer for tile in RecastNavMesh " + filename);
			file.close();
			return;
		}
		
		memset(data, 0, tileHeader.dataSize);
		if (stream.read(data, tileHeader.dataSize) != tileHeader.dataSize)
		{
			error("Failed to read tileData:" + String::valueOf(i) + " in RecastNavMesh " + filename);
			file.close();
			return;
		}
		
		mesh->addTile(data, tileHeader.dataSize, DT_TILE_FREE_DATA, tileHeader.tileRef, 0);
	}
	
	file.close();
	
	navMesh = mesh;
}
