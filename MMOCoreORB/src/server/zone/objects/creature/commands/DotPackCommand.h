/*
 * DotPackCommand.h
 *
 *  Created on: 11/08/2010
 *      Author: victor
 */

#ifndef DOTPACKCOMMAND_H_
#define DOTPACKCOMMAND_H_

#include "server/zone/objects/scene/SceneObject.h"
#include "server/zone/objects/tangible/pharmaceutical/DotPack.h"
#include "server/zone/ZoneServer.h"
#include "server/zone/Zone.h"
#include "server/zone/managers/player/PlayerManager.h"
#include "server/zone/managers/collision/CollisionManager.h"
#include "server/zone/objects/creature/events/InjuryTreatmentTask.h"
#include "server/zone/objects/creature/buffs/Buff.h"
#include "server/zone/objects/creature/BuffAttribute.h"
#include "server/zone/objects/creature/buffs/DelayedBuff.h"
#include "server/zone/packets/object/CombatAction.h"
#include "QueueCommand.h"
#include "server/zone/managers/combat/CombatManager.h"
#include "server/zone/objects/tangible/threat/ThreatMap.h"

class DotPackCommand : public QueueCommand {
protected:
	String effectName;
	String skillName;
public:
	DotPackCommand(const String& name, ZoneProcessServer* server)
		: QueueCommand(name, server) {
		effectName = "clienteffect/healing_healenhance.cef";
		//defaultTime = 0;
	}

	void doAnimationsRange(CreatureObject* creature, CreatureObject* creatureTarget, uint64 oid, float range, bool area) {
		String crc;

		if (range < 10.0f) {
			if (area)
				crc = "throw_grenade_near_area_poison";
			else
				crc = "throw_grenade_near_poison";
		}
		else if (10.0f <= range && range < 20.f) {
			if (area)
				crc = "throw_grenade_medium_area_poison";
			else
				crc = "throw_grenade_medium_poison";
		}
		else {
			if (area)
				crc = "throw_grenade_far_area_poison";
			else
				crc = "throw_grenade_far_poison";
		}

		CombatAction* action = new CombatAction(creature, creatureTarget,  crc.hashCode(), 1, 0L);

		creature->broadcastMessage(action, true);
	}

	void parseModifier(const String& modifier, uint64& objectId) {
		if (!modifier.isEmpty())
			objectId = Long::valueOf(modifier);
		else
			objectId = 0;
	}

	bool checkTarget(CreatureObject* creature, CreatureObject* targetCreature) {
		if (!targetCreature->isAttackableBy(creature))
			return false;
		/*else if (targetCreature->isPlayingMusic())
			targetCreature->stopPlayingMusic();
		else if (targetCreature->isDancing())
			targetCreature->stopDancing();
		 */

		PlayerManager* playerManager = server->getPlayerManager();

		if (creature != targetCreature && !CollisionManager::checkLineOfSight(creature, targetCreature)) {
			return false;
		}

		return true;
	}

	void awardXp(CreatureObject* creature, const String& type, int power) {
		if (!creature->isPlayerCreature())
			return;

		CreatureObject* player = cast<CreatureObject*>(creature);

		int amount = (int)round((float)power);

		if (amount <= 0)
			return;

		PlayerManager* playerManager = server->getZoneServer()->getPlayerManager();
		playerManager->awardExperience(player, type, amount, true);
	}

	void handleArea(CreatureObject* creature, CreatureObject* areaCenter, DotPack* pharma,
			float range) {

		Zone* zone = creature->getZone();

		if (zone == NULL)
			return;

		try {
			SortedVector<ManagedReference<QuadTreeEntry*> > closeObjects;
			CloseObjectsVector* vec = (CloseObjectsVector*) areaCenter->getCloseObjects();
			vec->safeCopyTo(closeObjects);

			for (int i = 0; i < closeObjects.size(); i++) {
				SceneObject* object = cast<SceneObject*>( closeObjects.get(i).get());

				if (!object->isCreatureObject())
					continue;

				if (object == areaCenter || object == creature)
					continue;

				if (!areaCenter->isInRange(object, range))
					continue;

				CreatureObject* creatureTarget = cast<CreatureObject*>( object);

				try {
					Locker crossLocker(creatureTarget, creature);

					if (!creatureTarget->isAttackableBy(creature))
						continue;

					if (checkTarget(creature, creatureTarget)) {
						doAreaMedicActionTarget(creature, creatureTarget, pharma);
					}

				} catch (Exception& e) {
				}
			}
		} catch (Exception& e) {
		}
	}

