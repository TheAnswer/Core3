/*
 * LuaTangibleObject.cpp
 *
 *  Created on: 24/01/2012
 *      Author: xyborn
 */

#include "LuaTangibleObject.h"
#include "server/zone/objects/tangible/TangibleObject.h"

const char LuaTangibleObject::className[] = "LuaTangibleObject";

Luna<LuaTangibleObject>::RegType LuaTangibleObject::Register[] = {
		{ "_setObject", &LuaTangibleObject::_setObject },
		{ "setOptionsBitmask", &LuaTangibleObject::setOptionsBitmask },
		{ "setPvpStatusBitmask", &LuaTangibleObject::setPvpStatusBitmask },
		{ "getPvpStatusBitmask", &LuaTangibleObject::getPvpStatusBitmask },
		{ "setCustomizationVariable", &LuaTangibleObject::setCustomizationVariable },
		{ "setConditionDamage", &LuaTangibleObject::setConditionDamage },
		{ "setFaction", &LuaTangibleObject::setFaction },
		{ "getFaction", &LuaTangibleObject::getFaction },
		{ "isImperial", &LuaTangibleObject::isImperial },
		{ "isRebel", &LuaTangibleObject::isRebel },
		{ "hasActiveArea", &LuaTangibleObject::hasActiveArea},
		{ 0, 0 }
};

LuaTangibleObject::LuaTangibleObject(lua_State *L) : LuaSceneObject(L) {
	realObject = static_cast<TangibleObject*>(lua_touserdata(L, 1));
}

LuaTangibleObject::~LuaTangibleObject(){
}

int LuaTangibleObject::_setObject(lua_State* L) {
	realObject = static_cast<TangibleObject*>(lua_touserdata(L, -1));

	return 0;
}

int LuaTangibleObject::setCustomizationVariable(lua_State* L) {
	String type = lua_tostring(L, -2);
	byte value = lua_tointeger(L, -1);

	realObject->setCustomizationVariable(type, value, true);

	return 0;
}

int LuaTangibleObject::setOptionsBitmask(lua_State* L) {
	uint32 bitmask = lua_tointeger(L, -1);

	realObject->setOptionsBitmask(bitmask, true);

	return 0;
}

int LuaTangibleObject::setPvpStatusBitmask(lua_State* L) {
	uint32 bitmask = lua_tointeger(L, -1);

	realObject->setPvpStatusBitmask(bitmask, true);

	return 0;
}

int LuaTangibleObject::getPvpStatusBitmask(lua_State* L) {
	uint32 bitmask = realObject->getPvpStatusBitmask();

	lua_pushinteger(L, bitmask);

	return 1;
}

int LuaTangibleObject::setConditionDamage(lua_State* L) {
	float damage = lua_tonumber(L, -1);

	realObject->setConditionDamage(damage, true);

	return 0;
}

int LuaTangibleObject::setFaction(lua_State* L){
	uint32 faction = lua_tointeger(L, -1);

	realObject->setFaction(faction);

	return 0;
}

int LuaTangibleObject::getFaction(lua_State* L){
	uint32 faction = realObject->getFaction();

	lua_pushinteger(L, faction);

	return 1;

}

int LuaTangibleObject::isImperial(lua_State* L){
	lua_pushboolean(L, realObject->isImperial());

	return 1;
}

int LuaTangibleObject::isRebel(lua_State* L){
	lua_pushboolean(L, realObject->isRebel());

	return 1;
}

int LuaTangibleObject::hasActiveArea(lua_State* L) {
	uint64 objectid = lua_tointeger(L, -1);

	bool res = realObject->hasActiveArea(objectid);

	lua_pushboolean(L, res);

	return 1;
}
