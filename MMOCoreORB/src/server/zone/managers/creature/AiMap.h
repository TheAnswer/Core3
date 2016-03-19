/*
 * AiMap.h
 *
 *  Created on: May 17, 2012
 *      Author: da
 */

#ifndef AIMAP_H_
#define AIMAP_H_

#include "engine/engine.h"
#include "system/util/VectorMap.h"

#include "server/zone/objects/creature/ai/AiAgent.h"
#include "server/zone/objects/creature/ai/bt/Behavior.h"
#include "server/zone/objects/creature/ai/bt/BehaviorTreeSlot.h"
#include "server/zone/objects/creature/CreatureFlag.h"
#include "server/zone/managers/creature/PetManager.h"
#include "server/zone/objects/intangible/PetControlDevice.h"

// include all behaviors we want to register
#include "server/zone/objects/creature/ai/bt/TreeSocket.h"
#include "server/zone/objects/creature/ai/bt/node/Selector.h"
#include "server/zone/objects/creature/ai/bt/node/Sequence.h"
#include "server/zone/objects/creature/ai/bt/leaf/Checks.h"
#include "server/zone/objects/creature/ai/bt/leaf/FollowActions.h"
#include "server/zone/objects/creature/ai/bt/leaf/SimpleActions.h"
#include "server/zone/objects/creature/ai/bt/decorator/AlwaysFail.h"
#include "server/zone/objects/creature/ai/bt/decorator/AlwaysSucceed.h"
#include "server/zone/objects/creature/ai/bt/decorator/Not.h"
#include "server/zone/objects/creature/ai/bt/decorator/UntilFailure.h"
#include "server/zone/objects/creature/ai/bt/decorator/UntilSuccess.h"
#include "server/zone/objects/creature/ai/bt/decorator/LookForTarget.h"

#define _REGISTERBASE(name) factory.reg(#name, new BehaviorCreator<name>)
#define _REGISTERNODE(name) factory.reg(#name, new BehaviorCreator<node::name>)
#define _REGISTERLEAF(name) factory.reg(#name, new BehaviorCreator<leaf::name>)
#define _REGISTERDECO(name) factory.reg(#name, new BehaviorCreator<decorator::name>)

class BehaviorCreatorHelper {
public:
	virtual ~BehaviorCreatorHelper() {}
	virtual Behavior* create(const String&, const uint32, const String&) const = 0;
};

template <typename T>
class BehaviorCreator : public BehaviorCreatorHelper {
public:
	Behavior* create(const String& name, const uint32 id, const String& args) const {
		return new T(name, id, args);
	}
};

class BehaviorFactory {
public:
	~BehaviorFactory() {
		HashTableIterator<String, const BehaviorCreatorHelper*> it = fnMap.iterator();
		while (it.hasNext())
			delete it.next();
	}
	
	void reg(const String& key, const BehaviorCreatorHelper* fn) {
		if (!fnMap.containsKey(key))
			fnMap.put(key, fn);
	}
	
	Behavior* create(const String& key, const uint32 id, const String& args) {
		if (!fnMap.containsKey(key) || fnMap.get(key) == NULL)
			return NULL;

		return fnMap.get(key)->create(key, id, args);
	}

private:
	HashTable<String, const BehaviorCreatorHelper*> fnMap;
};

class AiMap : public Singleton<AiMap>, public Logger, public Object {
public:
	HashTable<String, Reference<Behavior*> > aiMap;
	HashTable<unsigned int, Reference<Behavior*> > bitmaskMap;

	AtomicInteger activeMoveEvents;
	AtomicInteger scheduledMoveEvents;
	AtomicInteger moveEventsWithFollowObject;
	AtomicInteger moveEventsRetreating;

	AtomicInteger activeAwarenessEvents;
	AtomicInteger scheduledAwarenessEvents;

	AtomicInteger activeRecoveryEvents;
	AtomicInteger activeWaitEvents;

	AiMap() : Logger("AiMap") {
		aiMap.setNullValue(NULL);
		bitmaskMap.setNullValue(NULL);

		initialize();
	}

	~AiMap() {}

	void initialize() {
		if (DEBUG_MODE)
			info("Initializing...", true);

		registerBehaviors();
	}

	void loadTemplates() {
		if (DEBUG_MODE)
			info("Loading templates...", true);

		Lua* lua = new Lua();
		lua->init();

		lua_register(lua->getLuaState(), "includeFile", includeFile);
		lua_register(lua->getLuaState(), "addAiTemplate", addAiTemplate);
		lua->runFile("scripts/ai/templates.lua");

		putBitmask(lua, "bitmaskLookup");

		delete lua;
	}

	int getTemplateSize() {
		return aiMap.size();
	}

	// put the root node of a btree into this map
	void putTemplate(const String& name, Reference<Behavior*> ait) {
		aiMap.put(name, ait);
	}

	// returns the root node of a behavior tree
	Behavior* getTemplate(const String& name) {
		if (name == "none")
			return NULL;

		return aiMap.get(name);
	}
	
	// returns the root node of a behavior tree
	// TODO: this. (and all other bitmask stuff)
	Behavior* getTemplate(unsigned int bitMask) {
		HashTableIterator<unsigned int, Reference<Behavior*> > iter = bitmaskMap.iterator();

		unsigned int finalKey = CreatureFlag::NONE;

		while (iter.hasNext()) {
			unsigned int key = iter.getNextKey();

			if ((key & bitMask) == key && finalKey < key)
				finalKey = key;
		}

		return bitmaskMap.get(finalKey);
	}
	
