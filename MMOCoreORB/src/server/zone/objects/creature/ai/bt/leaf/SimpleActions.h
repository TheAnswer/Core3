#ifndef SIMPLEACTIONS_H_
#define SIMPLEACTIONS_H_

#include "server/zone/objects/creature/ai/bt/Behavior.h"
#include "server/zone/objects/creature/ai/bt/BlackboardData.h"
#include "server/zone/objects/creature/ai/AiAgent.h"

namespace server {
namespace zone {
namespace objects {
namespace creature {
namespace ai {
namespace bt {
namespace leaf {

class Dummy : public Behavior {
public:
	Dummy(const String& className, const uint32 id, const LuaObject& args)
			: Behavior(className, id, args) {
	}

	Dummy(const Dummy& d)
			: Behavior(d) {
	}

	Behavior::Status execute(AiAgent*, unsigned int) const {
		return SUCCESS; // this is meant to be decorated with AlwaysXX, so this return won't matter.
	}
};

class GeneratePatrol : public Behavior {
public:
	GeneratePatrol(const String& className, const uint32 id, const LuaObject& args)
			: Behavior(className, id, args), numPoints(0), distFromHome(0.0) {
		parseArgs(args);
	}

	GeneratePatrol(const GeneratePatrol& a)
			: Behavior(a), numPoints(a.numPoints), distFromHome(a.distFromHome) {
	}

	GeneratePatrol& operator=(const GeneratePatrol& a) {
		if (this == &a)
			return *this;
		Behavior::operator=(a);
		numPoints = a.numPoints;
		distFromHome = a.distFromHome;
		return *this;
	}

	void parseArgs(const LuaObject& args) {
		numPoints = getArg<int>()(args, "numPoints");
		distFromHome = getArg<float>()(args, "distFromHome");
	}

	Behavior::Status execute(AiAgent* agent, unsigned int startIdx = 0) const {
		return agent->generatePatrol(numPoints, distFromHome) ? SUCCESS : FAILURE;
	}

	String print() const {
		StringBuffer msg;
		msg << className << "-" << numPoints << ":" << distFromHome;

		return msg.toString();
	}

private:
	int numPoints;
	float distFromHome;
};

class ExitCombat : public Behavior {
public:
	ExitCombat(const String& className, const uint32 id, const LuaObject& args)
			: Behavior(className, id, args), clearDefenders(false) {
		parseArgs(args);
	}

	ExitCombat(const ExitCombat& a)
			: Behavior(a), clearDefenders(a.clearDefenders) {
	}

	ExitCombat& operator=(const ExitCombat& a) {
		if (this == &a)
			return *this;
		Behavior::operator=(a);
		clearDefenders = a.clearDefenders;
		return *this;
	}

	void parseArgs(const LuaObject& args) {
		clearDefenders = getArg<bool>()(args, "clearDefenders");
	}

	Behavior::Status execute(AiAgent* agent, unsigned int startIdx = 0) const {
		agent->clearCombatState(clearDefenders);

		return !agent->isInCombat() ? SUCCESS : FAILURE;
	}

	String print() const {
		StringBuffer msg;
		msg << className << "-" << clearDefenders;

		return msg.toString();
	}

private:
	bool clearDefenders;
};

class EquipStagedWeapon : public Behavior {
public:
	EquipStagedWeapon(const String& className, const uint32 id, const LuaObject& args)
			: Behavior(className, id, args) {
	}

	EquipStagedWeapon(const EquipStagedWeapon& a)
			: Behavior(a) {
	}

	Behavior::Status execute(AiAgent* agent, unsigned int startIdx = 0) const {
		if (!agent->peekBlackboard("stagedWeapon"))
			return FAILURE;

		uint32 weaponType = agent->readBlackboard("stagedWeapon").get<uint32>();
		agent->setWeapon(weaponType);
		return SUCCESS;
	}
};

class WriteBlackboard : public Behavior {
public:
	WriteBlackboard(const String& className, const uint32 id, const LuaObject& args)
			: Behavior(className, id, args) {
		parseArgs(args);
	}

	WriteBlackboard(const WriteBlackboard& a)
			: Behavior(a), key(a.key), val(a.val) {
	}

