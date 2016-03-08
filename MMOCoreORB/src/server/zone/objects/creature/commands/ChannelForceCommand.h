/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef CHANNELFORCECOMMAND_H_
#define CHANNELFORCECOMMAND_H_

#include "server/zone/objects/scene/SceneObject.h"
#include "server/zone/objects/creature/buffs/ChannelForceBuff.h"

class ChannelForceCommand : public QueueCommand {
public:

	ChannelForceCommand(const String& name, ZoneProcessServer* server)
		: QueueCommand(name, server) {

	}

	int doQueueCommand(CreatureObject* creature, const uint64& target, const UnicodeString& arguments) const {

		if (!checkStateMask(creature))
			return INVALIDSTATE;

		if (!checkInvalidLocomotions(creature))
			return INVALIDLOCOMOTION;

		if (creature->hasAttackDelay())
			return GENERALERROR;

		if (isWearingArmor(creature)) {
			return NOJEDIARMOR;
		}

		// Bonus is in between 200-300.
		int rand = System::random(10);
		int forceBonus = 200 + (rand * 10); // Needs to be divisible by amount of ticks.

		ManagedReference<PlayerObject*> playerObject = creature->getPlayerObject();

		if (playerObject == NULL)
			return GENERALERROR;

		// Do not execute if the player's force bar is full.
		if (playerObject->getForcePower() >= playerObject->getForcePowerMax())
			return GENERALERROR;

		// To keep it from going over max...
		if ((playerObject->getForcePowerMax() - playerObject->getForcePower()) < forceBonus)
			forceBonus = ((playerObject->getForcePowerMax() - playerObject->getForcePower()) / 10) * 10;

		int health = creature->getHAM(CreatureAttribute::HEALTH);
		int action = creature->getHAM(CreatureAttribute::ACTION);
		int mind = creature->getHAM(CreatureAttribute::MIND);

		if ((health <= forceBonus) || (action <= forceBonus) || (mind <= forceBonus)) {
			creature->sendSystemMessage("@jedi_spam:channel_ham_too_low"); // Your body is too weakened to perform that action.
			return GENERALERROR;
		}

		int maxHealth = creature->getMaxHAM(CreatureAttribute::HEALTH);
		int maxAction = creature->getMaxHAM(CreatureAttribute::ACTION);
		int maxMind = creature->getMaxHAM(CreatureAttribute::MIND);

		if ((maxHealth - creature->getWounds(CreatureAttribute::HEALTH) <= forceBonus) || (maxAction - creature->getWounds(CreatureAttribute::ACTION) <= forceBonus) || (maxMind - creature->getWounds(CreatureAttribute::MIND) <= forceBonus)) {
			creature->sendSystemMessage("@jedi_spam:channel_ham_too_low"); // Your body is too weakened to perform that action.
			return GENERALERROR;
		}

		// Give Force, and subtract HAM.
		playerObject->setForcePower(playerObject->getForcePower() + forceBonus);

		creature->addMaxHAM(CreatureAttribute::HEALTH, -forceBonus, true);
		creature->addMaxHAM(CreatureAttribute::ACTION, -forceBonus, true);
		creature->addMaxHAM(CreatureAttribute::MIND, -forceBonus, true);



		// Setup buffs.
		String buffname = "skill.buff.channelforce";
		ManagedReference<Buff*> buff = new ChannelForceBuff(creature, buffname, buffname.hashCode(), forceBonus, 2);

		Locker locker(buff);

		creature->addBuff(buff);

		return SUCCESS;
	}

	float getCommandDuration(CreatureObject* object, const UnicodeString& arguments) const {
		return defaultTime * 3.0;
	}

};

#endif //CHANNELFORCECOMMAND_H_
