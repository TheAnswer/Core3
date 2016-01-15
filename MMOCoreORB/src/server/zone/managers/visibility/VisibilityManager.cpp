/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions. */

#include "VisibilityManager.h"
#include "server/zone/managers/mission/MissionManager.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/managers/visibility/tasks/VisibilityDecayTask.h"
#include "server/zone/Zone.h"

const String VisibilityManager::factionStringRebel = "rebel";
const String VisibilityManager::factionStringImperial = "imperial";
const unsigned int VisibilityManager::factionRebel = factionStringRebel.hashCode();
const unsigned int VisibilityManager::factionImperial = factionStringImperial.hashCode();

void VisibilityManager::addPlayerToBountyList(CreatureObject* creature, int reward) {
	MissionManager* missionManager = creature->getZoneServer()->getMissionManager();
	missionManager->addPlayerToBountyList(creature->getObjectID(), reward);
	info("Adding player " + String::valueOf(creature->getObjectID()) + " to bounty hunter list.", true);
}

void VisibilityManager::removePlayerFromBountyList(CreatureObject* creature) {
	MissionManager* missionManager = creature->getZoneServer()->getMissionManager();
	missionManager->removePlayerFromBountyList(creature->getObjectID());
	info("Dropping player " + creature->getFirstName() + " from bounty hunter list.", true);
}

int VisibilityManager::calculateReward(CreatureObject* creature) {
	int minReward = 25000; // Minimum reward for a player bounty
	int maxReward = 250000; // Maximum reward for a player bounty

	int reward = minReward;

	Reference<PlayerObject*> ghost = creature->getSlottedObject("ghost").castTo<PlayerObject*>();

	if (ghost != NULL) {
		int skillPoints = ghost->getSpentJediSkillPoints();

		reward = skillPoints * 1000;

		if (reward < minReward)
			reward = minReward;
		else if (reward > maxReward)
			reward = maxReward;
	}

	return reward;
}

float VisibilityManager::calculateVisibilityIncrease(CreatureObject* creature) {
	ManagedReference<Zone*> zone = creature->getZone();

	float visibilityIncrease = 0;

	if (zone != NULL) {
		SortedVector<QuadTreeEntry*> closeObjects;
		CloseObjectsVector* closeObjectsVector = (CloseObjectsVector*) creature->getCloseObjects();
		if (closeObjectsVector == NULL) {
			zone->getInRangeObjects(creature->getWorldPositionX(), creature->getWorldPositionY(), 32, &closeObjects, true);
		} else {
			closeObjectsVector->safeCopyTo(closeObjects);
		}

		for (int i = 0; i < closeObjects.size(); ++i) {
			SceneObject* obj = cast<SceneObject*>(closeObjects.get(i));
			if (obj != NULL && obj->isCreatureObject() && creature->isInRange(obj, 32)) {
				ManagedReference<CreatureObject*> c = cast<CreatureObject*>(obj);
				if (c->isNonPlayerCreatureObject() || c->isPlayerCreature()) {
					if (creature->getFaction() == 0 || (c->getFaction() != factionImperial && c->getFaction() != factionRebel)) {
						visibilityIncrease += 0.5;
						//info(c->getCreatureName().toString() + " generating a 0.5 visibility modifier", true);
					} else {
						if (creature->getFaction() == c->getFaction()) {
							visibilityIncrease += 0.25;
							//info(c->getCreatureName().toString() + " generating a 0.25 visibility modifier", true);
						} else {
							visibilityIncrease += 1;
							//info( c->getCreatureName().toString() + " generating a 1.0 visibility modifier", true);
						}
					}
				}
			}
		}
	}

	//info("Increasing visibility for player " + String::valueOf(creature->getObjectID()) + " with " + String::valueOf(visibilityIncrease), true);
	return visibilityIncrease;
}

void VisibilityManager::decreaseVisibility(CreatureObject* creature) {

	Reference<PlayerObject*> ghost = creature->getSlottedObject("ghost").castTo<PlayerObject*>();

	if (ghost != NULL) {
		Locker locker(ghost);
		if (ghost->getVisibility() > 0)
		{

			//info("VisDecayTickRate: " + String::valueOf(visDecayTickRate) + " DecayPerTick: " + String::valueOf(visDecayPerTick), true);
			float visibilityDecrease = (((ghost->getLastVisibilityUpdateTimestamp().miliDifference() / 1000.0f) / visDecayTickRate) * visDecayPerTick);

			//info("Decreasing visibility of player " + creature->getFirstName() + " by " + String::valueOf(visibilityDecrease), true);
			if (ghost->getVisibility() <= visibilityDecrease) {
				clearVisibility(creature);
			} else {
				ghost->setVisibility(ghost->getVisibility() - visibilityDecrease);

				if (ghost->getVisibility() < falloffThreshold) {
					removePlayerFromBountyList(creature);
				}
			}
		}
	}
}

