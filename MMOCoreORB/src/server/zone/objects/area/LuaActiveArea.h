/*
 * LuaActiveArea.cpp
 *
 *  Created on: 21/01/2012
 *      Author: xyborn
 */

#ifndef LUAACIVEAREA_H_
#define LUAACIVEAREA_H_

#include "engine/engine.h"

#include "server/zone/objects/scene/LuaSceneObject.h"

namespace server {
namespace zone {
namespace objects {
namespace area {
	class ActiveArea;

	class LuaActiveArea : public LuaSceneObject {
	public:
		static const char className[];
		static Luna<LuaActiveArea>::RegType Register[];

		LuaActiveArea(lua_State *L);
		~LuaActiveArea();

		int _setObject(lua_State* L);

		int setRadius(lua_State *L);
		int getRadius(lua_State *L);

		int setNoBuildArea(lua_State* L);
		int isNoBuildArea(lua_State* L);

		int setCellObjectID(lua_State* L);
		int getCellObjectID(lua_State* L);

		int setNoSpawnArea(lua_State* L);
		int isNoSpawnArea(lua_State* L);

		int setPvpArea(lua_State* L);
		int isPvpArea(lua_State* L);

		int setOvertOnlyArea(lua_State* L);
		int isOvertOnlyArea(lua_State* L);

		int setRebelOnlyArea(lua_State* L);
		int isRebelOnlyArea(lua_State* L);

		int setImperialOnlyArea(lua_State* L);
		int isImperialOnlyArea(lua_State* L);

		int setNoCombatArea(lua_State* L);
		int isNoCombatArea(lua_State* L);

		int setNoPetArea(lua_State* L);
		int isNoPetArea(lua_State* L);

		int setLockedArea(lua_State* L);
		int isLockedArea(lua_State* L);

		int setCampingArea(lua_State* L);

		int getAreaName(lua_State* L);
	private:
		ActiveArea* realObject;
	};

}
}
}
}

using namespace server::zone::objects::area;


#endif /* LUAACIVEAREA_H_ */
