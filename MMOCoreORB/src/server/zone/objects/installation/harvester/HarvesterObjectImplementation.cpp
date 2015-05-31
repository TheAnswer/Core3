/*
 * HarvesterObjectImplementation.cpp
 *
 *  Created on: 10/06/2010
 *      Author: victor
 */

#include "server/zone/objects/installation/harvester/HarvesterObject.h"
#include "server/zone/packets/harvester/HarvesterObjectMessage7.h"
#include "server/zone/packets/installation/InstallationObjectDeltaMessage7.h"
#include "server/zone/objects/resource/ResourceSpawn.h"
#include "server/zone/objects/resource/ResourceContainer.h"
#include "server/zone/Zone.h"
#include "server/zone/packets/object/ObjectMenuResponse.h"
#include "server/zone/packets/harvester/ResourceHarvesterActivatePageMessage.h"
#include "server/zone/managers/resource/ResourceManager.h"
#include "server/zone/objects/area/ActiveArea.h"
#include "server/zone/templates/tangible/SharedStructureObjectTemplate.h"

void HarvesterObjectImplementation::notifyLoadFromDatabase() {
	InstallationObjectImplementation::notifyLoadFromDatabase();

	Reference<SharedStructureObjectTemplate*> ssot = cast<SharedStructureObjectTemplate*>(templateObject.get());

	//Fix for the errored harvesters.
	if (ssot != NULL)
		setBasePowerRate(ssot->getBasePowerRate());
}

void HarvesterObjectImplementation::fillObjectMenuResponse(ObjectMenuResponse* menuResponse, CreatureObject* player) {
	if (!isOnAdminList(player))
		return;

	InstallationObjectImplementation::fillObjectMenuResponse(menuResponse, player);

	menuResponse->addRadialMenuItemToRadialID(118, 78, 3, "@harvester:manage"); //Operate Machinery
}

void HarvesterObjectImplementation::synchronizedUIListen(SceneObject* player, int value) {
	if (!player->isPlayerCreature() || !isOnAdminList(cast<CreatureObject*>(player)) || getZone() == NULL)
		return;

	addOperator(cast<CreatureObject*>(player));

	updateInstallationWork();

	HarvesterObjectMessage7* msg = new HarvesterObjectMessage7(_this.getReferenceUnsafeStaticCast());
	player->sendMessage(msg);

	/// Have to send the spawns of items no in shift, or the dont show
	/// up in the hopper when you look.
	for (int i = 0; i < resourceHopper.size(); ++i) {
		ResourceContainer* container = resourceHopper.get(i);

		if (container != NULL) {
			container->sendTo(cast<CreatureObject*>(player), true);
		}
	}

	activateUiSync();
}

void HarvesterObjectImplementation::updateOperators() {
	HarvesterObjectMessage7* msg = new HarvesterObjectMessage7(_this.getReferenceUnsafeStaticCast());
	broadcastToOperators(msg);
}

void HarvesterObjectImplementation::synchronizedUIStopListen(SceneObject* player, int value) {
	if (!player->isPlayerCreature())
		return;

	removeOperator(cast<CreatureObject*>(player));
}

int HarvesterObjectImplementation::handleObjectMenuSelect(CreatureObject* player, byte selectedID) {
	if (!isOnAdminList(player))
		return 1;

	switch (selectedID) {
	case 78: {
		ResourceHarvesterActivatePageMessage* rhapm = new ResourceHarvesterActivatePageMessage(getObjectID());
		player->sendMessage(rhapm);
		break;
	}
	case 77:
		handleStructureAddEnergy(player);
		break;

	default:
		return InstallationObjectImplementation::handleObjectMenuSelect(player, selectedID);
	}

	return 0;
}

String HarvesterObjectImplementation::getRedeedMessage() {

	if(operating)
		return "destroy_deactivate_first";

	if(getHopperSize() > 0)
		return "destroy_empty_hopper";


	return "";
}
