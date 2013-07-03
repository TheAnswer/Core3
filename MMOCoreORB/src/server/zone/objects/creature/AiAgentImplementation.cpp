/*
 * AiAgentImplementation.cpp
 *
 *  Created on: 13/07/2010
 *      Author: victor
 */

#include "AiAgent.h"

#include "events/AiThinkEvent.h"
#include "events/AiMoveEvent.h"
#include "events/AiWaitEvent.h"
#include "events/AiAwarenessEvent.h"
#include "events/RespawnCreatureTask.h"
#include "events/DespawnCreatureOnPlayerDissappear.h"
#include "server/zone/managers/combat/CombatManager.h"
#include "server/zone/managers/creature/CreatureManager.h"
#include "server/zone/managers/conversation/ConversationManager.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/creature/conversation/ConversationObserver.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/objects/tangible/weapon/WeaponObject.h"
#include "server/zone/managers/templates/TemplateManager.h"
#include "server/zone/templates/mobile/CreatureTemplate.h"
#include "server/zone/managers/creature/CreatureTemplateManager.h"
#include "server/zone/managers/combat/CombatManager.h"
#include "server/zone/managers/player/PlayerManager.h"
#include "server/zone/managers/planet/PlanetManager.h"
#include "server/zone/managers/loot/LootManager.h"
#include "server/zone/managers/collision/PathFinderManager.h"
#include "server/zone/managers/collision/CollisionManager.h"
#include "server/zone/packets/scene/UpdateTransformMessage.h"
#include "server/zone/packets/scene/LightUpdateTransformMessage.h"
#include "server/zone/packets/scene/LightUpdateTransformWithParentMessage.h"
#include "server/zone/packets/scene/UpdateTransformWithParentMessage.h"
#include "server/zone/packets/object/StringList.h"
#include "server/zone/packets/object/NpcConversationMessage.h"
#include "server/zone/packets/object/StartNpcConversation.h"
#include "server/zone/packets/object/StopNpcConversation.h"
#include "server/zone/objects/creature/events/DespawnCreatureTask.h"
#include "server/zone/Zone.h"
#include "server/zone/ZoneServer.h"
#include "PatrolPoint.h"
#include "server/zone/templates/appearance/PortalLayout.h"
#include "server/zone/templates/appearance/FloorMesh.h"
#include "server/zone/objects/tangible/threat/ThreatMap.h"
#include "server/zone/packets/ui/CreateClientPathMessage.h"
#include "server/zone/packets/creature/CreatureObjectDeltaMessage4.h"
#include "server/zone/managers/components/ComponentManager.h"
#include "server/zone/objects/creature/components/AiDefaultComponent.h"
#include "events/CamoTask.h"

//#define SHOW_WALK_PATH
//#define DEBUG
//#define SHOW_NEXT_POSITION

void AiAgentImplementation::loadTemplateData(SharedObjectTemplate* templateData) {
	CreatureObjectImplementation::loadTemplateData(templateData);
}

int AiAgentImplementation::calculateAttackMinDamage(int level) {
	int minDmg = MAX(npcTemplate->getDamageMin(), 50 + (level * 5));

	return minDmg;
}

int AiAgentImplementation::calculateAttackMaxDamage(int level) {
	int dmg = MAX(npcTemplate->getDamageMax(), calculateAttackMinDamage(level) * 2);

	return dmg;
}

float AiAgentImplementation::calculateAttackSpeed(int level) {
	float speed = 3.5f - ((float)level / 100.f);

	return speed;
}

void AiAgentImplementation::loadTemplateData(CreatureTemplate* templateData) {
	npcTemplate = templateData;

	setPvpStatusBitmask(npcTemplate->getPvpBitmask());
	if (npcTemplate->getPvpBitmask() == 0)
		closeobjects = NULL;

	optionsBitmask = npcTemplate->getOptionsBitmask();
	//npcTemplate->getCreatureBitmask(); -- TODO: need to add a bitmask for AI (pack, herd, etc)
	level = npcTemplate->getLevel();

	float minDmg = calculateAttackMinDamage(level);
	float maxDmg = calculateAttackMaxDamage(level);
	float speed = calculateAttackSpeed(level);

	WeaponObject* defaultWeapon = cast<WeaponObject*>(getSlottedObject("default_weapon"));

	if (weapons.size() == 0) {
		Vector<String> wepgroups = npcTemplate->getWeapons();
		for (int i = 0; i < wepgroups.size(); ++i) {
			Vector<String> weptemps = CreatureTemplateManager::instance()->getWeapons(wepgroups.get(i));

			for (int i = 0; i < weptemps.size(); ++i) {
				uint32 crc = weptemps.get(i).hashCode();

				ManagedReference<WeaponObject*> weao = (server->getZoneServer()->createObject(crc, 0)).castTo<WeaponObject*>();

				if (weao != NULL) {
					weao->setMinDamage(minDmg * 0.5);
					weao->setMaxDamage(maxDmg * 0.5);
					weao->setAttackSpeed(speed);
					weapons.add(weao);

					if (i == 0)
						transferObject(weao, 4, false);
				} else {
					error("could not create weapon " + weptemps.get(i));
				}
			}
		}

		// add the default weapon for creatures
		if (isCreature()) {
			weapons.add(defaultWeapon);
		}
	}

	if (defaultWeapon != NULL) {
		// set the damage of the default weapon
		defaultWeapon->setMinDamage(minDmg);
		defaultWeapon->setMaxDamage(maxDmg);
		defaultWeapon->setAttackSpeed(speed);
	}

	int ham;
	baseHAM.removeAll();
	for (int i = 0; i < 9; ++i) {
		if (i % 3 == 0) {
			ham = System::random(npcTemplate->getBaseHAMmax() - npcTemplate->getBaseHAM()) + npcTemplate->getBaseHAM();
			baseHAM.add(ham);
		} else
			baseHAM.add(ham/100);
	}

	hamList.removeAll();
	for (int i = 0; i < 9; ++i) {
		hamList.add(baseHAM.get(i));
	}

	maxHamList.removeAll();
	for (int i = 0; i < 9; ++i) {
		maxHamList.add(baseHAM.get(i));
	}

	objectName = npcTemplate->getObjectName();

	setCustomObjectName(templateData->getCustomName(), false);

	setHeight(templateData->getScale(), false);

	String currentLogName = getLoggingName();

	if (!currentLogName.contains(npcTemplate->getTemplateName())) {
		StringBuffer logName;
		logName << getLoggingName() << "[" << npcTemplate->getTemplateName() << "]";

		setLoggingName(logName.toString());
	}

	String pvpFaction = npcTemplate->getPvpFaction();

	if (!pvpFaction.isEmpty() && (pvpFaction == "imperial" || pvpFaction == "rebel")) {
		setFaction(pvpFaction.hashCode());
	}

	if (!loadedOutfit) {
		loadedOutfit = true;
		String outfit = npcTemplate->getOutfit();

		if (!outfit.isEmpty()) {
			MobileOutfitGroup* group = CreatureTemplateManager::instance()->getMobileOutfitGroup(outfit);

			if (group != NULL) {
				Vector<MobileOutfit>* outfits = group->getOutfits();

				int num = System::random(outfits->size() - 1);

				if (num > -1) {
					MobileOutfit* clothes = &outfits->get(num);

					Vector<OutfitTangibleObject>* objects = clothes->getObjects();

					for (int i = 0; i < objects->size(); ++i) {
						OutfitTangibleObject* obj = &objects->get(i);

						String templ = obj->getObjectTemplate();

						ManagedReference<TangibleObject*> tano = (server->getZoneServer()->createObject(templ.hashCode(), 0)).castTo<TangibleObject*>();

						if (tano != NULL) {
							VectorMap<String, uint8>* cust = obj->getCustomizationVariables();

							for (int j = 0; j < cust->size(); ++j) {
								tano->setCustomizationVariable(cust->elementAt(j).getKey(), cust->elementAt(j).getValue());
							}

							transferObject(tano, 4, false);
						}

					}
				}
			} else {
				error("null outfit group " + outfit);
			}
		}
	}
}

