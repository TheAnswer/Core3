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
	Vector3 position;
	
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
};


#endif /* RECASTNAVMESH_H_ */
