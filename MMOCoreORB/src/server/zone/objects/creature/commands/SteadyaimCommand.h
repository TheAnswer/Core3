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

#ifndef STEADYAIMCOMMAND_H_
#define STEADYAIMCOMMAND_H_

#include "server/zone/objects/scene/SceneObject.h"
#include "SquadLeaderCommand.h"

class SteadyaimCommand : public SquadLeaderCommand {
public:

	SteadyaimCommand(const String& name, ZoneProcessServer* server)
		: SquadLeaderCommand(name, server) {
	}

	int doQueueCommand(CreatureObject* creature, const uint64& target, const UnicodeString& arguments) {

		if (!checkStateMask(creature))
			return INVALIDSTATE;

		if (!checkInvalidLocomotions(creature))
			return INVALIDLOCOMOTION;

		if (!creature->isPlayerCreature())
			return GENERALERROR;

		ManagedReference<CreatureObject*> player = cast<CreatureObject*>(creature);
		ManagedReference<GroupObject*> group = player->getGroup();

		if (!checkGroupLeader(player, group))
			return GENERALERROR;

		float skillMod = (float) creature->getSkillMod("steadyaim");
		int hamCost = (int) (100.0f * (1.0f - (skillMod / 100.0f))) * calculateGroupModifier(group);

		int healthCost = creature->calculateCostAdjustment(CreatureAttribute::STRENGTH, hamCost);
		int actionCost = creature->calculateCostAdjustment(CreatureAttribute::QUICKNESS, hamCost);
		int mindCost = creature->calculateCostAdjustment(CreatureAttribute::FOCUS, hamCost);

		if (!inflictHAM(player, healthCost, actionCost, mindCost))
			return GENERALERROR;

//		shoutCommand(player, group);

		int amount = 5 + skillMod;

		if (!doSteadyAim(player, group, amount))
			return GENERALERROR;
			
		if (player->isPlayerCreature() && player->getPlayerObject()->getCommandMessageString(String("steadyaim").hashCode()).isEmpty()==false && creature->checkCooldownRecovery("command_message")) {
			UnicodeString shout(player->getPlayerObject()->getCommandMessageString(String("steadyaim").hashCode()));
 	 	 	server->getChatManager()->broadcastMessage(player, shout, 0, 0, 80);
 	 	 	creature->updateCooldownTimer("command_message", 30 * 1000);
		}					

		return SUCCESS;
	}

	bool doSteadyAim(CreatureObject* leader, GroupObject* group, int amount) {
		if (leader == NULL || group == NULL)
			return false;

		for (int i = 0; i < group->getGroupSize(); i++) {

			ManagedReference<SceneObject*> member = group->getGroupMember(i);

			if (!member->isPlayerCreature() || member == NULL || member->getZone() != leader->getZone())
				continue;

			ManagedReference<CreatureObject*> memberPlayer = cast<CreatureObject*>( member.get());
			Locker clocker(memberPlayer, leader);

			sendCombatSpam(memberPlayer);

			ManagedReference<WeaponObject*> weapon = memberPlayer->getWeapon();

			if (!weapon->isRangedWeapon())
				continue;

			int duration = 300;

			ManagedReference<Buff*> buff = new Buff(memberPlayer, actionCRC, duration, BuffType::SKILL);
			buff->setSkillModifier("private_aim", amount);
			buff->setStartFlyText("combat_effects", "go_steady", 0, 0xFF, 0); // there is no corresponding no_steady fly text

			memberPlayer->addBuff(buff);
		}

		return true;
	}

};

#endif //STEADYAIMCOMMAND_H_
