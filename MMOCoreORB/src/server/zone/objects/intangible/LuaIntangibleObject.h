/*
 * LuaIntangibleObject.h
 *
 *  Created on: 13/01/2012
 *      Author: victor
 */

#ifndef LUAINTANGIBLEOBJECT_H_
#define LUAINTANGIBLEOBJECT_H_

#include "engine/lua/Luna.h"
#include "lua.h"
#include "server/zone/objects/intangible/IntangibleObject.h"
#include "server/zone/objects/scene/LuaSceneObject.h"

namespace server {
namespace zone {
namespace objects {
namespace intangible {
class IntangibleObject;
}  // namespace intangible
}  // namespace objects
}  // namespace zone
}  // namespace server

class LuaIntangibleObject : public LuaSceneObject {
public:
	// Constants
	static const char className[];
	static Luna<LuaIntangibleObject>::RegType Register[];

	// Initialize the pointer
	LuaIntangibleObject(lua_State *L);
	~LuaIntangibleObject();

	int _setObject(lua_State* L);
	// Methods we will need to use


private:
	// The pointer to the 'real object' defined in object.cc
	IntangibleObject* realObject;
};


#endif /* LUAINTANGIBLEOBJECT_H_ */
