/*
 * ZoneLoadManagersTask.h
 *
 *  Created on: Nov 18, 2010
 *      Author: oru
 */

#ifndef ZONELOADMANAGERSTASK_H_
#define ZONELOADMANAGERSTASK_H_

#include "engine/engine.h"

#include "server/zone/Zone.h"

class ZoneLoadManagersTask : public Task {
	ManagedReference<ZoneServer*> zoneServer;
	ManagedReference<Zone*> zone;
public:
	ZoneLoadManagersTask(ZoneServer* server, Zone* zone) {
		this->zone = zone;
		zoneServer = server;
	}

	void run() {
		if (zone == nullptr)
			return;

		if (zone->hasManagersStarted())
			return;

		zone->startManagers();
	}
};


#endif /* ZONELOADMANAGERSTASK_H_ */
