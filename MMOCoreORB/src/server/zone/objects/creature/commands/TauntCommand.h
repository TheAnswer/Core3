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

#ifndef TAUNTCOMMAND_H_
#define TAUNTCOMMAND_H_

#include "server/zone/objects/scene/SceneObject.h"

class TauntCommand : public CombatQueueCommand {
public:

	TauntCommand(const String& name, ZoneProcessServer* server)
		: CombatQueueCommand(name, server) {

	}

	int doQueueCommand(CreatureObject* creature, const uint64& target, const UnicodeString& arguments) const {

		if (!checkStateMask(creature))
			return INVALIDSTATE;

		if (!checkInvalidLocomotions(creature))
			return INVALIDLOCOMOTION;

		ManagedReference<SceneObject*> targetObject = creature->getZoneServer()->getObject(target);

		if (targetObject == NULL || !targetObject->isCreatureObject() || targetObject->isPlayerCreature())
			return INVALIDTARGET;

		CreatureObject* targetCreature = cast<CreatureObject*>(targetObject.get());

		if (targetCreature == NULL)
			return INVALIDTARGET;

		if (!targetCreature->isAttackableBy(creature))
			return INVALIDTARGET;

		int res = doCombatAction(creature, target);

		CombatManager* combatManager = CombatManager::instance();

		if (res == SUCCESS) {
			targetCreature->getThreatMap()->addAggro(creature, creature->getSkillMod("taunt") * 10, 0);
			targetCreature->getThreatMap()->setThreatState(creature, ThreatStates::TAUNTED,(uint64)creature->getSkillMod("taunt") / 10, (uint64)creature->getSkillMod("taunt") / 10);
			//creature->doCombatAnimation(creature,String("taunt").hashCode(),0,0xFF);
			creature->doAnimation("taunt");

			if (creature->isPlayerCreature())
				creature->sendSystemMessage("@cbt_spam:taunt_success_single");

		} else {

			if (creature->isPlayerCreature())
				creature->sendSystemMessage("@cbt_spam:taunt_fail_single");
		}

		return res;
	}

	void sendAttackCombatSpam(TangibleObject* attacker, TangibleObject* defender, int attackResult, int damage, const CreatureAttackData& data) const {
		if (attacker == NULL)
			return;

		Zone* zone = attacker->getZone();
		if (zone == NULL)
			return;

		String stringName = data.getCombatSpam();
		byte color = 1;

		switch (attackResult) {
		case CombatManager::HIT:
			stringName += "_success";
			break;
		case CombatManager::MISS:
		case CombatManager::DODGE:
		case CombatManager::COUNTER:
		case CombatManager::BLOCK:
		case CombatManager::RICOCHET:
			stringName += "_fail";
			color = 0;
			break;
		default:
			break;
		}

		CombatManager::instance()->broadcastCombatSpam(attacker, NULL, NULL, damage, "cbt_spam", stringName, color);

	}

};

#endif //TAUNTCOMMAND_H_