	void doAreaMedicActionTarget(CreatureObject* creature, CreatureObject* creatureTarget, DotPack* pharma)	{
		DotPack* dotPack = NULL;

		if (pharma->isPoisonDeliveryUnit() || pharma->isDiseaseDeliveryUnit())
			dotPack = cast<DotPack*>( pharma);

		int dotPower = dotPack->calculatePower(creature);

		//sendDotMessage(creature, creatureTarget, dotPower);

		int dotDMG = 0;
		if (dotPack->isPoisonDeliveryUnit()) {
			StringIdChatParameter stringId("healing", "apply_poison_self");
			stringId.setTT(creatureTarget->getObjectID());

			creature->sendSystemMessage(stringId);

			StringIdChatParameter stringId2("healing", "apply_poison_other");
			stringId2.setTU(creature->getObjectID());

			creatureTarget->sendSystemMessage(stringId2);

			dotDMG = creatureTarget->addDotState(CreatureState::POISONED, dotPack->getServerObjectCRC(), dotPower, dotPack->getPool(), dotPack->getDuration(), dotPack->getPotency(), creatureTarget->getSkillMod("resistance_poison") + creatureTarget->getSkillMod("poison_disease_resist"));
		} else {
			StringIdChatParameter stringId("healing", "apply_disease_self");
			stringId.setTT(creatureTarget->getObjectID());

			creature->sendSystemMessage(stringId);

			StringIdChatParameter stringId2("healing", "apply_disease_other");
			stringId2.setTU(creature->getObjectID());

			creatureTarget->sendSystemMessage(stringId2);

			dotDMG = creatureTarget->addDotState(CreatureState::DISEASED, dotPack->getServerObjectCRC(), dotPower, dotPack->getPool(), dotPack->getDuration(), dotPack->getPotency(), creatureTarget->getSkillMod("resistance_disease") + creatureTarget->getSkillMod("poison_disease_resist"));
		}

		if (dotDMG) {
			awardXp(creature, "medical", dotDMG); //No experience for healing yourself.

			creatureTarget->addDefender(creature);
			creatureTarget->getThreatMap()->addDamage(creature, dotDMG, "");
			creature->addDefender(creatureTarget);
		} else {
			StringIdChatParameter stringId("dot_message", "dot_resisted");
			stringId.setTT(creatureTarget->getObjectID());

			creature->sendSystemMessage(stringId);
		}
	}

	int hasCost(CreatureObject* creature) {
		if (!creature->isPlayerCreature())
			return 0;

		CreatureObject* player = cast<CreatureObject*>(creature);

		int wpnMind = 150;

		int mindAttackCost = wpnMind - (wpnMind * creature->getHAM(CreatureAttribute::FOCUS) / 1500);

		if (mindAttackCost < 0)
			mindAttackCost = 0;

		if (player->getHAM(CreatureAttribute::MIND) < mindAttackCost)
			return -1;

		return mindAttackCost;
	}

	void applyCost(CreatureObject* creature, int mindDamage) {
		if (mindDamage == 0)
			return;

		creature->inflictDamage(creature, CreatureAttribute::MIND, mindDamage, false);
	}