void AiAgentImplementation::setLevel(int lvl) {
	if (lvl == 0)
		return;

	CreatureObjectImplementation::setLevel(lvl);

	level = lvl;

	if (npcTemplate == NULL) {
		return;
	}

	int baseLevel = npcTemplate->getLevel();

	if (baseLevel == lvl)
		return;

	float minDmg = calculateAttackMinDamage(baseLevel);
	float maxDmg = calculateAttackMaxDamage(baseLevel);
	float speed = calculateAttackSpeed(baseLevel);

	WeaponObject* defaultWeapon = cast<WeaponObject*>(getSlottedObject("default_weapon"));

	float ratio = ((float)lvl) / (float)baseLevel;

	minDmg *= ratio;
	maxDmg *= ratio;

	if (ratio != 0) {
		float newSpeed = speed / ratio;

		speed = MIN(4, newSpeed);
	}

	for (int i = 0; i < weapons.size(); ++i) {
		WeaponObject* weao = weapons.get(i);

		weao->setMinDamage(minDmg * 0.5);
		weao->setMaxDamage(maxDmg * 0.5);
		weao->setAttackSpeed(speed);
	}

	if (defaultWeapon != NULL) {
		defaultWeapon->setMinDamage(minDmg);
		defaultWeapon->setMaxDamage(maxDmg);
		defaultWeapon->setAttackSpeed(speed);
	}

	int baseHamMax = ((float)npcTemplate->getBaseHAMmax()) * ratio;
	int baseHam = ((float)npcTemplate->getBaseHAM()) * ratio;

	int ham;

	for (int i = 0; i < 9; ++i) {
		if (i % 3 == 0) {
			ham = System::random(baseHamMax - baseHam) + baseHam;
			setBaseHAM(i, ham);
		} else
			setBaseHAM(i, ham / 100);
	}

	for (int i = 0; i < 9; ++i) {
		setHAM(i, baseHAM.get(i));
	}

	for (int i = 0; i < 9; ++i) {
		setMaxHAM(i, baseHAM.get(i));
	}
}

void AiAgentImplementation::initializeTransientMembers() {
	CreatureObjectImplementation::initializeTransientMembers();

	aiInterfaceComponents.add(ComponentManager::instance()->getComponent<AiDefaultComponent*>("AiDefaultComponent"));

}

void AiAgentImplementation::notifyPositionUpdate(QuadTreeEntry* entry) {
	for (int i = 0; i < aiInterfaceComponents.size(); i++) {
		AiInterfaceComponent* interface = aiInterfaceComponents.get(i);
		interface->notifyPositionUpdate(_this.getReferenceUnsafeStaticCast(), entry);
	}

	CreatureObjectImplementation::notifyPositionUpdate(entry);
}

void AiAgentImplementation::doAwarenessCheck(Coordinate& start, uint64 time, CreatureObject* target) {
	for (int i = 0; i < aiInterfaceComponents.size(); i++) {
		AiInterfaceComponent* interface = aiInterfaceComponents.get(i);
		interface->doAwarenessCheck(_this.getReferenceUnsafeStaticCast(), start, time, target);
	}
}

void AiAgentImplementation::doRecovery() {
	if (isDead() || getZone() == NULL)
		return;

	activateHAMRegeneration();
	activateStateRecovery();
	activatePostureRecovery();

	if (damageOverTimeList.hasDot() && damageOverTimeList.isNextTickPast()) {
		damageOverTimeList.activateDots(_this.get());
	}

	doAttack();

	activateRecovery();
}

void AiAgentImplementation::doAttack() {
	if (isDead()) {
		removeDefenders();

		setFollowObject(NULL);

		return;
	}

	CreatureObject* target = getThreatMap()->getHighestThreatCreature();

	if (target != NULL && !defenderList.contains(target) && (!target->isDead() && !target->isIncapacitated()) && target->getDistanceTo(_this.get()) < 128.f && target->isAttackableBy(_this.get()) && lastDamageReceived.miliDifference() < 20000)
		addDefender(target);
	else if (target != NULL && defenderList.contains(target) && (target->isDead() || target->isIncapacitated() || !target->isInRange(_this.get(), 128) || !target->isAttackableBy(_this.get()))) {
		removeDefender(target);
		target = NULL;
	}

	if (target == NULL && defenderList.size() > 0) {
		for (int i = 0; i < defenderList.size(); ++i) {
			SceneObject* tarObj = defenderList.get(i);

			if (tarObj->isCreatureObject()) {
				CreatureObject* targetCreature = cast<CreatureObject*>(tarObj);

				if (!targetCreature->isDead() && !targetCreature->isIncapacitated() && targetCreature->getDistanceTo(_this.get()) < 128.f && targetCreature->isAttackableBy(_this.get())) {
					target = targetCreature;

					break;
				}
			}
		}
	}

	if (isDead()) {
		if (target)
			removeDefender(target);

		return;
	}

	if (!isInCombat() || defenderList.size() <= 0 || target == NULL) {
		tryRetreat();
		activateRecovery();
		return;
	}

	if (target != NULL && (!target->isInRange(_this.get(), 128) || !target->isAttackableBy(_this.get()) || target->isDead() || target->isIncapacitated()
			|| (target->hasState(CreatureState::PEACE) && (System::random(level) == 1)))) {
		//Locker clocker(target, _this.get());
		/*if (defenderList.size() == 1 && defenderList.contains(target)) {
			CombatManager::instance()->forcePeace(_this.get()); //calling this on target will cause a deadlock, needs to be called in a new task
		} else*/

		removeDefender(target);

		activateRecovery();
		return;
	}

	if (commandQueue->size() > 5) {
		activateRecovery();
		return;
	}

	if (target != followObject) {
		//setDefender(target);
		setFollowObject(target);

		if (target != NULL)
			setDefender(target);
	}

	selectWeapon();

	if (npcTemplate != NULL && commandQueue->size() < 3) {
		// do special attack
		CreatureAttackMap* attackMap = npcTemplate->getAttacks();
		int attackNum = attackMap->getRandomAttackNumber();

		if (attackNum < 0 || attackNum >= attackMap->size()) {
			enqueueCommand(npcTemplate->getDefaultAttack().hashCode(), 0, target->getObjectID(), "");
		} else {
			String args = attackMap->getArguments(attackNum);

			if (!validateStateAttack(target, args)) {
				// do default attack
				enqueueCommand(npcTemplate->getDefaultAttack().hashCode(), 0, target->getObjectID(), "");
			} else {
				// queue special attack
				unsigned int actionCRC = attackMap->getCommand(attackNum).hashCode();
				enqueueCommand(actionCRC, 0, target->getObjectID(), args);

				//if (System::random(4) == 0) {
					// queue second special attack (rudimentary combo)
					int secondAttackNum = attackMap->getRandomAttackNumber();
					args = attackMap->getArguments(secondAttackNum);

					if (validateStateAttack(target, args) && secondAttackNum != attackNum) {
						actionCRC = attackMap->getCommand(attackNum).hashCode();
						enqueueCommand(actionCRC, 0, target->getObjectID(), args);
					} else {
						enqueueCommand(npcTemplate->getDefaultAttack().hashCode(), 0, target->getObjectID(), "");
					}
				//}
			}
		}
	} else if (npcTemplate == NULL)
		enqueueCommand(String("defaultattack").hashCode(), 0, target->getObjectID(), "");
}

