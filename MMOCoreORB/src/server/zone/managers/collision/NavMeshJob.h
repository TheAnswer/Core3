/*
 * NAVMESHJOB.h
 *
 *  Created on: 10/01/2016
 *      Author: gslomin
 */

#ifndef NAVMESHJOB_H_
#define NAVMESHJOB_H_

#include "server/zone/objects/pathfinding/NavArea.h"
#include "server/zone/Zone.h"
#include "engine/util/u3d/AABB.h"

class NavMeshJob : public Object {
protected:
	WeakReference<NavArea*> area;
	WeakReference<Zone*> zone;
	Vector<AABB> areas;
	RecastSettings settings;
	const String& queue;
	AtomicBoolean running;
	Mutex mutex;

public:
	NavMeshJob(NavArea *area, Zone* zone, const RecastSettings& config, const String& targetQueue) : queue(targetQueue), running(true)  {
		this->zone = zone;
		this->area = area;
		settings = config;
	}

	Vector<AABB>& getAreas() {
		return areas;
	}

	Reference<Zone*> getZone() {
		return zone.get();
	}

	Reference<NavArea*> getNavArea() {
		return area.get();
	}

	RecastSettings& getRecastConfig() {
		return settings;
	}

	Mutex* getMutex() {
		return &mutex;
	}

	const String& getQueue() {
	    return queue;
	}

	void cancel() {
		running.set(false);
	}

	const AtomicBoolean* getJobStatus() {
		return &running;
	}

	void addArea(const AABB& area);

};
#endif
