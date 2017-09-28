/*
 * LuaSceneObject.cpp
 *
 *  Created on: 27/05/2011
 *      Author: victor
 */

#include "server/zone/objects/scene/LuaSceneObject.h"
#include "server/zone/objects/scene/SceneObject.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/managers/stringid/StringIdManager.h"
#include "server/zone/managers/director/DirectorManager.h"
#include "server/zone/Zone.h"
#include "server/zone/managers/director/ScreenPlayTask.h"

const char LuaSceneObject::className[] = "LuaSceneObject";

Luna<LuaSceneObject>::RegType LuaSceneObject::Register[] = {
		{ "_setObject", &LuaSceneObject::_setObject },
		{ "_getObject", &LuaSceneObject::_getObject },
		{ "getParent", &LuaSceneObject::getParent },
		{ "getObjectID", &LuaSceneObject::getObjectID },
		{ "getPositionX", &LuaSceneObject::getPositionX },
		{ "getPositionY", &LuaSceneObject::getPositionY },
		{ "getPositionZ", &LuaSceneObject::getPositionZ },
		{ "getDirectionAngle", &LuaSceneObject::getDirectionAngle },
		{ "getWorldPositionX", &LuaSceneObject::getWorldPositionX },
		{ "getWorldPositionY", &LuaSceneObject::getWorldPositionY },
		{ "getWorldPositionZ", &LuaSceneObject::getWorldPositionZ },
		{ "getParentID", &LuaSceneObject::getParentID },
		{ "isInRange", &LuaSceneObject::isInRange },
		{ "isInRangeWithObject", &LuaSceneObject::isInRangeWithObject },
		{ "isInRangeWithObject3d", &LuaSceneObject::isInRangeWithObject3d },
		{ "setCustomObjectName", &LuaSceneObject::setCustomObjectName},
		{ "getDistanceTo", &LuaSceneObject::getDistanceTo },
		{ "getDistanceToPosition", &LuaSceneObject::getDistanceToPosition },
		{ "updateDirection", &LuaSceneObject::updateDirection },
		{ "getServerObjectCRC", &LuaSceneObject::getServerObjectCRC },
		{ "showFlyText", &LuaSceneObject::showFlyText },
		{ "getContainerObject", &LuaSceneObject::getContainerObject },
		{ "getContainerObjectById", &LuaSceneObject::getContainerObjectById },
		{ "getContainerObjectsSize", &LuaSceneObject::getContainerObjectsSize },
		{ "getCountableObjectsRecursive", &LuaSceneObject::getCountableObjectsRecursive },
		{ "getContainerVolumeLimit", &LuaSceneObject::getContainerVolumeLimit },
		{ "isContainerFull", &LuaSceneObject::isContainerFull },
		{ "isContainerFullRecursive", &LuaSceneObject::isContainerFullRecursive },
		{ "getSlottedObject", &LuaSceneObject::getSlottedObject },
		{ "transferObject", &LuaSceneObject::transferObject },
//		{ "removeObject", &LuaSceneObject::removeObject },
		{ "getGameObjectType", &LuaSceneObject::getGameObjectType },
		{ "faceObject", &LuaSceneObject::faceObject },
		{ "isFacingObject", &LuaSceneObject::isFacingObject },
		{ "destroyObjectFromWorld", &LuaSceneObject::destroyObjectFromWorld },
		{ "destroyObjectFromDatabase", &LuaSceneObject::destroyObjectFromDatabase },
		{ "isCreatureObject", &LuaSceneObject::isCreatureObject },
		{ "isAiAgent", &LuaSceneObject::isAiAgent },
		{ "isPlayerCreature", &LuaSceneObject::isPlayerCreature },
		{ "isCreature", &LuaSceneObject::isCreature },
		{ "isBuildingObject", &LuaSceneObject::isBuildingObject },
		{ "isActiveArea", &LuaSceneObject::isActiveArea },
		{ "sendTo", &LuaSceneObject::sendTo },
		{ "getCustomObjectName", &LuaSceneObject::getCustomObjectName },
		{ "getDisplayedName", &LuaSceneObject::getDisplayedName },
		{ "getObjectName", &LuaSceneObject::getObjectName },
		{ "setDirectionalHeading", &LuaSceneObject::setDirectionalHeading },
		{ "getZoneName", &LuaSceneObject::getZoneName },
		{ "getTemplateObjectPath", &LuaSceneObject::getTemplateObjectPath },
		{ "teleport", &LuaSceneObject::teleport },
		{ "setObjectMenuComponent", &LuaSceneObject::setObjectMenuComponent },
		{ "setContainerComponent", &LuaSceneObject::setContainerComponent },
		{ "switchZone", &LuaSceneObject::switchZone },
		{ "setContainerInheritPermissionsFromParent", &LuaSceneObject::setContainerInheritPermissionsFromParent },
		{ "setContainerAllowPermission", &LuaSceneObject::setContainerAllowPermission },
		{ "setContainerDenyPermission", &LuaSceneObject::setContainerDenyPermission },
		{ "setContainerDefaultAllowPermission", &LuaSceneObject::setContainerDefaultAllowPermission },
		{ "clearContainerDefaultAllowPermission", &LuaSceneObject::clearContainerDefaultAllowPermission },
		{ "setContainerDefaultDenyPermission", &LuaSceneObject::setContainerDefaultDenyPermission },
		{ "clearContainerDefaultDenyPermission", &LuaSceneObject::clearContainerDefaultDenyPermission },
		{ "setContainerOwnerID", &LuaSceneObject::setContainerOwnerID },
		{ "setObjectName", &LuaSceneObject::setObjectName },
		{ "isASubChildOf", &LuaSceneObject::isASubChildOf },
		{ "isOwned", &LuaSceneObject::isOwned },
		{ "playEffect", &LuaSceneObject::playEffect },
		{ "addPendingTask", &LuaSceneObject::addPendingTask },
		{ "cancelPendingTask", &LuaSceneObject::cancelPendingTask },
		{ "getChildObject", &LuaSceneObject::getChildObject },
		{ "getContainerOwnerID", &LuaSceneObject::getContainerOwnerID },
		{ "info", &LuaSceneObject::info },
		{ "getPlayersInRange", &LuaSceneObject::getPlayersInRange },
		{ "isInNavMesh", &LuaSceneObject::isInNavMesh },
		{ 0, 0 }

};