int AiAgentImplementation::notifyAttack(Observable* observable) {
	// TODO: add reaction attacks
	return 0;
}

int AiAgentImplementation::notifyCallForHelp(Observable* observable, ManagedObject* arg1) {
	// TODO: add aggroing
	return 0;
}

int AiAgentImplementation::handleObjectMenuSelect(CreatureObject* player, byte selectedID) {
	if (isDead()) {
		switch (selectedID) {
		case 35:
			player->executeObjectControllerAction(String("loot").hashCode(), getObjectID(), "");

			return 0;
		case 36:
			getZoneServer()->getPlayerManager()->lootAll(player, _this.get());

			return 0;
		}
	}

	return CreatureObjectImplementation::handleObjectMenuSelect(player, selectedID);
}

void AiAgentImplementation::selectWeapon() {
	float dist = 5.f;

	if (followObject != NULL)
		dist = getDistanceTo(followObject.get());

	float diff = 1024.f;
	WeaponObject* finalWeap = getWeapon();

	Vector<WeaponObject*> selections(5, 5);

	for (int i = 0; i < weapons.size(); ++i) {
		WeaponObject* weap = weapons.get(i);

		float range = abs(weap->getIdealRange() - dist);

		if (range < diff) {
			diff = range;
			finalWeap = weap;
		}
	}

	ManagedReference<WeaponObject*> currentWeapon = getWeapon();
	ManagedReference<WeaponObject*> defaultWeapon = dynamic_cast<WeaponObject*>(getSlottedObject("default_weapon"));

	if ((dist < 6) && (finalWeap->isRangedWeapon() || (finalWeap->isMeleeWeapon() && System::random(10) == 0))) {
		float range = fabs(defaultWeapon->getIdealRange() - dist);

		if (range < diff) {
			finalWeap = defaultWeapon;
		}
	}

	if (currentWeapon != finalWeap) {
		if (currentWeapon != NULL && currentWeapon != defaultWeapon) {
			currentWeapon->destroyObjectFromWorld(false);

			//info("removed weapon " + currentWeapon->getDisplayedName(), true);
		}

		if (finalWeap != defaultWeapon) {

			//info("selected weapon " + finalWeap->getDisplayedName(), true);

			transferObject(finalWeap, 4, false);
			broadcastObject(finalWeap, false);
		}
	}

	//setWeapon(finalWeap, true);
}

bool AiAgentImplementation::validateStateAttack(CreatureObject* target, String& args) {
	StringTokenizer tokenizer(args);
	tokenizer.setDelimeter(";");

	while (tokenizer.hasMoreTokens()) {
		String singleArg;
		tokenizer.getStringToken(singleArg);

		if (singleArg.indexOf("Chance") != -1) {
			String stateName = singleArg.subString(0, args.indexOf("Chance"));
			uint64 state = CreatureState::instance()->getState(stateName);
			if (target->hasState(state) || (stateName == "postureDown" && target->isProne()) || (stateName == "knockdown" && target->isKnockedDown()) || (stateName == "postureUp" && target->isStanding())) {
				return false;

			}
		}
	}

	return true;
}

void AiAgentImplementation::setDespawnOnNoPlayerInRange(bool val) {
	if (despawnOnNoPlayerInRange == val)
		return;

	despawnOnNoPlayerInRange = val;

	if (val && numberOfPlayersInRange <= 0) {
		if (despawnEvent == NULL) {
			despawnEvent = new DespawnCreatureOnPlayerDissappear(_this.get());
		}

		if (!despawnEvent->isScheduled())
			despawnEvent->schedule(30000);
	}
}

bool AiAgentImplementation::tryRetreat() {
	return false;
	//TODO: fix this

	try {

		if(homeLocation.isInRange(_this.get(), 1.5))
			return false;

		if (homeLocation.getPositionX() == 0 && homeLocation.getPositionY() == 0 && homeLocation.getPositionZ() == 0)
			return false;

		CombatManager::instance()->forcePeace(_this.get());

		Locker locker(&targetMutex);

		setOblivious();

		//showFlyText("npc_reaction/flytext", "afraid", 0xFF, 0, 0);

		homeLocation.setReached(false);

		if (threatMap != NULL)
			threatMap->removeAll();

		patrolPoints.removeAll();
		patrolPoints.add(homeLocation);

		activateMovementEvent();
	} catch (Exception& e) {
		error(e.getMessage());
		e.printStackTrace();
	}

	return true;
}

void AiAgentImplementation::setDefender(SceneObject* defender) {
	CreatureObjectImplementation::setDefender(defender);

	setFollowObject(defender);

	activateRecovery();
}

void AiAgentImplementation::queueDizzyFallEvent() {
	if (System::random(10) == 1)
		CreatureObjectImplementation::queueDizzyFallEvent();
}

void AiAgentImplementation::addDefender(SceneObject* defender) {
	if (defenderList.size() == 0)
		showFlyText("npc_reaction/flytext", "threaten", 0xFF, 0, 0);

	if(followState <= STALKING){
		followState = FOLLOWING;
	}

	CreatureObjectImplementation::addDefender(defender);

	activateRecovery();
}

void AiAgentImplementation::removeDefender(SceneObject* defender) {
	CreatureObjectImplementation::removeDefender(defender);

	if (defender != NULL) {
		if (defender->isCreatureObject())
			getThreatMap()->dropDamage(cast<CreatureObject*>(defender));
	}

	if (followObject == defender) {
		CreatureObject* target = getThreatMap()->getHighestThreatCreature();

		if (target == NULL && defenderList.size() > 0) {
			SceneObject* tarObj = defenderList.get(0);
			if (tarObj->isCreatureObject())
				target = cast<CreatureObject*>(tarObj);
		}

		if (target == NULL) {
			setOblivious();
		} else  {
			setDefender(target);
		}
	}

	activateRecovery();
}

/**
 * Cleares the combat state
 * @pre { this object is locked }
 * @post { this object is locked, this object is not in a combat state }
 * @param clearDefenders if true the defender vector willl be emptied
 */
void AiAgentImplementation::clearCombatState(bool clearDefenders) {
	CreatureObjectImplementation::clearCombatState(clearDefenders);

	if (threatMap != NULL)
		threatMap->removeAll();

	setOblivious();
}

void AiAgentImplementation::notifyInsert(QuadTreeEntry* entry) {
	SceneObject* scno = cast<SceneObject*>( entry);

	if (scno == _this.get())
		return;

	if (scno->isPlayerCreature())
		++numberOfPlayersInRange;
}

void AiAgentImplementation::clearDespawnEvent() {
	despawnEvent = NULL;
}

