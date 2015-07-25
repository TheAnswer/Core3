/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef FORCECUREDISEASECOMMAND_H_
#define FORCECUREDISEASECOMMAND_H_

#include "server/zone/objects/scene/SceneObject.h"
#include "server/zone/packets/object/CombatAction.h"
#include "ForceHealQueueCommand.h"

class ForceCureDiseaseCommand : public ForceHealQueueCommand {
public:

	ForceCureDiseaseCommand(const String& name, ZoneProcessServer* server)
		: QueueCommand(name, server) {

	}
};

#endif //FORCECUREPOISONCOMMAND_H_
