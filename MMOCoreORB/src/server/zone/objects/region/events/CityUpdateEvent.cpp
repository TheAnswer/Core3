/*
 * CityUpdateEvent.cpp
 *
 *  Created on: Feb 23, 2012
 *      Author: xyborn
 */

#include "server/zone/objects/region/NewCityRegion.h"
#include "CityUpdateEvent.h"
#include "server/zone/managers/city/CityManager.h"
#include "server/zone/ZoneServer.h"

CityUpdateEvent::CityUpdateEvent(NewCityRegion* city, ZoneServer* zserv) : Task() {
	cityRegion = city;
	zoneServer = zserv;

	setCustomTaskQueue("slowQueue");
}

void CityUpdateEvent::run() {
	if (zoneServer == nullptr || zoneServer->isServerShuttingDown())
		return;

	ManagedReference<NewCityRegion*> city = cityRegion.get();

	if (city == nullptr)
		return;

	Locker locker(city);

	if (zoneServer->isServerLoading()) {
		city->rescheduleUpdateEvent(600 + System::random(30 * 60));
		return;
	}

	CityManager* cityManager = zoneServer->getCityManager();
	cityManager->processCityUpdate(city);
}