void AiAgentImplementation::respawn(Zone* zone, int level) {
	if (getZone() != NULL)
		return;

	setLevel(level);

	initializePosition(homeLocation.getPositionX(), homeLocation.getPositionZ(), homeLocation.getPositionY());

	SceneObject* cell = homeLocation.getCell();

	setNextPosition(homeLocation.getPositionX(), homeLocation.getPositionZ(), homeLocation.getPositionY());
	nextStepPosition.setPosition(homeLocation.getPositionX(), homeLocation.getPositionZ(), homeLocation.getPositionY());
	nextStepPosition.setCell(cell);

	Locker zoneLocker(zone);

	if (cell != NULL)
		cell->transferObject(_this.get(), -1);
	else
		zone->transferObject(_this.get(), -1, true);
}

void AiAgentImplementation::sendBaselinesTo(SceneObject* player) {
	ReadLocker locker(&despawnMutex);

	CreatureObjectImplementation::sendBaselinesTo(player);
}

void AiAgentImplementation::notifyDespawn(Zone* zone) {
	if (moveEvent != NULL) {
		moveEvent->clearCreatureObject();
		moveEvent = NULL;
	}

	for (int i = 0; i < movementMarkers.size(); ++i)
		movementMarkers.get(i)->destroyObjectFromWorld(false);

	SceneObject* creatureInventory = getSlottedObject("inventory");

	if (creatureInventory != NULL) {
		creatureInventory->setContainerOwnerID(0);
	}

	if (npcTemplate == NULL)
		return;

	int oldLevel = level;

	Locker locker(&despawnMutex);

	loadTemplateData(templateObject);
	loadTemplateData(npcTemplate);

	if (oldLevel != level)
		setLevel(level);

	stateBitmask = 0;

	setPosture(CreaturePosture::UPRIGHT, false);

	shockWounds = 0;

	if (threatMap != NULL)
		threatMap->removeAll();

	locker.release();

	//Delete all loot out of inventory
	ManagedReference<SceneObject*> inventory = getSlottedObject("inventory");

	if (inventory != NULL) {
		while (inventory->getContainerObjectsSize() > 0) {
			ManagedReference<SceneObject*> obj = inventory->getContainerObject(0);
			inventory->removeFromContainerObjects(0);
			obj->destroyObjectFromWorld(false);
			obj->destroyObjectFromDatabase(true);
		}
	}

	setTargetObject(NULL);
	setFollowObject(NULL);

	//_this.get()->printReferenceHolders();

	//printf("%d ref count\n", _this.get()->getReferenceCount());

	if (respawnTimer == 0) {
		//zone->getCreatureManager()->addToReservePool(_this.get());
		return;
	}

	Reference<Task*> task = new RespawnCreatureTask(_this.get(), zone, level);
	task->schedule(respawnTimer * 1000);
}

void AiAgentImplementation::scheduleDespawn(int timeToDespawn) {
	if (getPendingTask("despawn") != NULL)
		return;

	Reference<DespawnCreatureTask*> despawn = new DespawnCreatureTask(_this.get());
	addPendingTask("despawn", despawn, timeToDespawn * 1000);
}

void AiAgentImplementation::notifyDissapear(QuadTreeEntry* entry) {
	SceneObject* scno = cast<SceneObject*>( entry);

	if (scno == _this.get())
		return;

	if (entry == followObject.get())
		setFollowObject(NULL);

	if (scno->isPlayerCreature()) {
		if ((--numberOfPlayersInRange <= 0)  && (despawnEvent == NULL)) {
			/*despawnEvent = new DespawnCreatureOnPlayerDissappear(_this.get());
			despawnEvent->schedule(30000);*/
		}
	}
}

void AiAgentImplementation::activateAwarenessEvent(CreatureObject *target) {

#ifdef DEBUG
	info("Starting activateAwarenessEvent check", true);
#endif
	Locker locker(&awernessEventMutex);

	if (awarenessEvent == NULL) {
		awarenessEvent = new AiAwarenessEvent(_this.get(), target);

		awarenessEvent->schedule(1000);

#ifdef DEBUG
		info("Scheduling new Awareness Event", true);
#endif
	}

	if (!awarenessEvent->isScheduled()) {
		awarenessEvent->setTarget(target);
		awarenessEvent->schedule(1000);

#ifdef DEBUG
		info("Rescheduling awareness event", true);
#endif
	}
}

void AiAgentImplementation::activateRecovery() {
	if (thinkEvent == NULL) {
		thinkEvent = new AiThinkEvent(_this.get());

		thinkEvent->schedule(2000);
	}

	if (!thinkEvent->isScheduled())
		thinkEvent->schedule(2000);
}

void AiAgentImplementation::activatePostureRecovery() {
	if (isProne() || isKnockedDown() || isKneeling())
		executeObjectControllerAction(0xA8A25C79); // stand
}

void AiAgentImplementation::updateCurrentPosition(PatrolPoint* pos) {
	PatrolPoint* nextPosition = pos;

	setPosition(nextPosition->getPositionX(), nextPosition->getPositionZ(),
			nextPosition->getPositionY());

	SceneObject* cell = nextPosition->getCell();

	/*StringBuffer reachedPosition;
	reachedPosition << "(" << positionX << ", " << positionY << ")";
	info("reached " + reachedPosition.toString(), true);*/

	if (getZone() == NULL)
		return;

	if (cell != NULL && cell->getParent() != NULL)
		updateZoneWithParent(cell, false, false);
	else
		updateZone(false, false);
}


void AiAgentImplementation::checkNewAngle() {
	if (followObject == NULL)
		return;

	faceObject(followObject);

	if (!nextStepPosition.isReached()) {
		broadcastNextPositionUpdate(&nextStepPosition);
	} else {
		++movementCounter;

		if (parent != NULL && getParent().get()->isCellObject())
			updateZoneWithParent(getParent().get(), true, true);
		else
			updateZone(true, true);
	}
}

