/*
Copyright (C) 2007 <SWGEmu>

This File is part of Core3.

This program is free software; you can redistribute
it and/or modify it under the terms of the GNU Lesser
General Public License as published by the Free Software
Foundation; either version 2 of the License,
or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU Lesser General Public License for
more details.

You should have received a copy of the GNU Lesser General
Public License along with this program; if not, write to
the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

Linking Engine3 statically or dynamically with other modules
is making a combined work based on Engine3.
Thus, the terms and conditions of the GNU Lesser General Public License
cover the whole combination.

In addition, as a special exception, the copyright holders of Engine3
give you permission to combine Engine3 program with free software
programs or libraries that are released under the GNU LGPL and with
code included in the standard release of Core3 under the GNU LGPL
license (or modified versions of such code, with unchanged license).
You may copy and distribute such a system following the terms of the
GNU LGPL for Engine3 and the licenses of the other code concerned,
provided that you include the source code of that other code when
and as the GNU LGPL requires distribution of source code.

Note that people who make modified versions of Engine3 are not obligated
to grant this special exception for their modified versions;
it is their choice whether to do so. The GNU Lesser General Public License
gives permission to release a modified version without this exception;
this exception also makes it possible to release a modified version
which carries forward this exception.
*/

#ifndef HEALDROIDDAMAGECOMMAND_H_
#define HEALDROIDDAMAGECOMMAND_H_

#include "server/zone/objects/scene/SceneObject.h"
#include "server/zone/objects/creature/DroidObject.h"
#include "server/zone/objects/tangible/pharmaceutical/StimPack.h"

class HealDroidDamageCommand : public QueueCommand {
	float range;
	float mindCost;

public:

	HealDroidDamageCommand(const String& name, ZoneProcessServer* server)
		: QueueCommand(name, server) {

		range = 5;
		mindCost = 50;
	}

	void deactivateInjuryTreatment(CreatureObject* creature) {
		int delay = 20;

		StringIdChatParameter message("healing_response", "healing_response_58"); //You are now ready to heal more damage.
		Reference<InjuryTreatmentTask*> task = new InjuryTreatmentTask(creature, message, "injuryTreatment");
		creature->addPendingTask("injuryTreatment", task, delay * 1000);
	}

	void doAnimations(CreatureObject* creature, CreatureObject* droid) {
		droid->playEffect("clienteffect/healing_healdamage.cef", "");

		creature->doAnimation("heal_other");
	}

	StimPack* findStimPack(CreatureObject* creature) {
		SceneObject* inventory = creature->getSlottedObject("inventory");

		if (inventory != NULL) {
			for (int i = 0; i < inventory->getContainerObjectsSize(); ++i) {
				SceneObject* item = inventory->getContainerObject(i);

				if (!item->isTangibleObject())
					continue;

				TangibleObject* tano = cast<TangibleObject*>( item);

				if (tano->isPharmaceuticalObject()) {
					PharmaceuticalObject* pharma = cast<PharmaceuticalObject*>( tano);

					if (pharma->isDroidRepairKit()) {
						StimPack* stimPack = cast<StimPack*>( pharma);

						return stimPack;
					}

				}
			}
		}

		return NULL;
	}

	bool canPerformSkill(CreatureObject* creature, CreatureObject* droid, StimPack* stimPack) {
		if (!creature->canTreatInjuries()) {
			creature->sendSystemMessage("@healing_response:healing_must_wait"); //You must wait before you can do that.
			return false;
		}

		if (stimPack == NULL) {
			creature->sendSystemMessage("No valid droid repair kit found.");
			return false;
		}

		if (creature->isProne() || creature->isMeditating()) {
			creature->sendSystemMessage("@error_message:wrong_state"); //You cannot complete that action while in your current state.
			return false;
		}

		if (creature->isRidingMount()) {
			creature->sendSystemMessage("@error_message:survey_on_mount"); //You cannot perform that action while mounted on a creature or driving a vehicle.
			return false;
		}

		if (creature->getHAM(CreatureAttribute::MIND) < mindCost) {
			creature->sendSystemMessage("@healing_response:not_enough_mind"); //You do not have enough mind to do that.
			return false;
		}

		if (!droid->isHealableBy(creature)) {
			creature->sendSystemMessage("@healing:pvp_no_help"); //It would be unwise to help such a patient.
			return false;
		}

		if (!droid->hasDamage(CreatureAttribute::HEALTH) && !droid->hasDamage(CreatureAttribute::ACTION) && !droid->hasDamage(CreatureAttribute::MIND)) {
			StringBuffer message;
			message << droid->getDisplayedName() << " has no damage to heal.";
			creature->sendSystemMessage(message.toString());
			return false;
		}

		if (!CollisionManager::checkLineOfSight(creature, droid)) {
			creature->sendSystemMessage("@container_error_message:container18");
			return false;
		}

		return true;
	}

