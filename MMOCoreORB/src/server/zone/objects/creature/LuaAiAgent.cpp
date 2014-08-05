/*
 * LuaAiAgent.cpp
 *
 *  Created on: 21/01/2012
 *      Author: victor
 */

#include "LuaAiAgent.h"

#include <engine/core/ManagedReference.h>
#include <engine/lua/Luna.h>
#include <lua.h>
#include <stddef.h>
#include <system/lang/ref/Reference.h>
#include <system/lang/String.h>
#include <system/platform.h>

#include "../../../../autogen/server/chat/ChatManager.h"
#include "../../../../autogen/server/zone/ZoneServer.h"
#include "../../../chat/StringIdChatParameter.h"
#include "../../../ServerCore.h"

#include "server/zone/managers/creature/AiMap.h"
#include "server/zone/managers/collision/CollisionManager.h"
#include "server/zone/objects/intangible/PetControlDevice.h"

//#include "server/zone/objects/creature/AiAgent.h"

const char LuaAiAgent::className[] = "LuaAiAgent";

Luna<LuaAiAgent>::RegType LuaAiAgent::Register[] = {
		{ "_setObject", &LuaAiAgent::_setObject },
		{ "_getObject", &LuaAiAgent::_getObject },
		{ "setAiTemplate", &LuaAiAgent::setAiTemplate },
		{ "setFollowObject", &LuaAiAgent::setFollowObject },
		{ "setOblivious", &LuaAiAgent::setOblivious },
		{ "setWatchObject", &LuaAiAgent::setWatchObject },
		{ "setStalkObject", &LuaAiAgent::setStalkObject },
		{ "getFollowObject", &LuaAiAgent::getFollowObject },
		{ "storeFollowObject", &LuaAiAgent::storeFollowObject },
		{ "restoreFollowObject", &LuaAiAgent::restoreFollowObject },
		{ "getTargetOfTargetID", &LuaAiAgent::getTargetOfTargetID },
		{ "getTargetID", &LuaCreatureObject::getTargetID },
		{ "getObjectID", &LuaSceneObject::getObjectID },
		{ "getFollowState", &LuaAiAgent::getFollowState },
		{ "findNextPosition", &LuaAiAgent::findNextPosition },
		{ "getMaxDistance", &LuaAiAgent::getMaxDistance },
		{ "generatePatrol", &LuaAiAgent::generatePatrol },
		{ "setDestination", &LuaAiAgent::setDestination },
		{ "completeMove", &LuaAiAgent::completeMove },
		{ "setWait", &LuaAiAgent::setWait },
		{ "getWait", &LuaAiAgent::getWait },
		{ "isWaiting", &LuaAiAgent::isWaiting },
		{ "stopWaiting", &LuaAiAgent::stopWaiting },
		{ "getCurrentSpeed", &LuaAiAgent::getCurrentSpeed },
		{ "setCurrentSpeed", &LuaAiAgent::setCurrentSpeed },
		{ "getTargetFromMap", &LuaAiAgent::getTargetFromMap },
		{ "getTargetFromDefenders", &LuaAiAgent::getTargetFromDefenders },
		{ "validateTarget", &LuaAiAgent::validateTarget },
		{ "followHasState", &LuaAiAgent::followHasState },
		{ "selectWeapon", &LuaAiAgent::selectWeapon },
		{ "selectDefaultWeapon", &LuaAiAgent::selectDefaultWeapon },
		{ "selectSpecialAttack", &LuaAiAgent::selectSpecialAttack },
		{ "selectDefaultAttack", &LuaAiAgent::selectDefaultAttack },
		{ "validateStateAttack", &LuaAiAgent::validateStateAttack },
		{ "removeDefender", &LuaAiAgent::removeDefender },
		{ "removeDefenders", &LuaAiAgent::removeDefenders },
		{ "enqueueAttack", &LuaAiAgent::enqueueAttack },
		{ "isRetreating", &LuaAiAgent::isRetreating },
		{ "isFleeing", &LuaAiAgent::isFleeing },
		{ "runAway", &LuaAiAgent::runAway },
		{ "getKinetic", &LuaAiAgent::getKinetic },
		{ "getEnergy", &LuaAiAgent::getEnergy },
		{ "getElectricity", &LuaAiAgent::getElectricity },
		{ "getStun", &LuaAiAgent::getStun },
		{ "getBlast", &LuaAiAgent::getBlast },
		{ "getHeat", &LuaAiAgent::getHeat },
		{ "getCold", &LuaAiAgent::getCold },
		{ "getAcid", &LuaAiAgent::getAcid },
		{ "getLightSaber", &LuaAiAgent::getLightSaber },
		{ "isStalker", &LuaAiAgent::isStalker },
		{ "isKiller", &LuaAiAgent::isKiller },
		{ "getFerocity", &LuaAiAgent::getFerocity },
		{ "getArmor", &LuaAiAgent::getArmor },
		{ "getDespawnOnNoPlayerInRange", &LuaAiAgent::getDespawnOnNoPlayerInRange },
		{ "getNumberOfPlayersInRange", &LuaAiAgent::getNumberOfPlayersInRange },
		{ "getFactionString", &LuaAiAgent::getFactionString },
		{ "getChanceHit", &LuaAiAgent::getChanceHit },
		{ "getDamageMin", &LuaAiAgent::getDamageMin },
		{ "getDamageMax", &LuaAiAgent::getDamageMax },
		{ "getBaseXp", &LuaAiAgent::getBaseXp },
		{ "getDiet", &LuaAiAgent::getDiet },
		{ "hasLoot", &LuaAiAgent::hasLoot },
		{ "isEventMob", &LuaAiAgent::isEventMob },
		{ "isPet", &LuaAiAgent::isPet },
		{ "getPvPFaction", &LuaAiAgent::getPvPFaction },
		{ "getAvgSpeed", &LuaAiAgent::getAvgSpeed },
		{ "isAggressiveTo", &LuaAiAgent::isAggressiveTo },
		{ "isAttackableBy", &LuaAiAgent::isAttackableBy },
		{ "isScentMasked", &LuaAiAgent::isScentMasked },
		{ "isConcealed", &LuaAiAgent::isConcealed },
		{ "isCamouflaged", &LuaAiAgent::isCamouflaged },
		{ "shouldRetreat", &LuaAiAgent::shouldRetreat },
		{ "clearCombatState", &LuaAiAgent::clearCombatState },
		{ "isInCombat", &LuaAiAgent::isInCombat },
		{ "checkLineOfSight", &LuaAiAgent::checkLineOfSight },
		{ "activateRecovery", &LuaAiAgent::activateRecovery },
		{ "activateAwareness", &LuaAiAgent::activateAwareness },
		{ "setBehaviorStatus", &LuaAiAgent::setBehaviorStatus },
		{ "getBehaviorStatus", &LuaAiAgent::getBehaviorStatus },
		{ "resetBehaviorList", &LuaAiAgent::resetBehaviorList },
		{ "executeBehavior", &LuaAiAgent::executeBehavior },
		{ "info", &LuaAiAgent::info },
		{ "spatialChat", &LuaAiAgent::spatialChat },
		{ "setDefender", &LuaAiAgent::setDefender },
		{ "addDefender", &LuaAiAgent::addDefender },
		{ "assist", &LuaAiAgent::assist },
		{ "checkRange", &LuaAiAgent::checkRange },
		{ "broadcastInterrupt", &LuaAiAgent::broadcastInterrupt },
		{ "getSocialGroup", &LuaAiAgent::getSocialGroup },
		{ "getOwner", &LuaAiAgent::getOwner },
		{ "getLastCommand", &LuaAiAgent::getLastCommand },
		{ "setAlertDuration", &LuaAiAgent::setAlertDuration },
		{ "alertedTimeIsPast", &LuaAiAgent::alertedTimeIsPast },
		{ 0, 0 }
};