	int doQueueCommand(CreatureObject* creature, const uint64& target, const UnicodeString& arguments) {

		if (!checkStateMask(creature))
			return INVALIDSTATE;

		if (!checkInvalidLocomotions(creature))
			return INVALIDLOCOMOTION;

		int cost = hasCost(creature);

		if (cost < 0)
			return INSUFFICIENTHAM;

		ManagedReference<SceneObject*> object = server->getZoneServer()->getObject(target);

		if (object != NULL && !object->isCreatureObject()) {
			return INVALIDTARGET;
		} else if (object == NULL)
			return INVALIDTARGET;

		if (creature == object)
			return INVALIDTARGET;

		uint64 objectId = 0;

		parseModifier(arguments.toString(), objectId);
		ManagedReference<DotPack*> dotPack = NULL;

		SceneObject* inventory = creature->getSlottedObject("inventory");

		if (inventory != NULL) {
			dotPack = inventory->getContainerObject(objectId).castTo<DotPack*>();
		}

		if (dotPack == NULL)
			return GENERALERROR;

		CreatureObject* creatureTarget = cast<CreatureObject*>( object.get());

		PlayerManager* playerManager = server->getPlayerManager();

		if (creature != creatureTarget && !CollisionManager::checkLineOfSight(creature, creatureTarget)) {
			creature->sendSystemMessage("@container_error_message:container18");
			return GENERALERROR;
		}

		int	range = int(dotPack->getRange() + creature->getSkillMod("healing_range") / 100 * 14);

		if (creature != creatureTarget && !creature->isInRange(creatureTarget, range)){
			creature->sendSystemMessage("@error_message:target_out_of_range"); //Your target is out of range for this action.
			return TOOFAR;
		}
		//timer
		if (!creature->checkCooldownRecovery(skillName)) {
			creature->sendSystemMessage("@healing_response:healing_must_wait"); //You must wait before you can do that.

			return GENERALERROR;
		} else {
			float modSkill = (float)creature->getSkillMod("healing_range_speed");
			int delay = (int)round(12.0f - (6.0f * modSkill / 100 ));

			if (creature->hasBuff(BuffCRC::FOOD_HEAL_RECOVERY)) {
				DelayedBuff* buff = cast<DelayedBuff*>( creature->getBuff(BuffCRC::FOOD_HEAL_RECOVERY));

				if (buff != NULL) {
					float percent = buff->getSkillModifierValue("heal_recovery");

					delay = round(delay * (100.0f - percent) / 100.0f);
				}
			}

			//Force the delay to be at least 4 seconds.
			delay = (delay < 4) ? 4 : delay;

			creature->addCooldown(skillName, delay * 1000);
		}

		Locker clocker(creatureTarget, creature);

		if (!CombatManager::instance()->startCombat(creature, creatureTarget))
			return INVALIDTARGET;

		applyCost(creature, cost);

		int dotPower = dotPack->calculatePower(creature);

		int dotDMG = 0;
		if (dotPack->isPoisonDeliveryUnit()) {
			StringIdChatParameter stringId("healing", "apply_poison_self");
			stringId.setTT(creatureTarget->getObjectID());

			creature->sendSystemMessage(stringId);

			StringIdChatParameter stringId2("healing", "apply_poison_other");
			stringId2.setTU(creature->getObjectID());

			creatureTarget->sendSystemMessage(stringId2);

			dotDMG = creatureTarget->addDotState(CreatureState::POISONED, dotPack->getServerObjectCRC(), dotPower, dotPack->getPool(), dotPack->getDuration(), dotPack->getPotency(), creatureTarget->getSkillMod("resistance_poison") + creatureTarget->getSkillMod("poison_disease_resist"));
		} else {
			StringIdChatParameter stringId("healing", "apply_disease_self");
			stringId.setTT(creatureTarget->getObjectID());

			creature->sendSystemMessage(stringId);

			StringIdChatParameter stringId2("healing", "apply_disease_other");
			stringId2.setTU(creature->getObjectID());

			creatureTarget->sendSystemMessage(stringId2);

			dotDMG = creatureTarget->addDotState(CreatureState::DISEASED, dotPack->getServerObjectCRC(), dotPower, dotPack->getPool(), dotPack->getDuration(), dotPack->getPotency(), creatureTarget->getSkillMod("resistance_disease") + creatureTarget->getSkillMod("poison_disease_resist"));
		}

		if (dotDMG) {
			awardXp(creature, "medical", dotDMG); //No experience for healing yourself.
			creatureTarget->getThreatMap()->addDamage(creature, dotDMG, "");
		} else {
			StringIdChatParameter stringId("dot_message", "dot_resisted");
			stringId.setTT(creatureTarget->getObjectID());

			creature->sendSystemMessage(stringId);
		}

		if (dotPack->isArea()) {
			if (creatureTarget != creature)
				clocker.release();

			handleArea(creature, creatureTarget, dotPack, dotPack->getArea());
		}

		if (dotPack != NULL)
			dotPack->decreaseUseCount();

		doAnimationsRange(creature, creatureTarget, dotPack->getObjectID(), creature->getDistanceTo(creatureTarget), dotPack->isArea());

		creature->notifyObservers(ObserverEventType::MEDPACKUSED);

		return SUCCESS;
	}

};


#endif /* DOTPACKCOMMAND_H_ */