	WriteBlackboard& operator=(const WriteBlackboard& a) {
		if (this == &a)
			return *this;
		Behavior::operator=(a);
		key = a.key;
		val = a.val;
		return *this;
	}

	void parseArgs(const LuaObject& args) {
		key = getArg<String>()(args, "key");
		val = getArg<uint32>()(args, "val");
	}

	Behavior::Status execute(AiAgent* agent, unsigned int startIdx = 0) const {
		agent->writeBlackboard(key, val);

		return SUCCESS;
	}

	String print() const {
		StringBuffer msg;
		msg << className << "-" << key << ":" << val;

		return msg.toString();
	}

private:
	String key;
	uint32 val;
};

class EraseBlackboard : public Behavior {
public:
	EraseBlackboard(const String& className, const uint32 id, const LuaObject& args)
			: Behavior(className, id, args), param("") {
		parseArgs(args);
	}

	EraseBlackboard(const EraseBlackboard& a)
			: Behavior(a), param(a.param) {
	}

	EraseBlackboard& operator=(const EraseBlackboard& a) {
		if (this == &a)
			return *this;
		Behavior::operator=(a);
		param = a.param;
		return *this;
	}

	void parseArgs(const LuaObject& args) {
		param = getArg<String>()(args, "param");
	}

	Behavior::Status execute(AiAgent* agent, unsigned int startIdx = 0) const {
		agent->eraseBlackboard(param);
		return SUCCESS;
	}

	String print() const {
		StringBuffer msg;
		msg << className << "-" << param;

		return msg.toString();

	}

private:
	String param;
};

class SelectAttack : public Behavior {
public:
	SelectAttack(const String& className, const uint32 id, const LuaObject& args)
			: Behavior(className, id, args), attackNum(-1) {
		parseArgs(args);
	}

	SelectAttack(const SelectAttack& a)
			: Behavior(a), attackNum(a.attackNum) {
	}

	SelectAttack& operator=(const SelectAttack& a) {
		if (this == &a)
			return *this;
		Behavior::operator=(a);
		attackNum = a.attackNum;
		return *this;
	}

	void parseArgs(const LuaObject& args) {
		attackNum = getArg<int>()(args, "attackNum", -1);
	}

	Behavior::Status execute(AiAgent* agent, unsigned int startIdx = 0) const {
		//agent->info("SelectAttack::execute", true);

		if (agent->peekBlackboard("attackType")) {
			//agent->info("SelectAttack::execute has attackType", true);

			if (agent->readBlackboard("attackType").get<uint32>() == static_cast<uint32>(DataVal::DEFAULT)) {
				//agent->info("SelectAttack::execute has attackType DEFAULT", true);

				return agent->selectDefaultAttack() ? SUCCESS : FAILURE;
			}

			if (agent->readBlackboard("attackType").get<uint32>() == static_cast<uint32>(DataVal::RANDOM)) {
				//agent->info("SelectAttack::execute has attackType RANDOM", true);

				return agent->selectSpecialAttack(-1) ? SUCCESS : FAILURE;
			}
		}

		//agent->info("SelectAttack::execute has attackType attackNum", true);

		return agent->selectSpecialAttack(attackNum) ? SUCCESS : FAILURE;
	}

	String print() const {
		StringBuffer msg;
		msg << className << "-" << attackNum;

		return msg.toString();
	}

private:
	int attackNum;
};

class EnqueueAttack : public Behavior {
public:
	EnqueueAttack(const String& className, const uint32 id, const LuaObject& args)
			: Behavior(className, id, args) {
	}

	EnqueueAttack(const EnqueueAttack& a)
			: Behavior(a) {
	}

	EnqueueAttack& operator=(const EnqueueAttack& a) {
		if (this == &a)
			return *this;
		Behavior::operator=(a);

		return *this;
	}

	Behavior::Status execute(AiAgent* agent, unsigned int startIdx = 0) const {
		int res = agent->enqueueAttack(-1);
		Behavior::Status returnRes = FAILURE;

		if (!res)
			returnRes = SUCCESS;

		return returnRes;
	}

