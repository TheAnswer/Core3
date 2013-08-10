/*
 * CityHallZoneComponent.cpp
 *
 *  Created on: 31/03/2012
 *      Author: victor
 */

#include "CityHallZoneComponent.h"
#include "server/zone/objects/scene/SceneObject.h"
#include "server/zone/objects/region/CityRegion.h"
#include "server/zone/objects/structure/StructureObject.h"
#include "server/zone/managers/city/CityManager.h"
#include "server/zone/ZoneServer.h"

void CityHallZoneComponent::destroyObjectFromWorld(SceneObject* sceneObject, bool sendSelfDestroy) {
	ManagedReference<CityRegion*> cityRegion = sceneObject->getCityRegion();

	if (cityRegion != NULL ) {
		int i;
		for ( i = CityManager::METROPOLIS; i > 0; i--)
			cityRegion->destroyAllStructuresForRank(uint8(i));

		cityRegion->removeAllTerminals();
		cityRegion->removeAllSkillTrainers();
		cityRegion->removeAllDecorations();

		if (cityRegion->getCityHall() == sceneObject) {
			cityRegion->setCityHall(NULL);
		}

		CityManager* cityManager = sceneObject->getZoneServer()->getCityManager();

		cityManager->destroyCity(cityRegion);
	} else {
		info("city region is null sorry",true);
	}

	ZoneComponent::destroyObjectFromWorld(sceneObject, sendSelfDestroy);
}
