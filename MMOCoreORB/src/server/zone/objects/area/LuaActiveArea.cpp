/*
 * LuaActiveArea.cpp
 *
 *  Created on: 21/01/2012
 *      Author: xyborn
 */

#include "LuaActiveArea.h"
#include "server/zone/objects/area/ActiveArea.h"

const char LuaActiveArea::className[] = "LuaActiveArea";

Luna<LuaActiveArea>::RegType LuaActiveArea::Register[] = {
		{ "_setObject", &LuaActiveArea::_setObject },
		{ "_getObject", &LuaSceneObject::_getObject },
		{ "setRadius", &LuaActiveArea::setRadius },
		{ "getRadius", &LuaActiveArea::getRadius },
		{ "setNoBuildArea", &LuaActiveArea::setNoBuildArea },
		{ "isNoBuildArea", &LuaActiveArea::isNoBuildArea },
		{ "getObjectID", &LuaSceneObject::getObjectID },
		{ "setCellObjectID", &LuaActiveArea::setCellObjectID },
		{ "getCellObjectID", &LuaActiveArea::getCellObjectID },
		{ "isNoSpawnArea", &LuaActiveArea::isNoSpawnArea },
		{ "setNoSpawnArea", &LuaActiveArea::setNoSpawnArea },
		{ "isPvpArea", &LuaActiveArea::isPvpArea },
		{ "setPvpArea", &LuaActiveArea::setPvpArea },
		{ "isOvertOnlyArea", &LuaActiveArea::isOvertOnlyArea },
		{ "setOvertOnlyArea", &LuaActiveArea::setOvertOnlyArea },
		{ "isRebelOnlyArea", &LuaActiveArea::isRebelOnlyArea },
		{ "setRebelOnlyArea", &LuaActiveArea::setRebelOnlyArea },
		{ "isImperialOnlyArea", &LuaActiveArea::isImperialOnlyArea },
		{ "setImperialOnlyArea", &LuaActiveArea::setImperialOnlyArea },
		{ "isNoCombatArea", &LuaActiveArea::isNoCombatArea },
		{ "setNoCombatArea", &LuaActiveArea::setNoCombatArea },
		{ "isNoPetArea", &LuaActiveArea::isNoPetArea },
		{ "setNoPetArea", &LuaActiveArea::setNoPetArea },
		{ "isLockedArea", &LuaActiveArea::isLockedArea },
		{ "setLockedArea", &LuaActiveArea::setLockedArea },
		{ "setCampingArea", &LuaActiveArea::setCampingArea },
		{ "getAreaName", &LuaActiveArea::getAreaName },
		{ 0, 0 }
};

LuaActiveArea::LuaActiveArea(lua_State *L) : LuaSceneObject(L) {
#ifdef DYNAMIC_CAST_LUAOBJECTS
	realObject = dynamic_cast<ActiveArea*>(_getRealSceneObject());

	E3_ASSERT(!_getRealSceneObject() || realObject != nullptr);
#else
	realObject = static_cast<ActiveArea*>(lua_touserdata(L, 1));
#endif
}

LuaActiveArea::~LuaActiveArea(){
}

int LuaActiveArea::_setObject(lua_State* L) {
	LuaSceneObject::_setObject(L);

#ifdef DYNAMIC_CAST_LUAOBJECTS
	realObject = dynamic_cast<ActiveArea*>(_getRealSceneObject());

	E3_ASSERT(!_getRealSceneObject() || realObject != nullptr);
#else
	realObject = static_cast<ActiveArea*>(lua_touserdata(L, -1));
#endif

	return 0;
}

int LuaActiveArea::getRadius(lua_State* L) {
	lua_pushnumber(L, realObject->getRadius());

	return 1;
}

int LuaActiveArea::setRadius(lua_State* L) {
	int radius = lua_tonumber(L, -1);
	Locker realObjectLocker(realObject);
	realObject->setRadius(radius);

	return 0;
}

int LuaActiveArea::setNoBuildArea(lua_State* L) {
	bool val = lua_toboolean(L, -1);
	Locker realObjectLocker(realObject);

	if (val) {
		realObject->addAreaFlag(ActiveArea::NOBUILDZONEAREA);
	} else {
		realObject->removeAreaFlag(ActiveArea::NOBUILDZONEAREA);
	}

	return 0;
}

int LuaActiveArea::isNoBuildArea(lua_State* L) {
	bool val = realObject->isNoBuildZone();

	lua_pushboolean(L, val);

	return 1;
}