LuaAiAgent::LuaAiAgent(lua_State *L) : LuaCreatureObject(L) {
	realObject = static_cast<AiAgent*>(lua_touserdata(L, 1));
}

LuaAiAgent::~LuaAiAgent(){
}

int LuaAiAgent::_setObject(lua_State* L) {
	realObject = static_cast<AiAgent*>(lua_touserdata(L, -1));

	return 0;
}

int LuaAiAgent::_getObject(lua_State* L) {
	lua_pushlightuserdata(L, realObject.get());

	return 1;
}

int LuaAiAgent::setAiTemplate(lua_State* L) {
	String tempName = lua_tostring(L, -1);

	realObject->activateLoad(tempName);

	return 0;
}

int LuaAiAgent::setFollowObject(lua_State* L) {
	SceneObject* obj = (SceneObject*) lua_touserdata(L, -1);

	realObject->setFollowObject(obj);

	return 0;
}

int LuaAiAgent::setOblivious(lua_State* L) {
	realObject->setOblivious();
	return 0;
}

int LuaAiAgent::setWatchObject(lua_State* L) {
	SceneObject* obj = (SceneObject*) lua_touserdata(L, -1);
	realObject->setWatchObject(obj);
	return 0;
}

int LuaAiAgent::setStalkObject(lua_State* L) {
	SceneObject* obj = (SceneObject*) lua_touserdata(L, -1);
	realObject->setStalkObject(obj);
	return 0;
}