LuaSceneObject::LuaSceneObject(lua_State *L) {
	realObject = reinterpret_cast<SceneObject*>(lua_touserdata(L, 1));
}

LuaSceneObject::~LuaSceneObject(){
}

int LuaSceneObject::_getObject(lua_State* L) {
	if (realObject == NULL)
		lua_pushnil(L);
	else
		lua_pushlightuserdata(L, realObject.get());

	return 1;
}

int LuaSceneObject::_setObject(lua_State* L) {
	auto obj = reinterpret_cast<SceneObject*>(lua_touserdata(L, -1));

	if (obj != realObject)
		realObject = obj;

	return 0;
}

int LuaSceneObject::setObjectMenuComponent(lua_State* L) {
	String value = lua_tostring(L, -1);

	realObject->setObjectMenuComponent(value);

	return 0;
}

int LuaSceneObject::setContainerComponent(lua_State* L) {
	String value = lua_tostring(L, -1);

	realObject->setContainerComponent(value);

	return 0;
}

int LuaSceneObject::setCustomObjectName(lua_State* L) {
	int numberOfArguments = lua_gettop(L) - 1;

	String value;

	if (numberOfArguments == 2) {
		String file = lua_tostring(L, -2);
		String key = lua_tostring(L, -1);
		String fullPath = "@" + file + ":" + key;
		value = StringIdManager::instance()->getStringId(fullPath.hashCode()).toString();
	} else {
		value = lua_tostring(L, -1);
	}

	realObject->setCustomObjectName(value, true);

	return 0;
}

int LuaSceneObject::switchZone(lua_State* L) {
	uint64 parentid = lua_tointeger(L, -1);
	float y = lua_tonumber(L, -2);
	float z = lua_tonumber(L, -3);
	float x = lua_tonumber(L, -4);
	String planet = lua_tostring(L, -5);

	Locker locker(realObject);

	realObject->switchZone(planet, x, z, y, parentid);

	return 0;
}

int LuaSceneObject::getTemplateObjectPath(lua_State* L) {
	if (realObject != NULL) {
		String tempPath = realObject->getObjectTemplate()->getFullTemplateString();

		lua_pushstring(L, tempPath.toCharArray());
	} else {
		lua_pushstring(L, "");
	}

	return 1;
}

