/*
 * NavMeshManager.h
 *
 *  Created on: 10/01/2016
 *      Author: gslomin
 */

#ifndef NAVMESHMANAGER_H_
#define NAVMESHMANAGER_H_
#include <functional>
#include "engine/engine.h"
#include "engine/core/LambdaFunction.h"
#include "server/zone/objects/pathfinding/NavMeshRegion.h"
#include "pathfinding/RecastNavMesh.h"
#include "server/zone/Zone.h"
#include "engine/util/u3d/AABB.h"
#include "server/zone/managers/collision/NavMeshJob.h"

class NavMeshManager : public Singleton<NavMeshManager>, public Logger, public Object{

protected:
	int maxConcurrentJobs;
	VectorMap<String, Reference<RecastNavMesh*> > navMeshes;
	VectorMap<String, Reference<NavMeshJob*> > jobs;
	VectorMap<String, Reference<NavMeshJob*> > runningJobs;

	Mutex jobQueueMutex;


	void startJob(Reference<NavMeshJob*> job);
    void checkJobs();
public:
	NavMeshManager();

	~NavMeshManager() { }

	void enqueueJob(Zone* zone, NavMeshRegion* region, AABB areaToBuild, const RecastSettings& recastConfig = RecastSettings());

	RecastNavMesh* getNavMesh(String name) {
		return navMeshes.get(name);
	}


    static bool AABBEncompasessAABB(const AABB& lhs, const AABB& rhs);

	//void initialize()
	int createNavMesh(lua_State* L);
	int destroyNavMesh(lua_State* L);


};
#endif
