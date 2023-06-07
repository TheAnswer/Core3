#include "LuaShipObject.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/ship/ShipObject.h"
#include "server/zone/ZoneServer.h"
#include "server/zone/objects/intangible/ShipControlDevice.h"
#include "server/zone/objects/intangible/tasks/StoreShipTask.h"

const char LuaShipObject::className[] = "LuaShipObject";

Luna<LuaShipObject>::RegType LuaShipObject::Register[] = {
	{ "_setObject", &LuaShipObject::_setObject },
	{ "_getObject", &LuaSceneObject::_getObject },
	{ "getControlDeviceID", &LuaShipObject::getControlDeviceID },
	{ "storeShip", &LuaShipObject::storeShip },
	{ 0, 0}
};

LuaShipObject::LuaShipObject(lua_State *L) : LuaTangibleObject(L) {
#ifdef DYNAMIC_CAST_LUAOBJECTS
	realObject = dynamic_cast<ShipObject*>(_getRealSceneObject());
	assert(!_getRealSceneObject() || realObject != nullptr);
#else
	realObject = static_cast<ShipObject*>(lua_touserdata(L, 1));
#endif
}

LuaShipObject::~LuaShipObject(){
}

int LuaShipObject::_setObject(lua_State* L) {
	LuaTangibleObject::_setObject(L);

#ifdef DYNAMIC_CAST_LUAOBJECTS
	auto obj = dynamic_cast<ShipObject*>(_getRealSceneObject());

	if (obj != realObject)
		realObject = obj;

	assert(!_getRealSceneObject() || realObject != nullptr);
#else
	auto obj = static_cast<ShipObject*>(lua_touserdata(L, -1));

	if (realObject != obj)
		realObject = obj;
#endif

	return 0;
}

int LuaShipObject::getControlDeviceID(lua_State* L) {
	lua_pushinteger(L, realObject->getControlDeviceID());
	return 1;
}

int LuaShipObject::storeShip(lua_State* L) {
	CreatureObject* player = static_cast<CreatureObject*>(lua_touserdata(L, -1));

	if (player == nullptr)
		return 0;

	ZoneServer* zoneServer = player->getZoneServer();

	if (zoneServer == nullptr)
		return 0;

	ManagedReference<SceneObject*> device = zoneServer->getObject(realObject->getControlDeviceID());

	if (device == nullptr || !device->isShipControlDevice())
		return 0;

	ShipControlDevice* shipControlDevice = device.castTo<ShipControlDevice*>();

	if (shipControlDevice == nullptr)
		return 0;

	StoreShipTask* task = new StoreShipTask(player, shipControlDevice);

	if (task != nullptr)
		task->execute();

	return 0;
}