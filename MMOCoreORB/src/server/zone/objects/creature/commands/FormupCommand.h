/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef FORMUPCOMMAND_H_
#define FORMUPCOMMAND_H_

#include "server/zone/objects/scene/SceneObject.h"
#include "SquadLeaderCommand.h"

class FormupCommand : public SquadLeaderCommand {
public:

	FormupCommand(const String& name, ZoneProcessServer* server)
		: SquadLeaderCommand(name, server) {
	}

	int doQueueCommand(CreatureObject* creature, const uint64& target, const UnicodeString& arguments) const {

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

		int hamCost = (int) (50.0f * calculateGroupModifier(group));

		int healthCost = creature->calculateCostAdjustment(CreatureAttribute::STRENGTH, hamCost);
		int actionCost = creature->calculateCostAdjustment(CreatureAttribute::QUICKNESS, hamCost);
		int mindCost = creature->calculateCostAdjustment(CreatureAttribute::FOCUS, hamCost);

		if (!inflictHAM(player, healthCost, actionCost, mindCost))
			return GENERALERROR;

//		shoutCommand(player, group);

		if (!doFormUp(player, group))
			return GENERALERROR;

		if (player->isPlayerCreature() && player->getPlayerObject()->getCommandMessageString(STRING_HASHCODE("formup")).isEmpty()==false && creature->checkCooldownRecovery("command_message")) {
			UnicodeString shout(player->getPlayerObject()->getCommandMessageString(STRING_HASHCODE("formup")));
 	 	 	server->getChatManager()->broadcastMessage(player, shout, 0, 0, 80);
 	 	 	creature->updateCooldownTimer("command_message", 30 * 1000);
		}

		return SUCCESS;
	}

	bool doFormUp(CreatureObject* leader, GroupObject* group) const {
		if (leader == NULL || group == NULL)
			return false;

		for (int i = 0; i < group->getGroupSize(); i++) {

			ManagedReference<SceneObject*> member = group->getGroupMember(i);

			if (member == NULL || !member->isPlayerCreature() || member->getZone() != leader->getZone())
				continue;

			CreatureObject* memberPlayer = cast<CreatureObject*>( member.get());

			if (!isValidGroupAbilityTarget(leader, memberPlayer, false))
				continue;

			Locker clocker(memberPlayer, leader);

			sendCombatSpam(memberPlayer);

			if (memberPlayer->isDizzied())

					memberPlayer->removeStateBuff(CreatureState::DIZZY);
					

			if (memberPlayer->isStunned())
					memberPlayer->removeStateBuff(CreatureState::STUNNED);

			checkForTef(leader, memberPlayer);
		}

		return true;
	}

};

#endif //FORMUPCOMMAND_H_
