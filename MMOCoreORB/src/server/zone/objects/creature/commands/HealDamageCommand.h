/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef HEALDAMAGECOMMAND_H_
#define HEALDAMAGECOMMAND_H_

#include "server/zone/objects/building/BuildingObject.h"
#include "server/zone/objects/scene/SceneObject.h"
#include "server/zone/objects/tangible/pharmaceutical/StimPack.h"
#include "server/zone/objects/tangible/pharmaceutical/RangedStimPack.h"
#include "server/zone/ZoneServer.h"
#include "server/zone/managers/player/PlayerManager.h"
#include "server/zone/objects/creature/events/InjuryTreatmentTask.h"
#include "server/zone/objects/creature/buffs/DelayedBuff.h"
#include "server/zone/packets/object/CombatAction.h"
#include "server/zone/managers/collision/CollisionManager.h"

class HealDamageCommand : public QueueCommand {
	float range;
	float mindCost;

public:
	HealDamageCommand(const String& name, ZoneProcessServer* server)
		: QueueCommand(name, server) {

		range = 5;
		mindCost = 50;
	}

	void deactivateInjuryTreatment(CreatureObject* creature, bool isRangedStim) const {
		float modSkill = 0.0f;

		if (isRangedStim)
			modSkill = (float)creature->getSkillMod("healing_range_speed");
		else
			modSkill = (float)creature->getSkillMod("healing_injury_speed");

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
		Reference<InjuryTreatmentTask*> task = new InjuryTreatmentTask(creature, message, "injuryTreatment");
		creature->addPendingTask("injuryTreatment", task, delay * 1000);
	}

	void doAnimations(CreatureObject* creature, CreatureObject* creatureTarget) const {
		creatureTarget->playEffect("clienteffect/healing_healdamage.cef", "");

		if (creature == creatureTarget)
			creature->doAnimation("heal_self");
		else
			creature->doAnimation("heal_other");
	}

	void doAnimationsRange(CreatureObject* creature, CreatureObject* creatureTarget, int oid, float range) const {
		String crc;

		if (range < 10.0f) {
			crc = "throw_grenade_near_healing";
		}
		else if (10.0f <= range && range < 20.0f) {
			crc = "throw_grenade_medium_healing";
		}
		else {
			crc = "throw_grenade_far_healing";
		}

		CombatAction* action = new CombatAction(creature, creatureTarget,  crc.hashCode(), 1, 0L);
		creature->broadcastMessage(action, true);
	}

	StimPack* findStimPack(CreatureObject* creature) const {
		SceneObject* inventory = creature->getSlottedObject("inventory");

		if (inventory == NULL)
			return NULL;

		int medicineUse = creature->getSkillMod("healing_ability");
		int combatMedicineUse = creature->getSkillMod("combat_healing_ability");

		bool melee = range <= 5.0f;

		for (int i = 0; i < inventory->getContainerObjectsSize(); ++i) {
			SceneObject* item = inventory->getContainerObject(i);

			if (!item->isPharmaceuticalObject())
				continue;

			PharmaceuticalObject* pharma = cast<PharmaceuticalObject*>(item);

			if (melee && pharma->isStimPack() && !pharma->isRangedStimPack() && !pharma->isPetStimPack() && !pharma->isDroidRepairKit()) {
				StimPack* stimPack = cast<StimPack*>(pharma);

				if (stimPack->getMedicineUseRequired() <= medicineUse)
					return stimPack;
			}

			if (pharma->isRangedStimPack()) {
				RangedStimPack* stimPack = cast<RangedStimPack*>(pharma);

				if (stimPack->getMedicineUseRequired() <= combatMedicineUse && stimPack->getRange(creature))
					return stimPack;
			}
		}

		return NULL;
	}

