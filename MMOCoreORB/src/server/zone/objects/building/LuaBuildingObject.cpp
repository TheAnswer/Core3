/*
 * LuaBuildingObject.cpp
 *
 *  Created on: 27/05/2011
 *      Author: victor
 */

#include "LuaBuildingObject.h"
#include "BuildingObject.h"
#include "server/zone/objects/cell/CellObject.h"

const char LuaBuildingObject::className[] = "LuaBuildingObject";

Luna<LuaBuildingObject>::RegType LuaBuildingObject::Register[] = {
		{ "_setObject", &LuaBuildingObject::_setObject },
		{ "getCell", &LuaBuildingObject::getCell },
		{ "getParent", &LuaSceneObject::getParent },
		{ "getObjectID", &LuaSceneObject::getObjectID },
		{ "getPositionX", &LuaSceneObject::getPositionX },
		{ "getPositionY", &LuaSceneObject::getPositionY },
		{ "getPositionZ", &LuaSceneObject::getPositionZ },
		{ "getParentID", &LuaSceneObject::getParentID },
		{ "getServerObjectCRC", &LuaSceneObject::getServerObjectCRC },
		{ "grantPermission", &LuaBuildingObject::grantPermission },
		{ "broadcastSpecificCellPermissions", &LuaBuildingObject::broadcastSpecificCellPermissions },
		{ "spawnChildCreature", &LuaBuildingObject::spawnChildCreature },
		{ 0, 0 }
};

LuaBuildingObject::LuaBuildingObject(lua_State *L) : LuaTangibleObject(L) {
	realObject = (BuildingObject*)lua_touserdata(L, 1);
}

LuaBuildingObject::~LuaBuildingObject(){
}

int LuaBuildingObject::_setObject(lua_State* L) {
	realObject = (BuildingObject*)lua_touserdata(L, -1);

	return 0;
}

int LuaBuildingObject::getCell(lua_State* L) {
	int number = lua_tonumber(L, -1);

	lua_pushlightuserdata(L, realObject->getCell(number));

	return 1;
}

int LuaBuildingObject::grantPermission(lua_State* L) {
	String list = lua_tostring(L, -2);
	String name = lua_tostring(L, -1);

	int i = 0;

	if (list == "ADMIN" || list == "BAN" || list == "ENTRY") {
		realObject->grantPermission(list, name);
		i = 1;
	}

	lua_pushnumber(L, i);

	return 1;
}

int LuaBuildingObject::broadcastSpecificCellPermissions(lua_State* L) {
	uint64 cellid = lua_tointeger(L, -1);

	realObject->broadcastCellPermissions(cellid);

	return 0;
}

int LuaBuildingObject::spawnChildCreature(lua_State* L) {
	uint64 parentID = lua_tointeger(L, -1);
	float heading = lua_tonumber(L, -2);
	float y = lua_tonumber(L, -3);
	float z = lua_tonumber(L, -4);
	float x = lua_tonumber(L, -5);
	int respawnTimer = lua_tointeger(L, -6);
	String mobile = lua_tostring(L, -7);

	ChildCreatureObject object;

	object.setPosition(x, z, y);
	object.setCellId(parentID);
	object.setContainmentType(-1);
	object.setMobileName(mobile);
	object.setRespawnTime(respawnTimer);
	object.setHeading(heading);

	realObject->spawnChildCreature(&object);

	return 0;
}