int LuaSceneObject::teleport(lua_State* L) {
	uint64 parentID = lua_tointeger(L, -1);
	float y = lua_tonumber(L, -2);
	float z = lua_tonumber(L, -3);
	float x = lua_tonumber(L, -4);

	realObject->teleport(x, z, y, parentID);

	return 0;
}

int LuaSceneObject::getZoneName(lua_State* L) {
	Zone* zone = realObject->getZone();

	String name = "";

	if (zone != NULL) {
		name = zone->getZoneName();
	}

	lua_pushstring(L, name.toCharArray());

	return 1;
}

int LuaSceneObject::getPositionX(lua_State* L) {
	lua_pushnumber(L, realObject->getPositionX());

	return 1;
}

int LuaSceneObject::getPositionZ(lua_State* L) {
	lua_pushnumber(L, realObject->getPositionZ());

	return 1;
}

int LuaSceneObject::getDirectionAngle(lua_State* L) {
	lua_pushnumber(L, realObject->getDirectionAngle());

	return 1;
}

int LuaSceneObject::getWorldPositionY(lua_State* L) {
	lua_pushnumber(L, realObject->getWorldPositionY());

	return 1;
}

int LuaSceneObject::getWorldPositionX(lua_State* L) {
	lua_pushnumber(L, realObject->getWorldPositionX());

	return 1;
}

int LuaSceneObject::getWorldPositionZ(lua_State* L) {
	lua_pushnumber(L, realObject->getWorldPositionZ());

	return 1;
}

int LuaSceneObject::getPositionY(lua_State* L) {
	lua_pushnumber(L, realObject->getPositionY());

	return 1;
}

int LuaSceneObject::getObjectID(lua_State* L) {
	lua_pushinteger(L, realObject->getObjectID());

	return 1;
}

int LuaSceneObject::getParentID(lua_State* L) {
	lua_pushinteger(L, realObject->getParentID());

	return 1;
}

int LuaSceneObject::isInRange(lua_State* L) {
//public boolean isInRange(float x, float y, float range) {
	float range = lua_tonumber(L, -1);
	float y = lua_tonumber(L, -2);
	float x = lua_tonumber(L, -3);

	bool res = (static_cast<QuadTreeEntry*>(realObject))->isInRange(x, y, range);

	lua_pushnumber(L, res);

	return 1;
}

int LuaSceneObject::getGameObjectType(lua_State* L) {
	uint32 type = realObject->getGameObjectType();

	lua_pushnumber(L, type);

	return 1;
}

int LuaSceneObject::getDistanceTo(lua_State* L) {
	SceneObject* obj = (SceneObject*)lua_touserdata(L, -1);

	float res = realObject->getDistanceTo(obj);

	lua_pushnumber(L, res);

	return 1;
}

int LuaSceneObject::getDistanceToPosition(lua_State* L) {
	float y = lua_tonumber(L, -1);
	float z = lua_tonumber(L, -2);
	float x = lua_tonumber(L, -3);

	Coordinate position;
	position.setPosition(x, z, y);

	float dist = realObject->getDistanceTo(&position);

	lua_pushnumber(L, dist);

	return 1;
}

int LuaSceneObject::getServerObjectCRC(lua_State* L) {
	uint32 crc = realObject->getServerObjectCRC();

	lua_pushnumber(L, crc);

	return 1;
}

int LuaSceneObject::faceObject(lua_State* L) {
	bool notifyClient = lua_toboolean(L, -1);
	SceneObject* obj = (SceneObject*)lua_touserdata(L, -2);

	realObject->faceObject(obj, notifyClient);

	return 0;
}

int LuaSceneObject::isFacingObject(lua_State* L) {
	SceneObject* obj = (SceneObject*)lua_touserdata(L, -1);

	if (obj == NULL) {
		lua_pushboolean(L, false);

		return 1;
	}

	bool res = realObject->isFacingObject(obj);

	lua_pushboolean(L, res);

	return 1;
}

int LuaSceneObject::isInRangeWithObject(lua_State* L) {
	float range = lua_tonumber(L, -1);
	SceneObject* obj = (SceneObject*)lua_touserdata(L, -2);

	bool res = realObject->isInRange(obj, range);

	lua_pushboolean(L, res);

	return 1;
}

