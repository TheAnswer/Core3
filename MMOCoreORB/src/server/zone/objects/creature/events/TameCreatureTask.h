
#ifndef TAMECREATURETASK_H_
#define TAMECREATURETASK_H_

#include "server/zone/managers/combat/CombatManager.h"
#include "server/zone/managers/creature/CreatureManager.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/intangible/PetControlDevice.h"
#include "server/chat/ChatManager.h"
#include "engine/engine.h"

class TameCreatureTask : public Task {

private:
	enum Phase { INITIAL, SECOND, THIRD, FOURTH, FINAL} currentPhase;
	int originalMask;
	ManagedReference<Creature*> creature;
	ManagedReference<CreatureObject*> player;

public:
	TameCreatureTask(Creature* cre, CreatureObject* playo) : Task() {
		currentPhase = INITIAL;
		creature = cre;
		player = playo;
		originalMask = 0;
	}

	void run() {
		Locker locker(creature);

		Locker _clocker(player, creature);

		player->removePendingTask("tame_pet");

		if (!creature->isInRange(player, 8.0f)) {
			player->sendSystemMessage("@hireling/hireling:taming_toofar"); // You are too far away to continue taming.
			creature->setPvpStatusBitmask(originalMask, true);
			return;
		}

		if (!creature->canTameMe(player)) {
			player->sendSystemMessage("@pet/pet_menu:sys_cant_tame"); // You can't tame that
			creature->setPvpStatusBitmask(originalMask, true);
			return;
		}

		ChatManager* chatManager = player->getZoneServer()->getChatManager();

		switch (currentPhase) {
		case INITIAL:
			originalMask = creature->getPvpStatusBitmask();
			creature->clearState(CreatureFlag::ATTACKABLE, true);
			chatManager->broadcastMessage(player, "@hireling/hireling:taming_2"); // Steady.
			player->doAnimation("");
			currentPhase = SECOND;
			player->addPendingTask("tame_pet", this, 8000);
			break;
		case SECOND:
			chatManager->broadcastMessage(player, "@hireling/hireling:taming_3"); // Don't be scared.
			currentPhase = THIRD;
			player->addPendingTask("tame_pet", this, 8000);
			break;
		case THIRD:
			chatManager->broadcastMessage(player, "@hireling/hireling:taming_4"); // Don't bite me.
			currentPhase = FOURTH;
			player->addPendingTask("tame_pet", this, 8000);
			break;
		case FOURTH:
			chatManager->broadcastMessage(player, "@hireling/hireling:taming_5"); // Good... er... boy?
			currentPhase = FINAL;
			player->addPendingTask("tame_pet", this, 8000);
			break;
		case FINAL:
			int skill = 0;
			int cl = creature->getLevel();
			int ferocity = creature->getFerocity();
			float tamingChance = creature->getTame();

			if (creature->isAggressiveTo(player))
				skill = player->getSkillMod("tame_aggro");
			else
				skill = player->getSkillMod("tame_non_aggro");

			// TODO: possibly come up with a better formula for determining success or failure
			if ((System::random((100.0f * tamingChance) + skill)) > (cl + ferocity))
				success(player, creature);
			else {
				player->sendSystemMessage("@hireling/hireling:taming_fail"); // You fail to tame the creature.
				creature->setPvpStatusBitmask(originalMask, true);

				if (System::random(10) == 1)
					CombatManager::instance()->startCombat(creature,player,true);
			}

			break;
		}

		return;
	}

	void success(CreatureObject* player, Creature* creature) {
		ZoneServer* zoneServer = player->getZoneServer();

		String objectString = creature->getControlDeviceTemplate();
		if (objectString == "")
			objectString = "object/intangible/pet/pet_control.iff";

		ManagedReference<PetControlDevice*> controlDevice = zoneServer->createObject(objectString.hashCode(), 1).castTo<PetControlDevice*>();

		if (controlDevice == NULL)
			return;

		SceneObject* datapad = player->getSlottedObject("datapad");
		PlayerManager* playerManager = zoneServer->getPlayerManager();
		ObjectManager* objectManager = zoneServer->getObjectManager();

		if (datapad == NULL || playerManager == NULL || objectManager == NULL)
			return;

		controlDevice->setControlledObject(creature);

		creature->setCustomObjectName("", false);
		StringId s;
		s.setStringId(creature->getDisplayedName());
		controlDevice->setObjectName(s);
		controlDevice->setPetType(PetControlDevice::CREATUREPET);
		controlDevice->setMaxVitality(100);
		controlDevice->setVitality(100);
		controlDevice->updateStatus(1);
		datapad->transferObject(controlDevice, -1);

		objectManager->persistSceneObjectsRecursively(creature, 1);

		creature->setControlDevice(controlDevice);
		creature->setObjectMenuComponent("PetMenuComponent");
		creature->setCreatureLink(player);
		creature->setFaction(player->getFaction());
		creature->setPvpStatusBitmask(player->getPvpStatusBitmask() - CreatureFlag::PLAYER, false);
		creature->setBaby(false);
		creature->setFollowObject(player);

		creature->getZone()->broadcastObject(creature, true);
		datapad->broadcastObject(controlDevice, true);

		ManagedReference<PlayerObject*> ghost = player->getPlayerObject();
		ghost->addToActivePets(creature);

		playerManager->awardExperience(player, "creaturehandler", 25 * creature->getLevel());
		player->sendSystemMessage("@hireling/hireling:taming_success"); // You successfully tame the creature.
	}
};

#endif /* TAMECREATURETASK_H_ */