	Behavior* createBehavior(const String& name, const uint32 id, const String& args) {
		return factory.create(name, id, args);
	}

private:
	static const bool DEBUG_MODE = true;
	BehaviorFactory factory;
	
	void registerBehaviors() {
		// special behaviors
		_REGISTERBASE(TreeSocket);
		// composite behaviors
		_REGISTERNODE(Selector);
        _REGISTERNODE(ParallelSelector);
        _REGISTERNODE(NonDeterministicSelector);
		_REGISTERNODE(Sequence);
        _REGISTERNODE(ParallelSequence);
        _REGISTERNODE(NonDeterministicSequence);
		// decorator behaviors
		_REGISTERDECO(AlwaysFail);
		_REGISTERDECO(AlwaysSucceed);
		_REGISTERDECO(Not);
		_REGISTERDECO(UntilFailure);
		_REGISTERDECO(UntilSuccess);
		_REGISTERDECO(LookForTarget);
		// check behaviors
		_REGISTERLEAF(CheckPosture);
		_REGISTERLEAF(CheckDestination);
		_REGISTERLEAF(CheckFollowState);
		_REGISTERLEAF(CheckHasFollow);
		_REGISTERLEAF(CheckFollowHasState);
		_REGISTERLEAF(CheckFollowInRange);
		_REGISTERLEAF(CheckFollowAttackable);
		_REGISTERLEAF(CheckFollowAggression);
		_REGISTERLEAF(CheckFollowPosture);
		_REGISTERLEAF(CheckFollowInWeaponRange);
		_REGISTERLEAF(CheckFollowClosestIdealRange);
		_REGISTERLEAF(CheckRandomLevel);
        _REGISTERLEAF(CheckAttackInRange);
        _REGISTERLEAF(CheckAttackIsValid);
		_REGISTERLEAF(CheckLastCommand); //TODO: handle methods from non-AiAgent classes (like pet devices)
        _REGISTERLEAF(CheckRetreat);
        _REGISTERLEAF(CheckSpeed);
		// action behaviors
		_REGISTERLEAF(Dummy);
		_REGISTERLEAF(GeneratePatrol);
		_REGISTERLEAF(ExitCombat);
		_REGISTERLEAF(EquipStagedWeapon);
		_REGISTERLEAF(WriteBlackboard);
		_REGISTERLEAF(EraseBlackboard);
        _REGISTERLEAF(SelectAttack);
		_REGISTERLEAF(GetFollowFromThreatMap);
		_REGISTERLEAF(GetFollowFromDefenders);
		_REGISTERLEAF(RestoreFollow);
		_REGISTERLEAF(DropFollowFromDefenders);
		_REGISTERLEAF(SetDefenderFromFollow);
		_REGISTERLEAF(UpdateRangeToFollow);
		_REGISTERLEAF(FindNextPosition);
		_REGISTERLEAF(Leash);
		_REGISTERLEAF(CompleteMove);
		_REGISTERLEAF(Wait);
	}

	void putBitmask(Lua* lua, String key) {
		LuaObject obj = lua->getGlobalObject(key);
		if (obj.isValidTable()) {
			for (int i = 1; i <= obj.getTableSize(); i++) {
				lua_rawgeti(lua->getLuaState(), -1, i);
				LuaObject temp(lua->getLuaState());
				bitmaskMap.put(temp.getIntAt(1), AiMap::instance()->getTemplate(temp.getStringAt(2)));
				temp.pop();
			}
		}
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
		if (!obj.isValidTable()) {
			AiMap::instance()->error("Lua AI template not found: " + name);
			obj.pop();
			return 0;
		}
	
		Vector<Reference<Behavior*> > loadVec;
		VectorMap<uint32, Behavior*> loadMap;
		VectorMap<uint32, uint32> parentMap;
		
		for (int i = 1; i <= obj.getTableSize(); ++i) {
			lua_rawgeti(L, -1, i);
			LuaObject behavior(L);

			uint32 id = behavior.getStringField("id").hashCode();
			String className = behavior.getStringField("name");
			uint32 parent = behavior.getStringField("pid").hashCode();
			String args = behavior.getStringField("args");
			
			Behavior* b = AiMap::instance()->createBehavior(className, id, args);
			loadVec.add(b);
			loadMap.put(id, b);
			parentMap.put(id, parent);

			behavior.pop();
		}
		
		for (int idx = 0; idx < loadVec.size(); ++idx) {
			Reference<Behavior*> child = loadVec.get(idx);
			if (child == NULL)
				continue;
			
			uint32 parentID = parentMap.get(child->getID());
			if (parentID == STRING_HASHCODE("none")) // this is the root
				continue;
			
			Reference<Behavior*> parent = loadMap.get(parentID);
			child->setParent(parent); // this method asserts that parent is proper
			
			if (parent->isComposite())
				static_cast<node::Composite*>(parent.get())->addChild(child);
			else if (parent->isDecorator())
				static_cast<decorator::Decorator*>(parent.get())->setChild(child);
		}
		
		// root will be the final result of any behavior in the tree
		Reference<Behavior*> root = loadMap.get(0);
		while (root->getParent())
			root = root->getParent();

		AiMap::instance()->putTemplate(name, root);

		if (DEBUG_MODE)
			AiMap::instance()->info("Loaded AI template " + name, true);

		obj.pop();
		return 0;
	}
};

#endif /* AIMAP_H_ */
