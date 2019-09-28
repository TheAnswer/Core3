/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef RETREATCOMMAND_H_
#define RETREATCOMMAND_H_

#include "SquadLeaderCommand.h"

class RetreatCommand : public SquadLeaderCommand {
public:

	RetreatCommand(const String& name, ZoneProcessServer* server)
		: SquadLeaderCommand(name, server) {
	}

	bool checkRetreat(CreatureObject* creature) const {
		if (creature->isRidingMount()) {
			creature->sendSystemMessage("@cbt_spam:no_burst"); // You cannot burst-run while mounted on a creature or vehicle.
			return false;
		}

		Zone* zone = creature->getZone();

		if (zone == nullptr) {
			return false;
		}

		if (zone->getZoneName() == "dungeon1") {
			creature->sendSystemMessage("@combat_effects:burst_run_space_dungeon"); //The artificial gravity makes burst running impossible here.
			return false;
		}

		uint32 burstCRC = STRING_HASHCODE("burstrun");
		uint32 forceRun1CRC = BuffCRC::JEDI_FORCE_RUN_1;
		uint32 forceRun2CRC = BuffCRC::JEDI_FORCE_RUN_2;
		uint32 forceRun3CRC = BuffCRC::JEDI_FORCE_RUN_3;

		if (creature->hasBuff(burstCRC) || creature->hasBuff(forceRun1CRC) || creature->hasBuff(forceRun2CRC) || creature->hasBuff(forceRun3CRC)) {
			creature->sendSystemMessage("@combat_effects:burst_run_no"); //You cannot burst run right now.
			return false;
		}

		if (!creature->checkCooldownRecovery("retreat")) {
			creature->sendSystemMessage("@combat_effects:burst_run_no"); //You cannot burst run right now.
			return false;
		}

		return true;
	}

	int doQueueCommand(CreatureObject* creature, const uint64& target, const UnicodeString& arguments) const {

		if (!checkStateMask(creature))
			return INVALIDSTATE;

		if (!checkInvalidLocomotions(creature))
			return INVALIDLOCOMOTION;

		if (!creature->isPlayerCreature())
			return GENERALERROR;

		ManagedReference<CreatureObject*> player = cast<CreatureObject*>(creature);

		if (player == nullptr)
			return GENERALERROR;

		ManagedReference<PlayerObject*> ghost = player->getPlayerObject();

		if (ghost == nullptr)
			return GENERALERROR;

		ManagedReference<GroupObject*> group = player->getGroup();

		if (!checkGroupLeader(player, group))
			return GENERALERROR;

		float groupBurstRunMod = (float) player->getSkillMod("group_burst_run");
		int hamCost = (int) (100.0f * (1.0f - (groupBurstRunMod / 100.0f))) * calculateGroupModifier(group);

		int actionCost = creature->calculateCostAdjustment(CreatureAttribute::QUICKNESS, hamCost);
		int mindCost = creature->calculateCostAdjustment(CreatureAttribute::FOCUS, hamCost);

		if (!inflictHAM(player, 0, actionCost, mindCost))
			return GENERALERROR;

		for (int i = 1; i < group->getGroupSize(); ++i) {
			ManagedReference<CreatureObject*> member = group->getGroupMember(i);

			if (member == nullptr || !member->isPlayerCreature())
				continue;

			if (!isValidGroupAbilityTarget(creature, member, false))
				continue;

			Locker clocker(member, player);

			sendCombatSpam(member);
			doRetreat(member);

			checkForTef(player, member);
		}

		if (!ghost->getCommandMessageString(STRING_HASHCODE("retreat")).isEmpty() && creature->checkCooldownRecovery("command_message")) {
			UnicodeString shout(ghost->getCommandMessageString(STRING_HASHCODE("retreat")));
 	 	 	server->getChatManager()->broadcastChatMessage(player, shout, 0, 80, player->getMoodID(), 0, ghost->getLanguageID());
 	 	 	creature->updateCooldownTimer("command_message", 30 * 1000);
		}

		return SUCCESS;
	}


	void doRetreat(CreatureObject* player) const {
		if (player == nullptr)
			return;

		if (!checkRetreat(player))
			return;

		uint32 actionCRC = STRING_HASHCODE("retreat");

		if (player->hasBuff(actionCRC)) {
			return;
		}

		float groupRunMod = (float) player->getSkillMod("group_burst_run");

		if (groupRunMod > 100.0f)
			groupRunMod = 100.0f;

		StringIdChatParameter startStringId("cbt_spam", "burstrun_start_single");
		StringIdChatParameter endStringId("cbt_spam", "burstrun_stop_single");

		int duration = 30;

		ManagedReference<Buff*> buff = new Buff(player, actionCRC, duration, BuffType::SKILL);

		Locker locker(buff);

		buff->setSpeedMultiplierMod(1.822f);
		buff->setAccelerationMultiplierMod(1.822f);
		buff->setStartMessage(startStringId);
		buff->setEndMessage(endStringId);

		player->addBuff(buff);

		player->updateCooldownTimer("retreat", 30000);

	}

};

#endif //RETREATCOMMAND_H_
