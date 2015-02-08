/*
 * ForceHealQueueCommand.h
 *
 *  Created on: Mar 15, 2014
 *      Author: swgemu
 */

#ifndef FORCEHEALQUEUECOMMAND_H_
#define FORCEHEALQUEUECOMMAND_H_


#include "server/zone/objects/scene/SceneObject.h"
#include "server/chat/ChatManager.h"
#include "server/zone/managers/stringid/StringIdManager.h"
#include "QueueCommand.h"

class ForceHealQueueCommand : public QueueCommand {
protected:
	float forceCost;
	uint32 buffCRC;

public:
	ForceHealQueueCommand(const String& name, ZoneProcessServer* server)
: QueueCommand(name, server) {
		forceCost = 0;
	}

	void doAnimations(CreatureObject* creature, CreatureObject* creatureTarget) {

		if (creature == creatureTarget)
			creature->playEffect("clienteffect/pl_force_heal_self.cef", "");
		else
			creature->doCombatAnimation(creatureTarget,String("force_healing_1").hashCode(),0,0xFF);
	}

	bool isWarcried(CreatureObject* creature) {
		if (creature->hasAttackDelay()) {
			return true;
		}
		return false;
	}

	bool checkForceCost(CreatureObject* creature) {
		ManagedReference<PlayerObject*> playerObject = creature->getPlayerObject();

		if (playerObject != NULL) {
			if (playerObject->getForcePower() < forceCost) {
				return false;
			}
			return true;
		}
		return false;
	}

	void sendHealMessage(CreatureObject* object, CreatureObject* target, int healthDamage, int actionDamage, int mindDamage) {
		if (!object->isPlayerCreature())
			return;

		if (!target->isPlayerCreature())
			return;

		CreatureObject* creature = cast<CreatureObject*>( object);
		CreatureObject* creatureTarget = cast<CreatureObject*>( target);

		StringBuffer msgPlayer, msgTarget, msgBody, msgTail;

		if (healthDamage > 0 && actionDamage > 0 && mindDamage > 0) {
			msgBody << healthDamage << " health, " << actionDamage << " action, and "  << mindDamage << " mind";
		} else if (healthDamage > 0) {
			msgBody << healthDamage << " health";
		} else if (actionDamage > 0) {
			msgBody << actionDamage << " action";
		} else if (mindDamage > 0) {
			msgBody << mindDamage << " mind";
		} else {
			creature->sendSystemMessage("@jedi_spam:no_damage_heal_other"); //Your target has no damage of that type to heal.
			return;
		}

		msgTail << " damage.";

			msgPlayer << "You heal " << creatureTarget->getFirstName() << " for " << msgBody.toString() << msgTail.toString(); // You heal %TT for %DI points of %TO.
			msgTarget << "You are healed for " << msgBody.toString() << " points of damage by " << creature->getFirstName(); // You are healed for %DI points of %TO by %TT.

			creature->sendSystemMessage(msgPlayer.toString());
			creatureTarget->sendSystemMessage(msgTarget.toString());

	}

	int healBattleFatigue(CreatureObject* creature, int damage){

		int currentValue = creature->getShockWounds();

		int healedValue = MIN(currentValue, damage);

		creature->addShockWounds(-healedValue);

		return healedValue;

	}

	int doQueueCommand(CreatureObject* creature, const uint64& target, const UnicodeString& arguments) {

		return SUCCESS;
	}

	bool isForceHealCommand() {
		return true;
	}

	void setForceCost(int fpc) {
		forceCost = fpc;
	}

	float getCommandDuration(CreatureObject* object, const UnicodeString& arguments) {
		return defaultTime * 3.0;
	}
};


#endif /* FORCEHEALQUEUECOMMAND_H_ */
