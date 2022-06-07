/*
* CommandQueue.h
*
* Created on: Jan. 1, 2022
* Author: Hakry
*/

#ifndef COMMANDQUEUE_H_
#define COMMANDQUEUE_H_

#include "engine/engine.h"
#include "server/zone/objects/creature/variables/CommandQueueActionVector.h"

class CommandQueueTask;

class CommandQueue {
	enum State {
		NONE, WAITING, RUNNING, DELAY
	};

	WeakReference<CreatureObject*> weakCreature;
	CommandQueueActionVector queueVector;
	CommandQueueTask* queueTask;
	State state = NONE;
	static const int DELAYCHECK = 200;

public:
	void initialize(CreatureObject* creature);
	void run();
	int handleRunningState();

	bool checkForImmediateActions(CreatureObject* creature);
	void removeAction(CommandQueueAction* actionToDelete);
	void enqueueCommand(unsigned int actionCRC, unsigned int actionCount, uint64 targetID, const UnicodeString& arguments, int priority, int compareCounter);
	void sendCommand(uint32 crc, const UnicodeString& args, uint64 targetID, int priority);
	void clearQueueAction(unsigned int actioncntr, float timer, unsigned int tab1, unsigned int tab2);
	void clearQueueActions(bool combatOnly);

	int getQueueSize() const {
		return queueVector.size();
	}

	void deleteQueueAction(uint32 actionCount);
};

#endif /* COMMANDQUEUE_H_ */