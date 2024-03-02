/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef TAUNTCOMMAND_H_
#define TAUNTCOMMAND_H_

#include "server/zone/objects/scene/SceneObject.h"
#include "server/zone/objects/tangible/threat/ThreatMap.h"

class TauntCommand : public CombatQueueCommand {
public:
	TauntCommand(const String& name, ZoneProcessServer* server) : CombatQueueCommand(name, server) {
	}

	int doQueueCommand(CreatureObject* creature, const uint64& target, const UnicodeString& arguments) const {
		if (!checkStateMask(creature))
			return INVALIDSTATE;

		if (!checkInvalidLocomotions(creature))
			return INVALIDLOCOMOTION;

		auto zoneServer = creature->getZoneServer();

		if (zoneServer == nullptr)
			return GENERALERROR;

		ManagedReference<SceneObject*> targetObject = zoneServer->getObject(target);

		if (targetObject == nullptr || !targetObject->isAiAgent())
			return INVALIDTARGET;

		AiAgent* agent = targetObject->asAiAgent();

		if (agent == nullptr)
			return INVALIDTARGET;

		int res = doCombatAction(creature, target);

		if (res == SUCCESS) {
			Locker clocker(agent, creature);

			if (!agent->isTauntable()) {
				creature->sendSystemMessage("@cbt_spam:taunt_fail_single");
				return res;
			}

			ThreatMap* threatMap = agent->getThreatMap();

			if (threatMap != nullptr) {
				int tauntMod = creature->getSkillMod("taunt");
				int levelCombine = agent->getLevel() + creature->getLevel();

				if (System::random(levelCombine + tauntMod) >= System::random(levelCombine - tauntMod)) {
					threatMap->setThreatState(creature, ThreatStates::TAUNTED, (uint64)tauntMod * 1000, (uint64)tauntMod * 1000);
					threatMap->addAggro(creature, tauntMod * 10, (uint64)tauntMod * 1000);

					/* Returns no grammar data
					CombatManager::instance()->broadcastCombatSpam(creature, agent, nullptr, 0, "cbt_spam", "taunt_success", 0);
					stringFiles[29].addEntry("taunt_success", "~~~~~You taunt your enemy, enraging them!~~%TU taunts his enemy!~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~")
					*/

					creature->sendSystemMessage("@cbt_spam:taunt_success_single");
				} else {
					creature->sendSystemMessage("@cbt_spam:taunt_fail_single");

					/* Returns no grammar data
					stringFiles[29].addEntry("taunt_fail", "~~~~~You try to go taunt your enemy, but they don't fall for it.~~%TU taunts his enemy to no avail.~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
					CombatManager::instance()->broadcastCombatSpam(creature, agent, nullptr, 0, "cbt_spam", "taunt_fail", 0);
					*/;
				}
			}
		}

		return res;
	}
};

#endif // TAUNTCOMMAND_H_
