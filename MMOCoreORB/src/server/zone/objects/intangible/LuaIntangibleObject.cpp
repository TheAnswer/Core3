/*
 * LuaIntangibleObject.cpp
 *
 *  Created on: 13/01/2012
 *      Author: victor
 */

#include "LuaIntangibleObject.h"

#include <assert.h>
#include <stddef.h>

#include "server/zone/objects/intangible/IntangibleObject.h"
#include "server/zone/objects/scene/SceneObject.h"

const char LuaIntangibleObject::className[] = "LuaIntangibleObject";

Luna<LuaIntangibleObject>::RegType LuaIntangibleObject::Register[] = {
		{ "_setObject", &LuaIntangibleObject::_setObject },
		{ "_getObject", &LuaSceneObject::_getObject },
		{ 0, 0 }
};


LuaIntangibleObject::LuaIntangibleObject(lua_State *L) : LuaSceneObject(L) {
#ifdef DYNAMIC_CAST_LUAOBJECTS
	realObject = dynamic_cast<IntangibleObject*>(_getRealSceneObject());

	assert(!_getRealSceneObject() || realObject != NULL);
#else
	realObject = reinterpret_cast<IntangibleObject*>(lua_touserdata(L, 1));
#endif
}

LuaIntangibleObject::~LuaIntangibleObject() {
}

int LuaIntangibleObject::_setObject(lua_State* L) {
	LuaSceneObject::_setObject(L);

#ifdef DYNAMIC_CAST_LUAOBJECTS
	realObject = dynamic_cast<IntangibleObject*>(_getRealSceneObject());

	assert(!_getRealSceneObject() || realObject != NULL);
#else
	realObject = reinterpret_cast<IntangibleObject*>(lua_touserdata(L, -1));
#endif

	return 0;
}
