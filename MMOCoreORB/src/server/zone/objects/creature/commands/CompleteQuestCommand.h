/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.
*/

#ifndef COMPLETEQUESTCOMMAND_H_
#define COMPLETEQUESTCOMMAND_H_

#include "server/zone/objects/scene/SceneObject.h"

class CompleteQuestCommand : public QueueCommand {
public:

	CompleteQuestCommand(const String& name, ZoneProcessServer* server)
		: QueueCommand(name, server) {

	}

	int doQueueCommand(CreatureObject* creature, const uint64& target, const UnicodeString& arguments) const {

		if (!checkStateMask(creature))
			return INVALIDSTATE;

		if (!checkInvalidLocomotions(creature))
			return INVALIDLOCOMOTION;

		StringTokenizer args(arguments.toString());

		ManagedReference<PlayerObject*> ghost = creature->getPlayerObject();

		if (ghost == NULL || !ghost->isPrivileged()) {
			creature->sendSystemMessage("@error_message:insufficient_permissions"); //You do not have sufficient permissions to perform the requested action.
			return INSUFFICIENTPERMISSION;
		}

		try {
			String commandType;
			args.getStringToken(commandType);

			if (commandType.beginsWith("screenplaystate")) {
				ManagedReference<SceneObject*> obj = server->getZoneServer()->getObject(target);

				if (obj == NULL || !obj->isCreatureObject()) {
					return INVALIDTARGET;
				}

				CreatureObject* targetCreature = cast<CreatureObject*>(obj.get());

				if (targetCreature == NULL) {
					return INVALIDTARGET;
				}

				String screenPlayName;
				args.getStringToken(screenPlayName);
				uint64 state = args.getLongToken();

				Locker targetLock(targetCreature);

				targetCreature->setScreenPlayState(screenPlayName, state | targetCreature->getScreenPlayState(screenPlayName));

				creature->sendSystemMessage(targetCreature->getFirstName() + " " + screenPlayName + " set to " + String::valueOf(targetCreature->getScreenPlayState(screenPlayName)));
			} else {
				creature->sendSystemMessage("SYNTAX: /completeQuest screenplaystate <screenPlayStateName> <state>");

				return INVALIDPARAMETERS;
			}
		} catch (Exception& e) {
			creature->sendSystemMessage("SYNTAX: /completeQuest screenplaystate <screenPlayStateName> <state>");

			return INVALIDPARAMETERS;
		}

		return SUCCESS;
	}

};

#endif //COMPLETEQUESTCOMMAND_H_
