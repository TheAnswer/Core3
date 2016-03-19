/*
 * SecurityTerminalMenuComponent.cpp
 *
 *  Created on: Oct 31, 2012
 *      Author: root
 */

#include "SecurityTerminalMenuComponent.h"
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
#include "server/zone/objects/player/sessions/SlicingSession.h"
#include "server/zone/objects/tangible/tool/smuggler/PrecisionLaserKnife.h"

void SecurityTerminalMenuComponent::fillObjectMenuResponse(SceneObject* sceneObject, ObjectMenuResponse* menuResponse, CreatureObject* player) const {

	ManagedReference<BuildingObject*> building = cast<BuildingObject*>(sceneObject->getParentRecursively(SceneObjectType::FACTIONBUILDING).get().get());

	if (building == NULL || player->isDead() || player->isIncapacitated())
			return;

	Zone* zone = building->getZone();

	if (zone == NULL)
		return;

	GCWManager* gcwMan = zone->getGCWManager();

	if (gcwMan == NULL)
		return;

	if (!gcwMan->isBaseVulnerable(building))
		return;

	if (gcwMan->isTerminalDamaged(cast<TangibleObject*>(sceneObject)))
		menuResponse->addRadialMenuItem(20, 3, "@ui:repair");
	else
		menuResponse->addRadialMenuItem(20, 3, "@hq:mnu_slice"); // Slice Security
}

int SecurityTerminalMenuComponent::handleObjectMenuSelect(SceneObject* sceneObject, CreatureObject* player, byte selectedID) const {
	if (sceneObject == NULL || !sceneObject->isTangibleObject() || player == NULL || player->isDead() || player->isIncapacitated() || selectedID != 20)
		return 1;

	ManagedReference<BuildingObject*> building = cast<BuildingObject*>(sceneObject->getParentRecursively(SceneObjectType::FACTIONBUILDING).get().get());
	ManagedReference<TangibleObject*> securityTerminal = cast<TangibleObject*>(sceneObject);

	if (building == NULL)
		return 1;

	Zone* zone = building->getZone();

	if (zone == NULL)
		return 1;

	ManagedReference<GCWManager*> gcwMan = zone->getGCWManager();

	if (gcwMan == NULL)
		return 1;

	if (!gcwMan->isBaseVulnerable(building))
		return 1;


	if (!gcwMan->canStartSlice(player, securityTerminal))
		return 1;

	if (gcwMan->isTerminalDamaged(securityTerminal)) {
		EXECUTE_TASK_3(player, gcwMan, securityTerminal, {
				gcwMan_p->repairTerminal(player_p, securityTerminal_p);
		});

	} else {
		if (player->containsActiveSession(SessionFacadeType::SLICING)) {
			player->sendSystemMessage("@slicing/slicing:already_slicing");
			return 1;
		}

		ManagedReference<SlicingSession*> session = new SlicingSession(player);
		session->setBaseSlice(true);
		session->initalizeSlicingMenu(player, securityTerminal);
	}

	return 0;
}
