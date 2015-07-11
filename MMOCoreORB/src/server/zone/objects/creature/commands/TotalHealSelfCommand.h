/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions. */

#ifndef TOTALHEALSELFCOMMAND_H_
#define TOTALHEALSELFCOMMAND_H_

#include "server/zone/objects/scene/SceneObject.h"
#include "server/zone/packets/object/CombatAction.h"
#include "server/zone/managers/objectcontroller/ObjectController.h"
#include "server/zone/packets/chat/ChatSystemMessage.h"
#include "server/chat/StringIdChatParameter.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/creature/CreatureAttribute.h"
#include "QueueCommand.h"

class TotalHealSelfCommand : public QueueCommand {
protected:

	int forceCost;
	int heal;	

	int healthHealed;
	int actionHealed;
	int mindHealed;

	int healthWoundHealed;
	int strengthWoundHealed;
	int constitutionWoundHealed;
	
	int actionWoundHealed;
	int quicknessWoundHealed;
	int staminaWoundHealed;	
	
	int mindWoundHealed;
	int focusWoundHealed;
	int willpowerWoundHealed;	

	float speed;
	float range;
	String effectName;
	
public:
	TotalHealSelfCommand(const String& name, ZoneProcessServer* server)
		: QueueCommand(name, server) {

		forceCost = 0;
		heal = 6000;

		healthHealed = 0;
		actionHealed = 0;
		mindHealed = 0;

		healthWoundHealed = 0;
		strengthWoundHealed = 0;
		constitutionWoundHealed = 0;

		actionWoundHealed = 0;
		quicknessWoundHealed = 0;
		staminaWoundHealed = 0;

		mindWoundHealed = 0;
		focusWoundHealed = 0;
		willpowerWoundHealed = 0;

		speed = 3.0f;
		range = 0;
	
	}
	
	void doAnimations(CreatureObject* creature) const {
			creature->playEffect("clienteffect/pl_force_healing.cef", "");
	}
		
	
	void sendWoundMessage(CreatureObject* creature, int healthWound, int actionWound, int mindWound, int strengthWound, int constitutionWound, int quicknessWound, int staminaWound, int focusWound, int willpowerWound) const {

		StringBuffer msgPlayer, msgBody, msgTail;

		if (healthWound > 0 && strengthWound > 0 && constitutionWound > 0 && actionWound > 0 && quicknessWound > 0 && staminaWound > 0 && mindWound > 0 && focusWound > 0 && willpowerWound > 0) {
			msgBody << healthWound << " health, " << strengthWound << " strength, " << constitutionWound << " constitution " << actionWound << " action, " << quicknessWound << " quickness, " << staminaWound << " stamina " << mindWound << " mind, " << focusWound << " focus, and "  << willpowerWound << " willpower";
		} else if (healthWound > 0) {
			msgBody << healthWound << " health";
		} else if (strengthWound > 0) {
			msgBody << strengthWound << " strength";
		} else if (constitutionWound > 0) {
			msgBody << constitutionWound << " constitution";
		} else if (actionWound > 0) {
			msgBody << actionWound << " action";
		} else if (quicknessWound > 0) {
			msgBody << quicknessWound << " quickness";
		} else if (staminaWound > 0) {
			msgBody << staminaWound << " stamina";
		} else if (mindWound > 0) {
			msgBody << mindWound << " mind";
		} else if (focusWound > 0) {
			msgBody << focusWound << " focus";
		} else if (willpowerWound > 0) {
			msgBody << willpowerWound << " willpower";			
		} else {
			creature->sendSystemMessage("@healing_response:healing_response_67"); // You have no wounds of that type.
			return;
		}

		msgTail << " wounds.";

			msgPlayer << "You heal yourself for " << msgBody.toString() << msgTail.toString();
			creature->sendSystemMessage(msgPlayer.toString());

	}
	
