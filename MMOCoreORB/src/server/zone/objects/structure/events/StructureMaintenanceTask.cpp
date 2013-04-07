/*
 * StructureMaintenanceTask.cpp
 *
 *  Created on: Aug 1, 2012
 *      Author: swgemu
 */

#include "StructureMaintenanceTask.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/managers/structure/StructureManager.h"
#include "server/chat/ChatManager.h"
#include "server/zone/objects/building/BuildingObject.h"
#include "server/zone/ZoneServer.h"

void StructureMaintenanceTask::run() {
	ManagedReference<StructureObject*> strongRef = structureObject.get();

	if (strongRef == NULL)
		return;

	ZoneServer* zoneServer = strongRef->getZoneServer();

	if (zoneServer != NULL && zoneServer->isServerLoading()) {
		schedule(1000);

		return;
	}

	if (strongRef->getSurplusMaintenance() > 0) {
		//Incorrect scheduling, reschedule.
		Locker locker(strongRef);

		strongRef->scheduleMaintenanceExpirationEvent();
		return;
	}

	//Structure is out of maintenance. Start the decaying process...
	strongRef->updateStructureStatus();

	ManagedReference<CreatureObject*> owner = strongRef->getOwnerCreatureObject();

	if (owner == NULL || !owner->isPlayerCreature())
		return;

	ManagedReference<PlayerObject*> ghost = owner->getPlayerObject();

	//Calculate one week of maintenance +- any existing maintenance/decay.
	int oneWeekMaintenance = 7 * 24 * strongRef->getMaintenanceRate() - strongRef->getSurplusMaintenance();

	Locker _ownerLock(owner);
	Locker _lock(strongRef, owner);

	//Structure not decaying.
	if (!strongRef->isDecaying()) {
		//Check if owner got money in the bank
		if (owner->getBankCredits() >= oneWeekMaintenance) {
		//Withdraw 1 week maintenance from owner bank account and add to the structure
		//maintenance pool.
		strongRef->payMaintenance(oneWeekMaintenance, owner, false);

		//Send email notification to owner.
		sendMailMaintenanceWithdrawnFromBank(owner, strongRef);

		//Reschedule task in 1 week.
		strongRef->scheduleMaintenanceExpirationEvent();
		}

		//Start decay process.
	} else {
		if (!strongRef->isDecayed()) {
			//Notify owner about decay.
			sendMailDecay(owner, strongRef);

			//Reschedule task in 1 day.
			reschedule(oneDayTime);
		} else {
			if (strongRef->isBuildingObject() && !shouldBuildingBeDestroyed(strongRef)) {
				BuildingObject* building = strongRef.castTo<BuildingObject*>();

				//Building is condemned since it has decayed.
				sendMailCondemned(owner, strongRef);

				strongRef->info("Structure decayed, it is now condemned.");

				building->updateSignName(true);
			} else {
				strongRef->info("Structure decayed, destroying it.");

				StructureManager::instance()->destroyStructure(strongRef);
			}
		}
	}
}

void StructureMaintenanceTask::sendMailMaintenanceWithdrawnFromBank(CreatureObject* owner, StructureObject* structure) {
	ManagedReference<ChatManager*> chatManager = structure->getZoneServer()->getChatManager();

	if (chatManager != NULL) {
		UnicodeString subject = "@player_structure:structure_maintenance_empty_subject";

		//Your %TT %TO has an empty maintenance pool. It will start deducting from your bank account automatically.
		StringIdChatParameter emailBody("@player_structure:structure_maintenance_empty_body");
		emailBody.setTT(structure->getObjectName());
		emailBody.setTO("(" + String::valueOf((int)structure->getPositionX()) + ", " + String::valueOf((int)structure->getPositionY()) + ")");

		chatManager->sendMail("@player_structure:your_structure_prefix", subject, emailBody, owner->getFirstName());
	}
}

void StructureMaintenanceTask::sendMailDecay(CreatureObject* owner, StructureObject* structure) {
	ManagedReference<ChatManager*> chatManager = structure->getZoneServer()->getChatManager();

	if (chatManager != NULL) {
		UnicodeString subject = "@player_structure:mail_structure_damage_sub";

		//Your %TT %TO is currently at %DI percent condition. It will be destroyed if it reaches 0. If you wish to keep this structure, you should immediately add maintenance.
		String bodyName = "mail_structure_damage";
		if (structure->isBuildingObject()) {
			//Your %TT %TO is currently at %DI percent condition. It will be condemned if it reaches 0. If you wish to keep this structure, you should immediately add maintenance.
			bodyName = "mail_structure_damage_condemn";
		}

		StringIdChatParameter emailBody("@player_structure:" + bodyName);
		emailBody.setTT(structure->getObjectName());
		emailBody.setTO("(" + String::valueOf((int)structure->getPositionX()) + ", " + String::valueOf((int)structure->getPositionY()) + ")");
		emailBody.setDI(structure->getDecayPercentage());

		chatManager->sendMail("@player_structure:your_structure_prefix", subject, emailBody, owner->getFirstName());
	}
}

void StructureMaintenanceTask::sendMailCondemned(CreatureObject* owner, StructureObject* structure) {
	//Create an email.
	ManagedReference<ChatManager*> chatManager = structure->getZoneServer()->getChatManager();

	if (chatManager != NULL) {
		UnicodeString subject = "@player_structure:structure_condemned_subject";

		//Your %TT %TO has been condemned by the order of the Empire due to lack of maintenance. You must pay %DI credits to uncondemn this structure.
		StringIdChatParameter emailBody("@player_structure:structure_condemned_body");
		emailBody.setTT(structure->getObjectName());
		emailBody.setTO("(" + String::valueOf((int)structure->getPositionX()) + ", " + String::valueOf((int)structure->getPositionY()) + ")");
		emailBody.setDI(-structure->getSurplusMaintenance());
		chatManager->sendMail("@player_structure:your_structure_prefix", subject, emailBody, owner->getFirstName());
	}
}

bool StructureMaintenanceTask::shouldBuildingBeDestroyed(StructureObject* structure) {
	int threeMonthsOfMaintenance = 3 * 30 * 24 * structure->getMaintenanceRate();

	if (threeMonthsOfMaintenance + structure->getSurplusMaintenance() < 0) {
		return true;
	} else {
		return false;
	}
}