int LuaSceneObject::isInRangeWithObject3d(lua_State* L) {
	float range = lua_tonumber(L, -1);
	SceneObject* obj = (SceneObject*)lua_touserdata(L, -2);

	bool res = realObject->isInRange3d(obj, range);

	lua_pushboolean(L, res);

	return 1;
}

int LuaSceneObject::getParent(lua_State* L) {
	SceneObject* obj = realObject->getParent().get().get();

	if (obj == NULL) {
		lua_pushnil(L);
	} else {
		obj->_setUpdated(true);
		lua_pushlightuserdata(L, obj);
	}

	return 1;
}

int LuaSceneObject::getContainerObject(lua_State* L) {
	int idx = lua_tonumber(L, -1);

	SceneObject* obj = realObject->getContainerObject(idx);

	if (obj == NULL) {
		lua_pushnil(L);
	} else {
		obj->_setUpdated(true);
		lua_pushlightuserdata(L, obj);
	}

	return 1;
}

int LuaSceneObject::getContainerObjectById(lua_State* L) {
	uint64 objectID = lua_tointeger(L, -1);

	SceneObject* obj = realObject->getContainerObject(objectID);

	if (obj != NULL) {
		obj->_setUpdated(true);
		lua_pushlightuserdata(L, obj);
	} else {
		lua_pushnil(L);
	}

    return 1;
}

int LuaSceneObject::getContainerObjectsSize(lua_State* L) {
	int num = realObject->getContainerObjectsSize();

	lua_pushnumber(L, num);

	return 1;
}

int LuaSceneObject::getCountableObjectsRecursive(lua_State* L) {
	int num = realObject->getCountableObjectsRecursive();

	lua_pushnumber(L, num);

	return 1;
}

int LuaSceneObject::getContainerVolumeLimit(lua_State* L) {
	int num = realObject->getContainerVolumeLimit();

	lua_pushnumber(L, num);

	return 1;
}

int LuaSceneObject::isContainerFull(lua_State* L) {
	bool full = realObject->isContainerFull();

	lua_pushboolean(L, full);

	return 1;
}

int LuaSceneObject::isContainerFullRecursive(lua_State* L) {
	bool full = realObject->isContainerFullRecursive();

	lua_pushboolean(L, full);

	return 1;
}

int LuaSceneObject::getSlottedObject(lua_State* L) {
	String slot = lua_tostring(L, -1);

	SceneObject* obj = realObject->getSlottedObject(slot);
	if (obj == NULL) {
		lua_pushnil(L);
	} else {
		obj->_setUpdated(true);
		lua_pushlightuserdata(L, obj);
	}

	return 1;
}

int LuaSceneObject::transferObject(lua_State* L) {
	//transferObject(SceneObject object, int containmentType, boolean notifyClient = false);

	bool notifyClient = lua_tonumber(L, -1);
	int containmentType = lua_tonumber(L, -2);
	SceneObject* obj = (SceneObject*) lua_touserdata(L, -3);

	realObject->transferObject(obj, containmentType, notifyClient);

	return 0;
}

/*int LuaSceneObject::removeObject(lua_State* L) {

	//removeObject(SceneObject object, boolean notifyClient = false);

	bool notifyClient = lua_tonumber(L, -1);
	SceneObject* obj = (SceneObject*) lua_touserdata(L, -2);

	realObject->removeObject(obj, notifyClient);

	return 0;
}*/

int LuaSceneObject::showFlyText(lua_State* L) {
	//final string file, final string uax, byte red, byte green, byte blue

	byte blue = lua_tonumber(L, -1);
	byte green = lua_tonumber(L, -2);
	byte red = lua_tonumber(L, -3);

	String aux = lua_tostring(L, -4);
	String file = lua_tostring(L, -5);

	realObject->showFlyText(file, aux, red, green, blue);

	return 0;
}

int LuaSceneObject::playEffect(lua_State* L) {
	//public native void playEffect(final string file, final string aux);

	String aux = lua_tostring(L, -1);
	String file = lua_tostring(L, -2);

	realObject->playEffect(file, aux);

	return 0;
}