	void sendHealMessage(CreatureObject* creature, int healthDamage, int actionDamage, int mindDamage) const {

		StringBuffer msgPlayer, msgBody, msgTail;

		if (healthDamage > 0 && actionDamage > 0 && mindDamage > 0) {
			msgBody << healthDamage << " health, " << actionDamage << " action, and "  << mindDamage << " mind";
		} else if (healthDamage > 0) {
			msgBody << healthDamage << " health";
		} else if (actionDamage > 0) {
			msgBody << actionDamage << " action";
		} else if (mindDamage > 0) {
			msgBody << mindDamage << " mind";
		} else {
			creature->sendSystemMessage("@jedi_spam:no_damage_heal_self"); // You have no damage of that type.
			return;
		}	

		msgTail << " damage.";

			msgPlayer << "You heal yourself for " << msgBody.toString() << " points of" << msgTail.toString(); // You heal yourself for %DI points of %TO.
			creature->sendSystemMessage(msgPlayer.toString());

	}	
		
		
	int doQueueCommand(CreatureObject* creature, const uint64& target, const UnicodeString& arguments) const {

		int result = doCommonMedicalCommandChecks(creature);

		if (result != SUCCESS)
			return result;

		if (isWearingArmor(creature)) {
			return NOJEDIARMOR;
		}	
				
		if (!creature->getWounds(CreatureAttribute::HEALTH) && !creature->getWounds(CreatureAttribute::STRENGTH) && !creature->getWounds(CreatureAttribute::CONSTITUTION) && !creature->getWounds(CreatureAttribute::ACTION) && !creature->getWounds(CreatureAttribute::QUICKNESS) && !creature->getWounds(CreatureAttribute::STAMINA) && !creature->getWounds(CreatureAttribute::MIND) && !creature->getWounds(CreatureAttribute::FOCUS) && !creature->getWounds(CreatureAttribute::WILLPOWER) && !creature->hasDamage(CreatureAttribute::HEALTH) && !creature->hasDamage(CreatureAttribute::ACTION) && !creature->hasDamage(CreatureAttribute::MIND) && !creature->hasState(CreatureState::STUNNED) && !creature->hasState(CreatureState::DIZZY) && !creature->hasState(CreatureState::INTIMIDATED) && !creature->hasState(CreatureState::BLINDED)) {
			creature->sendSystemMessage("You have nothing of that type to heal."); // You have nothing of that type to heal.
			return false;
		}
		
		int healDisease = creature->healDot(CreatureState::DISEASED, 300);
		int healPoison = creature->healDot(CreatureState::POISONED, 300);
		int healBleeding = creature->healDot(CreatureState::BLEEDING, 300);
		int healOnFire = creature->healDot(CreatureState::ONFIRE, 300);			
		
		int healedHealthWound = creature->healWound(creature, CreatureAttribute::HEALTH, heal);
		int healedStrengthWound = creature->healWound(creature, CreatureAttribute::STRENGTH, heal);
		int healedConstitutionWound = creature->healWound(creature, CreatureAttribute::CONSTITUTION, heal);
		
		int healedActionWound = creature->healWound(creature, CreatureAttribute::ACTION, heal);
		int healedQuicknessWound = creature->healWound(creature, CreatureAttribute::QUICKNESS, heal);
		int healedStaminaWound = creature->healWound(creature, CreatureAttribute::STAMINA, heal);
		
		int healedMindWound = creature->healWound(creature, CreatureAttribute::MIND, heal);
		int healedFocusWound = creature->healWound(creature, CreatureAttribute::FOCUS, heal);
		int healedWillpowerWound = creature->healWound(creature, CreatureAttribute::WILLPOWER, heal);
		
		int healedHealth = creature->healDamage(creature, CreatureAttribute::HEALTH, heal);
		int healedAction = creature->healDamage(creature, CreatureAttribute::ACTION, heal);
		int healedMind = creature->healDamage(creature, CreatureAttribute::MIND, heal, true, false);		

		creature->addShockWounds(-1000, true, false);
		
		creature->removeStateBuff(CreatureState::STUNNED);

		creature->removeStateBuff(CreatureState::DIZZY);

		creature->removeStateBuff(CreatureState::BLINDED);

		creature->removeStateBuff(CreatureState::INTIMIDATED);
			
		ManagedReference<PlayerObject*> playerObject = creature->getPlayerObject();
		
		if (playerObject->getForcePower() <= 400) {
			creature->sendSystemMessage("@jedi_spam:no_force_power"); //You do not have enough force to do that.
			return GENERALERROR;
		}
		
		
		float forceCost = MIN(((healedHealth + healedAction + healedMind + healedHealthWound + healedStrengthWound + healedConstitutionWound + healedActionWound + healedQuicknessWound + healedStaminaWound + healedMindWound + healedFocusWound + healedWillpowerWound + healDisease + healPoison + healBleeding + healOnFire) / 20), 400);

		
		playerObject->setForcePower(playerObject->getForcePower() - forceCost); // Deduct force.	

		sendHealMessage(creature, healedHealth, healedAction, healedMind);
		
		sendWoundMessage(creature, healedHealthWound, healedActionWound,  healedMindWound, healedStrengthWound, healedConstitutionWound, healedQuicknessWound, healedStaminaWound, healedFocusWound, healedWillpowerWound);
				
		doAnimations(creature);	
			
		return SUCCESS;
	}

};

#endif /* TOTALHEALSELFCOMMAND_H_ */