	void sendHealMessage(CreatureObject* creature, DroidObject* droid, int healthDamage, int actionDamage, int mindDamage) {
		if (!creature->isPlayerCreature())
			return;

		StringIdChatParameter stringId("healing", "droid_repair_damage_self"); // You have repaired %TO and healed a total of %DI point of damage.
		stringId.setTO(droid);
		stringId.setDI(healthDamage + actionDamage + mindDamage);
		creature->sendSystemMessage(stringId);

		ManagedReference<CreatureObject*> droidOwner = droid->getLinkedCreature().get();

		if (droidOwner != NULL && droidOwner != creature) {
			StringIdChatParameter stringId("healing", "droid_repair_damage_other"); // %TT has repaired %TO and healed a total of %DI point of damage.
			stringId.setTT(creature);
			stringId.setDI(healthDamage + actionDamage + mindDamage);
			droidOwner->sendSystemMessage(stringId);
		}
	}

	int doQueueCommand(CreatureObject* creature, const uint64& target, const UnicodeString& arguments) {

		if (!checkStateMask(creature))
			return INVALIDSTATE;

		if (!checkInvalidLocomotions(creature))
			return INVALIDLOCOMOTION;

		ManagedReference<SceneObject*> object = server->getZoneServer()->getObject(target);

		if (object == NULL || !object->isDroidObject()) {
			creature->sendSystemMessage("Invalid Target.");
			return GENERALERROR;
		}

		DroidObject* droid = cast<DroidObject*>( object.get());

		Locker clocker(droid, creature);

		if (!droid->isPet() || droid->isDead() || droid->isAttackableBy(creature)) {
			creature->sendSystemMessage("Invalid Target.");
			return GENERALERROR;
		}

		if (!creature->isInRange(droid, range + droid->getTemplateRadius() + creature->getTemplateRadius()))
			return TOOFAR;

		uint64 objectID = 0;

		try {
			if (!arguments.isEmpty())
				objectID = UnsignedLong::valueOf(arguments.toString());
		} catch (Exception& e) {

		}

		ManagedReference<StimPack*> stimPack = NULL;

		if (objectID == 0) {
			stimPack = findStimPack(creature);
		} else {
			SceneObject* inventory = creature->getSlottedObject("inventory");

			if (inventory != NULL) {
				stimPack = inventory->getContainerObject(objectID).castTo<StimPack*>();
			}
		}

		if (!canPerformSkill(creature, droid, stimPack))
			return GENERALERROR;

		uint32 stimPower = 0;
		stimPower = stimPack->calculatePower(creature, droid, false);

		int healthHealed = droid->healDamage(creature, CreatureAttribute::HEALTH, stimPower);
		int actionHealed = droid->healDamage(creature, CreatureAttribute::ACTION, stimPower, true, false);
		int mindHealed = droid->healDamage(creature, CreatureAttribute::MIND, stimPower, true, false);

		sendHealMessage(creature, droid, healthHealed, actionHealed, mindHealed);

		creature->inflictDamage(creature, CreatureAttribute::MIND, mindCost, false);
		stimPack->decreaseUseCount();

		doAnimations(creature, droid);

		deactivateInjuryTreatment(creature);

		return SUCCESS;
	}

};

#endif //HEALDROIDDAMAGECOMMAND_H_
