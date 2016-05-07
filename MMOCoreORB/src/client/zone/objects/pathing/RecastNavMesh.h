/*
 * RecastNavMesh.h
 *
 *  Created on: April 29, 2016
 *      Author: gslomin
 */

#ifndef RECASTNAVMESH_H_
#define RECASTNAVMESH_H_

#include "engine/engine.h"
class dtNavMesh;

class RecastNavMesh : public Object, Logger {
	void loadAll(String filename);
	dtNavMesh *navMesh;
	
public:
	RecastNavMesh(String filename);
};


#endif /* RECASTNAVMESH_H_ */
