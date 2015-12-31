/*
 * HQMenuComponent.cpp
 *
 *  Created on: Oct 25, 2012
 *      Author: root
 */
#include "HQMenuComponent.h"
#include "server/zone/Zone.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/objects/scene/components/ObjectMenuComponent.h"
#include "server/zone/packets/object/ObjectMenuResponse.h"

#include "server/zone/objects/scene/SceneObjectType.h"
#include "server/zone/objects/scene/SceneObject.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/creature/CreatureFlag.h"
#include "server/zone/objects/player/FactionStatus.h"
#include "server/zone/objects/structure/StructureObject.h"

#include "server/zone/objects/building/components/DestructibleBuildingDataComponent.h"
#include "server/zone/objects/building/BuildingObject.h"
#include "server/chat/StringIdChatParameter.h"


#include "server/zone/objects/player/sui/messagebox/SuiMessageBox.h"
#include "server/zone/objects/player/sui/transferbox/SuiTransferBox.h"
#include "server/zone/objects/player/sui/inputbox/SuiInputBox.h"

#include "server/zone/managers/gcw/GCWManager.h"
#include "server/zone/managers/gcw/tasks/ShutdownSequenceTask.h"

void HQMenuComponent::fillObjectMenuResponse(SceneObject* sceneObject, ObjectMenuResponse* menuResponse, CreatureObject* player) {
	ManagedReference<BuildingObject*> building = cast<BuildingObject*>(sceneObject->getParentRecursively(SceneObjectType::FACTIONBUILDING).get().get());

	if (building == NULL)
		return;

	if ( player  == NULL || player->isDead() || player->isIncapacitated())
		return;

	Zone* zone = building->getZone();

	if (zone == NULL)
		return;

	GCWManager* gcwMan = zone->getGCWManager();

	if (gcwMan == NULL)
		return;

	if (!gcwMan->canUseTerminals(player, building, sceneObject))
		return;

	if (building->getFaction() == player->getFaction()) {

		menuResponse->addRadialMenuItem(210, 3, "@player_structure:management");

		menuResponse->addRadialMenuItemToRadialID(210, 227, 3, "@player_structure:management_status");

		if (building->getOwnerCreatureObject() == player) {
			menuResponse->addRadialMenuItemToRadialID(210, 228, 3, "@hq:mnu_defense_status");

			if ((building->getPvpStatusBitmask() & CreatureFlag::OVERT) && !gcwMan->isBaseVulnerable(building)) {
				if (gcwMan->hasResetTimerPast(building))
					menuResponse->addRadialMenuItemToRadialID(210, 235, 3, "@hq:mnu_reset_vulnerability"); // Reset Vulnerability
				menuResponse->addRadialMenuItemToRadialID(210, 236, 3, "@player_structure:permission_destroy");
			}
		}

		if (gcwMan->isShutdownSequenceStarted(building) && (building->getPvpStatusBitmask() & CreatureFlag::OVERT))
			menuResponse->addRadialMenuItem(231, 3, "@hq:mnu_shutdown");  // Shutdown facility

		menuResponse->addRadialMenuItem(37, 3, "@hq:mnu_donate"); // Donate
		menuResponse->addRadialMenuItemToRadialID(37, 226, 3, "@hq:mnu_donate_deed"); // Donate Defense
	} else {
		if (gcwMan->isPowerOverloaded(building) && !gcwMan->isShutdownSequenceStarted(building))
			menuResponse->addRadialMenuItem(230, 3, "@hq:mnu_overload");  // Activate Overload
	}
}

int HQMenuComponent::handleObjectMenuSelect(SceneObject* sceneObject, CreatureObject* creature, byte selectedID) {
	if (creature->isDead() || creature->isIncapacitated())
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

	if (!gcwMan->canUseTerminals(creature, building, sceneObject))
		return 1;

	if (creature->getFaction() == building->getFaction()) {
		if (selectedID == 210 || selectedID == 20 || selectedID == 227) {
			creature->executeObjectControllerAction(0x13F7E585, building->getObjectID(), ""); //structureStatus
		} else if (selectedID == 228){
			gcwMan->sendBaseDefenseStatus(creature, building);
		} else if (selectedID == 235) {
			if (building->getOwnerCreatureObject() == creature)
				gcwMan->resetVulnerability(creature, building);
		} else if (selectedID == 236) {
			creature->executeObjectControllerAction(0x18FC1726, building->getObjectID(), ""); //destroyStructure
		} else if (selectedID == 231) {
			ShutdownSequenceTask* task = new ShutdownSequenceTask(gcwMan, building, creature, false);
			task->execute();
		} else if (selectedID == 226)
				gcwMan->sendSelectDeedToDonate(building,creature,0);
	} else {
		if (selectedID == 230) {
			if (creature->hasSkill("outdoors_squadleader_novice")) {
				ShutdownSequenceTask* task = new ShutdownSequenceTask(gcwMan, building, creature, true);
				task->execute();
			} else
				creature->sendSystemMessage(("@faction/faction_hq/faction_hq_response:terminal_response03")); // only an experienced squad leader could expect to coordinate a reactor overload

		} else if (selectedID == 20) {
			creature->executeObjectControllerAction(0x13F7E585, building->getObjectID(), ""); //structureStatus
		}
	}

	return 0;
}

