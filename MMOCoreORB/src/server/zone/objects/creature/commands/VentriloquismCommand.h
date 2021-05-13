/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef VENTRILOQUISMCOMMAND_H_
#define VENTRILOQUISMCOMMAND_H_

class VentriloquismCommand : public QueueCommand {
public:

	VentriloquismCommand(const String& name, ZoneProcessServer* server)
		: QueueCommand(name, server) {

	}

	int doQueueCommand(CreatureObject* creature, const uint64& target, const UnicodeString& arguments) const {

		if (!checkStateMask(creature))
			return INVALIDSTATE;

		if (!checkInvalidLocomotions(creature))
			return INVALIDLOCOMOTION;

		ManagedReference<EntertainingSession*> session = creature->getActiveSession(SessionFacadeType::ENTERTAINING).castTo<EntertainingSession*>();

		if (session == nullptr || (!session->isPlayingMusic() && !session->isDancing())) {
			creature->sendSystemMessage("@performance:effect_not_performing"); // You must be performing in order to execute this special effect.
			return GENERALERROR;
		}

		int effectLevel = Integer::valueOf(arguments.toString());
		session->doPerformEffect(PerformEffect::VENTRILOQUISM, effectLevel);

		return SUCCESS;
	}

};

#endif //VENTRILOQUISMCOMMAND_H_
