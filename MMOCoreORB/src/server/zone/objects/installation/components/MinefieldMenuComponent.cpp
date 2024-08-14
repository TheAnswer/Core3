/*
 * MinefieldMenuComponent.cpp
 *
 *  Created on: Jan 31, 2013
 *  Author: root
 */

#include "MinefieldMenuComponent.h"
#include "server/zone/packets/object/ObjectMenuResponse.h"
#include "server/zone/Zone.h"
#include "server/zone/objects/scene/SceneObject.h"
#include "server/zone/objects/installation/InstallationObject.h"
#include "templates/params/creature/ObjectFlag.h"
#include "templates/faction/Factions.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/managers/collision/CollisionManager.h"
#include "server/zone/managers/gcw/GCWManager.h"

void MinefieldMenuComponent::fillObjectMenuResponse(SceneObject* sceneObject, ObjectMenuResponse* menuResponse, CreatureObject* player) const {
	if (sceneObject == nullptr || !sceneObject->isMinefield() || sceneObject->getZone() == nullptr) {
		return;
	}

	if (player == nullptr || !player->isPlayerCreature() || player->isDead() || player->isIncapacitated()) {
		return;
	}

	if (player->getFaction() == Factions::FACTIONNEUTRAL) {
		return;
	}

	auto zoneServer = sceneObject->getZoneServer();

	if (zoneServer == nullptr) {
		return;
	}

	ManagedReference<InstallationObject*> minefield = cast<InstallationObject*>(sceneObject);

	if (minefield == nullptr) {
		return;
	}

	// GCW Base Parent
	auto baseParent = zoneServer->getObject(minefield->getOwnerObjectID());

	if (baseParent == nullptr || !baseParent->isBuildingObject()) {
		return;
	}

	auto baseBuilding = cast<BuildingObject*>(baseParent.get());

	if (baseBuilding == nullptr) {
		return;
	}

	auto ghost = player->getPlayerObject();
	bool isPrivileged = (ghost != nullptr && ghost->isPrivileged());

	// Allow privileged access to the mine unless the player is the same faction
	if (!isPrivileged) {
		bool similarStatus = (minefield->getFactionStatus() <= player->getFactionStatus());

		// Player and minefield are opposite faction
		if (player->getFaction() != minefield->getFaction()) {
			// Player and minefield are opposite faction but same faction status
			if (similarStatus) {
				menuResponse->addRadialMenuItem(RadialOptions::SERVER_MENU1, 3, "@player_structure:disarm_minefield"); // "Disarm Minefield"
				return;
			}

			return;
		}

		// Player and minefield are opposite faction status
		if (!similarStatus) {
			return;
		}
	}

	if (baseBuilding->getOwnerObjectID() == player->getObjectID()) {
		menuResponse->addRadialMenuItem(RadialOptions::SERVER_MENU2, 3, "@player_structure:management_mine_inv"); // "Mine Inventory"
	}

	menuResponse->addRadialMenuItem(RadialOptions::SERVER_MENU3, 3, "@player_structure:mnu_donate_mines"); // "Donate Mines"
}

int MinefieldMenuComponent::handleObjectMenuSelect(SceneObject* sceneObject, CreatureObject* player, byte selectedID) const {
	if (sceneObject == nullptr || player == nullptr) {
		return 1;
	}

	auto zone = sceneObject->getZone();

	if (zone == nullptr) {
		return 1;
	}

	auto zoneServer = sceneObject->getZoneServer();

	if (zoneServer == nullptr) {
		return 1;
	}

	ManagedReference<InstallationObject*> minefield = cast<InstallationObject*>(sceneObject);

	if (minefield == nullptr) {
		return 1;
	}

	auto gcwManager = zone->getGCWManager();

	if (gcwManager == nullptr) {
		return 1;
	}

	if (!CollisionManager::checkLineOfSight(player, minefield)) {
		player->sendSystemMessage("@container_error_message:container18"); // "You can't see that object.  You may have to move closer to it."
		return 1;
	} else if (!player->isInRange(minefield, 7.f)) {
		player->sendSystemMessage("@container_error_message:container09"); // "You are out of range."
		return 1;
	}

	auto ghost = player->getPlayerObject();

	if (ghost == nullptr) {
		return 1;
	}

	if (ghost->hasSuiBoxWindowType(SuiWindowType::HQ_TERMINAL)) {
		ghost->closeSuiWindowType(SuiWindowType::HQ_TERMINAL);
	}

	bool isPrivileged = ghost->isPrivileged();

	switch(selectedID) {
		// Disarm Minefield
		case RadialOptions::SERVER_MENU1: {

			break;
		}
		// Manage Mine Inventory
		case RadialOptions::SERVER_MENU2: {
			// GCW Base Parent
			auto baseParent = zoneServer->getObject(minefield->getOwnerObjectID());

			if (baseParent == nullptr || !baseParent->isBuildingObject()) {
				return 1;
			}

			auto baseBuilding = cast<BuildingObject*>(baseParent.get());

			if (baseBuilding == nullptr) {
				return 1;
			}

			if (isPrivileged || (baseBuilding->getOwnerObjectID() == player->getObjectID())) {
				minefield->sendWithoutParentTo(player);
				minefield->openContainerTo(player);
				minefield->notifyObservers(ObserverEventType::OPENCONTAINER, player);
			} else {
				player->sendSystemMessage("@error_message:perm_no_open"); // You do not have permission to access this container
			}

			break;
		}
		// Donate Mines
		case RadialOptions::SERVER_MENU3: {
			gcwManager->sendSelectMineToDonate(minefield, player);
			break;
		}
		default:
			break;
	}

	return 0;
}