int LuaAiAgent::getFollowObject(lua_State* L) {
	SceneObject* followObject = realObject->getFollowObject();

	if (followObject == NULL)
		lua_pushnil(L);
	else
		lua_pushlightuserdata(L, followObject);

	return 1;
}

int LuaAiAgent::storeFollowObject(lua_State* L) {
	realObject->storeFollowObject();
	return 0;
}

int LuaAiAgent::restoreFollowObject(lua_State* L) {
	realObject->restoreFollowObject();
	return 0;
}

int LuaAiAgent::getTargetOfTargetID(lua_State* L) {
	SceneObject* target = realObject->getFollowObject();
	if (target == NULL || !target->isCreatureObject()) {
		lua_pushnil(L);
		return 1;
	}

	CreatureObject* targetCreo = cast<CreatureObject*>(target);
	if (targetCreo == NULL) {
		lua_pushnil(L);
		return 1;
	}

	lua_pushinteger(L, targetCreo->getTargetID());

	return 1;
}

int LuaAiAgent::getFollowState(lua_State* L) {
	unsigned int state = realObject->getFollowState();

	lua_pushinteger(L, state);

	return 1;
}

/**
 * @param takes a float maxDistance argument
 * @return bool to lua depending on if a new position was found or not
 */
int LuaAiAgent::findNextPosition(lua_State* L) {
	bool walk = lua_toboolean(L, -1);
	float maxDistance = lua_tonumber(L, -2);

	bool found = realObject->findNextPosition(maxDistance, walk);

	lua_pushboolean(L, found);

	return 1;
}

int LuaAiAgent::getMaxDistance(lua_State* L) {
	float maxDistance = realObject->getMaxDistance();

	lua_pushnumber(L, maxDistance);

	return 1;
}

int LuaAiAgent::generatePatrol(lua_State* L) {
	float dist = lua_tonumber(L, -1);
	int num = lua_tointeger(L, -2);

	bool retVal = realObject->generatePatrol(num, dist);

	lua_pushboolean(L, retVal);

	return 1;
}

int LuaAiAgent::setDestination(lua_State* L) {
	int retVal = realObject->setDestination();

	lua_pushinteger(L, retVal);

	return 1;
}

int LuaAiAgent::completeMove(lua_State* L) {
	bool retVal = realObject->completeMove();

	lua_pushboolean(L, retVal);

	return 1;
}

int LuaAiAgent::setWait(lua_State* L) {
	float seconds = lua_tonumber(L, -1);

	realObject->setWait((int)(seconds*1000));

	return 0;
}

int LuaAiAgent::getWait(lua_State* L) {
	int waitTime = realObject->getWait();

	lua_pushinteger(L, waitTime);

	return 1;
}

int LuaAiAgent::isWaiting(lua_State* L) {
	bool retVal = realObject->isWaiting();

	lua_pushboolean(L, retVal);

	return 1;
}

int LuaAiAgent::stopWaiting(lua_State* L) {
	realObject->stopWaiting();

	return 0;
}

int LuaAiAgent::getCurrentSpeed(lua_State* L) {
	float currentSpeed = realObject->getCurrentSpeed();

	lua_pushnumber(L, currentSpeed);

	return 1;
}

int LuaAiAgent::setCurrentSpeed(lua_State* L) {
	float currentSpeed = lua_tonumber(L, -1);

	realObject->setCurrentSpeed(currentSpeed);

	return 0;
}

int LuaAiAgent::getTargetFromMap(lua_State* L) {
	SceneObject* retVal = realObject->getTargetFromMap();

	if (retVal == NULL)
		lua_pushnil(L);
	else
		lua_pushlightuserdata(L, retVal);

	return 1;
}

int LuaAiAgent::getTargetFromDefenders(lua_State* L) {
	SceneObject* retVal = realObject->getTargetFromDefenders();

	if (retVal == NULL)
		lua_pushnil(L);
	else
		lua_pushlightuserdata(L, retVal);

	return 1;
}