bool AiAgentImplementation::findNextPosition(float maxDistance, WorldCoordinates* nextPosition) {
	Locker locker(&targetMutex);

	Vector3 thisWorldPos = getWorldPosition();

	ManagedReference<SceneObject*> strongFollow = followObject.get();

	float newSpeed = runSpeed * 1.5f;

	/*if (followObject == NULL && !isFleeing() && !isRetreating() && !isInCombat()) // TODO: think about implementing a more generic "walk, don't run" criterion
		newSpeed = walkSpeed;*/

	if(hasState(CreatureState::IMMOBILIZED))
		newSpeed = newSpeed / 2.f;

	if(hasState(CreatureState::FROZEN))
		newSpeed = 0.01f;

	float updateTicks = float(UPDATEMOVEMENTINTERVAL) / 1000.f;
	currentSpeed = newSpeed;

	newSpeed *= updateTicks;

	float newPositionX = 0, newPositionZ = 0, newPositionY = 0;
	PathFinderManager* pathFinder = PathFinderManager::instance();
	//float maxDist = MIN(maxDistance, newSpeed);
	float maxDist = newSpeed;

	bool found = false;
	float dist = 0;
	float dx = 0, dy = 0;
	ManagedReference<SceneObject*> cellObject;
	Zone* zone = getZone();

#ifdef SHOW_WALK_PATH
	CreateClientPathMessage* pathMessage = new CreateClientPathMessage();
	if (getParent() == NULL) {
		pathMessage->addCoordinate(getPositionX(), getZone()->getHeight(getPositionX(), getPositionY()), getPositionY());
	} else {
		pathMessage->addCoordinate(getPositionX(), getPositionZ(), getPositionY());
	}
#endif

	while (!found && patrolPoints.size() != 0) {
		PatrolPoint* targetPosition = &patrolPoints.get(0);

		if (targetPosition->getCell() == NULL && zone != NULL) {
			PlanetManager* planetManager = zone->getPlanetManager();

			targetMutex.unlock();

			targetPosition->setPositionZ(planetManager->findClosestWorldFloor(targetPosition->getPositionX(), targetPosition->getPositionY(), targetPosition->getPositionZ(), this->getSwimHeight()));

			targetMutex.lock();
		}

		SceneObject* targetCoordinateCell = targetPosition->getCell();

		Reference<Vector<WorldCoordinates>* > path;

		if (targetCoordinateCell != NULL && dynamic_cast<CellObject*>(targetCoordinateCell)) {
			if (targetCellObject == targetCoordinateCell && currentFoundPath != NULL) {
				Vector<Triangle*>* nodes = NULL;

				if (targetCellObject == parent.get()) {
					CellObject* cell = dynamic_cast<CellObject*>(targetCoordinateCell);

					FloorMesh* floor = PathFinderManager::getFloorMesh(cell);

					pathFinder->getFloorPath(getPosition(), targetPosition->getCoordinates().getPoint(), floor, nodes);
				}

				if (nodes == NULL) {
					WorldCoordinates curr(_this.get().get());
					path = currentFoundPath;

					path->set(0, curr);
				} else {
					delete nodes;

					if (currentFoundPath->get(currentFoundPath->size() - 1).getWorldPosition().distanceTo(targetPosition->getCoordinates().getWorldPosition()) > 3) {
						path = currentFoundPath = static_cast<CurrentFoundPath*>(pathFinder->findPath(_this.get().get(), targetPosition->getCoordinates()));
						targetCellObject = targetCoordinateCell;
					} else {
						WorldCoordinates curr(_this.get().get());
						path = currentFoundPath;

						path->set(0, curr);
					}
				}
			} else {
				path = currentFoundPath = static_cast<CurrentFoundPath*>(pathFinder->findPath(_this.get().get(), targetPosition->getCoordinates()));
				targetCellObject = targetCoordinateCell;
			}
		} else {
			path = pathFinder->findPath(_this.get().get(), targetPosition->getCoordinates());
			targetCellObject = NULL;
			currentFoundPath = NULL;
		}

		if (path == NULL) {
			patrolPoints.remove(0);

			continue;
		}

		pathFinder->filterPastPoints(path, _this.get().get());

		WorldCoordinates* oldCoord = NULL;
		float pathDistance = 0;

		bool remove = true;

		float targetDistance = targetPosition->getWorldPosition().distanceTo(getWorldPosition());

		if (targetDistance > maxDistance)
			maxDist = MIN(newSpeed, targetDistance - maxDistance);
		else {
			return false;
		}

		for (int i = 1; i < path->size() && !found; ++i) { // i = 0 is our position
			WorldCoordinates* coord = &path->get(i);

			Vector3 nextWorldPos = coord->getWorldPosition();

#ifdef SHOW_WALK_PATH
			if (coord->getCell() == NULL)
				pathMessage->addCoordinate(nextWorldPos.getX(), getZone()->getHeight(nextWorldPos.getX(), nextWorldPos.getY()), nextWorldPos.getY());
			else
				pathMessage->addCoordinate(nextWorldPos.getX(), nextWorldPos.getZ(), nextWorldPos.getY());
#endif

			if (oldCoord == NULL) {
				oldCoord = &path->get(0);
			}

			pathDistance += oldCoord->getWorldPosition().distanceTo(nextWorldPos);

			if (i == path->size() - 1 || pathDistance >= maxDist || coord->getCell() != parent.get()) { //last waypoint
				cellObject = coord->getCell();

				dist = nextWorldPos.distanceTo(thisWorldPos);

				*nextPosition = *coord;
				found = true;

				if ((dist <= maxDistance && cellObject == parent.get())) {
					if (i == path->size() - 1) {
						patrolPoints.remove(0);
						remove = false;
					}

					found = false;
				} else {
					//lets convert source and target coordinates to model or world space
					Vector3 oldCoordinates = oldCoord->getPoint();

					if (coord->getCell() != NULL) { //target coord in cell
						if (oldCoord->getCell() == NULL)  // convert old coord to model space
							oldCoordinates = PathFinderManager::transformToModelSpace(oldCoord->getPoint(), coord->getCell()->getParent().get());

					} else { // target coord in world
						oldCoordinates = oldCoord->getWorldPosition();
					}

					if (pathDistance > maxDist) {
						Vector3 oldWorldCoord = oldCoord->getWorldPosition();

						dist = oldWorldCoord.distanceTo(nextWorldPos);

						float distanceToTravel = dist - (pathDistance - maxDist);

						if (distanceToTravel <= 0) {
							newPositionX = nextPosition->getX();
							newPositionY = nextPosition->getY();
						} else {
							//float rest = Math::sqrt(distanceToTravel);

							float rest = distanceToTravel;

							//dist = Math::sqrt(dist);

							if (dist != 0 && !isnan(dist)) {
								dx = nextPosition->getX() - oldCoordinates.getX();
								dy = nextPosition->getY() - oldCoordinates.getY();

								newPositionX = oldCoordinates.getX() + (rest * (dx / dist));// (newSpeed * (dx / dist));
								newPositionY = oldCoordinates.getY() + (rest * (dy / dist)); //(newSpeed * (dy / dist));
							} else {
								newPositionX = nextPosition->getX();
								newPositionY = nextPosition->getY();
							}
						}

						if (nextPosition->getCell() == NULL) {
							

							if (zone != NULL) {
								targetMutex.unlock();

								newPositionZ = zone->getPlanetManager()->findClosestWorldFloor(newPositionX, newPositionY, targetPosition->getPositionZ(), this->getSwimHeight());

								targetMutex.lock();
							}
							//newPositionZ = nextPosition.getZ();
						} else {
							newPositionZ = nextPosition->getZ();
						}
					} else {
						//info("setting nextPosition point", true);
						newPositionX = nextPosition->getX();
						newPositionY = nextPosition->getY();
						newPositionZ = nextPosition->getZ();
					}
#ifdef SHOW_NEXT_POSITION
					if (showNextMovementPosition) {
						for (int i = 0; i < movementMarkers.size(); ++i)
							movementMarkers.get(i)->destroyObjectFromWorld(false);

						movementMarkers.removeAll();

						Reference<SceneObject*> movementMarker = getZoneServer()->createObject(String("object/path_waypoint/path_waypoint.iff").hashCode(), 0);

						movementMarker->initializePosition(newPositionX, newPositionZ, newPositionY);
						StringBuffer msg;
						msg << "Next Position: path distance: " << pathDistance << " maxDist:" << maxDist;
						movementMarker->setCustomObjectName(msg.toString(), false);

						if (cellObject != NULL) {
							cellObject->transferObject(movementMarker, -1, true);
						} else {
							getZone()->transferObject(movementMarker, -1, false);
						}

						movementMarkers.add(movementMarker);

						for (int i = 0; i < path->size(); ++i) {
							WorldCoordinates* coord = &path->get(i);
							SceneObject* coordCell = coord->getCell();

							movementMarker = getZoneServer()->createObject(String("object/path_waypoint/path_waypoint.iff").hashCode(), 0);
							movementMarker->initializePosition(coord->getPoint().getX(), coord->getPoint().getZ(), coord->getPoint().getY());

							if (coordCell != NULL) {
								coordCell->transferObject(movementMarker, -1, true);
							} else {
								getZone()->transferObject(movementMarker, -1, false);
							}

							movementMarkers.add(movementMarker);
						}
					}
#endif
				}
			}

			oldCoord = coord;
		}

		if (!found && remove) {
			patrolPoints.remove(0);
		}
	}

#ifdef SHOW_WALK_PATH
	broadcastMessage(pathMessage, false);
#endif

	nextPosition->setX(newPositionX);
	nextPosition->setY(newPositionY);
	nextPosition->setZ(newPositionZ);
	nextPosition->setCell(cellObject);

	return found;
}

