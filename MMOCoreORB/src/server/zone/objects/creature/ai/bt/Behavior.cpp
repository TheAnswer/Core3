/*
 * Behavior.cpp
 *
 *  Created on: Jun 10, 2014
 *      Author: swgemu
 */

#include "server/zone/objects/creature/ai/bt/Behavior.h"
#include "server/zone/objects/creature/ai/AiAgent.h"

// DEBUG
#include "server/zone/ZoneServer.h"
#include "server/chat/ChatManager.h"

using namespace server::zone::objects::creature::ai::bt;

Behavior::Behavior(const String& className, const uint32 id, const LuaObject& args)
	: Object(), className(className), id(id), parent() {}

bool Behavior::checkConditions(AiAgent* agent) const {
	// placeholder for more robust checkConditions
	return agent != nullptr && !agent->isDead() && !agent->isIncapacitated() && agent->getZone() != nullptr;
}

Behavior::Status Behavior::doAction(AiAgent* agent) const {
#ifdef DEBUG_AI
	//agent->info("0x" + String::hexvalueOf((int)id) + " " + print().toCharArray(), true);
	if (agent->peekBlackboard("aiDebug") && agent->readBlackboard("aiDebug") == true) {
		StringBuffer msg;
		msg << "0x" << hex << id << " " << print().toCharArray();
		agent->info(msg.toString(), true);

		ChatManager* chatManager = agent->getZoneServer()->getChatManager();
		chatManager->broadcastChatMessage(agent, print(), 0, 0, 0);
	} else {
		static Mutex mutext;
		Locker guard(&mutext);
		static AtomicBoolean verbose = false;
		static AtomicInteger lastConfigVersion = 0;
		auto currentConfigVersion = ConfigManager::instance()->getConfigVersion();

		if (currentConfigVersion > lastConfigVersion) {
			lastConfigVersion.set(currentConfigVersion);
			verbose.set(ConfigManager::instance()->getBool("Core3.AiAgent.Verbose", false));
			Logger::console.info(true) << "Core3.AiAgent.Verbose=" << verbose;
		}

		if (verbose) {
			ChatManager* chatManager = agent->getZoneServer()->getChatManager();
			chatManager->broadcastChatMessage(agent, print(), 0, 0, 0);
		}
	}
#endif // DEBUG_AI

	if (!this->checkConditions(agent)) {
		if (agent != nullptr) {
			// TODO: Should this be done here or in realizations of specific actions?
			agent->clearCombatState(true);
			agent->info() << "Invalid return from " << print() << " in " << agent->getErrorContext();
		}

		return INVALID; // TODO: should this be FAILURE?
	}

	// Step 1:	check if this behavior is in the running chain
	//			If it isn't, call start()
	//			We can assume here that if we are running here, that this is the
	//			leaf that is actually running because all other running states
	//			will be composites and would have a different reaction to being
	//			in the running vector
	if (!agent->isRunningBehavior(id))
		this->start(agent);
	else
		agent->popRunningChain();

	// Step 2:	evaluate the action result (method to do this will be abstract
	//			and implemented in the concrete behaviors)
	Behavior::Status result = this->execute(agent);

#ifdef DEBUG_AI
	//agent->info("0x" + String::hexvalueOf((int)id) + " " + print().toCharArray() + " result: " + result, true);
	if (agent->peekBlackboard("aiDebug") && agent->readBlackboard("aiDebug") == true) {
		StringBuffer msg;
		msg << "0x" << hex << id << " " << print() << " result: " << result;

		agent->info(msg.toString(), true);
	}
#endif // DEBUG_AI

	// Step 3:	If the result is RUNNING, clear the agent's run chain and and
	//			add this->id to the front. As the chain unwinds, the composite
	//			parents will push_front to the chain until the root node
	if (result == RUNNING) {
		if (!isSocket()) agent->clearRunningChain();
		agent->addRunningID(id);
	} else
		this->end(agent);

	// Step 4:	If the result is anything else (TODO: SUSPEND?) just return that
	//			value; the parent will deal with the next action based on the
	//			type of parent.
	return result;
}