int LuaAiAgent::validateTarget(lua_State* L) {
	bool retVal = realObject->validateTarget();

	lua_pushboolean(L, retVal);

	return 1;
}

int LuaAiAgent::followHasState(lua_State* L) {
	SceneObject* follow = realObject->getFollowObject();

	if (follow == NULL || !follow->isCreatureObject()) {
		lua_pushboolean(L, false);
		return 1;
	}

	int state = lua_tointeger(L, -1);

	bool retVal = cast<CreatureObject*>(follow)->hasState(state);

	lua_pushboolean(L, retVal);

	return 1;
}

int LuaAiAgent::selectWeapon(lua_State* L) {
	realObject->selectWeapon();
	return 0;
}

int LuaAiAgent::selectDefaultWeapon(lua_State* L) {
	realObject->selectDefaultWeapon();
	return 0;
}

int LuaAiAgent::selectSpecialAttack(lua_State* L) {
	int attackNum = lua_tointeger(L, -1);

	if (attackNum == -1)
		realObject->selectSpecialAttack();
	else
		realObject->selectSpecialAttack(attackNum);

	return 0;
}

int LuaAiAgent::selectDefaultAttack(lua_State* L) {
	realObject->selectDefaultAttack();

	return 0;
}

int LuaAiAgent::validateStateAttack(lua_State* L) {
	bool retVal = realObject->validateStateAttack();

	lua_pushboolean(L, retVal);

	return 1;
}

int LuaAiAgent::removeDefender(lua_State* L) {
	realObject->removeDefender(realObject->getFollowObject());

	return 0;
}

int LuaAiAgent::removeDefenders(lua_State* L) {
	realObject->removeDefenders();

	return 0;
}

int LuaAiAgent::enqueueAttack(lua_State* L) {
	realObject->enqueueAttack();

	return 0;
}

int LuaAiAgent::isRetreating(lua_State* L) {
	bool retVal = realObject->isRetreating();
	lua_pushboolean(L, retVal);
	return 1;
}

int LuaAiAgent::isFleeing(lua_State* L) {
	bool retVal = realObject->isFleeing();
	lua_pushboolean(L, retVal);
	return 1;
}

int LuaAiAgent::runAway(lua_State* L) {
	CreatureObject* target = static_cast<CreatureObject*>(lua_touserdata(L, -2));
	float range = lua_tonumber(L, -1);

	if (target != NULL)
		realObject->runAway(target, range);

	return 0;
}

int LuaAiAgent::getKinetic(lua_State* L) {
	lua_pushnumber(L, realObject->getKinetic());
	return 1;
}

int LuaAiAgent::getEnergy(lua_State* L) {
	lua_pushnumber(L, realObject->getEnergy());
	return 1;
}

int LuaAiAgent::getElectricity(lua_State* L) {
	lua_pushnumber(L, realObject->getElectricity());
	return 1;
}

int LuaAiAgent::getStun(lua_State* L) {
	lua_pushnumber(L, realObject->getStun());
	return 1;
}

int LuaAiAgent::getBlast(lua_State* L) {
	lua_pushnumber(L, realObject->getBlast());
	return 1;
}

int LuaAiAgent::getHeat(lua_State* L) {
	lua_pushnumber(L, realObject->getHeat());
	return 1;
}

int LuaAiAgent::getCold(lua_State* L) {
	lua_pushnumber(L, realObject->getCold());
	return 1;
}

int LuaAiAgent::getAcid(lua_State* L) {
	lua_pushnumber(L, realObject->getAcid());
	return 1;
}

int LuaAiAgent::getLightSaber(lua_State* L) {
	lua_pushnumber(L, realObject->getLightSaber());
	return 1;
}

int LuaAiAgent::isStalker(lua_State* L) {
	lua_pushboolean(L, realObject->isStalker());
	return 1;
}

int LuaAiAgent::isKiller(lua_State* L) {
	lua_pushboolean(L, realObject->isKiller());
	return 1;
}

int LuaAiAgent::getFerocity(lua_State* L) {
	lua_pushinteger(L, realObject->getFerocity());
	return 1;
}

int LuaAiAgent::getArmor(lua_State* L) {
	lua_pushinteger(L, realObject->getArmor());
	return 1;
}

int LuaAiAgent::getDespawnOnNoPlayerInRange(lua_State* L) {
	lua_pushboolean(L, realObject->getDespawnOnNoPlayerInRange());
	return 1;
}