void AiAgentImplementation::doMovement() {
	//info("doMovement", true);
	ManagedReference<SceneObject*> storage = followObject.get();

	if (currentSpeed != 0) {
		updateCurrentPosition(&nextStepPosition);
		nextStepPosition.setReached(true);
	}

	if (isDead() || (getZone() == NULL)) {
		setFollowObject(NULL);
		return;
	}

	if (!isStanding()) {
		activateMovementEvent();
		return;
	}

	doAttack();

	float maxDistance = 5;

	ManagedReference<SceneObject*> follow = followObject.get();

	if (follow != NULL) {
		// drop everything and go after the target, this will also chase the target without having to reach them to change direction
		//patrolPoints.removeAll();

		//Locker locker(&targetMutex);

		switch (followState) {
		case AiAgent::OBLIVIOUS:
			break;
		case AiAgent::WATCHING:
			setNextPosition(getPositionX(), getPositionZ(), getPositionY(), getParent().get());
			setDirection(atan2(follow->getPositionX() - getPositionX(), follow->getPositionX() - getPositionX()));
			checkNewAngle();
			if (patrolPoints.size() > 0)
				updateCurrentPosition(&patrolPoints.get(0));
			break;
		case AiAgent::STALKING:
			setNextPosition(follow->getPositionX(), follow->getPositionZ(), follow->getPositionY(), follow->getParent().get());
			maxDistance = 25;
			break;
		case AiAgent::FOLLOWING: {

			//we might get out of the floor
			/*float newX = followObject->getPositionX() + (-3 + System::random(6));
			float newY = followObject->getPositionY() + (-3 + System::random(6));*/

			setNextPosition(follow->getPositionX(), follow->getPositionZ(), follow->getPositionY(), follow->getParent().get());
			// stop in weapons range
			if (weapon != NULL ) {
				maxDistance = MAX(0.5, weapon->getIdealRange() - 2);
			}

			if (follow != NULL && !CollisionManager::checkLineOfSight(_this.get(), follow)) {
				maxDistance = 0.5;
			}

			break;
		}
		default:
			setOblivious();
			break;
		}
	} /*else {
		switch (followState) {
		case AiAgent::PATROLING:

			break;
		}
	}*/

	if (isRetreating() || isFleeing())
		maxDistance = 0.5;

	WorldCoordinates nextPosition;

	bool found = findNextPosition(maxDistance, &nextPosition);

	if (!found) {
		currentSpeed = 0;
		//info("not found in doMovement", true);

		if (isRetreating())
			homeLocation.setReached(true);

		if (isFleeing())
			fleeing = false;

		if (followObject != NULL)
			activateMovementEvent();
		else
			notifyObservers(ObserverEventType::DESTINATIONREACHED);

		return;
	} else {
		/*if (getLoggingName().contains("bandit")) {
			StringBuffer msg;
			msg << "finding next position with maxDistance " << maxDistance << " next pos:x" << nextPosition.getX() << " y:" << nextPosition.getY();
			//info( + String::valueOf(maxDistance), true);
			info(msg.toString(), true);
		}*/
	}

	nextStepPosition.setPosition(nextPosition.getX(), nextPosition.getZ(), nextPosition.getY());
	nextStepPosition.setCell(nextPosition.getCell());

	float directionangle = atan2(nextPosition.getX() - getPositionX(), nextPosition.getY() - getPositionY());

	if (directionangle < 0)
		directionangle = 2 * M_PI + directionangle;

	direction.setHeadingDirection(directionangle);

	nextStepPosition.setReached(false);

	broadcastNextPositionUpdate(&nextStepPosition);

	activateMovementEvent();
}

bool AiAgentImplementation::isScentMasked(CreatureObject* target) {
	return false;

	Locker locker(&targetMutex);

	// Check masked scent
	if (!target->hasState(CreatureState::MASKSCENT)) {
		if(camouflagedObjects.contains(target)) camouflagedObjects.removeElement(target);
		return false;
	}

	// Don't do anything if object is ignored (Camo / Masked Scent)
	if (camouflagedObjects.contains(target))
		return true;

	int camoSkill = target->getSkillMod("mask_scent");
	int creatureLevel = getLevel();

	bool success = false;

	if (System::random(100) <= (-1 * (1 / ((camoSkill / 100.0f) * 20)) * creatureLevel) + 100) {
		camouflagedObjects.add(target);
		success = true;
	}

	Reference<Task*> ct = new CamoTask(target, _this.get(), true, success);
	ct->execute();

	return success;
}

bool AiAgentImplementation::isConcealed(CreatureObject* target) {
	return false;

	Locker locker(&targetMutex);

	if (!target->hasState(CreatureState::MASKSCENT)) {
		if(camouflagedObjects.contains(target)) camouflagedObjects.removeElement(target);
		return false;
	}

	// Don't do anything if object is ignored (Camo / Masked Scent)
	if (camouflagedObjects.contains(target))
		return true;

	// Check if camo breaks
	int camoSkill = target->getSkillMod("private_conceal");
	int creatureLevel = getLevel();

	if (!isCreature())
		creatureLevel *= 2;

	bool success = false;

	if (System::random(100) < (-1 * (1 / ((camoSkill / 100.0f) * 20)) * creatureLevel) + 100) {
		camouflagedObjects.add(target);
		success = true;
	}

	Reference<Task*> ct = new CamoTask(target, _this.get(), false, success);
	ct->execute();

	return success;
}

void AiAgentImplementation::activateMovementEvent() {
	if (getZone() == NULL)
		return;

	if (moveEvent == NULL) {
		moveEvent = new AiMoveEvent(_this.get());

		moveEvent->schedule(UPDATEMOVEMENTINTERVAL);
	}

	try {
		if (!moveEvent->isScheduled())
			moveEvent->schedule(UPDATEMOVEMENTINTERVAL);
	} catch (IllegalArgumentException& e) {

	}
}

void AiAgentImplementation::activateWaitEvent() {
	if (getZone() == NULL)
		return;

	if (waitEvent == NULL) {
		waitEvent = new AiWaitEvent(_this.get());

		waitEvent->schedule(UPDATEMOVEMENTINTERVAL * 10);
	}

	if (!waitEvent->isScheduled())
		waitEvent->schedule(UPDATEMOVEMENTINTERVAL * 10);
}

void AiAgentImplementation::setNextPosition(float x, float z, float y, SceneObject* cell) {
	Locker locker(&targetMutex);

	PatrolPoint point(x, z, y, cell);

	if (patrolPoints.size() == 0)
		patrolPoints.add(point);
	else
		patrolPoints.setElementAt(0, point);
}

