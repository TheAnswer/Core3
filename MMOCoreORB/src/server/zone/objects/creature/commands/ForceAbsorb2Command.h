/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef FORCEABSORB2COMMAND_H_
#define FORCEABSORB2COMMAND_H_

#include "server/zone/objects/scene/SceneObject.h"
#include "server/zone/objects/creature/buffs/SingleUseBuff.h"

class ForceAbsorb2Command : public QueueCommand {
public:

	ForceAbsorb2Command(const String& name, ZoneProcessServer* server)
		: QueueCommand(name, server) {

	}

	int doQueueCommand(CreatureObject* creature, const uint64& target, const UnicodeString& arguments) const {

		if (!checkStateMask(creature))
			return INVALIDSTATE;

		if (!checkInvalidLocomotions(creature))
			return INVALIDLOCOMOTION;

		if (creature->hasAttackDelay() || !creature->checkPostureChangeDelay())
			return GENERALERROR;

		if (isWearingArmor(creature)) {
			return NOJEDIARMOR;
		}

		// Force cost of skill.
		int forceCost = 100;


		//Check for and deduct Force cost.

		ManagedReference<PlayerObject*> playerObject = creature->getPlayerObject();


		if (playerObject->getForcePower() <= forceCost) {
			creature->sendSystemMessage("@jedi_spam:no_force_power"); //"You do not have enough Force Power to peform that action.

			return GENERALERROR;
		}

		uint32 buffcrc1 = BuffCRC::JEDI_FORCE_ABSORB_1;
		uint32 buffcrc2 = BuffCRC::JEDI_FORCE_ABSORB_2;

		if (creature->hasBuff(buffcrc1)) {
			creature->removeBuff(buffcrc1);
		}

		playerObject->setForcePower(playerObject->getForcePower() - forceCost);

		StringIdChatParameter startStringId("jedi_spam", "apply_forceabsorb2");
		StringIdChatParameter endStringId("jedi_spam", "remove_forceabsorb2");

		int duration = 60;

		Vector<unsigned int> eventTypes;
		eventTypes.add(ObserverEventType::FORCEBUFFHIT);

		ManagedReference<SingleUseBuff*> buff = new SingleUseBuff(creature, buffcrc2, duration, BuffType::JEDI, getNameCRC());

		Locker locker(buff);

		buff->setStartMessage(startStringId);
		buff->setEndMessage(endStringId);
		buff->setSkillModifier("force_absorb", 1);
		buff->init(&eventTypes);

		creature->addBuff(buff);
		creature->playEffect("clienteffect/pl_force_absorb_self.cef", "");

		return SUCCESS;
	}

};

#endif //FORCEABSORB2COMMAND_H_