int LuaAiAgent::getNumberOfPlayersInRange(lua_State* L) {
	lua_pushinteger(L, realObject->getNumberOfPlayersInRange());
	return 1;
}

int LuaAiAgent::getFactionString(lua_State* L) {
	lua_pushstring(L, realObject->getFactionString().toCharArray());
	return 1;
}

int LuaAiAgent::getChanceHit(lua_State* L) {
	lua_pushnumber(L, realObject->getChanceHit());
	return 1;
}

int LuaAiAgent::getDamageMin(lua_State* L) {
	lua_pushinteger(L, realObject->getDamageMin());
	return 1;
}

int LuaAiAgent::getDamageMax(lua_State* L) {
	lua_pushinteger(L, realObject->getDamageMax());
	return 1;
}

int LuaAiAgent::getBaseXp(lua_State* L) {
	lua_pushinteger(L, realObject->getBaseXp());
	return 1;
}

int LuaAiAgent::getDiet(lua_State* L) {
	lua_pushinteger(L, realObject->getDiet());
	return 1;
}

int LuaAiAgent::hasLoot(lua_State* L) {
	lua_pushboolean(L, realObject->hasLoot());
	return 1;
}

int LuaAiAgent::isEventMob(lua_State* L) {
	lua_pushboolean(L, realObject->isEventMob());
	return 1;
}

int LuaAiAgent::isPet(lua_State* L) {
	lua_pushboolean(L, realObject->isPet());
	return 1;
}

int LuaAiAgent::getPvPFaction(lua_State* L) {
	lua_pushstring(L, realObject->getPvPFaction().toCharArray());
	return 1;
}

int LuaAiAgent::getAvgSpeed(lua_State* L) {
	lua_pushnumber(L, realObject->getAvgSpeed());
	return 1;
}

int LuaAiAgent::isAggressiveTo(lua_State* L) {
	CreatureObject* obj = (CreatureObject*) lua_touserdata(L, -1);

	bool retVal = realObject->isAggressiveTo(obj);
	lua_pushboolean(L, retVal);

	return 1;
}

int LuaAiAgent::isAttackableBy(lua_State* L) {
	CreatureObject* obj = (CreatureObject*) lua_touserdata(L, -1);

	bool retVal = realObject->isAttackableBy(obj);
	lua_pushboolean(L, retVal);

	return 1;
}

int LuaAiAgent::isScentMasked(lua_State* L) {
	CreatureObject* obj = (CreatureObject*) lua_touserdata(L, -1);

	bool retVal = realObject->isScentMasked(obj);
	lua_pushboolean(L, retVal);

	return 1;
}

int LuaAiAgent::isConcealed(lua_State* L) {
	CreatureObject* obj = (CreatureObject*) lua_touserdata(L, -1);

	bool retVal = realObject->isConcealed(obj);
	lua_pushboolean(L, retVal);

	return 1;
}

int LuaAiAgent::isCamouflaged(lua_State* L) {
	CreatureObject* obj = (CreatureObject*) lua_touserdata(L, -1);

	bool retVal = realObject->isCamouflaged(obj);
	lua_pushboolean(L, retVal);

	return 1;
}

int LuaAiAgent::shouldRetreat(lua_State* L) {
	float range = lua_tonumber(L, -1);
	PatrolPoint* homeLocation = realObject->getHomeLocation();

	bool retVal;
	SceneObject* target = realObject->getFollowObject();

	if (target != NULL)
		retVal = !homeLocation->isInRange(target, range);
	else
		retVal = !homeLocation->isInRange(realObject, range);

	lua_pushboolean(L, retVal);

	return 1;
}

int LuaAiAgent::clearCombatState(lua_State* L) {
	bool clearDefenders = lua_toboolean(L, -1);

	realObject->clearCombatState(clearDefenders);

	return 0;
}

int LuaAiAgent::isInCombat(lua_State* L) {
	bool retVal = realObject->isInCombat();

	lua_pushboolean(L, retVal);

	return 1;
}

int LuaAiAgent::checkLineOfSight(lua_State* L) {
	SceneObject* obj = (SceneObject*) lua_touserdata(L, -1);
	bool retVal = CollisionManager::checkLineOfSight(realObject.get(), obj);

	lua_pushboolean(L, retVal);

	return 1;
}

int LuaAiAgent::activateRecovery(lua_State* L) {
	realObject->activateRecovery();

	return 0;
}