	bool checkTarget(CreatureObject* creature, CreatureObject* creatureTarget) const {
		if (!creatureTarget->hasDamage(CreatureAttribute::HEALTH) && !creatureTarget->hasDamage(CreatureAttribute::ACTION)) {
			return false;
		}

		PlayerManager* playerManager = server->getPlayerManager();

		if (creature != creatureTarget && !CollisionManager::checkLineOfSight(creature, creatureTarget)) {
			return false;
		}

		if (creatureTarget->isDead())
			return false;

		return true;
	}

	bool canPerformSkill(CreatureObject* creature, CreatureObject* creatureTarget, StimPack* stimPack, int mindCostNew) const {
		if (!creature->canTreatInjuries()) {
			creature->sendSystemMessage("@healing_response:healing_must_wait"); //You must wait before you can do that.
			return false;
		}

		if (stimPack == NULL) {
			creature->sendSystemMessage("@healing_response:healing_response_60"); //No valid medicine found.
			return false;
		}

		if (creature->getHAM(CreatureAttribute::MIND) < mindCostNew) {
			creature->sendSystemMessage("@healing_response:not_enough_mind"); //You do not have enough mind to do that.
			return false;
		}

		if (creature != creatureTarget && checkForArenaDuel(creatureTarget)) {
			creature->sendSystemMessage("@jedi_spam:no_help_target"); // You are not permitted to help that target.
			return false;
		}

		if (!creatureTarget->isHealableBy(creature)) {
			creature->sendSystemMessage("@healing:pvp_no_help"); //It would be unwise to help such a patient.
			return false;
		}

		Vector<byte> atts = stimPack->getAttributes();
		bool needsHeals = false;

		for (int i = 0; i < atts.size(); i++) {
			if (creatureTarget->hasDamage(atts.get(i)))
				needsHeals = true;
		}

		if (!needsHeals) {
			if (creatureTarget == creature) {
				creature->sendSystemMessage("@healing_response:healing_response_61"); //You have no damage to heal.
			} else if (creatureTarget->isPlayerCreature()) {
				StringIdChatParameter stringId("healing_response", "healing_response_63"); //%NT has no damage to heal.
				stringId.setTT(creatureTarget->getObjectID());
				creature->sendSystemMessage(stringId);
			} else {
				StringBuffer message;
				message << creatureTarget->getDisplayedName() << " has no damage to heal.";
				creature->sendSystemMessage(message.toString());
			}

			return false;
		}

		if (stimPack->isRangedStimPack()) {
			RangedStimPack* rangedStimPack = cast<RangedStimPack*>(stimPack);
			int combatMedicineUse = creature->getSkillMod("combat_healing_ability");

			if (rangedStimPack->getMedicineUseRequired() > combatMedicineUse || !rangedStimPack->getRange(creature)) {
				creature->sendSystemMessage("@error_message:insufficient_skill"); //You lack the skill to use this item.
				return false;

			}
		}

		return true;
	}

	void sendHealMessage(CreatureObject* creature, CreatureObject* creatureTarget, int healthDamage, int actionDamage, int mindDamage) const {
		if (!creature->isPlayerCreature())
			return;

		CreatureObject* player = cast<CreatureObject*>(creature);

		StringBuffer msgPlayer, msgTarget, msgBody, msgTail;

		if (healthDamage > 0 && actionDamage > 0 && mindDamage > 0) {
			msgBody << healthDamage << " health, " << actionDamage << " action, and " << mindDamage << " mind";
		} else if (healthDamage > 0 && actionDamage > 0) {
			msgBody << healthDamage << " health and " << actionDamage << " action";
		} else if (healthDamage > 0 && mindDamage > 0) {
			msgBody << healthDamage << " health and " << mindDamage << " mind";
		} else if (actionDamage > 0 && mindDamage > 0) {
			msgBody << actionDamage << " action and " << mindDamage << " mind";
		} else if (healthDamage > 0) {
			msgBody << healthDamage << " health";
		} else if (actionDamage > 0) {
			msgBody << actionDamage << " action";
		} else if (mindDamage > 0) {
			msgBody << mindDamage << " mind";
		} else {
			return; //No damage to heal.
		}

		msgTail << " damage.";

		if (creature == creatureTarget) {
			msgPlayer << "You heal yourself for " << msgBody.toString() << msgTail.toString();
			creature->sendSystemMessage(msgPlayer.toString());
		} else if (creatureTarget->isPlayerCreature()) {
			msgPlayer << "You heal " << creatureTarget->getFirstName() << " for " << msgBody.toString() << msgTail.toString();
			player->sendSystemMessage(msgPlayer.toString());

			msgTarget << player->getFirstName() << " heals you for " << msgBody.toString() << msgTail.toString();
			creatureTarget->sendSystemMessage(msgTarget.toString());
		} else {
			msgPlayer << "You heal " << creatureTarget->getDisplayedName() << " for " << msgBody.toString() << msgTail.toString();
			player->sendSystemMessage(msgPlayer.toString());
		}
	}