int LuaSceneObject::updateDirection(lua_State* L) {
	int numberOfArguments = lua_gettop(L) - 1;

	if (numberOfArguments == 1) {
		float angle = lua_tonumber(L, -1);
		realObject->updateDirection(angle);
	} else {
		float fz = lua_tonumber(L, -1);
		float fy = lua_tonumber(L, -2);
		float fx = lua_tonumber(L, -3);
		float fw = lua_tonumber(L, -4);

		realObject->updateDirection(fw, fx, fy, fz);
	}

	return 0;
}

int LuaSceneObject::destroyObjectFromWorld(lua_State* L) {
	Locker locker(realObject);

	realObject->destroyObjectFromWorld(true);

	return 0;
}

int LuaSceneObject::destroyObjectFromDatabase(lua_State* L) {
	realObject->destroyObjectFromDatabase(true);

	return 0;
}

int LuaSceneObject::isCreatureObject(lua_State* L) {
	bool val = realObject->isCreatureObject();

	lua_pushboolean(L, val);

	return 1;
}

int LuaSceneObject::isAiAgent(lua_State* L) {
	bool val = realObject->isAiAgent();

	lua_pushboolean(L, val);

	return 1;
}

int LuaSceneObject::isPlayerCreature(lua_State* L) {
	bool val = realObject->isPlayerCreature();

	lua_pushboolean(L, val);

	return 1;
}

int LuaSceneObject::isCreature(lua_State* L) {
	bool val = realObject->isCreature();

	lua_pushboolean(L, val);

	return 1;
}

int LuaSceneObject::isBuildingObject(lua_State* L) {
	bool val = realObject->isBuildingObject();

	lua_pushboolean(L, val);

	return 1;
}

int LuaSceneObject::isActiveArea(lua_State* L) {
	bool val = realObject->isActiveArea();

	lua_pushboolean(L, val);

	return 1;
}

int LuaSceneObject::wlock(lua_State* L) {
	return 0;
}

int LuaSceneObject::unlock(lua_State* L) {
	return 0;
}

int LuaSceneObject::sendTo(lua_State* L) {
	SceneObject* obj = (SceneObject*) lua_touserdata(L, -1);

	realObject->sendTo(obj, true);

	return 0;
}

int LuaSceneObject::getCustomObjectName(lua_State* L) {
	String objname = realObject->getCustomObjectName().toString();

	lua_pushstring(L, objname.toCharArray());

	return 1;
}

int LuaSceneObject::getDisplayedName(lua_State* L) {
	String objname = realObject->getDisplayedName();

	lua_pushstring(L, objname.toCharArray());

	return 1;
}

int LuaSceneObject::getObjectName(lua_State* L) {
	String objname = realObject->getObjectName()->getStringID();

	lua_pushstring(L, objname.toCharArray());

	return 1;
}

int LuaSceneObject::setDirectionalHeading(lua_State* L) {
	float heading = lua_tonumber(L, -1);

	//SceneObject* obj = (SceneObject*) lua_touserdata(L, -2);

	realObject->setDirection(heading);

	return 0;
}

int LuaSceneObject::setContainerInheritPermissionsFromParent(lua_State* L) {
	bool val = lua_toboolean(L, -1);

	Locker locker(realObject);

	realObject->setContainerInheritPermissionsFromParent(val);

	return 0;
}

int LuaSceneObject::setContainerAllowPermission(lua_State* L) {
	String group = lua_tostring(L, -2);
	uint16 perm = lua_tointeger(L, -1);

	Locker locker(realObject);

	realObject->setContainerAllowPermission(group, perm);

	return 0;
}

int LuaSceneObject::setContainerDenyPermission(lua_State* L) {
	String group = lua_tostring(L, -2);
	uint16 perm = lua_tointeger(L, -1);

	Locker locker(realObject);

	realObject->setContainerDenyPermission(group, perm);

	return 0;
}

int LuaSceneObject::setContainerDefaultAllowPermission(lua_State* L) {
	uint16 perm = lua_tointeger(L, -1);

	Locker locker(realObject);

	realObject->setContainerDefaultAllowPermission(perm);

	return 0;
}

int LuaSceneObject::clearContainerDefaultAllowPermission(lua_State* L) {
	uint16 perm = lua_tointeger(L, -1);

	Locker locker(realObject);

	realObject->clearContainerDefaultAllowPermission(perm);

	return 0;
}