int LuaAiAgent::activateAwareness(lua_State* L) {
	CreatureObject* target = (CreatureObject*) lua_touserdata(L, -1);
	realObject->activateAwarenessEvent(target);

	return 0;
}

int LuaAiAgent::setBehaviorStatus(lua_State* L) {
	uint8 status = (uint8) lua_tointeger(L, -1);

	realObject->setBehaviorStatus(status);

	return 0;
}

int LuaAiAgent::getBehaviorStatus(lua_State* L) {
	lua_pushnumber(L, realObject->getBehaviorStatus());

	return 1;
}

int LuaAiAgent::resetBehaviorList(lua_State* L) {
	realObject->resetBehaviorList();
	return 0;
}

int LuaAiAgent::executeBehavior(lua_State* L) {
	realObject->activateMovementEvent();
	return 0;
}

int LuaAiAgent::info(lua_State* L) {
	String msg = lua_tostring(L, -1);

	realObject->info(msg, true);

	return 0;
}

int LuaAiAgent::spatialChat(lua_State* L) {
	ZoneServer* zoneServer = ServerCore::getZoneServer();
	if (zoneServer == NULL)
		return 0;

	ChatManager* chatManager = zoneServer->getChatManager();

	if (lua_islightuserdata(L, -1)) {
		StringIdChatParameter* message = (StringIdChatParameter*)lua_touserdata(L, -1);

		if (realObject != NULL && message != NULL) {
			chatManager->broadcastMessage(realObject, *message, 0, 0, 0);
		}
	} else {
		String message = lua_tostring(L, -1);

		if (realObject != NULL) {
			chatManager->broadcastMessage(realObject, message, 0, 0, 0);
		}
	}

	return 0;
}

int LuaAiAgent::setDefender(lua_State* L) {
	SceneObject* obj = (SceneObject*) lua_touserdata(L, -1);

	Locker locker(realObject);

	realObject->setDefender(obj);

	return 0;
}

int LuaAiAgent::addDefender(lua_State* L) {
	SceneObject* obj = (SceneObject*) lua_touserdata(L, -1);

	Locker locker(realObject);

	realObject->addDefender(obj);

	return 0;
}

int LuaAiAgent::assist(lua_State* L) {
	SceneObject* obj = (SceneObject*) lua_touserdata(L, -1);
	if (obj == NULL || !obj->isAiAgent())
		return 0;

	AiAgent* agent = cast<AiAgent*>(obj);
	if (agent == NULL)
		return 0;

	SceneObject* target = agent->getFollowObject();

	Locker locker(realObject);

	realObject->setDefender(target);

	return 0;
}

int LuaAiAgent::checkRange(lua_State* L) {
	float dist = lua_tonumber(L, -1);
	SceneObject* obj = (SceneObject*) lua_touserdata(L, -2);

	bool retVal = false;
	if (realObject->isInRange(obj, dist))
		retVal = true;

	lua_pushboolean(L, retVal);
	return 1;
}

int LuaAiAgent::broadcastInterrupt(lua_State* L) {
	int msg = lua_tointeger(L, -1);

	realObject->broadcastInterrupt(msg);

	return 0;
}

int LuaAiAgent::getSocialGroup(lua_State* L) {
	String socGroup = realObject->getSocialGroup();

	lua_pushstring(L, socGroup.toCharArray());

	return 1;
}

int LuaAiAgent::getOwner(lua_State* L) {
	CreatureObject* retVal = realObject->getLinkedCreature().get();

	if (retVal == NULL)
		lua_pushnil(L);
	else
		lua_pushlightuserdata(L, retVal);
	return 1;
}

int LuaAiAgent::getLastCommand(lua_State* L) {
	ManagedReference<PetControlDevice*> controlDevice = realObject->getControlDevice().castTo<PetControlDevice*>();

	if (controlDevice == NULL)
		lua_pushinteger(L, 0);
	else
		lua_pushinteger(L, controlDevice->getLastCommand());

	return 1;
}

int LuaAiAgent::setAlertDuration(lua_State* L) {
	int duration = lua_tointeger(L, -1);

	Time* alert = realObject->getAlertedTime();
	if (alert != NULL) {
		alert->updateToCurrentTime();
		alert->addMiliTime(duration);
	}

	return 0;
}

int LuaAiAgent::alertedTimeIsPast(lua_State* L) {
	Time* alert = realObject->getAlertedTime();

	if (alert != NULL)
		lua_pushboolean(L, alert->isPast());
	else
		lua_pushboolean(L, true);

	return 1;
}