	void awardXp(CreatureObject* creature, const String& type, int power) const {
		if (!creature->isPlayerCreature())
			return;

		CreatureObject* player = cast<CreatureObject*>(creature);

		int amount = (int)round((float)power * 0.25f);

		if (amount <= 0)
			return;

		PlayerManager* playerManager = server->getZoneServer()->getPlayerManager();
		playerManager->awardExperience(player, type, amount, true);
	}

	void doAreaMedicActionTarget(CreatureObject* creature, CreatureObject* targetCreature, StimPack* stimPack) const {
		if (stimPack->isRangedStimPack()) {
			RangedStimPack* rangeStim = cast<RangedStimPack*>(stimPack);

			if (rangeStim == NULL)
				return;

			uint32 stimPower = rangeStim->calculatePower(creature, targetCreature);

			Vector<byte> atts = stimPack->getAttributes();
			int healthHealed = 0, actionHealed = 0, mindHealed = 0;
			bool notifyObservers = true;


			if (atts.contains(CreatureAttribute::HEALTH)) {
				healthHealed = targetCreature->healDamage(creature, CreatureAttribute::HEALTH, stimPower);
				notifyObservers = false;
			}

			if (atts.contains(CreatureAttribute::ACTION)) {
				if (notifyObservers) {
					actionHealed = targetCreature->healDamage(creature, CreatureAttribute::ACTION, stimPower);
					notifyObservers = false;
				} else {
					actionHealed = targetCreature->healDamage(creature, CreatureAttribute::ACTION, stimPower, true, false);
				}
			}

			if (atts.contains(CreatureAttribute::MIND)) {
				if (notifyObservers) {
					mindHealed = targetCreature->healDamage(creature, CreatureAttribute::MIND, stimPower);
				} else {
					mindHealed = targetCreature->healDamage(creature, CreatureAttribute::MIND, stimPower, true, false);
				}
			}

			if (creature->isPlayerCreature()) {
				PlayerManager* playerManager = server->getZoneServer()->getPlayerManager();
				playerManager->sendBattleFatigueMessage(creature, targetCreature);
			}

			sendHealMessage(creature, targetCreature, healthHealed, actionHealed, mindHealed);

			if (targetCreature != creature && !targetCreature->isPet())
				awardXp(creature, "medical", (healthHealed + actionHealed)); //No experience for healing yourself or pets.

			checkForTef(creature, targetCreature);
		}
	}