void AiAgentImplementation::broadcastNextPositionUpdate(PatrolPoint* point) {
	BasePacket* msg = NULL;
	++movementCounter;

	if (point == NULL) {
		if (parent != NULL)
			msg = new UpdateTransformWithParentMessage(_this.get());
		else
			msg = new UpdateTransformMessage(_this.get());
	} else {
		if (point->getCell() != NULL)
			msg = new LightUpdateTransformWithParentMessage(_this.get(), point->getPositionX(), point->getPositionZ(), point->getPositionY(), point->getCell()->getObjectID());
		else
			msg = new LightUpdateTransformMessage(_this.get(), point->getPositionX(), point->getPositionZ(), point->getPositionY());
	}

	broadcastMessage(msg, false);
}

int AiAgentImplementation::notifyObjectDestructionObservers(TangibleObject* attacker, int condition) {
	if (getZone() != NULL) {
		CreatureManager* creatureManager = getZone()->getCreatureManager();

		creatureManager->notifyDestruction(attacker, _this.get(), condition);
	}

	return CreatureObjectImplementation::notifyObjectDestructionObservers(attacker, condition);
}

int AiAgentImplementation::notifyConverseObservers(CreatureObject* converser) {
	notifyObservers(ObserverEventType::CONVERSE, converser);

	return 1;
}

int AiAgentImplementation::inflictDamage(TangibleObject* attacker, int damageType, float damage, bool destroy, bool notifyClient) {
	lastDamageReceived.updateToCurrentTime();

	activateRecovery();
	
	if (attacker->isPlayerCreature()) {
		CreatureObject* player = cast<CreatureObject*>( attacker);

		if (damage > 0) {
			getThreatMap()->addDamage(player, damage);

			if (System::random(5) == 1) {
				setDefender(player);
			}
		}
	}

	return CreatureObjectImplementation::inflictDamage(attacker, damageType, damage, destroy, notifyClient);
}

int AiAgentImplementation::inflictDamage(TangibleObject* attacker, int damageType, float damage, bool destroy, const String& xp, bool notifyClient) {
	lastDamageReceived.updateToCurrentTime();

	activateRecovery();

	if (attacker->isPlayerCreature()) {
		CreatureObject* player = cast<CreatureObject*>( attacker);

		if (damage > 0) {
			getThreatMap()->addDamage(player, damage, xp);

			if (System::random(5) == 1) {
				setDefender(player);
			}
		}
	}

	return CreatureObjectImplementation::inflictDamage(attacker, damageType, damage, destroy, notifyClient);
}


void AiAgentImplementation::fillAttributeList(AttributeListMessage* alm, CreatureObject* player) {

	if (isDead()) {
		return;
	}

	if (getArmor() == 0)
		alm->insertAttribute("armorrating", "None");
	else if (getArmor() == 1)
		alm->insertAttribute("armorrating", "Light");
	else if (getArmor() == 2)
		alm->insertAttribute("armorrating", "Medium");
	else if (getArmor() == 3)
		alm->insertAttribute("armorrating", "Heavy");

	if (getKinetic() == 100)
		alm->insertAttribute("cat_armor_special_protection.armor_eff_kinetic", "100%");

	if (getEnergy() == 100)
		alm->insertAttribute("cat_armor_special_protection.armor_eff_energy", "100%");

	if (getElectricity() == 100)
		alm->insertAttribute("cat_armor_special_protection.armor_eff_elemental_electrical", "100%");

	if (getStun() == 100)
		alm->insertAttribute("cat_armor_special_protection.armor_eff_stun", "100%");

	if (getBlast() == 100)
		alm->insertAttribute("cat_armor_special_protection.armor_eff_blast", "100%");

	if (getHeat() == 100)
		alm->insertAttribute("cat_armor_special_protection.armor_eff_elemental_heat", "100%");

	if (getCold() == 100)
		alm->insertAttribute("cat_armor_special_protection.armor_eff_elemental_cold", "100%");

	if (getAcid() == 100)
		alm->insertAttribute("cat_armor_special_protection.armor_eff_elemental_acid", "100%");

	if (getLightSaber() == 100)
		alm->insertAttribute("cat_armor_special_protection.armor_eff_restraint", "100%");

	if (getKinetic() > 0 && getKinetic() < 100) {
		StringBuffer txt;
		txt << round(getKinetic()) << "%";
		alm->insertAttribute("cat_armor_effectiveness.armor_eff_kinetic", txt.toString());
	}

	if (getEnergy() > 0 && getEnergy() < 100) {
		StringBuffer txt;
		txt << round(getEnergy()) << "%";
		alm->insertAttribute("cat_armor_effectiveness.armor_eff_energy", txt.toString());
	}

	if (getElectricity() > 0 && getElectricity() < 100) {
		StringBuffer txt;
		txt << round(getElectricity()) << "%";
		alm->insertAttribute("cat_armor_effectiveness.armor_eff_elemental_electrical", txt.toString());
	}

	if (getStun() > 0 && getStun() < 100) {
		StringBuffer txt;
		txt << round(getStun()) << "%";
		alm->insertAttribute("cat_armor_effectiveness.armor_eff_stun", txt.toString());
	}

	if (getBlast() > 0 && getBlast() < 100) {
		StringBuffer txt;
		txt << round(getBlast()) << "%";
		alm->insertAttribute("cat_armor_effectiveness.armor_eff_blast", txt.toString());
	}

	if (getHeat() > 0 && getHeat() < 100) {
		StringBuffer txt;
		txt << round(getHeat()) << "%";
		alm->insertAttribute("cat_armor_effectiveness.armor_eff_elemental_heat", txt.toString());
	}

	if (getCold() > 0 && getCold() < 100) {
		StringBuffer txt;
		txt << round(getCold()) << "%";
		alm->insertAttribute("cat_armor_effectiveness.armor_eff_elemental_cold", txt.toString());
	}

	if (getAcid() > 0 && getAcid() < 100) {
		StringBuffer txt;
		txt << round(getAcid()) << "%";
		alm->insertAttribute("cat_armor_effectiveness.armor_eff_elemental_acid", txt.toString());
	}

	if (getLightSaber() > 0 && getLightSaber() < 100) {
		StringBuffer txt;
		txt << round(getLightSaber()) << "%";
		alm->insertAttribute("cat_armor_effectiveness.armor_eff_restraint", txt.toString());
	}

	if (getKinetic() == 0)
		alm->insertAttribute("cat_armor_vulnerability.armor_eff_kinetic", "-");

	if (getEnergy() == 0)
		alm->insertAttribute("cat_armor_vulnerability.armor_eff_energy", "-");

	if (getElectricity() == 0)
		alm->insertAttribute("cat_armor_vulnerability.armor_eff_elemental_electrical", "-");

	if (getStun() == 0)
		alm->insertAttribute("cat_armor_vulnerability.armor_eff_stun", "-");

	if (getBlast() == 0)
		alm->insertAttribute("cat_armor_vulnerability.armor_eff_blast", "-");

	if (getHeat() == 0)
		alm->insertAttribute("cat_armor_vulnerability.armor_eff_elemental_heat", "-");

	if (getCold() == 0)
		alm->insertAttribute("cat_armor_vulnerability.armor_eff_elemental_cold", "-");

	if (getAcid() == 0)
		alm->insertAttribute("cat_armor_vulnerability.armor_eff_elemental_acid", "-");

	if (getLightSaber() == 0)
		alm->insertAttribute("cat_armor_vulnerability.armor_eff_restraint", "-");
}


