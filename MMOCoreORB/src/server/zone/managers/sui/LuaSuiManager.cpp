/*
 * LuaSuiManager.cpp
 *
 *  Created on: Jan 28, 2012
 *      Author: xyborn
 */

#include "server/ServerCore.h"
#include "SuiManager.h"
#include "LuaSuiManager.h"
#include "server/zone/objects/scene/SceneObject.h"
#include "server/zone/objects/player/sui/SuiWindowType.h"
#include "server/zone/objects/player/sui/SuiPageData.h"

const char LuaSuiManager::className[] = "LuaSuiManager";

Luna<LuaSuiManager>::RegType LuaSuiManager::Register[] = {
		{ "sendKeypadSui", &LuaSuiManager::sendKeypadSui },
		{ "sendConfirmSui", &LuaSuiManager::sendConfirmSui },
		{ "sendMessageBox", &LuaSuiManager::sendMessageBox },
		{ "sendInputBox", &LuaSuiManager::sendInputBox },
		{ "sendListBox", &LuaSuiManager::sendListBox },
		{ "sendTransferBox", &LuaSuiManager::sendTransferBox },
		{ "sendSuiPage", &LuaSuiManager::sendSuiPage },
		{ 0, 0 }
};

LuaSuiManager::LuaSuiManager(lua_State* L) {
	realObject = SuiManager::instance();
}

LuaSuiManager::~LuaSuiManager(){
}

int LuaSuiManager::sendSuiPage(lua_State* L) {
	if (lua_gettop(L) - 1 < 4) {
		Logger::console.error("incorrect number of arguments for LuaSuiManager::sendSuiPage");
		return 0;
	}

	String callback = lua_tostring(L, -1);
	String play = lua_tostring(L, -2);
	SuiPageData* page = (SuiPageData*) lua_touserdata(L, -3);
	CreatureObject* creo = (CreatureObject*) lua_touserdata(L, -4);

	int32 pageId = realObject->sendSuiPage(creo, page, play, callback);

	lua_pushinteger(L, pageId);

	return 1;
}

int LuaSuiManager::sendKeypadSui(lua_State* L) {
	if (lua_gettop(L) - 1 < 4) {
		Logger::console.error("incorrect number of arguments for LuaSuiManager::sendKeypadSui");
		return 0;
	}

	String callback = lua_tostring(L, -1);
	String play = lua_tostring(L, -2);
	SceneObject* creatureSceneObject = (SceneObject*) lua_touserdata(L, -3);
	SceneObject* keypad = (SceneObject*) lua_touserdata(L, -4);

	realObject->sendKeypadSui(keypad, creatureSceneObject, play, callback);

	return 0;
}

int LuaSuiManager::sendConfirmSui(lua_State* L) {
	if (lua_gettop(L) - 1 < 6) {
		Logger::console.error("incorrect number of arguments for LuaSuiManager::sendConfirmSui");
		return 0;
	}

	String button = lua_tostring(L, -1);
	String prompt = lua_tostring(L, -2);
	String callback = lua_tostring(L, -3);
	String play = lua_tostring(L, -4);
	SceneObject* scno = (SceneObject*)lua_touserdata(L, -5);
	SceneObject* scno2 = (SceneObject*)lua_touserdata(L, -6);

	realObject->sendConfirmSui(scno2, scno, play, callback, prompt, button);

	return 0;
}

int LuaSuiManager::sendInputBox(lua_State* L) {
	if (lua_gettop(L) - 1 < 6) {
		Logger::console.error("incorrect number of arguments for LuaSuiManager::sendInputBox");
		return 0;
	}

	String button = lua_tostring(L, -1);
	String prompt = lua_tostring(L, -2);
	String callback = lua_tostring(L, -3);
	String play = lua_tostring(L, -4);
	SceneObject* player = (SceneObject*) lua_touserdata(L, -5);
	SceneObject* usingObject = (SceneObject*) lua_touserdata(L, -6);

	realObject->sendInputBox(usingObject, player, play, callback, prompt, button);

	return 0;
}

int LuaSuiManager::sendMessageBox(lua_State* L) {
        int argn = lua_gettop(L) - 1;
	int index = 0;
	if ( !(argn == 7 || argn == 8) ){
		Logger::console.error("incorrect number of arguments for LuaSuiManager::sendMessageBox");
		return 0;
	}
	unsigned int windowType = 0;
	
	if( argn == 8 ){
	        windowType = (unsigned)lua_tointeger(L, --index);
	}

	String callback = lua_tostring(L, --index);
	String screenplay = lua_tostring(L, --index);
	String okButton = lua_tostring(L, --index );
	String text = lua_tostring(L, --index);
	String title = lua_tostring(L, --index);
	SceneObject* targetPlayer = (SceneObject*) lua_touserdata(L, --index);
	SceneObject* usingObject = (SceneObject*) lua_touserdata(L, --index);

	realObject->sendMessageBox(usingObject, targetPlayer, title, text, okButton, screenplay, callback, windowType );

	return 0;
}

int LuaSuiManager::sendListBox(lua_State* L) {
	if (lua_gettop(L) - 1 < 11) {
		Logger::console.error("incorrect number of arguments for LuaSuiManager::sendListBox");
		return 0;
	}

	SceneObject* usingObject = (SceneObject*) lua_touserdata(L, -11);
	SceneObject* targetPlayer = (SceneObject*) lua_touserdata(L, -10);
	String title = lua_tostring(L, -9);
	String text = lua_tostring(L, -8);
	int numOfButtons = lua_tointeger(L, -7);
	String cancelButton = lua_tostring(L, -6);
	String otherButton = lua_tostring(L, -5);
	String okButton = lua_tostring(L, -4);
	String screenplay = lua_tostring(L, -3);
	String callback = lua_tostring(L, -2);
	LuaObject options(L);

	realObject->sendListBox(usingObject, targetPlayer, title, text, numOfButtons, cancelButton, otherButton, okButton, options, screenplay, callback);

	return 0;
}

int LuaSuiManager::sendTransferBox(lua_State* L) {
	if (lua_gettop(L) - 1 < 9) {
		Logger::console.error("incorrect number of arguments for LuaSuiManager::sendTransferBox");
		return 0;
	}

	SceneObject* usingObject = (SceneObject*) lua_touserdata(L, -9);
	SceneObject* targetPlayer = (SceneObject*) lua_touserdata(L, -8);
	String title = lua_tostring(L, -7);
	String text = lua_tostring(L, -6);
	String okButton = lua_tostring(L, -5);
	String screenplay = lua_tostring(L, -4);
	String callback = lua_tostring(L, -3);
	LuaObject optionsFrom(L);
	LuaObject optionsTo(L);

	realObject->sendTransferBox(usingObject, targetPlayer, title, text, optionsFrom, optionsTo, screenplay, callback);

	return 0;
}

