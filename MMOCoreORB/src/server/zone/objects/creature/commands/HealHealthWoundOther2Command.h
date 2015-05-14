/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions. */

#ifndef HEALHEALTHWOUNDOTHER2COMMAND_H_
#define HEALHEALTHWOUNDOTHER2COMMAND_H_

#include "server/zone/objects/scene/SceneObject.h"
#include "server/zone/packets/object/CombatAction.h"
#include "server/zone/managers/objectcontroller/ObjectController.h"
#include "server/zone/packets/chat/ChatSystemMessage.h"
#include "server/chat/StringIdChatParameter.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/creature/CreatureAttribute.h"
#include "QueueCommand.h"

class HealHealthWoundOther2Command : public QueueCommand {
protected:
	int forceCost;

	int healthWoundHealed;
	int strengthWoundHealed;
	int constitutionWoundHealed;
	int heal;

	float speed;
	float range;
	String effectName;
public:
	HealHealthWoundOther2Command(const String& name, ZoneProcessServer* server)
		: QueueCommand(name, server) {
		forceCost = 0;

		range = 32;

		healthWoundHealed = 0;
		strengthWoundHealed = 0;
		constitutionWoundHealed = 0;

		heal = 250;

		speed = 3.0;
	}

	void doAnimations(CreatureObject* creature, CreatureObject* creatureTarget) const {
		if (creatureTarget == creature)
			creature->playEffect("clienteffect/pl_force_healing.cef", "");
		else
			creature->doCombatAnimation(creatureTarget,STRING_HASHCODE("force_healing_1"),0,0xFF);
	}

	void sendWoundMessage(CreatureObject* object, CreatureObject* target, int healthWound, int strengthWound, int constitutionWound) const {
		if (!object->isPlayerCreature())
			return;

		if (!target->isPlayerCreature())
			return;

		CreatureObject* creature = cast<CreatureObject*>( object);
		CreatureObject* creatureTarget = cast<CreatureObject*>( target);

		StringBuffer msgPlayer, msgTarget, msgBody, msgTail;

		if (healthWound > 0 && strengthWound > 0 && constitutionWound > 0) {
			msgBody << healthWound << " health, " << strengthWound << " strength, and "  << constitutionWound << " constitution";
		} else if (healthWound > 0) {
			msgBody << healthWound << " health";
		} else if (strengthWound > 0) {
			msgBody << strengthWound << " strength";
		} else if (constitutionWound > 0) {
			msgBody << constitutionWound << " constitution";
		} else {
			creature->sendSystemMessage("Your target has no wounds of that type to heal."); //%NT has no wounds of that type to heal.
			return;
		}

		msgTail << " wounds.";

		msgPlayer << "You heal " << creatureTarget->getFirstName() << " for " << msgBody.toString() << msgTail.toString();
		msgTarget << creature->getFirstName() << " heals you for " << msgBody.toString() << msgTail.toString();

		creature->sendSystemMessage(msgPlayer.toString());
		creatureTarget->sendSystemMessage(msgTarget.toString());
	}

	bool canPerformSkill(CreatureObject* creature, CreatureObject* creatureTarget) const {
		if (!creatureTarget->getWounds(CreatureAttribute::HEALTH) && !creatureTarget->getWounds(CreatureAttribute::STRENGTH) && !creatureTarget->getWounds(CreatureAttribute::CONSTITUTION)) {
			creature->sendSystemMessage("Your target has no wounds of that type to heal."); //%NT has no wounds of that type to heal.
			return false;
		}

		if (creature != creatureTarget && !CollisionManager::checkLineOfSight(creature, creatureTarget)) {
			creature->sendSystemMessage("@container_error_message:container18");
			return false;
		}

		return true;
	}

	int doQueueCommand(CreatureObject* creature, const uint64& target, const UnicodeString& arguments) const {

		int result = doCommonMedicalCommandChecks(creature);

		if (result != SUCCESS)
			return result;

		if (isWearingArmor(creature))
			return NOJEDIARMOR;

		ManagedReference<SceneObject*> object = server->getZoneServer()->getObject(target);

		if (object != NULL) {
			if (!object->isCreatureObject()) {
				creature->sendSystemMessage("@jedi_spam:not_this_target"); //This command cannot be used on this target.
				return GENERALERROR;
			}
		} else {
			object = creature;
		}

		CreatureObject* creatureTarget = cast<CreatureObject*>( object.get());

		Locker clocker(creatureTarget, creature);

		if (creatureTarget->isAiAgent() || creatureTarget->isDead() || creatureTarget->isRidingMount() || creatureTarget->isAttackableBy(creature))
			creatureTarget = creature;

		if (creatureTarget == creature) {
			return GENERALERROR;
		}

		if (!canPerformSkill(creature, creatureTarget))
			return GENERALERROR;

		if (!creatureTarget->isHealableBy(creature)) {
			creature->sendSystemMessage("@healing:pvp_no_help");
			return GENERALERROR;
		}

		uint32 healedHealthWound = creatureTarget->healWound(creature, CreatureAttribute::HEALTH, heal, true);
		uint32 healedStrengthWound = creatureTarget->healWound(creature, CreatureAttribute::STRENGTH, heal, true);
		uint32 healedConstitutionWound = creatureTarget->healWound(creature, CreatureAttribute::CONSTITUTION, heal, true);

		ManagedReference<PlayerObject*> playerObject = creature->getPlayerObject();

		if (playerObject->getForcePower() <= 150) {
			creature->sendSystemMessage("@jedi_spam:no_force_power"); //You do not have enough force to do that.
			return GENERALERROR;
		}

		float forceCost = MIN(((healedHealthWound + healedStrengthWound + healedConstitutionWound) / 25), 150);

		playerObject->setForcePower(playerObject->getForcePower() - forceCost); // Deduct force.

		sendWoundMessage(creature, creatureTarget, healedHealthWound, healedStrengthWound, healedConstitutionWound);

		doAnimations(creature, creatureTarget);

		checkForTef(creature, creatureTarget);

		return SUCCESS;
	}

};

#endif /* HEALHEALTHWOUNDOTHER2COMMAND_H_ */