	void handleArea(CreatureObject* creature, CreatureObject* areaCenter, StimPack* pharma, float range) const {

		// TODO: Replace this with a CombatManager::getAreaTargets() call
		
		Zone* zone = creature->getZone();

		if (zone == NULL)
			return;

		try {
			//zone->rlock();

			CloseObjectsVector* closeObjectsVector = (CloseObjectsVector*) areaCenter->getCloseObjects();

			SortedVector<QuadTreeEntry*> closeObjects;
			closeObjectsVector->safeCopyReceiversTo(closeObjects, CloseObjectsVector::CREOTYPE);

			for (int i = 0; i < closeObjects.size(); i++) {
				SceneObject* object = static_cast<SceneObject*>( closeObjects.get(i));

				if (!object->isPlayerCreature() && !object->isPet())
					continue;

				if (object == areaCenter || object->isDroidObject())
					continue;

				if (areaCenter->getWorldPosition().distanceTo(object->getWorldPosition()) - object->getTemplateRadius() > range)
					continue;

				CreatureObject* creatureTarget = cast<CreatureObject*>( object);

				if (creatureTarget->isAttackableBy(creature))
					continue;

				if (!creatureTarget->isHealableBy(creature))
					continue;

				if (creature->isPlayerCreature() && object->getParentID() != 0 && creature->getParentID() != object->getParentID()) {
					Reference<CellObject*> targetCell = object->getParent().get().castTo<CellObject*>();

					if (targetCell != nullptr) {
						if (object->isPlayerCreature()) {
							ContainerPermissions* perms = targetCell->getContainerPermissions();

							if (!perms->hasInheritPermissionsFromParent()) {
								if (!targetCell->checkContainerPermission(creature, ContainerPermissions::WALKIN))
									continue;
							}
						}

						ManagedReference<SceneObject*> parentSceneObject = targetCell->getParent().get();

						if (parentSceneObject != nullptr) {
							BuildingObject* buildingObject = parentSceneObject->asBuildingObject();

							if (buildingObject != nullptr && !buildingObject->isAllowedEntry(creature))
								continue;
						}
					}
				}


				if (creature != creatureTarget && checkForArenaDuel(creatureTarget))
					continue;

				//zone->runlock();

				try {

					Locker crossLocker(creatureTarget, creature);

					if (checkTarget(creature, creatureTarget)) {
						doAreaMedicActionTarget(creature, creatureTarget, pharma);
					}

				} catch (Exception& e) {

				}

				//zone->rlock();

			}

			//zone->runlock();
		} catch (Exception& e) {
			//zone->runlock();
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
					creature->sendSystemMessage("@healing_response:healing_response_62"); //Target must be a player or a creature pet in order to heal damage. 
					return GENERALERROR;
				}
			}
		} else {
			object = creature;
		}

		CreatureObject* targetCreature = cast<CreatureObject*>( object.get());

		Locker clocker(targetCreature, creature);

		if ((targetCreature->isAiAgent() && !targetCreature->isPet()) || targetCreature->isDroidObject() || targetCreature->isVehicleObject() || targetCreature->isDead() || targetCreature->isRidingMount() || targetCreature->isAttackableBy(creature))
			targetCreature = creature;

		uint64 pharmaceuticalObjectID = 0;

		try {
			if (!arguments.isEmpty())
				pharmaceuticalObjectID = UnsignedLong::valueOf(arguments.toString());
		} catch (Exception& e) {

		}

		ManagedReference<StimPack*> stimPack;

		if (pharmaceuticalObjectID == 0) {
			stimPack = findStimPack(creature);
		} else {
			SceneObject* inventory = creature->getSlottedObject("inventory");

			if (inventory != NULL) {
				stimPack = inventory->getContainerObject(pharmaceuticalObjectID).castTo<StimPack*>();
			}
		}

		int mindCostNew = creature->calculateCostAdjustment(CreatureAttribute::FOCUS, mindCost);

		if (!canPerformSkill(creature, targetCreature, stimPack, mindCostNew))
			return GENERALERROR;

		float rangeToCheck = 7;

		if (stimPack->isRangedStimPack())
			rangeToCheck = (cast<RangedStimPack*>(stimPack.get()))->getRange();

		if(!checkDistance(creature, targetCreature, rangeToCheck))
			return TOOFAR;

		if (creature != targetCreature && !CollisionManager::checkLineOfSight(creature, targetCreature)) {
			creature->sendSystemMessage("@healing:no_line_of_sight"); // You cannot see your target.
			return GENERALERROR;
		}

		if (creature->isPlayerCreature() && targetCreature->getParentID() != 0 && creature->getParentID() != targetCreature->getParentID()) {
			Reference<CellObject*> targetCell = targetCreature->getParent().get().castTo<CellObject*>();

			if (targetCell != nullptr) {
				if (!targetCreature->isPlayerCreature()) {
					ContainerPermissions* perms = targetCell->getContainerPermissions();

					if (!perms->hasInheritPermissionsFromParent()) {
						if (!targetCell->checkContainerPermission(creature, ContainerPermissions::WALKIN)) {
							creature->sendSystemMessage("@combat_effects:cansee_fail"); // You cannot see your target.
							return GENERALERROR;
						}
					}
				}

				ManagedReference<SceneObject*> parentSceneObject = targetCell->getParent().get();

				if (parentSceneObject != nullptr) {
					BuildingObject* buildingObject = parentSceneObject->asBuildingObject();

					if (buildingObject != nullptr && !buildingObject->isAllowedEntry(creature)) {
						creature->sendSystemMessage("@combat_effects:cansee_fail"); // You cannot see your target.
						return GENERALERROR;
					}
				}
			}
		}

		uint32 stimPower = stimPack->calculatePower(creature, targetCreature);

		Vector<byte> atts = stimPack->getAttributes();
		int healthHealed = 0, actionHealed = 0, mindHealed = 0;
		bool notifyObservers = true;


		if (atts.contains(CreatureAttribute::HEALTH)) {
			healthHealed = targetCreature->healDamage(creature, CreatureAttribute::HEALTH, stimPower);
			notifyObservers = false;
		}

		if (atts.contains(CreatureAttribute::ACTION)) {
			if (notifyObservers) {
				actionHealed = targetCreature->healDamage(creature, CreatureAttribute::ACTION, stimPower);
				notifyObservers = false;
			} else {
				actionHealed = targetCreature->healDamage(creature, CreatureAttribute::ACTION, stimPower, true, false);
			}
		}

		if (atts.contains(CreatureAttribute::MIND)) {
			if (notifyObservers) {
				mindHealed = targetCreature->healDamage(creature, CreatureAttribute::MIND, stimPower);
			} else {
				mindHealed = targetCreature->healDamage(creature, CreatureAttribute::MIND, stimPower, true, false);
			}
		}

		if (creature->isPlayerCreature()) {
			PlayerManager* playerManager = server->getPlayerManager();
			playerManager->sendBattleFatigueMessage(creature, targetCreature);
		}

		sendHealMessage(creature, targetCreature, healthHealed, actionHealed, mindHealed);

		creature->inflictDamage(creature, CreatureAttribute::MIND, mindCostNew, false);

		Locker locker(stimPack);
		stimPack->decreaseUseCount();

		if (targetCreature != creature && !targetCreature->isPet())
			awardXp(creature, "medical", (healthHealed + actionHealed)); //No experience for healing yourself.

		if (targetCreature != creature)
			clocker.release();

		if (stimPack->isArea()) {
			handleArea(creature, targetCreature, stimPack, stimPack->getArea());
		}

		if (stimPack->isRangedStimPack()) {
			doAnimationsRange(creature, targetCreature, stimPack->getObjectID(), creature->getWorldPosition().distanceTo(targetCreature->getWorldPosition()));
		} else {
			doAnimations(creature, targetCreature);
		}

		deactivateInjuryTreatment(creature, stimPack->isRangedStimPack());

		creature->notifyObservers(ObserverEventType::MEDPACKUSED);

		checkForTef(creature, targetCreature);

		return SUCCESS;
	}

};

#endif //HEALDAMAGECOMMAND_H_
