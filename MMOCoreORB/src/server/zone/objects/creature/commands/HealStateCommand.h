/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef HEALSTATECOMMAND_H_
#define HEALSTATECOMMAND_H_

#include "server/zone/objects/scene/SceneObject.h"
#include "server/chat/StringIdChatParameter.h"
#include "server/zone/objects/tangible/pharmaceutical/StatePack.h"
#include "server/zone/ZoneServer.h"
#include "server/zone/managers/player/PlayerManager.h"
#include "server/zone/objects/creature/events/InjuryTreatmentTask.h"
#include "server/zone/objects/creature/buffs/DelayedBuff.h"
#include "server/zone/managers/collision/CollisionManager.h"

class HealStateCommand : public QueueCommand {
	float mindCost;
	float range;
	Vector<uint64> healableStates;
public:

	HealStateCommand(const String& name, ZoneProcessServer* server)
		: QueueCommand(name, server) {

		mindCost = 20;
		range = 6;
		healableStates.add(CreatureState::STUNNED);
		healableStates.add(CreatureState::DIZZY);
		healableStates.add(CreatureState::BLINDED);
		healableStates.add(CreatureState::INTIMIDATED);
	}

	void deactivateStateTreatment(CreatureObject* creature) const {
		float modSkill = (float)creature->getSkillMod("healing_injury_speed");
		int delay = (int)round(20.0f - (modSkill / 5));

		if (creature->hasBuff(BuffCRC::FOOD_HEAL_RECOVERY)) {
			DelayedBuff* buff = cast<DelayedBuff*>( creature->getBuff(BuffCRC::FOOD_HEAL_RECOVERY));

			if (buff != NULL) {
				float percent = buff->getSkillModifierValue("heal_recovery");

				delay = round(delay * (100.0f - percent) / 100.0f);
			}
		}

		//Force the delay to be at least 4 seconds.
		delay = (delay < 4) ? 4 : delay;

		StringIdChatParameter message("healing_response", "healing_response_58"); //You are now ready to heal more damage.
		Reference<InjuryTreatmentTask*> task = new InjuryTreatmentTask(creature, message, "stateTreatment");
		creature->addPendingTask("stateTreatment", task, delay * 1000);
	}

	void awardXp(CreatureObject* creature, String type, int power) const {
		if (!creature->isPlayerCreature())
			return;

		CreatureObject* player = cast<CreatureObject*>(creature);

		int amount = (int)round((float)power * 1.f);

		if (amount <= 0)
			return;

		PlayerManager* playerManager = server->getZoneServer()->getPlayerManager();
		playerManager->awardExperience(player, type, amount, true);
	}

	void sendStateMessage(CreatureObject* creature, CreatureObject* creatureTarget, uint64 state) const {
		if (!creature->isPlayerCreature())
			return;

		CreatureObject* player = cast<CreatureObject*>(creature);
		StringBuffer msgPlayer, msgTarget;

		String stateName = CreatureState::instance()->getName(state, true);

		if (player == creatureTarget) {
			msgTarget << "You remove the " << stateName << " state from yourself.";
			creatureTarget->sendSystemMessage(msgTarget.toString());
		} else if (creatureTarget->isPlayerCreature()) {
			msgPlayer << "You remove the " << stateName << " state from " << creatureTarget->getFirstName() << ".";
			msgTarget << player->getFirstName() << " removes the " << stateName << " from you.";

			creatureTarget->sendSystemMessage(msgTarget.toString());
			player->sendSystemMessage(msgPlayer.toString());
		} else {
			msgPlayer << "You remove the " << stateName << " state from " << creatureTarget->getDisplayedName() << ".";
			player->sendSystemMessage(msgPlayer.toString());
		}
	}

	void doAnimations(CreatureObject* creature, CreatureObject* creatureTarget) const {
		creatureTarget->playEffect("clienteffect/healing_healstate.cef", "");

		if (creature == creatureTarget)
			creature->doAnimation("heal_self");
		else
			creature->doAnimation("heal_other");
	}

	bool canPerformSkill(CreatureObject* creature, CreatureObject* creatureTarget, StatePack* statePack, int mindCostNew) const {
		if (!creature->canTreatStates()) {
			creature->sendSystemMessage("@healing_response:healing_must_wait"); //You must wait before you can do that.
			return false;
		}

		if (statePack == NULL) {
			creature->sendSystemMessage("@healing_response:healing_response_60"); //No valid medicine found.
			return false;
		}

		if (!creatureTarget->isHealableBy(creature)) {
			creature->sendSystemMessage("@healing:pvp_no_help");  //It would be unwise to help such a patient.
			return false;
		}

		if (creature->getHAM(CreatureAttribute::MIND) < mindCostNew) {
			creature->sendSystemMessage("@healing_response:not_enough_mind"); //You do not have enough mind to do that.
			return false;
		}

		return true;
	}

	StatePack* findStatePack(CreatureObject* creature, uint64 state) const {
		SceneObject* inventory = creature->getSlottedObject("inventory");

		int medicineUse = creature->getSkillMod("healing_ability");

		if (inventory != NULL) {
			for (int i = 0; i < inventory->getContainerObjectsSize(); i++) {
				SceneObject* object = inventory->getContainerObject(i);

				if (!object->isPharmaceuticalObject())
					continue;

				PharmaceuticalObject* pharma = cast<PharmaceuticalObject*>(object);

				if (pharma->isStatePack()) {
					StatePack* statePack = cast<StatePack*>(pharma);

					if (statePack->getMedicineUseRequired() <= medicineUse && statePack->getState() == state)
						return statePack;
				}
			}
		}

		return NULL;
	}