VisibilityManager::VisibilityManager() : Logger("VisibilityManager") {
	Reference<Task*> decayTask = new VisibilityDecayTask();
	decayTask->schedule(visDecayTickRate * 1000);
	loadConfiguration();
}

void VisibilityManager::login(CreatureObject* creature) {
	//info("Logging in " + creature->getFirstName(), true);
	Reference<PlayerObject*> ghost = creature->getSlottedObject("ghost").castTo<PlayerObject*>();

	if (ghost != NULL) {

		//You only gain visibility after completing the padawan trials
		if(!creature->hasSkill("force_title_jedi_rank_02")) {
			//info("Player " + creature->getFirstName() + " does not qualify for visibility", true);
			return;
		}

		decreaseVisibility(creature);

		Locker locker(&visibilityListLock);

		if ((ghost->getVisibility() > 0) && (!visibilityList.contains(creature->getObjectID()))) {
			//info("Adding player " + String::valueOf(creature->getObjectID()) + " to visibility list.", true);
			visibilityList.put(creature->getObjectID(), creature);
		}

		locker.release();

		if (ghost->getVisibility() >= terminalVisThreshold) {
			// TODO: Readjust after FRS implementation.
			// +100k per FRS level
			int reward = calculateReward(creature);
			addPlayerToBountyList(creature, reward);
		}
	}
}

void VisibilityManager::logout(CreatureObject* creature) {
	//info("Logging out " + creature->getFirstName(), true);
	Locker locker(&visibilityListLock);

	if (visibilityList.contains(creature->getObjectID())) {
		//info("Dropping player " + String::valueOf(creature->getObjectID()) + " from visibility list.", true);
		visibilityList.drop(creature->getObjectID());

		removePlayerFromBountyList(creature);
	}
}

void VisibilityManager::increaseVisibility(CreatureObject* creature, int visibilityMultiplier) {
	//info("Increasing visibility for " + creature->getFirstName(), true);
	Reference<PlayerObject*> ghost = creature->getSlottedObject("ghost").castTo<PlayerObject*>();

	if (ghost != NULL  && !ghost->hasGodMode()) {
		Locker locker(ghost);
		decreaseVisibility(creature);

		float newVis = ghost->getVisibility() + (calculateVisibilityIncrease(creature) * visibilityMultiplier); // Calculate new total vis
		newVis = MIN(maxVisibility,  newVis); // Cap visibility

		ghost->setVisibility(newVis);

		//info("New visibility for " + creature->getFirstName() + " is " + String::valueOf(ghost->getVisibility()), true);
		locker.release();

		login(creature);
	}
}

void VisibilityManager::clearVisibility(CreatureObject* creature) {
	Reference<PlayerObject*> ghost = creature->getSlottedObject("ghost").castTo<PlayerObject*>();

	if (ghost != NULL  && !ghost->hasGodMode()) {
		//info("Clearing visibility for player " + String::valueOf(creature->getObjectID()), true);

		Locker locker(ghost);
		ghost->setVisibility(0);
		locker.release();

		logout(creature);
	}
}

void VisibilityManager::performVisiblityDecay() {
	//info("Performing visibility decay (List Size: " + String::valueOf(visibilityList.size()) +")", true);
	Locker locker(&visibilityListLock);

	for (int i = 0; i < visibilityList.size(); i++) {
		ManagedReference<CreatureObject*> creature = visibilityList.get(i);
		decreaseVisibility(creature);
	}
}

void VisibilityManager::loadConfiguration() {
	try {

		Lua* lua = new Lua();
		lua->init();

		lua->runFile("scripts/managers/jedi/visibility_manager.lua");

		maxVisibility  = (float)lua->getGlobalInt(String("maxVisibility"));
		terminalVisThreshold = (float)lua->getGlobalInt(String("termThreshold"));
		falloffThreshold = (float)lua->getGlobalInt(String("falloffThreshold"));
		pvpRatingDivisor = (float)lua->getGlobalInt(String("pvpRatingDivisor"));

		totalDecayTimeInDays = lua->getGlobalInt(String("totalDecayTimeInDays"));
		visDecayTickRate = lua->getGlobalInt(String("tickRateInSeconds"));

		visDecayPerTick = (maxVisibility / ((totalDecayTimeInDays * (float)(60*60*24) / visDecayTickRate)));

		delete lua;

	} catch(Exception e) {
		error(e.getMessage());
	}
}