int LuaActiveArea::setCellObjectID(lua_State* L) {
	uint64 val = lua_tointeger(L, -1);
	Locker realObjectLocker(realObject);
	realObject->setCellObjectID(val);

	return 0;
}

int LuaActiveArea::getCellObjectID(lua_State* L) {
	uint64 val = realObject->getCellObjectID();

	lua_pushinteger(L, val);

	return 1;
}

int LuaActiveArea::setNoSpawnArea(lua_State* L) {
	bool val = lua_toboolean(L, -1);
	Locker realObjectLocker(realObject);

	if (val) {
		realObject->addAreaFlag(ActiveArea::NOSPAWNAREA);
		realObject->addAreaFlag(ActiveArea::NOWORLDSPAWNAREA);
	} else {
		realObject->removeAreaFlag(ActiveArea::NOSPAWNAREA);
		realObject->removeAreaFlag(ActiveArea::NOWORLDSPAWNAREA);
	}

	return 0;
}

int LuaActiveArea::isNoSpawnArea(lua_State* L) {
	bool val = realObject->isNoSpawnArea();

	lua_pushboolean(L, val);

	return 1;
}

int LuaActiveArea::setPvpArea(lua_State* L) {
	bool val = lua_toboolean(L, -1);
	Locker realObjectLocker(realObject);
	realObject->setPvpArea(val);

	return 0;
}

int LuaActiveArea::isPvpArea(lua_State* L) {
	bool val = realObject->isPvpArea();

	lua_pushboolean(L, val);

	return 1;
}

int LuaActiveArea::setOvertOnlyArea(lua_State* L) {
	bool val = lua_toboolean(L, -1);
	Locker realObjectLocker(realObject);
	realObject->setOvertOnlyArea(val);

	return 0;
}

int LuaActiveArea::isOvertOnlyArea(lua_State* L) {
	bool val = realObject->isOvertOnlyArea();

	lua_pushboolean(L, val);

	return 1;
}

int LuaActiveArea::setRebelOnlyArea(lua_State* L) {
	bool val = lua_toboolean(L, -1);
	Locker realObjectLocker(realObject);
	realObject->setRebelOnlyArea(val);

	return 0;
}

int LuaActiveArea::isRebelOnlyArea(lua_State* L) {
	bool val = realObject->isRebelOnlyArea();

	lua_pushboolean(L, val);

	return 1;
}

int LuaActiveArea::setImperialOnlyArea(lua_State* L) {
	bool val = lua_toboolean(L, -1);
	Locker realObjectLocker(realObject);
	realObject->setImperialOnlyArea(val);

	return 0;
}

int LuaActiveArea::isImperialOnlyArea(lua_State* L) {
	bool val = realObject->isImperialOnlyArea();

	lua_pushboolean(L, val);

	return 1;
}

int LuaActiveArea::setNoCombatArea(lua_State* L) {
	bool val = lua_toboolean(L, -1);
	Locker realObjectLocker(realObject);
	realObject->setNoCombatArea(val);

	return 0;
}

int LuaActiveArea::isNoCombatArea(lua_State* L) {
	bool val = realObject->isNoCombatArea();

	lua_pushboolean(L, val);

	return 1;
}

int LuaActiveArea::setNoPetArea(lua_State* L) {
	bool val = lua_toboolean(L, -1);
	Locker realObjectLocker(realObject);
	realObject->setNoPetArea(val);

	return 0;
}

int LuaActiveArea::isNoPetArea(lua_State* L) {
	bool val = realObject->isNoPetArea();

	lua_pushboolean(L, val);

	return 1;
}

int LuaActiveArea::setLockedArea(lua_State* L) {
	bool val = lua_toboolean(L, -1);
	Locker realObjectLocker(realObject);
	realObject->setLockedArea(val);

	return 0;
}

int LuaActiveArea::isLockedArea(lua_State* L) {
	bool val = realObject->isLockedArea();

	lua_pushboolean(L, val);

	return 1;
}

int LuaActiveArea::setCampingArea(lua_State* L) {
	bool val = lua_toboolean(L, -1);
	Locker realObjectLocker(realObject);

	if (val) {
		realObject->addAreaFlag(ActiveArea::CAMPINGAREA);
	} else {
		realObject->removeAreaFlag(ActiveArea::CAMPINGAREA);
	}

	return 0;
}

int LuaActiveArea::getAreaName(lua_State* L) {
	String name = realObject->getAreaName();

	lua_pushstring(L, name.toCharArray());

	return 1;
}