	void parseModifier(const String& modifier, uint64& state, uint64& objectId) const {
		if (!modifier.isEmpty()) {
			StringTokenizer tokenizer(modifier);
			tokenizer.setDelimeter("|");

			String stateName;

			tokenizer.getStringToken(stateName);

			state = CreatureState::instance()->getState(stateName);

			if (tokenizer.hasMoreTokens())
				objectId = tokenizer.getLongToken();
		} else {
			state = CreatureState::INVALID;
			objectId = 0;
		}
	}

	int doQueueCommand(CreatureObject* creature, const uint64& target, const UnicodeString& arguments) const {

		int result = doCommonMedicalCommandChecks(creature);

		if (result != SUCCESS)
			return result;

		ManagedReference<SceneObject*> object = server->getZoneServer()->getObject(target);

		if (object != NULL) {
			if (!object->isCreatureObject()) {
				TangibleObject* tangibleObject = dynamic_cast<TangibleObject*>(object.get());

				if (tangibleObject != NULL && tangibleObject->isAttackableBy(creature)) {
					object = creature;
				} else {
					creature->sendSystemMessage("@healing_response:healing_response_73"); //Target must be a player or a creature pet in order to heal a state.
					return GENERALERROR;
				}
			}
		} else {
			object = creature;
		}

		CreatureObject* creatureTarget = cast<CreatureObject*>( object.get());

		Locker clocker(creatureTarget, creature);

		if ((creatureTarget->isAiAgent() && !creatureTarget->isPet()) || creatureTarget->isDroidObject() || creatureTarget->isVehicleObject() || creatureTarget->isDead() || creatureTarget->isRidingMount() || creatureTarget->isAttackableBy(creature))
			creatureTarget = creature;

		uint64 state = CreatureState::INVALID;
		uint64 objectId = 0;

		parseModifier(arguments.toString(), state, objectId);

		SceneObject* inventory = creature->getSlottedObject("inventory");

		ManagedReference<StatePack*> statePack = NULL;

		if(state != CreatureState::INVALID || objectId != 0) {
			if (inventory != NULL) {
				statePack = inventory->getContainerObject(objectId).castTo<StatePack*>();
			}

			if (statePack == NULL)
				statePack = findStatePack(creature, state);
		}else {
			uint64 targetStateBitmask = creatureTarget->getStateBitmask();
			for(int i=0; i<healableStates.size(); i++) {

				uint64 healableState = healableStates.get(i);

				if(!(targetStateBitmask & healableState))
					continue;


				state = healableState;
				statePack = findStatePack(creature, healableState);

				if(statePack != NULL) {
					break;
				}
			}

			//if state is INVALID they had no healable states
			//if it is valid but statePack is NULL they had no valid medicine for *any* state and will error in canPerformSkill
			if(state == CreatureState::INVALID) {
				StringIdChatParameter stringId("healing", "no_state_to_heal"); // %TT has no state that you can heal.
				stringId.setTT(creatureTarget->getDisplayedName());
				creature->sendSystemMessage(stringId);
				return GENERALERROR;
			}
		}

		int mindCostNew = creature->calculateCostAdjustment(CreatureAttribute::FOCUS, mindCost);

		if (!canPerformSkill(creature, creatureTarget, statePack, mindCostNew))
			return GENERALERROR;

		if(!checkDistance(creature, creatureTarget, range))
			return TOOFAR;

		PlayerManager* playerManager = server->getPlayerManager();

		if (creature != creatureTarget && !CollisionManager::checkLineOfSight(creature, creatureTarget)) {
			creature->sendSystemMessage("@healing:no_line_of_sight"); // You cannot see your target.
			return GENERALERROR;
		}

		if (statePack->getState() != state)
			state = statePack->getState();

		if (!creatureTarget->removeStateBuff(state)) {
			if (creature == creatureTarget)
				creature->sendSystemMessage("@healing_response:healing_response_72"); //You have no state of that type to heal.
			else if (creatureTarget->isPlayerCreature()){
				StringIdChatParameter msg("healing_response", "healing_response_74"); //%NT has no state of that type to heal.
				msg.setTT(creatureTarget->getObjectID());
				creature->sendSystemMessage(msg);
			} else {
				StringBuffer message;
				message << creatureTarget->getDisplayedName() << " has no state of that type to heal.";
				creature->sendSystemMessage(message.toString());
			}

			return GENERALERROR;
		}

		creature->inflictDamage(creature, CreatureAttribute::MIND, mindCostNew, false);

		sendStateMessage(creature, creatureTarget, state);

		deactivateStateTreatment(creature);

		if (statePack != NULL) {
			Locker locker(statePack);
			statePack->decreaseUseCount();
		}

		if (creatureTarget != creature && !creatureTarget->isPet())
			awardXp(creature, "medical", 50); //No experience for healing yourself or pets.

		doAnimations(creature, creatureTarget);

		creature->notifyObservers(ObserverEventType::MEDPACKUSED);

		checkForTef(creature, creatureTarget);

		return SUCCESS;
	}

};

#endif //HEALSTATECOMMAND_H_
