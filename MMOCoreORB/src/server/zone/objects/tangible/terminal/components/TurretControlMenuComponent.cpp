/*
 * TurretControlMenuComponent.cpp
 *
 *  Created on: Dec 17, 2012
 *      Author: root
 */


#include "TurretControlMenuComponent.h"
#include "server/zone/Zone.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/packets/object/ObjectMenuResponse.h"
#include "server/zone/objects/scene/SceneObject.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/player/FactionStatus.h"
#include "server/zone/objects/structure/StructureObject.h"
#include "server/zone/objects/building/BuildingObject.h"
#include "server/zone/managers/gcw/GCWManager.h"
#include "server/zone/objects/tangible/TangibleObject.h"
#include "TurretControlTerminalDataComponent.h"


void TurretControlMenuComponent::fillObjectMenuResponse(SceneObject* sceneObject, ObjectMenuResponse* menuResponse, CreatureObject* player) {

	ManagedReference<BuildingObject*> building = cast<BuildingObject*>(sceneObject->getParentRecursively(SceneObjectType::FACTIONBUILDING).get().get());
	ManagedReference<PlayerObject*> thisPlayer = player->getPlayerObject();


	if (thisPlayer == NULL || building == NULL || player->isDead() || player->isIncapacitated())
		return;


	if (player->getFaction() == 0) {
		player->sendSystemMessage("@faction_recruiter:must_be_declared_use"); // Your faction affiliation must be delcared in order to use that item.
		return;
	}

	Zone* zone = building->getZone();

	if (zone == NULL)
		return;

	GCWManager* gcwMan = zone->getGCWManager();

	if (gcwMan == NULL)
		return;

	if (!gcwMan->canUseTerminals(player, building, sceneObject))
		return;

	if (building->getFaction() == player->getFaction()) {
		menuResponse->addRadialMenuItem(222, 3, "@hq:mnu_turret_control"); // "Turret Control"
	}

	//menuResponse->addRadialMenuItem(222, 3, "OH YEA");
}

int TurretControlMenuComponent::handleObjectMenuSelect(SceneObject* sceneObject, CreatureObject* player, byte selectedID) {
	if (sceneObject == NULL || !sceneObject->isTangibleObject() || player == NULL)
		return 0;

	ManagedReference<PlayerObject*> ghost = player->getPlayerObject();
	if (ghost == NULL)
		return 1;

	ManagedReference<BuildingObject*> building = cast<BuildingObject*>(sceneObject->getParentRecursively(SceneObjectType::FACTIONBUILDING).get().get());

	if (building == NULL)
		return 1;

	Zone* zone = building->getZone();

	if (zone == NULL)
		return 1;

	ManagedReference<GCWManager*> gcwMan = zone->getGCWManager();

	if (gcwMan == NULL)
		return 1;

	if (!gcwMan->canUseTerminals(player, building, sceneObject))
		return 1;

	if (selectedID == 222) {
		gcwMan->sendTurretAttackListTo(player,sceneObject);
	}

	return 0;
}




