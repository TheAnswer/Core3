/*
 * AiMap.h
 *
 *  Created on: May 17, 2012
 *      Author: da
 */

#ifndef AIMAP_H_
#define AIMAP_H_

#include "engine/engine.h"

#include "server/zone/templates/AiTemplate.h"
#include "server/zone/objects/creature/AiAgent.h"
#include "server/zone/objects/creature/ai/bt/Behavior.h"
#include "server/zone/objects/creature/ai/bt/SequenceBehavior.h"
#include "server/zone/objects/creature/ai/bt/SelectorBehavior.h"
#include "server/zone/objects/creature/ai/bt/NonDeterministicSequenceBehavior.h"
#include "server/zone/objects/creature/ai/bt/NonDeterministicSelectorBehavior.h"
#include "server/zone/objects/creature/ai/bt/ParallelSequenceBehavior.h"
#include "server/zone/objects/creature/ai/bt/ParallelSelectorBehavior.h"
#include "server/zone/objects/creature/ai/bt/LuaBehavior.h"
#include "server/zone/objects/creature/CreatureFlag.h"

class AiMap : public Singleton<AiMap>, public Logger, public Object {
public:
	static const uint8 INVALID = 0;   // finished without result
	static const uint8 SUCCESS = 1;   // finished with success
	static const uint8 FAILURE = 2;   // finished with failure
	static const uint8 RUNNING = 3;   // running and not finished
	static const uint8 SUSPEND = 4;   // not yet started, or temporarily paused

	// class types
	static const uint8 BEHAVIOR = 0;
	static const uint8 SEQUENCEBEHAVIOR = 1;
	static const uint8 SELECTORBEHAVIOR = 2;
	static const uint8 NONDETERMINISTICSEQUENCEBEHAVIOR = 3;
	static const uint8 NONDETERMINISTICSELECTORBEHAVIOR = 4;
	static const uint8 PARALLELSEQUENCEBEHAVIOR = 5;
	static const uint8 PARALLELSELECTORBEHAVIOR = 6;

	HashTable<String, Reference<AiTemplate*> > aiMap;
	HashTable<String, Reference<LuaBehavior*> > behaviors;
	HashTable<unsigned int, Reference<AiTemplate*> > getTargets;
	HashTable<unsigned int, Reference<AiTemplate*> > selectAttacks;
	HashTable<unsigned int, Reference<AiTemplate*> > combatMoves;
	HashTable<unsigned int, Reference<AiTemplate*> > idles;
	bool loaded;

	AiMap() : Logger("AiMap") {
		aiMap.setNullValue(NULL);
		behaviors.setNullValue(NULL);
		getTargets.setNullValue(NULL);
		selectAttacks.setNullValue(NULL);
		combatMoves.setNullValue(NULL);
		idles.setNullValue(NULL);

		loaded = false;
	}

	~AiMap() {
	}

	void initialize(Lua* lua) {
		if (lua == NULL) {
			error("Could not get lua AiMap::instance from DirectorManager");
			return;
		}

		registerGlobals(lua);
		registerFunctions(lua);

		if (DEBUG_MODE)
			info("Initializing...", true);

		lua->runFile("scripts/ai/ais.lua");
	}

	void loadTemplates(Lua* lua) {
		if (lua == NULL) {
			error("Could not get lua AiMap::instance from DirectorManager");
			return;
		}

		if (DEBUG_MODE)
			info("Loading templates...", true);

		lua->runFile("scripts/ai/templates/templates.lua");

		putTemplate(lua, "getTarget", &getTargets);
		putTemplate(lua, "selectAttack", &selectAttacks);
		putTemplate(lua, "combatMove", &combatMoves);
		putTemplate(lua, "idle", &idles);
		loaded = true;
	}

	bool isLoaded() {
		return loaded;
	}

	int getTemplateSize() {
		return aiMap.size();
	}

	int getBehaviorSize() {
		return behaviors.size();
	}

	void putTemplate(String name, Reference<AiTemplate*> ait) {
		aiMap.put(name, ait);
	}

	Reference<AiTemplate*> getTemplate(String name) {
		if (name == "none")
			return NULL;

		return aiMap.get(name);
	}

	void putBehavior(String name, Reference<LuaBehavior*> b) {
		behaviors.put(name, b);
	}

	Reference<LuaBehavior*> getBehavior(String name) {
		return behaviors.get(name);
	}

	Reference<AiTemplate*> getGetTargetTemplate(unsigned int bitMask) {
		return getTemplate(bitMask, getTargets);
	}

	Reference<AiTemplate*> getSelectAttackTemplate(unsigned int bitMask) {
		return getTemplate(bitMask, selectAttacks);
	}

	Reference<AiTemplate*> getCombatMoveTemplate(unsigned int bitMask) {
		return getTemplate(bitMask, combatMoves);
	}

	Reference<AiTemplate*> getIdleTemplate(unsigned int bitMask) {
		return getTemplate(bitMask, idles);
	}

private:
	static const uint8 DEBUG_MODE = 0;

	void registerFunctions(Lua* lua) {
		lua_register(lua->getLuaState(), "addAiTemplate", addAiTemplate);
		lua_register(lua->getLuaState(), "addAiBehavior", addAiBehavior);
		lua_register(lua->getLuaState(), "includeAiFile", includeFile);
	}

