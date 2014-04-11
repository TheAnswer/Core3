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
#include "ForceHealQueueCommand.h"

class TotalHealSelfCommand : public ForceHealQueueCommand {

public:
	TotalHealSelfCommand(const String& name, ZoneProcessServer* server)
: ForceHealQueueCommand(name, server) {

	}

	bool canPerformSkill(CreatureObject* creature) {
		if (!creature->getWounds(CreatureAttribute::HEALTH) && !creature->getWounds(CreatureAttribute::STRENGTH) && !creature->getWounds(CreatureAttribute::CONSTITUTION) && !creature->getWounds(CreatureAttribute::ACTION) && !creature->getWounds(CreatureAttribute::QUICKNESS) && !creature->getWounds(CreatureAttribute::STAMINA) && !creature->getWounds(CreatureAttribute::MIND) && !creature->getWounds(CreatureAttribute::FOCUS) && !creature->getWounds(CreatureAttribute::WILLPOWER) && !creature->hasDamage(CreatureAttribute::HEALTH) && !creature->hasDamage(CreatureAttribute::ACTION) && !creature->hasDamage(CreatureAttribute::MIND) && !creature->hasState(CreatureState::STUNNED) && !creature->hasState(CreatureState::DIZZY) && !creature->hasState(CreatureState::INTIMIDATED) && !creature->hasState(CreatureState::BLINDED)) {
			creature->sendSystemMessage("@jedi_spam:no_damage_heal_self"); // You have no damage of that type.
			return false;
		}

		return true;
	}	

	int doQueueCommand(CreatureObject* creature, const uint64& target, const UnicodeString& arguments) {

		if (!checkStateMask(creature))
			return INVALIDSTATE;

		if (!checkInvalidLocomotions(creature))
			return INVALIDLOCOMOTION;

		if (isWearingArmor(creature)) {
			return NOJEDIARMOR;
		}

		if (isWarcried(creature)) {
			return GENERALERROR;
		}


		if (!checkForceCost(creature)) {
			creature->sendSystemMessage("@jedi_spam:no_force_power");
			return GENERALERROR;
		}

		// At this point, the player has enough Force... Can they perform skill?

		if (!canPerformSkill(creature))
			return GENERALERROR;


		int forceCostDeducted = forceCost;

		// Lets see how much healing they are doing.
		int healAmount = 1500;
		int healDotAmount = 300;
		int healWoundAmount = 500;

		uint32 healDisease = creature->healDot(CreatureState::DISEASED, healDotAmount);
		uint32 healPoison = creature->healDot(CreatureState::POISONED, healDotAmount);
		uint32 healBleeding = creature->healDot(CreatureState::BLEEDING, healDotAmount);
		uint32 healOnFire = creature->healDot(CreatureState::ONFIRE, healDotAmount);

		uint32 healedHealthWound = creature->healWound(creature, CreatureAttribute::HEALTH, healWoundAmount);
		uint32 healedStrengthWound = creature->healWound(creature, CreatureAttribute::STRENGTH, healWoundAmount);
		uint32 healedConstitutionWound = creature->healWound(creature, CreatureAttribute::CONSTITUTION, healWoundAmoun);

		uint32 healedActionWound = creature->healWound(creature, CreatureAttribute::ACTION, healWoundAmoun);
		uint32 healedQuicknessWound = creature->healWound(creature, CreatureAttribute::QUICKNESS, healWoundAmoun);
		uint32 healedStaminaWound = creature->healWound(creature, CreatureAttribute::STAMINA, healWoundAmoun);

		uint32 healedMindWound = creature->healWound(creature, CreatureAttribute::MIND, healWoundAmoun);
		uint32 healedFocusWound = creature->healWound(creature, CreatureAttribute::FOCUS, healWoundAmoun);
		uint32 healedWillpowerWound = creature->healWound(creature, CreatureAttribute::WILLPOWER, healWoundAmoun);

		uint32 healthHealed = creature->healDamage(creature, CreatureAttribute::HEALTH, healAmount);
		uint32 actionHealed = creature->healDamage(creature, CreatureAttribute::ACTION, healAmount);
		uint32 mindHealed = creature->healDamage(creature, CreatureAttribute::MIND, healAmount);

		creature->removeStateBuff(CreatureState::STUNNED);
		creature->removeStateBuff(CreatureState::DIZZY);
		creature->removeStateBuff(CreatureState::BLINDED);
		creature->removeStateBuff(CreatureState::INTIMIDATED);

		healBattleFatigue(creature, 1000);

		forceCostDeducted = MIN(((healthHealed + actionHealed + mindHealed + healedHealthWound + healedStrengthWound + healedConstitutionWound + healedActionWound + healedQuicknessWound + healedStaminaWound + healedMindWound + healedFocusWound + healedWillpowerWound + healDisease + healPoison + healBleeding + healOnFire) / 20), forceCost);

		doAnimations(creature, creature);

		sendHealMessage(creature, creature, healthHealed, actionHealed, mindHealed);

		sendWoundMessage(creature, healedHealthWound, healedActionWound,  healedMindWound, healedStrengthWound, healedConstitutionWound, healedQuicknessWound, healedStaminaWound, healedFocusWound, healedWillpowerWound);

		ManagedReference<PlayerObject*> playerObject = creature->getPlayerObject();

		if (playerObject != NULL)
			playerObject->setForcePower(playerObject->getForcePower() - forceCostDeducted);

		return SUCCESS;
	}

};

#endif /* TOTALHEALSELFCOMMAND_H_ */
