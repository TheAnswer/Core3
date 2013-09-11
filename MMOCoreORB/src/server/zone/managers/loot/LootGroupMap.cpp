/*
 * LootGroupMap.cpp
 *
 *  Created on: Jun 24, 2011
 *      Author: kyle
 */

#include "LootGroupMap.h"
#include "server/zone/managers/crafting/CraftingManager.h"
#include "server/zone/templates/LootItemTemplate.h"
#include "server/zone/templates/LootGroupTemplate.h"

Lua* LootGroupMap::lua = NULL;
int LootGroupMap::ERROR_CODE = NO_ERROR;

LootGroupMap::LootGroupMap() {
	lua = NULL;

	itemTemplates.setNullValue(NULL);
	groupTemplates.setNullValue(NULL);
}

LootGroupMap::~LootGroupMap() {
	delete lua;
	lua = NULL;
}

int LootGroupMap::initialize() {
	if (lua != NULL)
		return ERROR_CODE;

	lua = new Lua();
	lua->init();

	registerFunctions();
	registerGlobals();

	bool res = lua->runFile("scripts/loot/lootgroup.lua");
	bool res2 = lua->runFile("scripts/loot/serverobjects.lua");

	if (!res || !res2)
		ERROR_CODE = GENERAL_ERROR;

	return ERROR_CODE;
}

void LootGroupMap::registerFunctions() {
	lua_register(lua->getLuaState(), "addLootGroupTemplate", addLootGroupTemplate);
	lua_register(lua->getLuaState(), "addLootItemTemplate", addLootItemTemplate);
	lua_register(lua->getLuaState(), "includeFile", includeFile);
}

void LootGroupMap::registerGlobals() {
}

int LootGroupMap::includeFile(lua_State* L) {
	String filename = Lua::getStringParameter(L);

	bool res = Lua::runFile("scripts/loot/" + filename, L);

	if (!res)
		ERROR_CODE = GENERAL_ERROR;

	return 0;
}

int LootGroupMap::addLootGroupTemplate(lua_State* L) {
	String name = lua_tostring(L, -2);

	LuaObject obj(L);

	Reference<LootGroupTemplate*> group = new LootGroupTemplate(name);
	group->readObject(&obj);

	instance()->putLootGroupTemplate(name, group);

	return 0;
}

int LootGroupMap::addLootItemTemplate(lua_State* L) {
	String name = lua_tostring(L, -2);

	LuaObject obj(L);

	Reference<LootItemTemplate*> item = new LootItemTemplate(name);
	item->readObject(&obj);

	instance()->putLootItemTemplate(name, item);

	return 0;
}
