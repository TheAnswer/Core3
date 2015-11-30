/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef GMFSVILLAGECOMMAND_H_
#define GMFSVILLAGECOMMAND_H_

#include "server/zone/objects/scene/SceneObject.h"
#include "server/zone/managers/director/DirectorManager.h"
#include "server/zone/managers/jedi/JediManager.h"

class GmFsVillageCommand : public QueueCommand {
public:

	GmFsVillageCommand(const String& name, ZoneProcessServer* server)
		: QueueCommand(name, server) {

	}

	int doQueueCommand(CreatureObject* creature, const uint64& target, const UnicodeString& arguments) const {

		if (!checkStateMask(creature))
			return INVALIDSTATE;

		if (!checkInvalidLocomotions(creature))
			return INVALIDLOCOMOTION;

		if (JediManager::instance()->getJediProgressionType() != JediManager::VILLAGEJEDIPROGRESSION)
			return GENERALERROR;

		String functionName = "";

		StringTokenizer args(arguments.toString());

		String argFunction = "";

		if (args.hasMoreTokens())
			args.getStringToken(argFunction);

		argFunction = argFunction.toLowerCase();

		if (argFunction == "getcurrentphase")
			functionName = "cmdGetCurrentPhase";
		else if (argFunction == "getcurrentphaseid")
			functionName = "cmdGetCurrentPhaseID";
		else if (argFunction == "getcurrentphaseduration")
			functionName = "cmdGetCurrentPhaseDuration";
		else if (argFunction == "changecurrentphase")
			functionName = "cmdChangeCurrentPhase";

		if (argFunction == "?") {
			creature->sendSystemMessage("Syntax: /gmFsVillage <command> (params)");
			creature->sendSystemMessage("   getCurrentPhase  -- returns the currently active Village phase.");
			creature->sendSystemMessage("   getCurrentPhaseID  -- returns the currently active Village phase id.");
			creature->sendSystemMessage("   getCurrentPhaseDuration  -- returns the time left in the currently active phase.");
			creature->sendSystemMessage("   changeCurrentPhase  -- triggers the village phase change (this will cause issues with the phase event timer. Use for testing only).");
			return SUCCESS;
		} else if (argFunction == "" || functionName == "") {
			creature->sendSystemMessage("Syntax: /gmFsVillage <command>    /gmFsVillage ? for help");
			return SUCCESS;
		}

		Lua* lua = DirectorManager::instance()->getLuaInstance();

		lua->runFile("scripts/managers/jedi/village/village_gm_commands.lua");

		Reference<LuaFunction*> luaVillageGmCmd = lua->createFunction("VillageGmCommands", functionName, 0);
		*luaVillageGmCmd << creature;

		luaVillageGmCmd->callFunction();

		return SUCCESS;
	}

};

#endif //GMFSVILLAGECOMMAND_H_