	void registerGlobals(Lua* lua) {
		lua->setGlobalInt("BEHAVIOR_SUCCESS",AiMap::SUCCESS);
		lua->setGlobalInt("BEHAVIOR_FAILURE",AiMap::FAILURE);
		lua->setGlobalInt("BEHAVIOR_RUNNING",AiMap::RUNNING);
		lua->setGlobalInt("BEHAVIOR_SUSPEND",AiMap::SUSPEND);
		lua->setGlobalInt("BEHAVIOR_INVALID",AiMap::INVALID);

		lua->setGlobalInt("BEHAVIOR",AiMap::BEHAVIOR);
		lua->setGlobalInt("SEQUENCEBEHAVIOR",AiMap::SEQUENCEBEHAVIOR);
		lua->setGlobalInt("SELECTORBEHAVIOR",AiMap::SELECTORBEHAVIOR);
		lua->setGlobalInt("NONDETERMINISTICSEQUENCEBEHAVIOR",AiMap::NONDETERMINISTICSEQUENCEBEHAVIOR);
		lua->setGlobalInt("NONDETERMINISTICSELECTORBEHAVIOR",AiMap::NONDETERMINISTICSELECTORBEHAVIOR);
		lua->setGlobalInt("PARALLELSEQUENCEBEHAVIOR",AiMap::PARALLELSEQUENCEBEHAVIOR);
		lua->setGlobalInt("PARALLELSELECTORBEHAVIOR",AiMap::PARALLELSELECTORBEHAVIOR);

		lua->setGlobalInt("OBLIVIOUS",AiAgent::OBLIVIOUS);
		lua->setGlobalInt("WATCHING",AiAgent::WATCHING);
		lua->setGlobalInt("STALKING",AiAgent::STALKING);
		lua->setGlobalInt("FOLLOWING",AiAgent::FOLLOWING);
		lua->setGlobalInt("PATROLLING",AiAgent::PATROLLING);

		lua->setGlobalInt("NONE",CreatureFlag::NONE);
		lua->setGlobalInt("PACK",CreatureFlag::PACK);
		lua->setGlobalInt("HERD",CreatureFlag::HERD);
		lua->setGlobalInt("KILLER",CreatureFlag::KILLER);
		lua->setGlobalInt("STALKER",CreatureFlag::STALKER);
		lua->setGlobalInt("BABY",CreatureFlag::BABY);
		lua->setGlobalInt("LAIR",CreatureFlag::LAIR);
		lua->setGlobalInt("HEALER",CreatureFlag::HEALER);
		lua->setGlobalInt("STATIC",CreatureFlag::STATIC);
	}

	void putTemplate(Lua* lua, String key, HashTable<unsigned int, Reference<AiTemplate*> >* table) {
		LuaObject obj = lua->getGlobalObject(key);
		if (obj.isValidTable()) {
			for (int i = 1; i <= obj.getTableSize(); i++) {
				lua_rawgeti(lua->getLuaState(), -1, i);
				LuaObject temp(lua->getLuaState());
				table->put(temp.getIntAt(1), AiMap::instance()->getTemplate(temp.getStringAt(2)));
				temp.pop();
			}
		}
	}

	Reference<AiTemplate*> getTemplate(unsigned int bitMask, HashTable<unsigned int, Reference<AiTemplate*> > table) {
		HashTableIterator<unsigned int, Reference<AiTemplate*> > iter = table.iterator();

		unsigned int key;
		Reference<AiTemplate*> val;

		while (iter.hasNext()) {
			iter.getNextKeyAndValue(key, val);

			if ((key & bitMask) == key)
				return table.get(key);
		}

		return table.get(CreatureFlag::NONE); // this is the default template, but the flow shouldn't get here because 0 & anything == 0
	}

	static int includeFile(lua_State* L) {
		String filename = Lua::getStringParameter(L);

		if (DEBUG_MODE)
			AiMap::instance()->info("Including file: " + filename, true);

		Lua::runFile("scripts/ai/" + filename, L);

		return 0;
	}

	static int addAiTemplate(lua_State* L) {
		String name = lua_tostring(L, -2);

		LuaObject obj(L);

		if (obj.isValidTable()) {
			Reference<AiTemplate*> ait = new AiTemplate(name);
			ait->readObject(&obj);

			AiMap::instance()->putTemplate(name, ait);

			if (DEBUG_MODE)
				AiMap::instance()->info("Loaded AI template " + name, true);
		} else
			AiMap::instance()->error("Lua AI template not found: " + name);

		obj.pop();

		return 0;
	}

	static int addAiBehavior(lua_State* L) {
		String name = lua_tostring(L, -1);

		Reference<LuaBehavior*> b = new LuaBehavior(name);

		if (b->initialize()) {
			AiMap::instance()->putBehavior(name, b);

			if (DEBUG_MODE)
				AiMap::instance()->info("Loaded AI behavior " + name, true);
		}

		return 0;
	}

public:
	static Behavior* createNewInstance(AiAgent* _agent, String _name, uint16 _type) {
		Behavior* newBehavior;

		switch (_type) {
		case AiMap::SEQUENCEBEHAVIOR:
			newBehavior = new SequenceBehavior(_agent, _name);
			break;
		case AiMap::SELECTORBEHAVIOR:
			newBehavior = new SelectorBehavior(_agent, _name);
			break;
		case AiMap::NONDETERMINISTICSEQUENCEBEHAVIOR:
			newBehavior = new NonDeterministicSequenceBehavior(_agent, _name);
			break;
		case AiMap::NONDETERMINISTICSELECTORBEHAVIOR:
			newBehavior = new NonDeterministicSelectorBehavior(_agent, _name);
			break;
		case AiMap::PARALLELSEQUENCEBEHAVIOR:
			newBehavior = new ParallelSequenceBehavior(_agent, _name);
			break;
		case AiMap::PARALLELSELECTORBEHAVIOR:
			newBehavior = new ParallelSelectorBehavior(_agent, _name);
			break;
		case AiMap::BEHAVIOR:
		default:
			newBehavior = new Behavior(_agent, _name);
			break;
		}

		return newBehavior;
	}
};

#endif /* AIMAP_H_ */