	String print() const {
		StringBuffer msg;
		msg << className;

		return msg.toString();
	}

};

class FindNextPosition : public Behavior {
public:
	FindNextPosition(const String& className, const uint32 id, const LuaObject& args)
			: Behavior(className, id, args) {
	}

	FindNextPosition(const FindNextPosition& a)
			: Behavior(a) {
	}

	Behavior::Status execute(AiAgent* agent, unsigned int startIdx = 0) const {
		DataVal mode = DataVal::WALK;
		if (agent->peekBlackboard("moveMode"))
			mode = static_cast<DataVal>(agent->readBlackboard("moveMode").get<uint32>());

		return agent->findNextPosition(agent->getMaxDistance(), mode == DataVal::WALK) ? RUNNING : SUCCESS;
	}
};

class Leash : public Behavior {
public:
	Leash(const String& className, const uint32 id, const LuaObject& args)
			: Behavior(className, id, args) {
	}

	Leash(const Leash& a)
			: Behavior(a) {
	}

	Behavior::Status execute(AiAgent* agent, unsigned int startIdx = 0) const {
		agent->leash();
		return SUCCESS;
	}
};

class CompleteMove : public Behavior {
public:
	CompleteMove(const String& className, const uint32 id, const LuaObject& args)
			: Behavior(className, id, args) {
	}

	CompleteMove(const CompleteMove& a)
			: Behavior(a) {
	}

	Behavior::Status execute(AiAgent* agent, unsigned int startIdx = 0) const {
		return agent->completeMove() ? SUCCESS : FAILURE;
	}
};

class Wait : public Behavior {
public:
	Wait(const String& className, const uint32 id, const LuaObject& args)
			: Behavior(className, id, args), duration(-1) {
		parseArgs(args);
	}

	Wait(const Wait& a)
			: Behavior(a), duration(a.duration) {
	}

	Wait& operator=(const Wait& a) {
		if (this == &a)
			return *this;
		Behavior::operator=(a);
		duration = a.duration;
		return *this;
	}

	void parseArgs(const LuaObject& args) {
		duration = (int) (getArg<float>()(args, "duration") * 1000);
	}

	Behavior::Status execute(AiAgent* agent, unsigned int startIdx = 0) const {
		// we don't need to check a value. Just checking to see if this value
		// exists on the blackboard is fine since it can never be false
		if (agent->peekBlackboard("isWaiting")) {
			if (agent->isWaiting() || duration < 0) // < 0 means indefinite wait
				return RUNNING;
			else {
				agent->eraseBlackboard("isWaiting");
				return SUCCESS;
			}
		}

		agent->setWait(duration);
		agent->writeBlackboard("isWaiting", true);

		return RUNNING;
	}

	String print() const {
		StringBuffer msg;
		msg << className << "-" << duration;

		return msg.toString();
	}

private:
	int duration;
};

class SetAlert : public Behavior {
public:
	SetAlert(const String& className, const uint32 id, const LuaObject& args)
			: Behavior(className, id, args), duration(0.f), show(true) {
		parseArgs(args);
	}

	SetAlert(const SetAlert& b)
			: Behavior(b), duration(b.duration), show(b.show) {
	}

	SetAlert& operator=(const SetAlert& b) {
		if (this == &b)
			return *this;
		Behavior::operator=(b);
		duration = b.duration;
		show = b.show;
		return *this;
	}

	Behavior::Status execute(AiAgent* agent, unsigned int startIdx = 0) const {
		Time* alert = agent->getAlertedTime();
		if (alert != NULL) {
			alert->updateToCurrentTime();
			alert->addMiliTime(duration);
		}

		if (show)
			agent->showFlyText("npc_reaction/flytext", "alert", 255, 0, 0);

		return SUCCESS;
	}

	void parseArgs(const LuaObject& args) {
		duration = (int) (getArg<float>()(args, "duration") * 1000);
		show = getArg<bool>()(args, "show");
	}

	String print() const {
		StringBuffer msg;
		msg << className << "-" << duration << ":" << show;

		return msg.toString();
	}

private:
	int duration;
	bool show;
};

}
}
}
}
}
}
}

#endif // SIMPLEACTIONS_H_