void AiAgentImplementation::sendConversationStartTo(SceneObject* player) {
	if (!player->isPlayerCreature())
		return;

	//Face player.
	faceObject(player);

	PatrolPoint current(coordinates.getPosition(), getParent().get());

	broadcastNextPositionUpdate(&current);

	CreatureObject* playerCreature = cast<CreatureObject*>( player);
	StartNpcConversation* conv = new StartNpcConversation(playerCreature, getObjectID(), "");
	player->sendMessage(conv);

	SortedVector<ManagedReference<Observer*> >* observers = getObservers(ObserverEventType::STARTCONVERSATION);

	if (observers != NULL) {
		for (int i = 0;  i < observers->size(); ++i) {
			if (dynamic_cast<ConversationObserver*>(observers->get(i).get()) != NULL)
				return;
		}
	}

	if (npcTemplate == NULL)
		return;
	//Create conversation observer.
	ConversationObserver* conversationObserver = ConversationManager::instance()->getConversationObserver(npcTemplate->getConversationTemplate());

	if (conversationObserver != NULL) {
		//Register observers.
		registerObserver(ObserverEventType::CONVERSE, conversationObserver);
		registerObserver(ObserverEventType::STARTCONVERSATION, conversationObserver);
		registerObserver(ObserverEventType::SELECTCONVERSATION, conversationObserver);
		registerObserver(ObserverEventType::STOPCONVERSATION, conversationObserver);
	} else {
		error("Could not create conversation observer.");
	}
}

bool AiAgentImplementation::isAggressiveTo(CreatureObject* target) {
	if (!isAttackableBy(target))
		return false;

	uint32 targetFaction = target->getFaction();

	if (getFaction() != 0 && targetFaction != 0) {
		PlayerObject* ghost = target->getPlayerObject();

		if (ghost == NULL && (targetFaction != getFaction()))
			return true;
		else if (ghost != NULL && (targetFaction != getFaction()) && ghost->getFactionStatus() != FactionStatus::ONLEAVE)
			return true;
	}

	if (getPvpStatusBitmask() & CreatureFlag::AGGRESSIVE)
		return true;

	return false;
}

bool AiAgentImplementation::hasLoot(){
	SceneObject* inventory = getSlottedObject("inventory");

	if(inventory == NULL)
		return false;

	return inventory->getContainerObjectsSize() > 0;
}

void AiAgentImplementation::sendDefaultConversationTo(SceneObject* player) {
	/*if (!player->isPlayerCreature())
		return;

	faceObject(player);

	PatrolPoint current(coordinates.getPosition(), getParent());

	broadcastNextPositionUpdate(&current);

	CreatureObject* playerCreature = cast<CreatureObject*>( player);

	ManagedReference<PlayerObject*> ghost = playerCreature->getPlayerObject();

	if (npcTemplate != NULL) {
		uint32 convoTemplate = npcTemplate->getConversationTemplate();

		Reference<ConversationTemplate*> convo = CreatureTemplateManager::instance()->getConversationTemplate(convoTemplate);

		if (convo != NULL) {
			Reference<ConversationScreen*> initialScreen = convo->getInitialScreen();

			if (initialScreen == NULL)
				return;

			//ghost->setLastNpcConvMessStr(root->getID());

			StartNpcConversation* conv = new StartNpcConversation(playerCreature, getObjectID(), "");
			player->sendMessage(conv);

			initialScreen->sendTo(playerCreature);

			return;
		}
	}

	//player->setLastNpcConvStr(("npc_" + getFu().toString()));
	ghost->setLastNpcConvMessStr("0,init");

	StartNpcConversation* conv = new StartNpcConversation(playerCreature, getObjectID(), "");
	player->sendMessage(conv);

	String responseFile, responseAttitude;

	if (responseFile == "") {
		if (isImperial()) {
			responseFile = "npc_reaction/stormtrooper";
		} else if (isRebel()) {
			responseFile = "npc_reaction/military";
		} else {
			short file = System::random(2);
			if (file == 0)
				responseFile = "npc_reaction/fancy";
			else if (file == 1)
				responseFile = "npc_reaction/slang";
			else
				responseFile = "npc_reaction/townperson";
		}
	}

	if (responseAttitude == "") {
		short type = System::random(2);
		if (type == 0)
			responseAttitude = "mean";
		else if (type == 1)
			responseAttitude = "mid";
		else
			responseAttitude = "nice";
	}

	StringBuffer convoChoice;
	convoChoice << "hi_" <<  responseAttitude << "_" << (System::random(15) + 1);

	//TODO: Revisit NPC Conversation
	//NpcConversationMessage* initial = new NpcConversationMessage(
	//		playerCreature, responseFile, convoChoice.toString());
	//player->sendMessage(initial);

	// Parse and send the options:
	//StringList* slist = new StringList(playerCreature);

	//String test = "I'm looking for work";
	//slist->insertOption(test);

	//player->sendMessage(slist);

	//playerCreature->setLastNpcConvMessStr("chitchat");*/
}

void AiAgentImplementation::selectConversationOption(int option, SceneObject* obj) {
	/*if (!obj->isCreatureObject())
		return;

	CreatureObject* player = cast<CreatureObject*>( obj);

	ManagedReference<PlayerObject*> ghost = player->getPlayerObject();

	if (ghost == NULL)
		return;

	String chk = ghost->getLastNpcConvMessStr();

	if (npcTemplate != NULL) {
			uint32 convoTemplate = npcTemplate->getConversationTemplate(); //TODO: Consider storing reference to the convo on the template?

			Reference<ConversationTemplate*> convo = CreatureTemplateManager::instance()->getConversationTemplate(convoTemplate);

			if (convo != NULL) {
				Reference<ConversationScreen*> lastScreen = convo->getScreen(chk);

				if (lastScreen == NULL)
					lastScreen = convo->getInitialScreen();

				Reference<ConversationOption*> convoOption = lastScreen->getOption(option);

				if (convoOption == NULL || !convoOption->isLinked())
					return; //Stop Conversation if no option exists or this option isn't linked to another option.

				Reference<ConversationScreen*> nextScreen = convo->getScreen(convoOption->getLinkedScreenID());

				if (nextScreen == NULL)
					return; //Couldn't find the linked screen in this conversation template.

				nextScreen->sendTo(player);

				return;
			}
	}

	if (chk != "chitchat") {
		return;
	}

	UnicodeString saying = "";
	switch(System::random(5)) {
	case 0:
		saying = "Why the heck would you want to work when welfare is free.";
		break;
	case 1:
		saying = "Pfft, work in this economy?  I'm waiting for the government to bail me out.";
		break;
	case 2:
		saying = "Check the missions terminals, they are overflowing with work.";
		break;
	case 3:
		saying = "Huh, work?  Whats that?";
		break;
	case 4:
		saying = "Did you check the mission terminals?";
		break;
	case 5:
		saying = "Me too.";
		break;
	}

	NpcConversationMessage* response = new NpcConversationMessage(player, saying);
	player->sendMessage(response);

	// Parse and send the options:
	StringList* slist = new StringList(player);
	player->sendMessage(slist);*/
}
