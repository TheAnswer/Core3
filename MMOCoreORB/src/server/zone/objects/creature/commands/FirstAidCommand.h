/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef FIRSTAIDCOMMAND_H_
#define FIRSTAIDCOMMAND_H_

#include "server/zone/objects/building/BuildingObject.h"
#include "server/zone/objects/scene/SceneObject.h"

class FirstAidCommand : public QueueCommand {
	float mindCost;
	float range;
public:

	FirstAidCommand(const String& name, ZoneProcessServer* server)
		: QueueCommand(name, server) {

		mindCost = 0;
		range = 6;
	}

	void doAnimations(CreatureObject* creature, CreatureObject* creatureTarget) const {
		creatureTarget->playEffect("clienteffect/healing_healdamage.cef", "");

		if (creature == creatureTarget)
			creature->doAnimation("heal_self");
		else
			creature->doAnimation("heal_other");
	}

	void sendCureMessage(CreatureObject* object, CreatureObject* target) const {
		if (!object->isPlayerCreature())
			return;

		CreatureObject* creature = cast<CreatureObject*>( object);
		CreatureObject* creatureTarget = cast<CreatureObject*>( target);

		if (creatureTarget != creature) {
			StringBuffer msgPlayer;
			if (creatureTarget->isPlayerCreature()) {
				StringBuffer msgTarget;
				msgTarget << creature->getFirstName() << " applies first aid to you.";
				creatureTarget->sendSystemMessage(msgTarget.toString());

				msgPlayer << "You apply first aid to " << creatureTarget->getFirstName() << ".";
				creature->sendSystemMessage(msgPlayer.toString());
			} else {
				msgPlayer << "You apply first aid to " << creatureTarget->getDisplayedName() << ".";
				creature->sendSystemMessage(msgPlayer.toString());
			}
		} else {
			creature->sendSystemMessage("@healing_response:first_aid_self"); //You apply first aid to yourself.
		}
	}

	bool canPerformSkill(CreatureObject* creature, CreatureObject* creatureTarget) const {
		if (!creatureTarget->isBleeding()) {
			if (creature == creatureTarget)
				creature->sendSystemMessage("@healing_response:healing_response_78"); //You are not bleeding.
			else if (creatureTarget->isPlayerCreature()) {
				StringIdChatParameter stringId("healing_response", "healing_response_80"); //%NT is not bleeding.
				stringId.setTT(creatureTarget->getObjectID());
				creature->sendSystemMessage(stringId);
			} else {
				StringBuffer message;
				message << creatureTarget->getDisplayedName() << " is not bleeding.";
				creature->sendSystemMessage(message.toString());
			}
			return false;
		}

		if (creature != creatureTarget && !CollisionManager::checkLineOfSight(creature, creatureTarget)) {
			creature->sendSystemMessage("@healing:no_line_of_sight"); // You cannot see your target.
			return false;
		}

		return true;
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
					creature->sendSystemMessage("@healing_response:healing_response_79"); //Target must be a player or a creature pet in order to apply first aid.
					return GENERALERROR;
				}
			}
		} else {
			object = creature;
		}

		CreatureObject* creatureTarget = cast<CreatureObject*>( object.get());

		Locker clocker(creatureTarget, creature);

		if ((creatureTarget->isAiAgent() && !creatureTarget->isPet()) || creatureTarget->isDroidObject() || creatureTarget->isDead() || creatureTarget->isRidingMount() || creatureTarget->isAttackableBy(creature))
			creatureTarget = creature;

		if(!checkDistance(creature, creatureTarget, range))
			return TOOFAR;

		if (creature != creatureTarget && checkForArenaDuel(creatureTarget)) {
			creature->sendSystemMessage("@jedi_spam:no_help_target"); // You are not permitted to help that target.
			return GENERALERROR;
		}

		if (!creatureTarget->isHealableBy(creature)) {
			creature->sendSystemMessage("@healing:pvp_no_help"); //It would be unwise to help such a patient.
			return GENERALERROR;
		}

		if (creature->isPlayerCreature() && creatureTarget->getParentID() != 0 && creature->getParentID() != creatureTarget->getParentID()) {
			Reference<CellObject*> targetCell = creatureTarget->getParent().get().castTo<CellObject*>();

			if (targetCell != nullptr) {
				if (!creatureTarget->isPlayerCreature()) {
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


		if (creature->getHAM(CreatureAttribute::MIND) < mindCost) {
			creature->sendSystemMessage("@healing_response:not_enough_mind"); //You do not have enough mind to do that.
			return GENERALERROR;
		}

		if (!canPerformSkill(creature, creatureTarget))
			return GENERALERROR;

		uint32 skillMod = creature->getSkillMod("healing_injury_treatment");

		creatureTarget->healDot(CreatureState::BLEEDING, skillMod*3);

		creature->inflictDamage(creature, CreatureAttribute::MIND, mindCost, false);

		sendCureMessage(creature, creatureTarget);

		doAnimations(creature, creatureTarget);

		checkForTef(creature, creatureTarget);

		return SUCCESS;
	}

};

#endif //FIRSTAIDCOMMAND_H_
