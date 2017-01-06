/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions. */

#ifndef FORCEMEDITATECOMMAND_H_
#define FORCEMEDITATECOMMAND_H_

#include "server/zone/objects/player/events/ForceMeditateTask.h"

class ForceMeditateCommand : public QueueCommand {
public:

	ForceMeditateCommand(const String& name, ZoneProcessServer* server)
	: QueueCommand(name, server) {

	}

	int doQueueCommand(CreatureObject* creature, const uint64& target, const UnicodeString& arguments) const {

		if (!checkStateMask(creature))
			return INVALIDSTATE;

		if (!checkInvalidLocomotions(creature))
			return INVALIDLOCOMOTION;

		if (!creature->isPlayerCreature())
			return GENERALERROR;

		if (isWearingArmor(creature)) {
			return NOJEDIARMOR;
		}
		
		if (creature->isInCombat()) {
			creature->sendSystemMessage("@jedi_spam:not_while_in_combat");
			return GENERALERROR;
		}

		if (creature->isMeditating()) {
			creature->sendSystemMessage("@jedi_spam:already_in_meditative_state");
			return GENERALERROR;
		}

		// Play Client Effect once.

		creature->playEffect("clienteffect/pl_force_meditate_self.cef", "");

		// Force Meditate Task
		ManagedReference<PlayerObject*> ghost = creature->getPlayerObject();
		
		creature->sendSystemMessage("@teraskasi:med_begin");
		Reference<ForceMeditateTask*> fmeditateTask = new ForceMeditateTask(creature);
		fmeditateTask->setMoodString(creature->getMoodString());
		creature->addPendingTask("forcemeditate", fmeditateTask, 3500);

		creature->setMeditateState();

		PlayerManager* playermgr = server->getZoneServer()->getPlayerManager();	
		creature->registerObserver(ObserverEventType::POSTURECHANGED, playermgr);

		return SUCCESS;

	}

};

#endif //FORCEMEDITATECOMMAND_H_
