/*
 * NAVMESHJOB.h
 *
 *  Created on: 10/01/2016
 *      Author: gslomin
 */

#ifndef NAVMESHJOB_H_
#define NAVMESHJOB_H_
#include <functional>
#include "engine/engine.h"
#include "engine/core/LambdaFunction.h"
#include "server/zone/objects/pathfinding/NavMeshRegion.h"
#include "pathfinding/RecastNavMesh.h"
#include "server/zone/Zone.h"
#include "engine/util/u3d/AABB.h"


class NavMeshJob : public Object {
protected:
	NavMeshJob(const NavMeshJob& rhs) : Object(rhs), queue(rhs.queue) {
		zone = rhs.zone;
		region = rhs.region;
	}
	Reference<NavMeshRegion*> region;
	WeakReference<Zone*> zone;
	Vector<AABB> areas;
	RecastSettings settings;
	const String& queue;
	//Vector<LambdaFunction<std::function<void(NavmeshRegion*, const Vector<AABB>*)> > > functions;
	Mutex mutex;
public:

	Vector<AABB>& getAreas() {
		return areas;
	}

	Reference<Zone*> getZone() {
		return zone.get();
	}

	NavMeshRegion* getRegion() {
		return region;
	}

	RecastSettings& getRecastConfig() {
		return settings;
	}
	NavMeshJob(NavMeshRegion *region, Zone* zone, const RecastSettings& config, const String& targetQueue) : queue(targetQueue)  {
		this->zone = zone;
		this->region = region;
		settings = config;
	}

	Mutex* getMutex() {
		return &mutex;
	}

	const String& getQueue() {
	    return queue;
	}

	void addArea(const AABB& area);

};
#endif
