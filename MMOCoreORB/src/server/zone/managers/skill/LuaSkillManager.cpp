#include "SkillManager.h"
#include "LuaSkillManager.h"
#include "server/zone/objects/scene/SceneObject.h"

const char LuaSkillManager::className[] = "LuaSkillManager";

Luna<LuaSkillManager>::RegType LuaSkillManager::Register[] = {
		{ "fulfillsSkillPrerequisitesAndXp", &LuaSkillManager::fulfillsSkillPrerequisitesAndXp },
		{ "fulfillsSkillPrerequisites", &LuaSkillManager::fulfillsSkillPrerequisites },
		{ "getSkill", &LuaSkillManager::getSkill },
		{ "awardSkill", &LuaSkillManager::awardSkill },
		{ "canLearnSkill", &LuaSkillManager::canLearnSkill },
		{ 0, 0 }
};

LuaSkillManager::LuaSkillManager(lua_State* L) {
	realObject = SkillManager::instance();
}

LuaSkillManager::~LuaSkillManager(){
}

int LuaSkillManager::fulfillsSkillPrerequisitesAndXp(lua_State* L) {
	if (lua_gettop(L) - 1 != 2) {
		Logger::console.error("incorrect number of arguments for LuaSkillManager::fulfillsSkillPrerequisitesAndXp");
		return 0;
	}

	String skillName = lua_tostring(L, -1);
	CreatureObject* creo = (CreatureObject*) lua_touserdata(L, -2);

	bool success = realObject->fulfillsSkillPrerequisitesAndXp(skillName, creo);

	lua_pushboolean(L, success);

	return 1;
}

int LuaSkillManager::fulfillsSkillPrerequisites(lua_State* L) {
	if (lua_gettop(L) - 1 != 3) {
		Logger::console.error("incorrect number of arguments for LuaSkillManager::fulfillsSkillPrerequisites");
		return 0;
	}

	bool xpReq = lua_toboolean(L, -1);
	String skillName = lua_tostring(L, -2);
	CreatureObject* creo = (CreatureObject*) lua_touserdata(L, -3);

	bool success = realObject->fulfillsSkillPrerequisites(skillName, xpReq);

	lua_pushboolean(L, success);

	return 1;
}

int LuaSkillManager::canLearnSkill(lua_State* L) {
	if (lua_gettop(L) - 1 != 2) {
		Logger::console.error("incorrect number of arguments for LuaSkillManager::canLearnSkill");
		return 0;
	}

	String skillName = lua_tostring(L, -1);
	CreatureObject* creo = (CreatureObject*) lua_touserdata(L, -2);

	bool canLearn = realObject->canLearnSkill(skillName, creo, false);

	lua_pushboolean(L, canLearn);

	return 1;
}

int LuaSkillManager::getSkill(lua_State* L) {
	if (lua_gettop(L) - 1 != 1) {
		Logger::console.error("incorrect number of arguments for LuaSkillManager::getSkill");
		return 0;
	}

	String skillName = lua_tostring(L, -1);

	lua_pushlightuserdata(L, realObject->getSkill(skillName));

	return 1;
}

int LuaSkillManager::awardSkill(lua_State* L) {
	if (lua_gettop(L) - 1 != 2) {
		Logger::console.error("incorrect number of arguments for LuaSkillManager::awardSkill");
		return 0;
	}

	String skillName = lua_tostring(L, -1);
	CreatureObject* creo = (CreatureObject*) lua_touserdata(L, -2);

	lua_pushboolean(L, realObject->awardSkill(skillName, creo, true, false, false));

	return 1;
}