int LuaSceneObject::setContainerDefaultDenyPermission(lua_State* L) {
	uint16 perm = lua_tointeger(L, -1);

	Locker locker(realObject);

	realObject->setContainerDefaultDenyPermission(perm);

	return 0;
}

int LuaSceneObject::clearContainerDefaultDenyPermission(lua_State* L) {
	uint16 perm = lua_tointeger(L, -1);

	Locker locker(realObject);

	realObject->clearContainerDefaultDenyPermission(perm);

	return 0;
}

int LuaSceneObject::setContainerOwnerID(lua_State* L) {
	uint64 ownerID = lua_tointeger(L, -1);

	Locker locker(realObject);

	realObject->setContainerOwnerID(ownerID);

	return 0;
}

int LuaSceneObject::setObjectName(lua_State* L) {
	String file = lua_tostring(L, -3);
	String key = lua_tostring(L, -2);
	bool notifyClient = lua_toboolean(L, -1);

	StringId stringid(file, key);

	realObject->setObjectName(stringid, notifyClient);

	return 0;
}

int LuaSceneObject::isASubChildOf(lua_State* L) {
	SceneObject* obj = (SceneObject*) lua_touserdata(L, -1);

	lua_pushboolean(L, realObject->isASubChildOf(obj));

	return 1;
}

int LuaSceneObject::isOwned(lua_State* L) {
	lua_pushboolean(L, realObject->isPet() || realObject->isVehicleObject());

	return 1;
}

int LuaSceneObject::addPendingTask(lua_State* L) {
	uint32 mili = lua_tonumber(L, -3);
	String play = lua_tostring(L, -2);
	String key = lua_tostring(L, -1);

	Reference<ScreenPlayTask*> task = new ScreenPlayTask(realObject, key, play, "");

	String name = play + ":" + key;

	realObject->addPendingTask(name, task, mili);

	return 0;
}

int LuaSceneObject::cancelPendingTask(lua_State* L) {
	String play = lua_tostring(L, -2);
	String key = lua_tostring(L, -1);

	String name = play + ":" + key;

	if (realObject->containsPendingTask(name)) {
		Reference<ScreenPlayTask*> task = realObject->getPendingTask(name).castTo<ScreenPlayTask*>();

		if (task != NULL && task->isScheduled()) {
			task->cancel();
		}

		realObject->removePendingTask(name);
	}

	return 0;
}

int LuaSceneObject::getChildObject(lua_State* L) {
	int index = lua_tonumber(L, -1);

	SceneObject* obj = realObject->getChildObjects()->get(index);

	if (obj == NULL) {
		lua_pushnil(L);
	} else {
		obj->_setUpdated(true);
		lua_pushlightuserdata(L, obj);
	}

	return 1;
}

int LuaSceneObject::getContainerOwnerID(lua_State *L) {

	lua_pushnumber(L, realObject->getContainerPermissions()->getOwnerID());

	return 1;
}

int LuaSceneObject::info(lua_State* L) {
	String msg = lua_tostring(L, -1);

	realObject->info(msg, true);

	return 0;
}

int LuaSceneObject::getPlayersInRange(lua_State *L) {
	int range = lua_tonumber(L, -1);

	Zone* thisZone = realObject->getZone();

	if (thisZone == NULL) {
		lua_pushnil(L);
		return 1;
	}

	lua_newtable(L);

	Reference<SortedVector<ManagedReference<QuadTreeEntry*> >*> closeObjects = new SortedVector<ManagedReference<QuadTreeEntry*> >();
	thisZone->getInRangeObjects(realObject->getWorldPositionX(), realObject->getWorldPositionY(), range, closeObjects, true);
	int numPlayers = 0;

	for (int i = 0; i < closeObjects->size(); ++i) {
		SceneObject* object = cast<SceneObject*>(closeObjects->get(i).get());

		if (object == NULL || !object->isPlayerCreature())
			continue;

		CreatureObject* player = object->asCreatureObject();

		if (player == NULL || player->isInvisible())
			continue;

		numPlayers++;
		lua_pushlightuserdata(L, object);
		lua_rawseti(L, -2, numPlayers);
	}

	return 1;
}

int LuaSceneObject::isInNavMesh(lua_State* L) {
	bool val = realObject->isInNavMesh();

	lua_pushboolean(L, val);

	return 1;
}
