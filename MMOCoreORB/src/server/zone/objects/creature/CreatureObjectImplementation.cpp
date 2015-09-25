/*
 				Copyright <SWGEmu>
		See file COPYING for copying conditions. */

#include "server/zone/objects/creature/CreatureObject.h"
#include "CreatureState.h"
#include "CreatureFlag.h"

#include "server/zone/managers/object/ObjectManager.h"
#include "server/zone/managers/objectcontroller/ObjectController.h"
#include "server/zone/managers/skill/SkillModManager.h"
#include "server/zone/managers/skill/SkillManager.h"
#include "server/zone/managers/player/PlayerManager.h"
#include "server/zone/managers/combat/CombatManager.h"
#include "server/zone/managers/mission/MissionManager.h"
#include "server/zone/managers/creature/PetManager.h"
#include "server/zone/managers/reaction/ReactionManager.h"
#include "server/zone/ZoneClientSession.h"
#include "server/zone/packets/creature/CreatureObjectMessage1.h"
#include "server/zone/packets/creature/CreatureObjectMessage3.h"
#include "server/zone/packets/creature/CreatureObjectMessage4.h"
#include "server/zone/packets/creature/CreatureObjectMessage6.h"
#include "server/zone/packets/creature/CreatureObjectDeltaMessage1.h"
#include "server/zone/packets/creature/CreatureObjectDeltaMessage3.h"
#include "server/zone/packets/creature/CreatureObjectDeltaMessage4.h"
#include "server/zone/packets/creature/CreatureObjectDeltaMessage6.h"
#include "server/zone/packets/chat/ChatSystemMessage.h"
#include "server/zone/packets/object/CombatSpam.h"
#include "server/zone/packets/object/PostureMessage.h"
#include "server/zone/packets/object/SitOnObject.h"
#include "server/zone/packets/object/CommandQueueRemove.h"
#include "server/zone/packets/object/CommandQueueAdd.h"
#include "server/zone/packets/object/CombatAction.h"
#include "server/zone/packets/object/WeaponRanges.h"
#include "server/zone/packets/player/PlayMusicMessage.h"
#include "server/zone/packets/player/NewbieTutorialRequest.h"
#include "server/zone/packets/ui/NewbieTutorialEnableHudElement.h"
#include "server/zone/packets/ui/OpenHolocronToPageMessage.h"
#include "server/zone/packets/object/Animation.h"
#include "server/zone/objects/creature/CreaturePosture.h"
#include "server/zone/objects/creature/commands/effect/CommandEffect.h"
#include "server/zone/objects/creature/events/CommandQueueActionEvent.h"
#include "server/zone/Zone.h"
#include "server/zone/ZoneServer.h"
#include "server/chat/StringIdChatParameter.h"
#include "server/zone/objects/scene/variables/DeltaVectorMap.h"
#include "server/zone/objects/creature/variables/CommandQueueAction.h"
#include "server/zone/objects/creature/commands/QueueCommand.h"
#include "server/zone/objects/group/GroupObject.h"
#include "server/zone/packets/tangible/UpdatePVPStatusMessage.h"
#include "server/zone/objects/player/Races.h"
#include "server/zone/objects/player/FactionStatus.h"
#include "server/zone/objects/area/ActiveArea.h"
#include "server/zone/objects/mission/MissionObject.h"
#include "server/zone/objects/area/CampSiteActiveArea.h"
#include "server/zone/managers/templates/TemplateManager.h"
#include "server/zone/objects/tangible/wearables/WearableObject.h"
#include "server/zone/objects/tangible/weapon/WeaponObject.h"
#include "server/zone/objects/intangible/VehicleControlDevice.h"
#include "server/zone/objects/guild/GuildObject.h"
#include "server/zone/objects/creature/events/DizzyFallDownEvent.h"
#include "server/zone/packets/ui/ExecuteConsoleCommand.h"
#include "server/zone/objects/creature/buffs/StateBuff.h"
#include "server/zone/objects/creature/buffs/PrivateBuff.h"
#include "server/zone/objects/creature/buffs/PrivateSkillMultiplierBuff.h"
#include "server/zone/objects/building/hospital/HospitalBuildingObject.h"

#include "server/zone/packets/object/SitOnObject.h"

#include "server/zone/packets/object/CombatSpam.h"

#include "server/zone/managers/planet/PlanetManager.h"
#include "server/zone/managers/terrain/TerrainManager.h"
#include "server/zone/managers/resource/resourcespawner/SampleTask.h"

#include "server/zone/templates/tangible/SharedCreatureObjectTemplate.h"

#include "variables/Skill.h"
#include "server/zone/objects/player/sessions/EntertainingSession.h"

#include "server/zone/packets/zone/unkByteFlag.h"
#include "server/zone/packets/zone/CmdStartScene.h"
#include "server/zone/packets/zone/CmdSceneReady.h"
#include "server/zone/packets/zone/ParametersMessage.h"

#include "server/zone/managers/guild/GuildManager.h"
#include "system/lang/ref/Reference.h"
#include "server/zone/objects/player/events/LogoutTask.h"
#include "server/zone/objects/player/events/StoreSpawnedChildrenTask.h"

#include "server/zone/objects/tangible/threat/ThreatMap.h"

#include "buffs/BuffDurationEvent.h"

float CreatureObjectImplementation::DEFAULTRUNSPEED = 5.376;

void CreatureObjectImplementation::initializeTransientMembers() {
	TangibleObjectImplementation::initializeTransientMembers();

	groupInviterID = 0;
	groupInviteCounter = 0;
	currentWeather = 0;
	currentWind = 0;

	lastActionCounter = 0x40000000;

	setContainerOwnerID(getObjectID());
	setMood(moodID);

	setLoggingName("CreatureObject");
}

void CreatureObjectImplementation::initializeMembers() {
	linkedCreature = NULL;
	controlDevice = NULL;

	bankCredits = 0;
	cashCredits = 0;

	pvpStatusBitmask = 0;

	posture = 0;
	factionRank = 0;
	faction = 0;

	stateBitmask = 0;
	terrainNegotiation = 0.0f;

	listenToID = 0;
	watchToID = 0;

	weapon = NULL;
	guild = NULL;
	group = NULL;
	groupInviterID = 0;
	groupInviteCounter = 0;
	targetID = 0;
	moodID = 0;
	performanceCounter = 0;
	instrumentID = 0;

	optionsBitmask = 0x80;

	moodString = "neutral";

	height = 1;

	shockWounds = 0;

	accelerationMultiplierBase = 1.f;
	accelerationMultiplierMod = 1.f;
	speedMultiplierBase = 1.f;
	speedMultiplierMod = 1.f;
	currentSpeed = 0.f;
	turnScale = 1.f;

	cooldownTimerMap = new CooldownTimerMap();
	commandQueue = new CommandQueueActionVector();
	immediateQueue = new CommandQueueActionVector();

	closeobjects = new CloseObjectsVector();
	closeobjects->setNoDuplicateInsertPlan();

	healthWoundHeal = 0;
	actionWoundHeal = 0;
	mindWoundHeal = 0;

	setContainerInheritPermissionsFromParent(false);
	setContainerDefaultDenyPermission(ContainerPermissions::MOVECONTAINER);
	setContainerDenyPermission("owner", ContainerPermissions::MOVECONTAINER);
}

void CreatureObjectImplementation::loadTemplateData(
		SharedObjectTemplate* templateData) {
	TangibleObjectImplementation::loadTemplateData(templateData);

	SharedCreatureObjectTemplate* creoData =
			dynamic_cast<SharedCreatureObjectTemplate*> (templateData);

	slopeModPercent = creoData->getSlopeModPercent();
	slopeModAngle = creoData->getSlopeModAngle();
	swimHeight = creoData->getSwimHeight();

	if (creoData->getMaxScale() == 0)
		height = creoData->getMinScale();
	else
		height = creoData->getMaxScale();

	if (height <= SMALL_NUM)
		height = 1.0;

	//info("loaded with height: " + String::valueOf(height), true);

	level = creoData->getLevel();

	encumbrances.removeAll();

	for (int i = 0; i < 3; ++i) {
		encumbrances.add(0);
	}

	Vector<int> base = creoData->getBaseHAM();

	baseHAM.removeAll();

	for (int i = 0; i < base.size(); ++i)
		baseHAM.add(base.get(i));

	wounds.removeAll();
	for (int i = 0; i < 9; ++i) {
		wounds.add(0);
	}

	hamList.removeAll();
	for (int i = 0; i < 9; ++i) {
		hamList.add(baseHAM.get(i));
	}

	maxHamList.removeAll();
	for (int i = 0; i < 9; ++i) {
		maxHamList.add(baseHAM.get(i));
	}

	frozen = 0;

	Vector<FloatParam> accel = creoData->getAcceleration();

	if (accel.size() > 0) {
		runAcceleration = accel.get(0);
		walkAcceleration = accel.get(1);
	} else {
		runAcceleration = 0;
		walkAcceleration = 0;
	}

	const Vector<FloatParam>& speedTempl = creoData->getSpeed();

	if (speedTempl.size() > 0) {
		runSpeed = speedTempl.get(0);
		walkSpeed = speedTempl.get(1);
	} else {
		runSpeed = 0;
		walkSpeed = 0;
	}
}

void CreatureObjectImplementation::finalize() {

}

void CreatureObjectImplementation::sendTo(SceneObject* player, bool doClose) {
	if (isInvisible() && player != asCreatureObject())
		return;

	TangibleObjectImplementation::sendTo(player, doClose);
}

void CreatureObjectImplementation::sendToOwner(bool doClose) {
	if (owner == NULL)
		return;

	setMovementCounter(0);

	owner->balancePacketCheckupTime();

	BaseMessage* byteFlag = new unkByteFlag();
	owner->sendMessage(byteFlag);

	BaseMessage* startScene = new CmdStartScene(asCreatureObject());
	owner->sendMessage(startScene);

	BaseMessage* parameters = new ParametersMessage();
	owner->sendMessage(parameters);

	ManagedReference<GuildManager*> guildManager =
			server->getZoneServer()->getGuildManager();
	guildManager->sendBaselinesTo(asCreatureObject());

	ManagedReference<SceneObject*> grandParent = getRootParent();

	if (grandParent != NULL) {
		grandParent->sendTo(asCreatureObject(), true);
	} else
		sendTo(asCreatureObject(), doClose);

	CloseObjectsVector* vec = (CloseObjectsVector*) getCloseObjects();

	assert(vec != NULL);

	SortedVector<QuadTreeEntry*> closeObjects;
	vec->safeCopyTo(closeObjects);

	for (int i = 0; i < closeObjects.size(); ++i) {
		SceneObject* obj = cast<SceneObject*> (closeObjects.get(i));

		if (obj != asCreatureObject()) {
			if (obj != grandParent) {
				notifyInsert(obj);
				//obj->sendTo(asCreatureObject(), true);
			}

			if (obj->isPlayerCreature()) { //we need to destroy object to reset movement counter on near clients
				obj->notifyDissapear(asCreatureObject());
			}

			//obj->notifyInsert(asCreatureObject());
			sendTo(obj, true);
		}

	}

	if (group != NULL)
		group->sendTo(asCreatureObject(), true);

	owner->resetPacketCheckupTime();
}

void CreatureObjectImplementation::sendBaselinesTo(SceneObject* player) {
	CreatureObject* thisPointer = asCreatureObject();
	Zone* zone = getZone();

	if (zone == NULL)
		return;

	if (player == thisPointer) {
		//info("sending baselines to myself", true);

		CreatureObjectMessage1* msg = new CreatureObjectMessage1(thisPointer);
		player->sendMessage(msg);
	}

	CreatureObjectMessage3* msg3 = new CreatureObjectMessage3(thisPointer);
	player->sendMessage(msg3);

	if (player == thisPointer) {
		CreatureObjectMessage4* msg4 = new CreatureObjectMessage4(thisPointer);
		player->sendMessage(msg4);
	}

	CreatureObjectMessage6* msg6 = new CreatureObjectMessage6(thisPointer);
	player->sendMessage(msg6);

	if (!player->isPlayerCreature())
		return;

	CreatureObject* playerCreature = cast<CreatureObject*> (player);

	sendPvpStatusTo(playerCreature);
}

void CreatureObjectImplementation::sendSlottedObjectsTo(SceneObject* player) {
	SortedVector<SceneObject*> objects(getSlottedObjectsSize(),
			getSlottedObjectsSize());
	objects.setNoDuplicateInsertPlan();

	try {
		for (int i = 0; i < getSlottedObjectsSize(); ++i) {
			Reference<SceneObject*> object = getSlottedObject(i);

			int arrangementSize = object->getArrangementDescriptorSize();

			bool sendWithoutContents = false;

			if (arrangementSize > 0) {
				const Vector<String>* descriptors = object->getArrangementDescriptor(0);

				if (descriptors->size() > 0) {
					const String& childArrangement = descriptors->get(0);

					if (player != asCreatureObject() && ((childArrangement == "bank")
							|| (childArrangement == "inventory") || (childArrangement
									== "datapad") || (childArrangement == "mission_bag"))) {

						sendWithoutContents = true;
					}
				}
			}

			if (objects.put(object) != -1) {
				if (sendWithoutContents)
					object->sendWithoutContainerObjectsTo(player);
				else
					object->sendTo(player, true);
			}
		}
	} catch (Exception& e) {
		error(e.getMessage());
		e.printStackTrace();
	}
}

void CreatureObjectImplementation::sendSystemMessage(const String& message) {
	if (!isPlayerCreature())
		return;

	UnicodeString msg(message);
	sendSystemMessage(msg);
}

void CreatureObjectImplementation::playMusicMessage(const String& file) {
	PlayMusicMessage* message = new PlayMusicMessage(file);
	sendMessage(message);
}

void CreatureObjectImplementation::sendOpenHolocronToPageMessage() {
	OpenHolocronToPageMessage* message = new OpenHolocronToPageMessage();
	sendMessage(message);
}

void CreatureObjectImplementation::sendNewbieTutorialRequest(
		const String& request) {
	//info("sending newbie request " + request, true);
	NewbieTutorialRequest* message = new NewbieTutorialRequest(request);
	sendMessage(message);
}

void CreatureObjectImplementation::sendNewbieTutorialEnableHudElement(
		const String& ui, bool enable, float blinkCount) {
	NewbieTutorialEnableHudElement* message =
			new NewbieTutorialEnableHudElement(ui, enable, blinkCount);
	sendMessage(message);
}

void CreatureObjectImplementation::sendSystemMessage(
		StringIdChatParameter& message) {
	if (!isPlayerCreature())
		return;

	ChatSystemMessage* msg = new ChatSystemMessage(message);
	sendMessage(msg);
}

void CreatureObjectImplementation::sendSystemMessage(UnicodeString& message) {
	if (!isPlayerCreature())
		return;

	ChatSystemMessage* smsg = new ChatSystemMessage(message);
	sendMessage(smsg);
}

void CreatureObjectImplementation::clearQueueAction(uint32 actioncntr,
		float timer, uint32 tab1, uint32 tab2) {
	if (actioncntr == 0 || !isPlayerCreature())
		return;

	BaseMessage* queuemsg = new CommandQueueRemove(asCreatureObject(), actioncntr, timer,
			tab1, tab2);
	sendMessage(queuemsg);
}

void CreatureObjectImplementation::clearQueueActions(bool combatOnly) {
	for (int i = commandQueue->size() - 1; i >= 0; i--) {
		CommandQueueAction* command = commandQueue->get(i);

		if (command == NULL)
			continue;

		if (combatOnly) {
			ZoneServer* zoneServer = getZoneServer();
			if (zoneServer == NULL)
				continue;

			ObjectController* objectController = zoneServer->getObjectController();
			if (objectController == NULL)
				continue;

			QueueCommand* qc = objectController->getQueueCommand(command->getCommand());
			if (qc == NULL)
				continue;

			if (!qc->isCombatCommand())
				continue;
		}

		if (command->getActionCounter() != 0)
			clearQueueAction(command->getActionCounter());

		commandQueue->remove(i);
	}
}

void CreatureObjectImplementation::setWeapon(WeaponObject* weao,
		bool notifyClient) {
	if (weao == NULL)
		weao = TangibleObjectImplementation::getSlottedObject("default_weapon").castTo<WeaponObject*>();

	weapon = weao;

	if (notifyClient) {
		CreatureObjectDeltaMessage6* msg = new CreatureObjectDeltaMessage6(
				asCreatureObject());
		msg->updateWeapon();
		msg->close();

		broadcastMessage(msg, true);

		WeaponRanges* ranges = new WeaponRanges(asCreatureObject(), getWeapon());
		sendMessage(ranges);
	}
}

void CreatureObjectImplementation::setLevel(int level, bool randomHam) {
	if (this->level == level && level >= 0)
		return;

	TangibleObjectImplementation::setLevel(level);

	CreatureObjectDeltaMessage6* msg = new CreatureObjectDeltaMessage6(asCreatureObject());

	msg->updateLevel(this->level);
	msg->close();

	broadcastMessage(msg, true);

	if (isGrouped()) {
		Locker clocker(group, asCreatureObject());

		group->calcGroupLevel();
	}
}

void CreatureObjectImplementation::setInstrumentID(int instrumentid,
		bool notifyClient) {
	if (instrumentid == instrumentID)
		return;

	instrumentID = instrumentid;

	if (notifyClient) {
		CreatureObjectDeltaMessage6* msg = new CreatureObjectDeltaMessage6(
				asCreatureObject());
		msg->updateInstrumentID(instrumentID);
		msg->close();

		broadcastMessage(msg, true);
	}
}

void CreatureObjectImplementation::setTargetID(uint64 targetID,
		bool notifyClient) {
	CreatureObjectImplementation::targetID = targetID;

	if (notifyClient) {
		CreatureObjectDeltaMessage6* msg = new CreatureObjectDeltaMessage6(
				asCreatureObject());
		msg->updateTarget();
		msg->close();

		broadcastMessage(msg, false);
	}
}

void CreatureObjectImplementation::setHeight(float height, bool notifyClient) {
	if (this->height == height)
		return;

	this->height = height;

	if (!notifyClient)
		return;

	CreatureObjectDeltaMessage3* msg = new CreatureObjectDeltaMessage3(asCreatureObject());
	msg->updateHeight();
	msg->close();

	broadcastMessage(msg, true);
}

void CreatureObjectImplementation::setShockWounds(int newShock,
		bool notifyClient) {
	if (shockWounds == newShock)
		return;

	shockWounds = newShock;

	if (notifyClient) {
		CreatureObjectDeltaMessage3* dcreo3 = new CreatureObjectDeltaMessage3(
				asCreatureObject());
		dcreo3->updateShockWounds();
		dcreo3->close();

		broadcastMessage(dcreo3, true);
	}
}

void CreatureObjectImplementation::addShockWounds(int shockToAdd, bool notifyClient, bool sendSpam) {
	int newShockWounds = shockWounds + shockToAdd;

	if (newShockWounds < 0) {
		newShockWounds = 0;
	} else if (newShockWounds > 1000) {
		newShockWounds = 1000;
	}

	if (sendSpam && shockToAdd > 0 && asCreatureObject()->isPlayerCreature())
		sendStateCombatSpam("cbt_spam", "shock_wound", 1, shockToAdd, false);

	setShockWounds(newShockWounds, notifyClient);
}

void CreatureObjectImplementation::addMountedCombatSlow() {
	if (!isPlayerCreature() || !isRidingMount() || !isInCombat())
		return;

	uint32 crc = STRING_HASHCODE("mounted_combat_slow");

	if (hasBuff(crc))
		return;

	ManagedReference<CreatureObject*> parent = getParent().get().castTo<CreatureObject*>();

	if (parent == NULL)
		return;

	if (parent->isVehicleObject()) {
		sendSystemMessage("@combat_effects:no_combat_while_driving"); // You cannot attack or react to an attack while driving.
		return;
	}

	if (!parent->isMount())
		return;

	if (hasBuff(STRING_HASHCODE("gallop"))) {
		sendSystemMessage("@combat_effects:no_combat_while_galloping"); // You cannot attack or react to an attack while galloping. Use /gallopStop to stop galloping.
		return;
	}

	ManagedReference<CreatureObject*> creo = asCreatureObject();

	EXECUTE_TASK_2(creo, parent, {
			Locker locker(creo_p);
			Locker clocker(parent_p, creo_p);

			float newSpeed = 1;
			SharedObjectTemplate* templateData = creo_p->getObjectTemplate();
			SharedCreatureObjectTemplate* playerTemplate = dynamic_cast<SharedCreatureObjectTemplate*> (templateData);

			if (playerTemplate != NULL) {
				const Vector<FloatParam>& speedTempl = playerTemplate->getSpeed();
				newSpeed = speedTempl.get(0);
			}

			float oldSpeed = 1;
			PetManager* petManager = creo_p->getZoneServer()->getPetManager();

			if (petManager != NULL) {
				oldSpeed = petManager->getMountedRunSpeed(parent_p);
			}

			float magnitude = newSpeed / oldSpeed;

			uint32 crc = STRING_HASHCODE("mounted_combat_slow");
			StringIdChatParameter startStringId("combat_effects", "mount_slow_for_combat"); // Your mount slows down to prepare for combat.
			StringIdChatParameter endStringId("combat_effects", "mount_speed_after_combat"); // Your mount speeds up.

			ManagedReference<Buff*> buff = new Buff(creo_p, crc, 604800, BuffType::OTHER);

			Locker blocker(buff);

			buff->setSpeedMultiplierMod(magnitude);
			buff->setAccelerationMultiplierMod(magnitude);
			buff->setStartMessage(startStringId);
			buff->setEndMessage(endStringId);

			creo_p->addBuff(buff);

			blocker.release();

			ManagedReference<Buff*> buff2 = new Buff(parent_p, crc, 604800, BuffType::OTHER);

			Locker blocker2(buff2);

			buff2->setSpeedMultiplierMod(magnitude);
			buff2->setAccelerationMultiplierMod(magnitude);

			parent_p->addBuff(buff2);
	});
}

void CreatureObjectImplementation::removeMountedCombatSlow() {
	ManagedReference<CreatureObject*> creo = asCreatureObject();

	EXECUTE_TASK_1(creo, {
			Locker locker(creo_p);

			uint32 crc = STRING_HASHCODE("mounted_combat_slow");
			creo_p->removeBuff(crc);

			ManagedReference<CreatureObject*> parent = creo_p->getParent().get().castTo<CreatureObject*>();

			if (parent != NULL) {
				Locker clocker(parent, creo_p);
				parent->removeBuff(crc);
			}
	});

}

void CreatureObjectImplementation::setCombatState() {
	//lastCombatAction.update();

	if (!(stateBitmask & CreatureState::COMBAT)) {
		stateBitmask |= CreatureState::COMBAT;

		lastSuccessfulCombatAction.updateToCurrentTime();

		if (stateBitmask & CreatureState::PEACE)
			stateBitmask &= ~CreatureState::PEACE;

		CreatureObjectDeltaMessage3* dcreo3 = new CreatureObjectDeltaMessage3(asCreatureObject());
		dcreo3->updateCreatureBitmask(0x80);
		dcreo3->updateState();
		dcreo3->close();

		broadcastMessage(dcreo3, true);

		if (posture == CreaturePosture::SITTING)
			setPosture(CreaturePosture::UPRIGHT);

		Reference<LogoutTask*> logoutTask = getPendingTask("logout").castTo<LogoutTask*>();

		if (logoutTask != NULL)
			logoutTask->cancelLogout();

		if (isEntertaining())
			stopEntertaining();

		addMountedCombatSlow();

		notifyObservers(ObserverEventType::STARTCOMBAT);
	}
}

void CreatureObjectImplementation::clearCombatState(bool removedefenders) {
	//info("trying to clear CombatState");
	if (stateBitmask & CreatureState::COMBAT) {
		if (stateBitmask & CreatureState::PEACE)
			stateBitmask &= ~CreatureState::PEACE;

		stateBitmask &= ~CreatureState::COMBAT;

		CreatureObjectDeltaMessage3* dcreo3 = new CreatureObjectDeltaMessage3(
				asCreatureObject());
		dcreo3->updateCreatureBitmask(getOptionsBitmask());
		dcreo3->updateState();
		dcreo3->close();

		broadcastMessage(dcreo3, true);
	}

	clearQueueActions(false);

	if (removedefenders)
		removeDefenders();

	removeMountedCombatSlow();

	//info("finished clearCombatState");
}

void CreatureObjectImplementation::setAlternateAppearance(const String& appearanceTemplate, bool notifyClient) {
	alternateAppearance = appearanceTemplate;

	if (!notifyClient)
		return;

	CreatureObjectDeltaMessage6* dcreo6 = new CreatureObjectDeltaMessage6(
			asCreatureObject());
	dcreo6->updateAlternateAppearance();
	dcreo6->close();

	broadcastMessage(dcreo6, true);
}

bool CreatureObjectImplementation::setState(uint64 state, bool notifyClient) {
	if (!(stateBitmask & state)) {
		stateBitmask |= state;

		if (notifyClient) {
			if (state == CreatureState::SITTINGONCHAIR) {
				//this is fucking wrong

				Zone* thisZone = getZone();

				setPosture(CreaturePosture::SITTING, false);

				SortedVector<ManagedReference<QuadTreeEntry*> > closeSceneObjects;
				int maxInRangeObjects = 0;

				if (thisZone != NULL) {
					//Locker locker(thisZone);

					if (closeobjects == NULL) {
						info("Null closeobjects vector in CreatureObjectImplementation::setState", true);
						thisZone->getInRangeObjects(getWorldPositionX(), getWorldPositionY(), ZoneServer::CLOSEOBJECTRANGE, &closeSceneObjects, true);
						maxInRangeObjects = closeSceneObjects.size();
					} else {
						closeobjects->safeCopyTo(closeSceneObjects);
						maxInRangeObjects = closeSceneObjects.size();
					}

					for (int i = 0; i < closeSceneObjects.size(); ++i) {
						SceneObject* object = cast<SceneObject*> (closeSceneObjects.get(i).get());

						if (object->getParent().get() == getParent().get()) {
							SitOnObject* soo = new SitOnObject(asCreatureObject(), getPositionX(), getPositionZ(), getPositionY());
							object->sendMessage(soo);
							CreatureObjectDeltaMessage3* dcreo3 = new CreatureObjectDeltaMessage3(asCreatureObject());
							dcreo3->updatePosture();
							dcreo3->updateState();
							dcreo3->close();
							object->sendMessage(dcreo3);
						} else {
							sendDestroyTo(object);
							sendTo(object, true);
						}
					}
				}
			} else {
				CreatureObjectDeltaMessage3* dcreo3 = new CreatureObjectDeltaMessage3(asCreatureObject());
				dcreo3->updateState();
				dcreo3->close();

				broadcastMessage(dcreo3, true);
			}

			switch (state) {
			case CreatureState::STUNNED:
				playEffect("clienteffect/combat_special_defender_stun.cef");
				sendSystemMessage("@cbt_spam:go_stunned_single");
				sendStateCombatSpam("cbt_spam", "go_stunned", 0);
				break;
			case CreatureState::BLINDED:
				playEffect("clienteffect/combat_special_defender_blind.cef");
				sendSystemMessage("@cbt_spam:go_blind_single");
				sendStateCombatSpam("cbt_spam", "go_blind", 0);
				break;
			case CreatureState::DIZZY: {
				playEffect("clienteffect/combat_special_defender_dizzy.cef");
				sendSystemMessage("@cbt_spam:go_dizzy_single");
				sendStateCombatSpam("cbt_spam", "go_dizzy", 0);
				break;
			}
			case CreatureState::POISONED:
				break;
			case CreatureState::DISEASED:
				break;
			case CreatureState::ONFIRE:
				break;
			case CreatureState::BLEEDING:
				break;
			case CreatureState::INTIMIDATED:
				playEffect("clienteffect/combat_special_defender_intimidate.cef");
				break;
			case CreatureState::IMMOBILIZED:
				//showFlyText("combat_effects", "go_snare", 0, 0xFF, 0);
				break;
			case CreatureState::FROZEN:
				//showFlyText("combat_effects", "go_rooted", 0, 0xFF, 0);
				break;
			case CreatureState::RALLIED:
				showFlyText("combat_effects", "go_rally", 0, 0xFF, 0);
				break;
			case CreatureState::BERSERK:
				playEffect("clienteffect/combat_special_attacker_berserk.cef");
				break;
			case CreatureState::AIMING:
				playEffect("clienteffect/combat_special_attacker_aim.cef");
				break;
			case CreatureState::COVER:
				playEffect("clienteffect/combat_special_attacker_cover.cef");
				sendSystemMessage("@cbt_spam:cover_success_single");
				sendStateCombatSpam("cbt_spam", "cover_success", 0);
				break;
			case CreatureState::PEACE:
				sendSystemMessage("@cbt_spam:peace_single");
				sendStateCombatSpam("cbt_spam", "peace", 0);
				break;

			default:
				break;
			}

		}

		return true;
	}

	return false;
}

bool CreatureObjectImplementation::clearState(uint64 state, bool notifyClient) {
	if (stateBitmask & state) {
		stateBitmask &= ~state;

		if (notifyClient) {
			CreatureObjectDeltaMessage3* dcreo3 =
					new CreatureObjectDeltaMessage3(asCreatureObject());
			dcreo3->updateState();
			dcreo3->close();

			broadcastMessage(dcreo3, true);
		}

		switch (state) {
		case CreatureState::STUNNED:
			sendSystemMessage("@cbt_spam:no_stunned_single");
			sendStateCombatSpam("cbt_spam", "no_stunned", 0);
			break;
		case CreatureState::BLINDED:
			sendSystemMessage("@cbt_spam:no_blind_single");
			sendStateCombatSpam("cbt_spam", "no_blind", 0);
			break;
		case CreatureState::DIZZY:
			sendSystemMessage("@cbt_spam:no_dizzy_single");
			sendStateCombatSpam("cbt_spam", "no_dizzy", 0);
			break;
		case CreatureState::POISONED:
			sendSystemMessage("@dot_message:stop_poisoned");
			break;
		case CreatureState::DISEASED:
			sendSystemMessage("@dot_message:stop_diseased");
			break;
		case CreatureState::ONFIRE:
			sendSystemMessage("@dot_message:stop_fire");
			break;
		case CreatureState::BLEEDING:
			sendSystemMessage("@dot_message:stop_bleeding");
			break;
		case CreatureState::INTIMIDATED:
			break;
		case CreatureState::IMMOBILIZED:
			//showFlyText("combat_effects", "no_snare", 0, 0xFF, 0);
			break;
		case CreatureState::FROZEN:
			//showFlyText("combat_effects", "no_rooted", 0, 0xFF, 0);
			break;
		case CreatureState::RALLIED:
			showFlyText("combat_effects", "no_rally", 0xFF, 0, 0);
			break;
		case CreatureState::BERSERK:
			break;
		case CreatureState::AIMING:
			break;
		case CreatureState::COVER: {
			if (hasBuff(CreatureState::COVER)) {
				removeBuff(CreatureState::COVER);
			}
			break;
		}
		default:
			break;
		}

		return true;
	} else {
		return false;
	}
}

void CreatureObjectImplementation::setHAM(int type, int value,
		bool notifyClient) {
	if (hamList.get(type) == value)
		return;

	/*StringBuffer msg;
	 msg << "setting ham type " << type << " to " << value;
	 info(msg.toString(), true);*/

	if (notifyClient) {
		CreatureObjectDeltaMessage6* msg = new CreatureObjectDeltaMessage6(
				asCreatureObject());
		msg->startUpdate(0x0D);
		hamList.set(type, value, msg);
		msg->close();

		broadcastMessage(msg, true);
	} else {
		hamList.set(type, value, NULL);
	}
}

int CreatureObjectImplementation::inflictDamage(TangibleObject* attacker, int damageType, float damage, bool destroy, const String& xp, bool notifyClient) {
	if (attacker->isCreatureObject()) {
		CreatureObject* creature = attacker->asCreatureObject();

		if (damage > 0) {
			getThreatMap()->addDamage(creature, damage, xp);
		}
	}

	return inflictDamage(attacker, damageType, damage, destroy, notifyClient);
}

int CreatureObjectImplementation::inflictDamage(TangibleObject* attacker, int damageType, float damage, bool destroy, bool notifyClient) {
	if (damageType < 0 || damageType >= hamList.size()) {
		error(
				"incorrect damage type in CreatureObjectImplementation::inflictDamage");
		return 0;
	}

	if (this->isIncapacitated() || this->isDead() || damage == 0)
		return 0;

	int currentValue = hamList.get(damageType);

	int newValue = currentValue - (int) damage;

	if (!destroy && newValue <= 0)
		newValue = 1;

	if (getSkillMod("avoid_incapacitation") > 0)
		newValue = 1;

	if (damageType % 3 != 0 && newValue < 0) // secondaries never should go negative
		newValue = 0;

	setHAM(damageType, newValue, notifyClient);

	if (attacker == NULL)
		attacker = asCreatureObject();

	if (newValue <= 0)
		notifyObjectDestructionObservers(attacker, newValue);

	return 0;
}

int CreatureObjectImplementation::healDamage(TangibleObject* healer,
		int damageType, int damage, bool notifyClient, bool notifyObservers) {
	if (damage == 0)
		return 0;

	if (damageType < 0 || damageType >= hamList.size()) {
		error(
				"incorrect damage type in CreatureObjectImplementation::healDamage");
		return 0;
	}

	int returnValue = damage;

	int currentValue = hamList.get(damageType);

	int newValue = currentValue + damage;

	int maxValue = maxHamList.get(damageType) - wounds.get(damageType);

	if (newValue > maxValue)
		returnValue = maxValue - currentValue;

	newValue = MIN(newValue, maxValue);

	if (currentValue <= 0 && (isIncapacitated() || isDead())) {
		if (newValue <= 0)
			newValue = 1;

		if (damageType % 3 == 0) {
			setPosture(CreaturePosture::UPRIGHT);
			if (isPet()) {
				AiAgent* pet = asAiAgent();
				ManagedReference<CreatureObject*> player = getLinkedCreature().get();

				if (pet != NULL && player != NULL) {
					pet->setFollowObject(player);
					pet->activateMovementEvent();
				}
			}
		}
	}

	if (damageType % 3 != 0 && newValue < 0)
		newValue = 0;

	setHAM(damageType, newValue, notifyClient);

	if(healer != NULL && notifyObservers) {
		asCreatureObject()->notifyObservers(ObserverEventType::HEALINGPERFORMED, healer, returnValue);
	}

	return returnValue;
}

int CreatureObjectImplementation::healWound(TangibleObject* healer,
		int damageType, int damage, bool notifyClient, bool notifyObservers) {
	if (damage == 0)
		return 0;

	if (damageType < 0 || damageType >= wounds.size()) {
		error("incorrect damage type in CreatureObjectImplementation::healWound");
		return 0;
	}

	int currentValue = wounds.get(damageType);

	int newValue = currentValue - damage;

	newValue = MAX(newValue, 0);

	int returnValue = currentValue - newValue; //How many wounds we had minus how many we now have = how much was healed.

	setWounds(damageType, newValue, notifyClient);

	if (healer != NULL && notifyObservers) {
		asCreatureObject()->notifyObservers(ObserverEventType::WOUNDHEALINGPERFORMED, healer, returnValue);
	}

	return returnValue;
}

void CreatureObjectImplementation::setBaseHAM(int type, int value,
		bool notifyClient) {
	if (baseHAM.get(type) == value)
		return;

	/*StringBuffer msg;
	 msg << "setting baseham type " << type << " to " << value;
	 info(msg.toString(), true);*/

	if (notifyClient) {
		CreatureObjectDeltaMessage1* msg =
				new CreatureObjectDeltaMessage1(this);
		msg->startUpdate(0x02);
		baseHAM.set(type, value, msg);
		msg->close();

		broadcastMessage(msg, true);
	} else {
		baseHAM.set(type, value, NULL);
	}
}

void CreatureObjectImplementation::setWounds(int type, int value,
		bool notifyClient) {
	if (value < 0)
		value = 0;

	if (value >= baseHAM.get(type))
		value = baseHAM.get(type) - 1;

	if (wounds.get(type) == value)
		return;

	if (notifyClient) {
		CreatureObjectDeltaMessage3* msg = new CreatureObjectDeltaMessage3(
				asCreatureObject());
		msg->startUpdate(0x11);
		wounds.set(type, value, msg);
		msg->close();

		broadcastMessage(msg, true);
	} else {
		wounds.set(type, value, NULL);
	}

	int maxHamValue = maxHamList.get(type) - wounds.get(type);

	if (getHAM(type) > maxHamValue) {
		setHAM(type, maxHamValue, notifyClient);
	} else if (type % 3 != 0) { //update secondary stats to the max
		setHAM(type, maxHamValue, notifyClient);
	}
}

int CreatureObjectImplementation::addWounds(int type, int value, bool notifyClient, bool doShockWounds) {
	if (type < 0 || type >= wounds.size()) {
		error("unknown wound type in addWounds");
		return 0;
	}

	int returnValue = value;

	int currentValue = wounds.get(type);

	int newValue = currentValue + value;

	if (newValue < 0)
		returnValue = value - newValue;

	if (value > 0 && asCreatureObject()->isPlayerCreature())
		sendStateCombatSpam("cbt_spam", "wounded", 1, value, false);

	setWounds(type, newValue, notifyClient);

	if (doShockWounds)
		addShockWounds(1, true);

	return returnValue;
}

void CreatureObjectImplementation::setMaxHAM(int type, int value,
		bool notifyClient) {
	if (maxHamList.get(type) == value)
		return;

	/*StringBuffer msg;
	 msg << "setting maxham type " << type << " to " << value;
	 info(msg.toString(), true);*/

	if (notifyClient) {
		CreatureObjectDeltaMessage6* msg = new CreatureObjectDeltaMessage6(
				asCreatureObject());
		msg->startUpdate(0x0E);
		maxHamList.set(type, value, msg);
		msg->close();

		broadcastMessage(msg, true);
	} else {
		maxHamList.set(type, value, NULL);
	}

	if (wounds.get(type) >= maxHamList.get(type)) // this will reset our wounds to not overflow max value
		setWounds(type, wounds.get(type), notifyClient);

	if (type % 3 != 0) { //changing secondary stats, updating current value
		healDamage(asCreatureObject(), type, value - hamList.get(type), notifyClient);
	}
}

void CreatureObjectImplementation::setEncumbrance(int type, int value,
		bool notifyClient) {
	if (encumbrances.get(type) == value)
		return;

	if (notifyClient) {
		CreatureObjectDeltaMessage4* msg =
				new CreatureObjectDeltaMessage4(asCreatureObject());
		msg->startUpdate(0x02);
		encumbrances.set(type, value, msg);
		msg->close();

		broadcastMessage(msg, true);
	} else {
		encumbrances.set(type, value, NULL);
	}
}

void CreatureObjectImplementation::addMaxHAM(int type, int value,
		bool notifyClient) {
	if (type < 0 || type > maxHamList.size()) {
		error("invalid type in CreatureObjectImplementation::addMaxHAM");
		return;
	}

	int currentValue = maxHamList.get(type);
	int newValue = currentValue + value;

	setMaxHAM(type, newValue, notifyClient);
}

void CreatureObjectImplementation::addEncumbrance(int type, int value,
		bool notifyClient) {
	if (type < 0 || type >= encumbrances.size()) {
		error("invalid type in CreatureObjectImplementation::addEncumbrance");
		return;
	}

	int currentValue = encumbrances.get(type);
	int newValue = currentValue + value;

	setEncumbrance(type, newValue, notifyClient);
}

void CreatureObjectImplementation::setBankCredits(int credits,
		bool notifyClient) {
	if (bankCredits == credits)
		return;

	bankCredits = credits;

	if (notifyClient) {
		CreatureObjectDeltaMessage1* delta = new CreatureObjectDeltaMessage1(
				this);
		delta->updateBankCredits();
		delta->close();

		sendMessage(delta);
	}
}

void CreatureObjectImplementation::setCashCredits(int credits,
		bool notifyClient) {
	if (cashCredits == credits)
		return;

	cashCredits = credits;

	if (notifyClient) {
		CreatureObjectDeltaMessage1* delta = new CreatureObjectDeltaMessage1(
				this);
		delta->updateCashCredits();
		delta->close();

		sendMessage(delta);
	}
}

void CreatureObjectImplementation::addSkill(Skill* skill, bool notifyClient) {
	if (skillList.contains(skill))
		return;

	if (notifyClient) {
		CreatureObjectDeltaMessage1* msg =
				new CreatureObjectDeltaMessage1(this);
		msg->startUpdate(0x03);
		skillList.add(skill, msg);
		msg->close();

		sendMessage(msg);
	} else {
		skillList.add(skill, NULL);
	}
}

void CreatureObjectImplementation::removeSkill(Skill* skill, bool notifyClient) {
	if (!skillList.contains(skill))
		return;

	if (notifyClient) {
		CreatureObjectDeltaMessage1* msg =
				new CreatureObjectDeltaMessage1(this);
		msg->startUpdate(0x03);
		skillList.remove(skill, msg);
		msg->close();

		sendMessage(msg);
	} else {
		skillList.remove(skill);
	}
}

void CreatureObjectImplementation::removeSkill(const String& skill,
		bool notifyClient) {
	SkillManager* skillManager = server->getZoneServer()->getSkillManager();

	Reference<Skill*> skillObject = skillManager->getSkill(skill);

	if (skillObject == NULL) {
		error("trying to remove null skill  " + skill);
		return;
	}

	removeSkill(skillObject, notifyClient);
}

void CreatureObjectImplementation::addSkillMod(const int modType, const String& skillMod, int value, bool notifyClient) {

	Locker locker(&skillModMutex);

	SkillModEntry oldMod;

	if(skillModList.contains(skillMod)) {
		oldMod = skillModList.get(skillMod);
	}

	skillModList.add(modType, skillMod, value);

	SkillModEntry newMod = skillModList.getVisibleSkillMod(skillMod);

	if(newMod == oldMod) {
		return;
	}

	if (notifyClient) {
		CreatureObjectDeltaMessage4* msg = new CreatureObjectDeltaMessage4(asCreatureObject());
		msg->startUpdate(0x03);
		if(newMod.getTotalSkill() != 0)
			skillModList.set(skillMod, newMod, msg, 1);
		else {
			skillModList.drop(skillMod, msg, 1);
		}
		msg->close();

		sendMessage(msg);
	} else {
		if(newMod.getTotalSkill() != 0)
			skillModList.set(skillMod, newMod);
		else
			skillModList.drop(skillMod);
	}
}

void CreatureObjectImplementation::removeSkillMod(const int modType, const String& skillMod, int value, bool notifyClient) {
	addSkillMod(modType, skillMod, -value, notifyClient);
}

void CreatureObjectImplementation::removeAllSkillModsOfType(const int modType, bool notifyClient) {
	SkillModGroup* modGroup = skillModList.getSkillModGroup(modType);

	if (notifyClient) {
		for (int i = modGroup->size() - 1; i >= 0; --i) {
			VectorMapEntry<String, int>* entry = &modGroup->elementAt(i);
			String key = entry->getKey();
			int val = entry->getValue();
			// use the type instead of hardcoding CITY here
			removeSkillMod(modType, key, val, true);
		}
	} else {
		modGroup->removeAll();
	}
}

int CreatureObjectImplementation::getSkillMod(const String& skillmod) {
	Locker locker(&skillModMutex);
	return skillModList.getSkillMod(skillmod);
}

int CreatureObjectImplementation::getSkillModOfType(const String& skillmod, const unsigned int modType) {
	Locker locker(&skillModMutex);

	return skillModList.getSkillModOfType(skillmod, modType);
}

void CreatureObjectImplementation::addSkill(const String& skill,
		bool notifyClient) {
	SkillManager* skillManager = server->getZoneServer()->getSkillManager();

	Reference<Skill*> skillObject = skillManager->getSkill(skill);

	if (skillObject == NULL) {
		error("trying to add null skill box " + skill);
		return;
	}

	addSkill(skillObject, notifyClient);
}

void CreatureObjectImplementation::setPosture(int newPosture, bool notifyClient) {
	if (posture == newPosture)
		return;

	if (posture == CreaturePosture::PRONE && isInCover()) {
		clearState(CreatureState::COVER);
	}

	posture = newPosture;
	
	updateSpeedAndAccelerationMods();
	
	// TODO: these two seem to be as of yet unused (maybe only necessary in client)
	//CreaturePosture::instance()->getTurnScale((uint8)newPosture);
	//CreaturePosture::instance()->getCanSeeHeightMod((uint8)newPosture);

	if (posture != CreaturePosture::SITTING && hasState(
			CreatureState::SITTINGONCHAIR))
		clearState(CreatureState::SITTINGONCHAIR);

	if (notifyClient) {
		Vector<BasePacket*> messages;

		PostureMessage* octrl = new PostureMessage(asCreatureObject());
		messages.add(octrl);

		CreatureObjectDeltaMessage3* dcreo3 = new CreatureObjectDeltaMessage3(
				asCreatureObject());
		dcreo3->updatePosture();
		//dcreo3->updateState();
		dcreo3->close();

		messages.add(dcreo3);

		broadcastMessages(&messages, true);

		if (!isProbotSpecies() && (isPlayerCreature() || isAiAgent())) {
			switch (posture) {
			case CreaturePosture::UPRIGHT:
				sendStateCombatSpam("cbt_spam", "stand", 11);
				break;
			case CreaturePosture::PRONE:
				sendStateCombatSpam("cbt_spam", "prone", 11);
				break;
			case CreaturePosture::CROUCHED:
				sendStateCombatSpam("cbt_spam", "kneel", 11);
				break;
			}
		}
	}

	if(posture != CreaturePosture::UPRIGHT && posture != CreaturePosture::DRIVINGVEHICLE
				&& posture != CreaturePosture::RIDINGCREATURE && posture != CreaturePosture::SKILLANIMATING ) {
		setCurrentSpeed(0);
	}

	updateLocomotion();
	notifyPostureChange(newPosture);
}

void CreatureObjectImplementation::updateSpeedAndAccelerationMods() {
	float speedboost = 0;

	if(posture == CreaturePosture::PRONE && !hasBuff(CreatureState::COVER)) {
		speedboost = getSkillMod("slope_move") >= 50
				? ((getSkillMod("slope_move") - 50.0f) / 100.0f) / 2 : 0;
	}

	setSpeedMultiplierMod(CreaturePosture::instance()->getMovementScale((uint8) posture) + speedboost, true);

	setAccelerationMultiplierMod(CreaturePosture::instance()->getAccelerationScale((uint8) posture), true);

	setTurnScale(CreaturePosture::instance()->getTurnScale((uint8) posture), true);
}

float CreatureObjectImplementation::calculateSpeed() {
	Time currentTime;
	uint32 deltaTime = currentTime.getMiliTime() - lastCombatActionTime.getMiliTime();
	Vector3 newPosition = getPosition();

	float dist = newPosition.distanceTo(lastCombatPosition);
	float speed = dist / (float) deltaTime * 1000;

	//update position and time
	lastCombatActionTime.updateToCurrentTime();
	lastCombatPosition = newPosition;

	return speed;
}

void CreatureObjectImplementation::updateLocomotion() {
	// 0: stationary, 0-walk: slow, walk-run+; fast
	// the movement table does not seem to want scaling factors...
	if (currentSpeed <= walkSpeed / 10.f)
		locomotion = CreaturePosture::instance()->getLocomotion(posture,
				CreatureLocomotion::STATIONARY);
	else if (currentSpeed <= walkSpeed + walkSpeed / 10.0f)
		locomotion = CreaturePosture::instance()->getLocomotion(posture,
				CreatureLocomotion::SLOW);
	else
		locomotion = CreaturePosture::instance()->getLocomotion(posture,
				CreatureLocomotion::FAST);
}

UnicodeString CreatureObjectImplementation::getCreatureName() {
	return getCustomObjectName();
}

void CreatureObjectImplementation::updateGroupInviterID(uint64 id,
		bool notifyClient) {
	groupInviterID = id;
	++groupInviteCounter;

	CreatureObjectDeltaMessage6* delta = new CreatureObjectDeltaMessage6(asCreatureObject());
	delta->updateInviterId();
	delta->close();

	broadcastMessage(delta, true);
}

void CreatureObjectImplementation::updateGroup(GroupObject* grp,
		bool notifyClient) {
	group = grp;

	CreatureObjectDeltaMessage6* delta = new CreatureObjectDeltaMessage6(asCreatureObject());
	delta->updateGroupID();
	delta->close();

	broadcastMessage(delta, true);
}

void CreatureObjectImplementation::setMood(byte mood, bool notifyClient) {
	moodID = mood;
	moodString = Races::getMoodStr(Races::getMood(moodID));

	if (notifyClient) {
		CreatureObjectDeltaMessage6* dcreo6 = new CreatureObjectDeltaMessage6(
				asCreatureObject());
		dcreo6->updateMoodID();
		dcreo6->updateMoodStr();
		dcreo6->close();

		broadcastMessage(dcreo6, true);
	}
}

void CreatureObjectImplementation::setAccelerationMultiplierBase(
		float newMultiplierBase, bool notifyClient) {
	if (accelerationMultiplierBase == newMultiplierBase)
		return;

	accelerationMultiplierBase = newMultiplierBase;

	if (notifyClient) {
		CreatureObjectDeltaMessage4* dcreo4 = new CreatureObjectDeltaMessage4(
				asCreatureObject());
		dcreo4->updateAccelerationMultiplierBase();
		dcreo4->close();

		sendMessage(dcreo4);
	}
}

void CreatureObjectImplementation::setAccelerationMultiplierMod(float newMultiplierMod, bool notifyClient) {
	float buffMod = getSkillMod("private_acceleration_multiplier") > 0 ? (float)getSkillMod("private_acceleration_multiplier") / 100.f : 1.f;

	if (accelerationMultiplierMod == newMultiplierMod * buffMod)
		return;

	accelerationMultiplierMod = newMultiplierMod * buffMod;

	if (notifyClient) {
		CreatureObjectDeltaMessage4* dcreo4 = new CreatureObjectDeltaMessage4(
				asCreatureObject());
		dcreo4->updateAccelerationMultiplierMod();
		dcreo4->close();

		sendMessage(dcreo4);
	}
}

void CreatureObjectImplementation::setSpeedMultiplierBase(
		float newMultiplierBase, bool notifyClient) {
	if (speedMultiplierBase == newMultiplierBase)
		return;

	speedMultiplierBase = newMultiplierBase;

	if (notifyClient) {
		CreatureObjectDeltaMessage4* dcreo4 = new CreatureObjectDeltaMessage4(
				asCreatureObject());
		dcreo4->updateSpeedMultiplierBase();
		dcreo4->close();

		sendMessage(dcreo4);
	}
}

void CreatureObjectImplementation::setTurnScale(
		float newMultiplierBase, bool notifyClient) {
	if (turnScale == newMultiplierBase)
		return;

	turnScale = newMultiplierBase;

	if (notifyClient) {
		CreatureObjectDeltaMessage4* dcreo4 = new CreatureObjectDeltaMessage4(
				asCreatureObject());
		dcreo4->updateTurnScale();
		dcreo4->close();

		sendMessage(dcreo4);
	}
}

void CreatureObjectImplementation::setFactionRank(int rank, bool notifyClient) {
	if (factionRank == rank)
		return;

	factionRank = rank;

	if (!notifyClient)
		return;

	CreatureObjectDeltaMessage3* msg = new CreatureObjectDeltaMessage3(asCreatureObject());
	msg->updateFactionRank();
	msg->close();
	broadcastMessage(msg, true);
}

void CreatureObjectImplementation::setSpeedMultiplierMod(float newMultiplierMod, bool notifyClient) {
	float buffMod = 1;

	if (posture == CreaturePosture::UPRIGHT) {
		buffMod = getSkillMod("private_speed_multiplier") > 0 ? (float)getSkillMod("private_speed_multiplier") / 100.f : 1.f;
	} else if(posture == CreaturePosture::PRONE && hasBuff(CreatureState::COVER)) {
		if (hasSkill("combat_rifleman_speed_03")) {
			buffMod = 0.5f;
		} else {
			buffMod = 0.f;
		}
	}

	if (speedMultiplierMod == newMultiplierMod * buffMod)
		return;

	speedMultiplierMod = newMultiplierMod * buffMod;

	int bufferSize = speedMultiplierModChanges.size();

	if (bufferSize > 5) {
		speedMultiplierModChanges.remove(0);
	}

	speedMultiplierModChanges.add(SpeedModChange(speedMultiplierMod));

	if (notifyClient) {
		CreatureObjectDeltaMessage4* dcreo4 = new CreatureObjectDeltaMessage4(
				asCreatureObject());
		dcreo4->updateSpeedMultiplierMod();
		dcreo4->close();

		sendMessage(dcreo4);
	}
}

void CreatureObjectImplementation::setRunSpeed(float newSpeed,
		bool notifyClient) {
	if (runSpeed == newSpeed)
		return;

	runSpeed = newSpeed;

	if (notifyClient) {
		CreatureObjectDeltaMessage4* dcreo4 = new CreatureObjectDeltaMessage4(
				asCreatureObject());
		dcreo4->updateRunSpeed();
		dcreo4->close();

		sendMessage(dcreo4);
	}
}

void CreatureObjectImplementation::setMoodString(
		const String& moodAnimationString, bool notifyClient) {
	moodString = moodAnimationString;

	if (notifyClient) {
		CreatureObjectDeltaMessage6* dcreo6 = new CreatureObjectDeltaMessage6(
				asCreatureObject());
		dcreo6->updateMoodStr();
		dcreo6->close();

		broadcastMessage(dcreo6, true);
	}
}

void CreatureObjectImplementation::setPerformanceCounter(int counter,
		bool notifyClient) {
	if (performanceCounter == counter)
		return;

	performanceCounter = counter;

	if (!notifyClient)
		return;

	CreatureObjectDeltaMessage6* codm4 = new CreatureObjectDeltaMessage6(asCreatureObject());
	codm4->updatePerformanceCounter(counter);
	codm4->close();
	broadcastMessage(codm4, true);
}

void CreatureObjectImplementation::setListenToID(uint64 id, bool notifyClient) {
	if (listenToID == id)
		return;

	listenToID = id;

	if (!notifyClient)
		return;

	CreatureObjectDeltaMessage4* codm4 = new CreatureObjectDeltaMessage4(asCreatureObject());
	codm4->updateListenToID(id);
	codm4->close();
	sendMessage(codm4);
}

void CreatureObjectImplementation::setPerformanceAnimation(
		const String& animation, bool notifyClient) {
	if (performanceAnimation == animation)
		return;

	performanceAnimation = animation;

	if (!notifyClient)
		return;

	CreatureObjectDeltaMessage6* codm4 = new CreatureObjectDeltaMessage6(asCreatureObject());
	codm4->updatePerformanceAnimation(animation);
	codm4->close();
	broadcastMessage(codm4, true);
}

void CreatureObjectImplementation::setTerrainNegotiation(float value,
		bool notifyClient) {
	terrainNegotiation = value;

	if (notifyClient)
		updateTerrainNegotiation();
}

void CreatureObjectImplementation::updateTerrainNegotiation()
{
	CreatureObjectDeltaMessage4* codm4 = new CreatureObjectDeltaMessage4(asCreatureObject());
	codm4->updateTerrainNegotiation();
	codm4->close();
	sendMessage(codm4);
}

float CreatureObjectImplementation::getTerrainNegotiation() {
    float slopeMod = ((float)getSkillMod("slope_move") / 50.0f) + terrainNegotiation;
	if(slopeMod > 1)
		slopeMod = 1;
	return slopeMod;
}

void CreatureObjectImplementation::enqueueCommand(unsigned int actionCRC,
		unsigned int actionCount, uint64 targetID,
		const UnicodeString& arguments, int priority,
		int compareCounter) {
	ManagedReference<ObjectController*> objectController =
			getZoneServer()->getObjectController();

	QueueCommand* queueCommand = objectController->getQueueCommand(actionCRC);

	if (queueCommand == NULL) {
		//StringBuffer msg;
		//msg << "trying to enqueue NULL QUEUE COMMAND 0x" << hex << actionCRC;
		//error(msg.toString());

		//StackTrace::printStackTrace();
		return;
	}

	if (priority < 0)
		priority = queueCommand->getDefaultPriority();

	Reference<CommandQueueAction*> action = NULL;

	if (priority == QueueCommand::IMMEDIATE) {
#ifndef WITH_STM
		objectController->activateCommand(asCreatureObject(), actionCRC, actionCount,
				targetID, arguments);
#else
		action = new CommandQueueAction(asCreatureObject(), targetID, actionCRC, actionCount, arguments);

		immediateQueue->put(action.get());

		if (immediateQueue->size() == 1) {
			Reference<CommandQueueActionEvent*> ev = new CommandQueueActionEvent(asCreatureObject(), CommandQueueActionEvent::IMMEDIATE);
			Core::getTaskManager()->executeTask(ev);
		}
#endif

		return;
	}

	if (commandQueue->size() > 15 && priority != QueueCommand::FRONT) {
		clearQueueAction(actionCount);

		return;
	}

	action = new CommandQueueAction(asCreatureObject(), targetID, actionCRC, actionCount,
			arguments);

	if (compareCounter >= 0)
		action->setCompareToCounter((int)compareCounter);

	if (commandQueue->size() != 0 || !nextAction.isPast()) {
		if (commandQueue->size() == 0) {
			Reference<CommandQueueActionEvent*> e =
					new CommandQueueActionEvent(asCreatureObject());
			e->schedule(nextAction);
		}

		if (priority == QueueCommand::NORMAL)
			commandQueue->put(action.get());
		else if (priority == QueueCommand::FRONT) {
			if (commandQueue->size() > 0)
				action->setCompareToCounter(
						commandQueue->get(0)->getCompareToCounter() - 1);

			commandQueue->put(action.get());
		}
	} else {
		nextAction.updateToCurrentTime();

		commandQueue->put(action.get());
		activateQueueAction();
	}
}

void CreatureObjectImplementation::sendCommand(const String& action, const UnicodeString& args, uint64 targetID, int priority) {
	sendCommand(action.hashCode(), args, targetID, priority);
}

void CreatureObjectImplementation::sendCommand(uint32 crc, const UnicodeString& args, uint64 targetID, int priority) {
	uint32 nextCounter = incrementLastActionCounter();
	CommandQueueAdd* msg = new CommandQueueAdd(asCreatureObject(), crc, nextCounter);
	sendMessage(msg);

	int compareCnt = -1;
	if (commandQueue->size() == 0 || priority == QueueCommand::FRONT)
		compareCnt = 0;
	else if (priority == QueueCommand::NORMAL)
		compareCnt = commandQueue->get(commandQueue->size() - 1)->getCompareToCounter() + 1;

	enqueueCommand(crc, nextCounter, targetID, args, priority, compareCnt);
}

void CreatureObjectImplementation::activateImmediateAction() {
	/*if (immediateQueue->size() == 0)
	 return;*/

	Reference<CommandQueueAction*> action = immediateQueue->get(0);

	immediateQueue->remove(0);

	ManagedReference<ObjectController*> objectController =
			getZoneServer()->getObjectController();

	float time = objectController->activateCommand(asCreatureObject(), action->getCommand(),
			action->getActionCounter(), action->getTarget(),
			action->getArguments());

	if (immediateQueue->size() > 0) {
		Reference<CommandQueueActionEvent*> ev = new CommandQueueActionEvent(
				asCreatureObject(), CommandQueueActionEvent::IMMEDIATE);
		Core::getTaskManager()->executeTask(ev);
	}
}

void CreatureObjectImplementation::activateQueueAction() {
	if (nextAction.isFuture()) {
		CommandQueueActionEvent* e = new CommandQueueActionEvent(asCreatureObject());
		e->schedule(nextAction);

		return;
	}

	if (commandQueue->size() == 0)
		return;

	Reference<CommandQueueAction*> action = commandQueue->get(0);
	commandQueue->remove(0);

	ManagedReference<ObjectController*> objectController =
			getZoneServer()->getObjectController();

	float time = objectController->activateCommand(asCreatureObject(), action->getCommand(),
			action->getActionCounter(), action->getTarget(),
			action->getArguments());

	//delete action;

	nextAction.updateToCurrentTime();

	if (time > 0)
		nextAction.addMiliTime((uint32)(time * 1000));

	if (commandQueue->size() != 0) {
		Reference<CommandQueueActionEvent*> e = new CommandQueueActionEvent(
				asCreatureObject());

		if (!nextAction.isFuture()) {
			nextAction.updateToCurrentTime();
			nextAction.addMiliTime(100);
		}

		e->schedule(nextAction);
	}
}

void CreatureObjectImplementation::deleteQueueAction(uint32 actionCount) {
	for (int i = 0; i < commandQueue->size(); ++i) {
		CommandQueueAction* action = commandQueue->get(i);

		if (action->getActionCounter() == actionCount) {
			commandQueue->remove(i);
			break;
		}
	}
}

void CreatureObjectImplementation::subtractBankCredits(int credits) {
	int newCredits = bankCredits - credits;

	assert(newCredits >= 0);

	setBankCredits(newCredits);
}

void CreatureObjectImplementation::subtractCashCredits(int credits) {
	int newCredits = cashCredits - credits;

	assert(newCredits >= 0);

	setCashCredits(newCredits);
}

void CreatureObjectImplementation::notifyLoadFromDatabase() {
	TangibleObjectImplementation::notifyLoadFromDatabase();
	/**
	 * Here we are loading the schematics based on the skills that the
	 * player has, we do this incase we change the items
	 * in the schematic group.
	 */
	PlayerObject* ghost = getPlayerObject();

	if (hasState(CreatureState::ALERT)) {
		clearState(CreatureState::ALERT);
		setMoodString("neutral");
	}

	listenToID = 0;
	watchToID = 0;

	if (cashCredits < 0)
		cashCredits = 0;

	if (bankCredits < 0)
		bankCredits = 0;

	if (isIncapacitated()) {
		int health = getHAM(CreatureAttribute::HEALTH);

		if (health < 0)
			setHAM(CreatureAttribute::HEALTH, 1);

		int action = getHAM(CreatureAttribute::ACTION);

		if (action < 0)
			setHAM(CreatureAttribute::ACTION, 1);

		int mind = getHAM(CreatureAttribute::MIND);

		if (mind < 0)
			setHAM(CreatureAttribute::MIND, 1);

		setPosture(CreaturePosture::UPRIGHT);
	}

	if (ghost == NULL)
		return;

	getZoneServer()->getPlayerManager()->fixHAM(asCreatureObject());
	getZoneServer()->getPlayerManager()->fixBuffSkillMods(asCreatureObject());

	for (int i = 0; i < creatureBuffs.getBuffListSize(); ++i) {
		ManagedReference<Buff*> buff = creatureBuffs.getBuffByIndex(i);

		buff->loadBuffDurationEvent(asCreatureObject());
	}

	ZoneServer* zoneServer = server->getZoneServer();
	SkillManager* skillManager = SkillManager::instance();

	SkillList* playerSkillList = getSkillList();

	int totalSkillPointsWasted = 250;

	for (int i = 0; i < playerSkillList->size(); ++i) {
		Skill* skill = playerSkillList->get(i);

		if (skill == NULL)
			continue;

		skillManager->awardDraftSchematics(skill, ghost, false);

		totalSkillPointsWasted -= skill->getSkillPointsRequired();
	}

	if (ghost->getSkillPoints() != totalSkillPointsWasted) {
		error("skill points on load mismatch calculated: " + String::valueOf(totalSkillPointsWasted) + " found: " + String::valueOf(ghost->getSkillPoints()));
		ghost->setSkillPoints(totalSkillPointsWasted);
	}

	ghost->getSchematics()->addRewardedSchematics(ghost);

	skillManager->updateXpLimits(ghost);

	if (getZone() != NULL)
		ghost->setLinkDead();
}

int CreatureObjectImplementation::notifyObjectInserted(SceneObject* object) {
	if (object->isWeaponObject())
		setWeapon( cast<WeaponObject*> (object));

	return TangibleObjectImplementation::notifyObjectInserted(object);
}

int CreatureObjectImplementation::notifyObjectRemoved(SceneObject* object) {
	if (object->isWeaponObject())
		setWeapon( NULL);

	return TangibleObjectImplementation::notifyObjectInserted(object);
}

void CreatureObjectImplementation::setCreatureLink(CreatureObject* object,
		bool notifyClient) {
	if (linkedCreature == object)
		return;

	linkedCreature = object;

	if (!notifyClient)
		return;

	CreatureObjectDeltaMessage3* delta = new CreatureObjectDeltaMessage3(asCreatureObject());
	delta->updateCreatureLinkID();
	delta->close();

	broadcastMessage(delta, true);
}

void CreatureObjectImplementation::executeObjectControllerAction(
		unsigned int actionCRC) {
	ManagedReference<ObjectController*> objectController =
			getZoneServer()->getObjectController();

	objectController->activateCommand(asCreatureObject(), actionCRC, 0, 0, "");
}

void CreatureObjectImplementation::executeObjectControllerAction(
		unsigned int actionCRC, uint64 targetID, const UnicodeString& args) {
	ManagedReference<ObjectController*> objectController =
			getZoneServer()->getObjectController();

	objectController->activateCommand(asCreatureObject(), actionCRC, 0, targetID, args);
}

void CreatureObjectImplementation::doCombatAnimation(CreatureObject* defender,
		uint32 animcrc, byte hit, byte trails) {
	CombatAction* action = new CombatAction(asCreatureObject(), defender, animcrc, hit, trails);

	broadcastMessage(action, true);
}

void CreatureObjectImplementation::doAnimation(const String& anim) {
	Animation* msg = new Animation(asCreatureObject(), anim);

	broadcastMessage(msg, true);
}

void CreatureObjectImplementation::dismount() {
	executeObjectControllerAction(STRING_HASHCODE("dismount"));
}

float CreatureObjectImplementation::calculateBFRatio() {
	if (shockWounds <= 250)
		return 0;
	else
		return ((((float) shockWounds) - 250.0f) / 1000.0f);
}

void CreatureObjectImplementation::setDizziedState(int durationSeconds) {
	uint32 buffCRC = Long::hashCode(CreatureState::DIZZY);

	Reference<Buff*> buff = getBuff(buffCRC);
	if (buff != NULL) {
		Locker locker(buff);
		if (buff->getTimeLeft() < durationSeconds) {
			buff->renew(durationSeconds);
			return;
		}
	}

	Reference<StateBuff*> state = new StateBuff(asCreatureObject(), CreatureState::DIZZY, durationSeconds);
	Locker locker(state);

	state->setStartFlyText("combat_effects", "go_dizzy", 0, 0xFF, 0);
	state->setEndFlyText("combat_effects", "no_dizzy", 0xFF, 0, 0);

	addBuff(state);
}

void CreatureObjectImplementation::setAimingState(int durationSeconds) {
	if (!hasState(CreatureState::AIMING)) {
		StateBuff* aiming = new StateBuff(asCreatureObject(), CreatureState::AIMING, durationSeconds);

		int aimMods = 0;
		Vector<String>* creatureAimMods = weapon->getCreatureAimModifiers();

		for (int i = 0; i < creatureAimMods->size(); ++i) {
			aimMods += getSkillMod(creatureAimMods->get(i));
		}

		Locker locker(aiming);

		aiming->setSkillModifier("private_aim", aimMods);

		addBuff(aiming);
	}
}

void CreatureObjectImplementation::setRalliedState(int durationSeconds) {
	if (!hasState(CreatureState::RALLIED)) {
		Reference<StateBuff*> state = new StateBuff(asCreatureObject(), CreatureState::RALLIED, durationSeconds);

		Locker locker(state);

		state->setStartFlyText("combat_effects", "go_rally", 0, 0xFF, 0);
		state->setEndFlyText("combat_effects", "no_rally", 0xFF, 0, 0);

		addBuff(state);
	}
}

void CreatureObjectImplementation::setCoverState(int durationSeconds) {
	setPosture(CreaturePosture::PRONE);

	if (!hasState(CreatureState::COVER)) {
		Reference<StateBuff*> buff = new StateBuff(asCreatureObject(), CreatureState::COVER, durationSeconds);

		Locker locker(buff);

		buff->setStartFlyText("combat_effects", "go_cover", 0, 0xFF, 0);
		buff->setEndFlyText("combat_effects", "no_cover", 0xFF, 0, 0);

		buff->setSkillModifier("private_defense", 10);
		buff->setSkillModifier("ranged_defense", 25);

		addBuff(buff);
	}
}

void CreatureObjectImplementation::setBerserkedState(uint32 duration) {
	if (!hasState(CreatureState::BERSERK)) {
		Reference<StateBuff*> state = new StateBuff(asCreatureObject(), CreatureState::BERSERK, duration);

		Locker locker(state);

		state->setStartFlyText("combat_effects", "go_berserk", 0, 0xFF, 0);
		state->setEndFlyText("combat_effects", "no_berserk", 0xFF, 0, 0);

		int mod = getSkillMod("berserk");

		state->setSkillModifier("private_melee_accuracy_bonus", 20 + mod);
		state->setSkillModifier("private_melee_damage_bonus", 75 + mod);
		state->setSkillModifier("private_melee_defense", -60);
		state->setSkillModifier("private_ranged_defense", -60);

		addBuff(state);
	}
}
void CreatureObjectImplementation::setStunnedState(int durationSeconds) {
	uint32 buffCRC = Long::hashCode(CreatureState::STUNNED);

	Reference<Buff*> buff = getBuff(buffCRC);
	if (buff != NULL) {
		Locker locker(buff);
		if (buff->getTimeLeft() < durationSeconds) {
			buff->renew(durationSeconds);
			return;
		}
	}

	Reference<StateBuff*> state = new StateBuff(asCreatureObject(), CreatureState::STUNNED, durationSeconds, STRING_HASHCODE("stunstate"));

	Locker locker(state);

	state->setStartFlyText("combat_effects", "go_stunned", 0, 0xFF, 0);
	state->setEndFlyText("combat_effects", "no_stunned", 0xFF, 0, 0);
	state->setSkillModifier("private_melee_defense", -50);
	state->setSkillModifier("private_ranged_defense", -50);

	addBuff(state);

	locker.release();

	Reference<PrivateSkillMultiplierBuff*> multBuff = new PrivateSkillMultiplierBuff(asCreatureObject(), STRING_HASHCODE("stunstate"), durationSeconds, BuffType::STATE);

	Locker blocker(multBuff);

	multBuff->setSkillModifier("private_damage_divisor", 5);
	multBuff->setSkillModifier("private_damage_multiplier", 4);

	addBuff(multBuff);
}

void CreatureObjectImplementation::setBlindedState(int durationSeconds) {
	uint32 buffCRC = Long::hashCode(CreatureState::BLINDED);

	Reference<Buff*> buff = getBuff(buffCRC);
	if (buff != NULL) {
		Locker locker(buff);
		if (buff->getTimeLeft() < durationSeconds) {
			buff->renew(durationSeconds);
			return;
		}
	}

	Reference<StateBuff*> state = new StateBuff(asCreatureObject(), CreatureState::BLINDED, durationSeconds);

	Locker locker(state);

	state->setStartFlyText("combat_effects", "go_blind", 0, 0xFF, 0);
	state->setEndFlyText("combat_effects", "no_blind", 0xFF, 0, 0);

	state->setSkillModifier("private_attack_accuracy", -60);
	state->setSkillModifier("private_dodge_attack", -60);

	addBuff(state);
}

void CreatureObjectImplementation::setIntimidatedState(uint32 mod, uint32 crc, int durationSeconds) {
	// put the skillmods into a crc buff so we can stack all forms of intim
	if (!hasBuff(crc)) {
		Reference<PrivateSkillMultiplierBuff*> buff = new PrivateSkillMultiplierBuff(asCreatureObject(), crc, durationSeconds, BuffType::STATE);

		Locker locker(buff);

		buff->setSkillModifier("private_damage_divisor", 2);

		addBuff(buff);

	} else { // already have this intimidation buff, don't keep stacking the multiplier, but do extend the duration
		Reference<Buff*> buff = getBuff(crc);

		if (buff != NULL && buff->getTimeLeft() < durationSeconds) {
			Locker locker(buff);

			buff->renew(durationSeconds);
		}
	}

	if (!hasState(CreatureState::INTIMIDATED)) {
		Reference<StateBuff*> state = new StateBuff(asCreatureObject(), CreatureState::INTIMIDATED, durationSeconds, crc);

		Locker locker(state);

		state->setStartFlyText("combat_effects", "go_intimidated", 0, 0xFF, 0);
		state->setEndFlyText("combat_effects", "no_intimidated", 0xFF, 0, 0);

		state->setSkillModifier("private_melee_defense", -20);
		state->setSkillModifier("private_ranged_defense", -20);

		addBuff(state);
	} else { // already have the intimidated state, so extend it. This is the buff that gets sent to the client
		Reference<Buff*> state = getBuff(Long::hashCode(CreatureState::INTIMIDATED));

		if (state == NULL) { // this shouldn't happen, but if it does, we want to make sure intim gets set
			//removeStateBuff(CreatureState::INTIMIDATED);
			//setIntimidatedState(mod, crc, durationSeconds);
			error("no intimidate state buff in setIntimidatedState");
			clearState(CreatureState::INTIMIDATED);
			removeBuff(crc);
			setIntimidatedState(mod, crc, durationSeconds);
			return;
		}
		// the intimidate flytext should show up everytime it succeeds
		showFlyText("combat_effects", "go_intimidated", 0, 0xFF, 0);

		Locker locker(state);

		state->addSecondaryBuffCRC(crc);

		if (state->getTimeLeft() < durationSeconds)
			state->renew(durationSeconds);
	}
}

void CreatureObjectImplementation::setSnaredState(int durationSeconds) {
	if (!hasState(CreatureState::IMMOBILIZED)) {
		Reference<StateBuff*> state = new StateBuff(asCreatureObject(), CreatureState::IMMOBILIZED, durationSeconds);

		Locker locker(state);

		state->setStartFlyText("combat_effects", "go_snare", 0, 0xFF, 0);
		state->setEndFlyText("combat_effects", "no_snare", 0xFF, 0, 0);

		addBuff(state);
	}
}

void CreatureObjectImplementation::setRootedState(int durationSeconds) {
	if (!hasState(CreatureState::FROZEN)) {
		Reference<StateBuff*> state = new StateBuff(asCreatureObject(), CreatureState::FROZEN, durationSeconds);

		Locker locker(state);

		state->setStartFlyText("combat_effects", "go_rooted", 0, 0xFF, 0);
		state->setEndFlyText("combat_effects", "no_rooted", 0xFF, 0, 0);

		addBuff(state);
	}
}

bool CreatureObjectImplementation::setNextAttackDelay(uint32 mod, int del) {
	if (cooldownTimerMap->isPast("nextAttackDelayRecovery")) {
		//del += mod;
		cooldownTimerMap->updateToCurrentAndAddMili("nextAttackDelay", del * 1000);
		cooldownTimerMap->updateToCurrentAndAddMili("nextAttackDelayRecovery", 30000 + (del * 1000));

		showFlyText("combat_effects", "warcry_hit", 0x00, 0xFF, 0x00);

		if (isPlayerCreature()) {
			StringIdChatParameter stringId("combat_effects", "delay_applied_self");
			stringId.setDI(del);
			sendSystemMessage(stringId);
		}

		return true;
	}

	return false;
}

void CreatureObjectImplementation::setMeditateState() {
	if (isInCombat())
		return;

	setMoodString("meditating");
	setPosture(CreaturePosture::SITTING);
	setState(CreatureState::ALERT);
}

void CreatureObjectImplementation::queueDizzyFallEvent() {
	if (hasDizzyEvent())
		return;

	dizzyFallDownEvent = new DizzyFallDownEvent(asCreatureObject());
	dizzyFallDownEvent->schedule(200);
}

void CreatureObjectImplementation::activateStateRecovery() {
	//applyDots();
	if (damageOverTimeList.hasDot() && damageOverTimeList.isNextTickPast()) {
		damageOverTimeList.activateDots(asCreatureObject());
	}

	//updateStates();
}

void CreatureObjectImplementation::updateToDatabaseAllObjects(bool startTask) {
	if (!isPersistent())
		return;

	creatureBuffs.updateBuffsToDatabase();

	TangibleObjectImplementation::updateToDatabaseAllObjects(startTask);
}

void CreatureObjectImplementation::addBuff(Buff* buff) {
	if (buff == NULL)
		return;

	uint32 buffcrc = buff->getBuffCRC();

	creatureBuffs.addBuff(buff);
}

bool CreatureObjectImplementation::removeBuff(uint32 buffcrc) {
	Reference<Buff*> buff = getBuff(buffcrc);

	//BuffList::removeBuff checks to see if the buffcrc exists in the map.
	bool ret = creatureBuffs.removeBuff(buffcrc);

	if (buff != NULL) {
		Vector<unsigned long long>* secondaryCRCs = buff->getSecondaryBuffCRCs();

		for (int i = 0; i < secondaryCRCs->size(); i++) {
			removeBuff(secondaryCRCs->get(i));
		}
	}

	return ret;
}

bool CreatureObjectImplementation::removeStateBuff(uint64 state) {
	bool ret = removeBuff(Long::hashCode(state));

	assert(!hasState(state));

	return ret;
}

void CreatureObjectImplementation::removeBuff(Buff* buff) {
	if (buff == NULL)
		return;

	uint32 buffcrc = buff->getBuffCRC();

	//BuffList::removeBuff checks to see if the buffcrc exists in the map.
	creatureBuffs.removeBuff(buff);
}

void CreatureObjectImplementation::clearBuffs(bool updateclient) {
	creatureBuffs.clearBuffs(updateclient);
}

void CreatureObjectImplementation::notifyPostureChange(int newPosture) {
	if (hasState(CreatureState::ALERT)) {
		// Following fixes beings 'stuck' in meditate after a server restart, by re-registering the observers if the player has the Alert (meditating) state.
		SortedVector<ManagedReference<Observer* > > observers = getObservers(ObserverEventType::POSITIONCHANGED);

		if (observers.size() == 0) {
			PlayerManager* playermgr = server->getZoneServer()->getPlayerManager();
			registerObserver(ObserverEventType::POSTURECHANGED, playermgr);
		}
	}

	notifyObservers(ObserverEventType::POSTURECHANGED, NULL, newPosture);
}

void CreatureObjectImplementation::notifySelfPositionUpdate() {
	if (getZone() != NULL) {
		ManagedReference<PlanetManager*> planetManager =
				getZone()->getPlanetManager();

		if (planetManager != NULL) {
			TerrainManager* terrainManager = planetManager->getTerrainManager();

			if (terrainManager != NULL)
				terrainManager->notifyPositionUpdate(asCreatureObject());
		}
	}

	updateLocomotion();

	TangibleObjectImplementation::notifySelfPositionUpdate();
}

void CreatureObjectImplementation::activateHAMRegeneration(int latency) {
	if (isIncapacitated() || isDead())
		return;

	if (!isPlayerCreature() && isInCombat())
		return;

	float modifier = (float)latency/1000.f;

	if (isKneeling())
		modifier *= 1.25f;
	else if (isSitting())
		modifier *= 1.75f;

	// this formula gives the amount of regen per second
	uint32 healthTick = (uint32) ceil((float) MAX(0, getHAM(
			CreatureAttribute::CONSTITUTION)) * 13.0f / 2100.0f * modifier);
	uint32 actionTick = (uint32) ceil((float) MAX(0, getHAM(
			CreatureAttribute::STAMINA)) * 13.0f / 2100.0f * modifier);
	uint32 mindTick = (uint32) ceil((float) MAX(0, getHAM(
			CreatureAttribute::WILLPOWER)) * 13.0f / 2100.0f * modifier);

	if (healthTick < 1)
		healthTick = 1;

	if (actionTick < 1)
		actionTick = 1;

	if (mindTick < 1)
		mindTick = 1;

	healDamage(asCreatureObject(), CreatureAttribute::HEALTH, healthTick, true, false);
	healDamage(asCreatureObject(), CreatureAttribute::ACTION, actionTick, true, false);
	healDamage(asCreatureObject(), CreatureAttribute::MIND, mindTick, true, false);

	activatePassiveWoundRegeneration();
}

void CreatureObjectImplementation::activatePassiveWoundRegeneration() {
	/// Health wound regen
	int healthRegen = getSkillMod("private_med_wound_health");

	if(healthRegen > 0) {
		healthWoundHeal += (int)(healthRegen * 0.2);
		if(healthWoundHeal >= 100) {
			healWound(asCreatureObject(), CreatureAttribute::HEALTH, 1, true, false);
			healWound(asCreatureObject(), CreatureAttribute::STRENGTH, 1, true, false);
			healWound(asCreatureObject(), CreatureAttribute::CONSTITUTION, 1, true, false);
			healthWoundHeal -= 100;
		}
	}

	/// Action wound regen
	int actionRegen = getSkillMod("private_med_wound_action");

	if(actionRegen > 0) {
		actionWoundHeal += (int)(actionRegen * 0.2);
		if(actionWoundHeal >= 100) {
			healWound(asCreatureObject(), CreatureAttribute::ACTION, 1, true, false);
			healWound(asCreatureObject(), CreatureAttribute::QUICKNESS, 1, true, false);
			healWound(asCreatureObject(), CreatureAttribute::STAMINA, 1, true, false);
			actionWoundHeal -= 100;
		}
	}

	/// Mind wound regen
	int mindRegen = getSkillMod("private_med_wound_mind");

	if(mindRegen > 0) {
		mindWoundHeal += (int)(mindRegen * 0.2);
		if(mindWoundHeal >= 100) {
			healWound(asCreatureObject(), CreatureAttribute::MIND, 1, true, false);
			healWound(asCreatureObject(), CreatureAttribute::FOCUS, 1, true, false);
			healWound(asCreatureObject(), CreatureAttribute::WILLPOWER, 1, true, false);
			mindWoundHeal -= 100;
		}
	}
}

bool CreatureObjectImplementation::isResuscitable() {
	return (isDead() && (timeOfDeath.miliDifference()) < DEAD_TOO_LONG);
}

bool CreatureObjectImplementation::isDancing() {
	ManagedReference<Facade*> facade = this->getActiveSession(
			SessionFacadeType::ENTERTAINING);

	if (facade == NULL)
		return false;

	EntertainingSession* session =
			dynamic_cast<EntertainingSession*> (facade.get());

	if (session == NULL)
		return false;

	return session->isDancing();
}

bool CreatureObjectImplementation::isPlayingMusic() {
	ManagedReference<Facade*> facade = this->getActiveSession(
			SessionFacadeType::ENTERTAINING);

	if (facade == NULL)
		return false;

	EntertainingSession* session =
			dynamic_cast<EntertainingSession*> (facade.get());

	if (session == NULL)
		return false;

	return session->isPlayingMusic();
}

void CreatureObjectImplementation::stopEntertaining() {
	ManagedReference<Facade*> facade = this->getActiveSession(
			SessionFacadeType::ENTERTAINING);

	if (facade == NULL)
		return;

	EntertainingSession* session =
			dynamic_cast<EntertainingSession*> (facade.get());

	if (session == NULL)
		return;

	session->cancelSession();
}

void CreatureObjectImplementation::sendMessage(BasePacket* msg) {
	ManagedReference<ZoneClientSession*> ownerClient = owner.get();

	if (ownerClient == NULL) {
		delete msg;
		return;
	} else {
		ownerClient->sendMessage(msg);
	}
}

void CreatureObjectImplementation::sendStateCombatSpam(const String& fileName, const String& stringName, byte color, int damage, bool broadcast) {
	Zone* zone = getZone();
	if (zone == NULL)
		return;

	if (isDead()) //We don't need to know when a corpse can see clearly again!
		return;

	ManagedReference<CreatureObject*> creature = asCreatureObject();

	if (broadcast) { //Send spam to all nearby players.
		CombatManager::instance()->broadcastCombatSpam(creature, NULL, NULL, 0, fileName, stringName, color);

	} else { //Send spam only to originating player.
		if (!creature->isPlayerCreature())
			return;

		CombatSpam* spam = new CombatSpam(creature, NULL, creature, NULL, damage, fileName, stringName, color);
		creature->sendMessage(spam);
	}
}

void CreatureObjectImplementation::sendCustomCombatSpam(const UnicodeString& customString, byte color) {
	if (!this->isPlayerCreature())
			return;
	CombatSpam* spam = new CombatSpam(asCreatureObject(), customString, color);
	sendMessage(spam);
}

String CreatureObjectImplementation::getFirstName() {
	UnicodeString fullName = getCustomObjectName();

	int idx = fullName.indexOf(' ');

	if (idx != -1) {
		return fullName.subString(0, idx).toString();
	} else {
		return fullName.toString();
	}
}

String CreatureObjectImplementation::getLastName() {
	UnicodeString lastName;

	UnicodeString fullName = getCustomObjectName();
	UnicodeTokenizer tokenizer(fullName);

	if (tokenizer.hasMoreTokens())
		tokenizer.shiftTokens(1);

	if (tokenizer.hasMoreTokens())
		tokenizer.getUnicodeToken(lastName);

	return lastName.toString();
}

void CreatureObjectImplementation::sendExecuteConsoleCommand(
		const String& command) {
	BaseMessage* msg = new ExecuteConsoleCommand(command);
	sendMessage(msg);
}

Reference<PlayerObject*> CreatureObjectImplementation::getPlayerObject() {
	return getSlottedObject("ghost").castTo<PlayerObject*> ();
}

bool CreatureObjectImplementation::isAggressiveTo(CreatureObject* object) {
	PlayerObject* ghost = getPlayerObject();
	PlayerObject* targetGhost = object->getPlayerObject();

	if (ghost == NULL || targetGhost == NULL)
		return false;

	if (ghost->isOnLoadScreen())
		return false;

	if (CombatManager::instance()->areInDuel(object, asCreatureObject()))
		return true;

	if ((pvpStatusBitmask & CreatureFlag::OVERT) && (object->getPvpStatusBitmask() & CreatureFlag::OVERT) && object->getFaction() != getFaction())
		return true;

	if (ghost->isInBountyLockList(object->getObjectID()) || targetGhost->isInBountyLockList(asCreatureObject()->getObjectID())) {
		return true;
	}

	ManagedReference<GuildObject*> guildObject = guild.get();
	if (guildObject != NULL && guildObject->isInWaringGuild(object))
		return true;

	return false;
}

bool CreatureObjectImplementation::isAttackableBy(TangibleObject* object) {
	if(object->isCreatureObject())
		return isAttackableBy(object->asCreatureObject());

	// TODO (dannuic): this will prevent TANOs from attacking mobs (turrets, minefields, etc)
	if(this->isAiAgent()) {
		return false;
	}

	PlayerObject* ghost = getPlayerObject();

	if(ghost == NULL)
		return false;

	if (ghost->isOnLoadScreen())
		return false;

	if (isDead() || isIncapacitated() || isInvisible())
		return false;

	if (getPvpStatusBitmask() == CreatureFlag::NONE)
		return false;

	if(object->getFaction() == 0 )
		return true;

	if(object->getFaction() == getFaction())
		return false;

	// if player is on leave, then faction object cannot attack it
	if (ghost->getFactionStatus() == FactionStatus::ONLEAVE || getFaction() == 0)
		return false;

	// if tano is overt, creature must be overt
	if((object->getPvpStatusBitmask() & CreatureFlag::OVERT) && !(getPvpStatusBitmask() & CreatureFlag::OVERT))
		return false;

	// the other options are overt creature / overt tano  and covert/covert, covert tano, overt creature..  all are attackable
	return true;

}

bool CreatureObjectImplementation::isAttackableBy(CreatureObject* object) {
	return isAttackableBy(object, false);
}

bool CreatureObjectImplementation::isAttackableBy(CreatureObject* object, bool bypassDeadCheck) {
	if (object == asCreatureObject())
		return false;

	if ((!bypassDeadCheck && isDead()) || isInvisible())
		return false;

	if (object->getZone() != getZone())
		return false;

	if (isPlayerCreature()) {
		PlayerObject* ghost = getPlayerObject();

		if (ghost != NULL && ghost->isOnLoadScreen()) {
			return false;
		}
	}

	if (object->isAiAgent()) {

		if (object->isPet()) {
			ManagedReference<PetControlDevice*> pcd = object->getControlDevice().get().castTo<PetControlDevice*>();
			if (pcd != NULL && pcd->getPetType() == PetManager::FACTIONPET && isNeutral()) {
				return false;
			}

			ManagedReference<CreatureObject*> owner = object->getLinkedCreature().get();

			if (owner == NULL)
				return false;

			return isAttackableBy(owner);
		}

		if(!object->isRebel() && !object->isImperial())
			return true;

		if(getFaction() == 0 || getFaction() == object->getFaction())
			return false;
		else if (isPlayerCreature()) {

			if(getPlayerObject() == NULL || getPlayerObject()->getFactionStatus() == FactionStatus::ONLEAVE)
				return false;
		}

		return true;
	}

	PlayerObject* ghost = getPlayerObject();
	PlayerObject* targetGhost = object->getPlayerObject();

	if (ghost == NULL || targetGhost == NULL)
		return false;

	bool areInDuel = (ghost->requestedDuelTo(object) && targetGhost->requestedDuelTo(asCreatureObject()));

	if (areInDuel)
		return true;

	if (getGroupID() != 0 && getGroupID() == object->getGroupID())
		return false;

	if ((pvpStatusBitmask & CreatureFlag::OVERT) && (object->getPvpStatusBitmask() & CreatureFlag::OVERT) && object->getFaction() != getFaction())
		return true;

	if (ghost->isInBountyLockList(object->getObjectID()) || targetGhost->isInBountyLockList(asCreatureObject()->getObjectID())) {
		return true;
	}

	if (isInBountyMission(object, asCreatureObject())) {
		return true;
	}

	ManagedReference<GuildObject*> guildObject = guild.get();
	if (guildObject != NULL && guildObject->isInWaringGuild(object))
		return true;

	return false;
}

bool CreatureObjectImplementation::isHealableBy(CreatureObject* object) {
	if (object == asCreatureObject())
		return true;

	if (isInvisible())
		return false;

	if (isAttackableBy(object))
		return false;

	PlayerObject* ghost = object->getPlayerObject(); // ghost is the healer
	PlayerObject* targetGhost = getPlayerObject();

	if (ghost == NULL)
		return false;

	if (ghost->isBountyLocked())
		return false;

	//if ((pvpStatusBitmask & CreatureFlag::OVERT) && (object->getPvpStatusBitmask() & CreatureFlag::OVERT) && object->getFaction() != getFaction())

	if (targetGhost == NULL)
		return true;

	uint32 targetFactionStatus = targetGhost->getFactionStatus();
	uint32 currentFactionStatus = ghost->getFactionStatus();

	if (getFaction() != object->getFaction() && !(targetFactionStatus == FactionStatus::ONLEAVE))
		return false;

	if ((targetFactionStatus == FactionStatus::OVERT) && !(currentFactionStatus == FactionStatus::OVERT))
		return false;

	if (!(targetFactionStatus == FactionStatus::ONLEAVE) && (currentFactionStatus == FactionStatus::ONLEAVE))
		return false;

	return true;
}

bool CreatureObjectImplementation::isInBountyMission(CreatureObject* bountyHunter, CreatureObject* target) {
	if (bountyHunter == NULL) {
		return false;
	}

	ZoneServer* zoneServer = bountyHunter->getZoneServer();

	if (zoneServer == NULL) {
		return false;
	}

	MissionManager* missionManager = zoneServer->getMissionManager();

	if (missionManager == NULL) {
		return false;
	}


	ManagedReference<MissionObject*> mission = missionManager->getBountyHunterMission(bountyHunter);

	if (mission == NULL || target == NULL) {
		return false;
	}

	return mission->getTargetObjectId() == target->getObjectID();
}

int CreatureObjectImplementation::notifyObjectDestructionObservers(TangibleObject* attacker, int condition) {
	PlayerObject* ghost = getPlayerObject();

	if (ghost != NULL) {
		PlayerManager* playerManager = getZoneServer()->getPlayerManager();

		playerManager->notifyDestruction(attacker, asCreatureObject(), condition);
	}

	if (attacker->isAiAgent()) {
		AiAgent* aiAgent = attacker->asAiAgent();
		aiAgent->sendReactionChat(ReactionManager::GLOAT);
	}

	return TangibleObjectImplementation::notifyObjectDestructionObservers(attacker, condition);
}

int CreatureObjectImplementation::notifyObjectKillObservers(TangibleObject* killer) {
	notifyObservers(ObserverEventType::PLAYERKILLED, killer, 0);

	return 0;
}

void CreatureObjectImplementation::createChildObjects() {
	ZoneServer* zoneServer = server->getZoneServer();

	for (int i = 0; i < templateObject->getChildObjectsSize(); ++i) {
		ChildObject* child = templateObject->getChildObject(i);

		if (child == NULL)
			continue;

		ManagedReference<SceneObject*> obj = zoneServer->createObject(
				child->getTemplateFile().hashCode(), getPersistenceLevel());

		if (obj == NULL)
			continue;

		ContainerPermissions* permissions = obj->getContainerPermissions();
		permissions->setOwner(getObjectID());
		permissions->setInheritPermissionsFromParent(false);
		permissions->setDefaultDenyPermission(ContainerPermissions::MOVECONTAINER);
		permissions->setDenyPermission("owner", ContainerPermissions::MOVECONTAINER);

		if (!transferObject(obj, child->getContainmentType())) {
			obj->destroyObjectFromDatabase(true);
			continue;
		}

		childObjects.put(obj);

		obj->initializeChildObject(asCreatureObject());
	}
}

void CreatureObjectImplementation::addWearableObject(TangibleObject* object, bool notifyClient) {
	if (wearablesVector.contains(object))
		return;

	if (notifyClient) {
		CreatureObjectDeltaMessage6* msg = new CreatureObjectDeltaMessage6(
				asCreatureObject());
		msg->startUpdate(0x0F);
		wearablesVector.add(object, msg);
		msg->close();

		broadcastMessage(msg, true);
	} else {
		wearablesVector.add(object);
	}
}

void CreatureObjectImplementation::removeWearableObject(TangibleObject* object, bool notifyClient) {
	int index = wearablesVector.find(object);

	if (index == -1)
		return;

	if (notifyClient) {
		CreatureObjectDeltaMessage6* msg = new CreatureObjectDeltaMessage6(
				asCreatureObject());
		msg->startUpdate(0x0F);
		wearablesVector.remove(index, msg);
		msg->close();

		broadcastMessage(msg, true);
	} else {
		wearablesVector.remove(index);
	}
}

CampSiteActiveArea* CreatureObjectImplementation::getCurrentCamp() {

	for(int i = 0; i < activeAreas.size(); ++i) {
		if(activeAreas.get(i)->isCampArea())
			return cast<CampSiteActiveArea*>(activeAreas.get(i).get());
	}
	return NULL;
}

int CreatureObjectImplementation::handleObjectMenuSelect(CreatureObject* player, byte selectedID) {
	if (isDead() && !isPet()) {
		switch (selectedID) {
		case 35:
			player->executeObjectControllerAction(STRING_HASHCODE("loot"), getObjectID(), "");

			return 0;
		case 36:
			player->executeObjectControllerAction(STRING_HASHCODE("loot"), getObjectID(), "all");

			return 0;
		}
	}

	return TangibleObjectImplementation::handleObjectMenuSelect(player, selectedID);
}

float CreatureObjectImplementation::calculateCostAdjustment(uint8 stat, float baseCost) {
	float cost = baseCost - (((float)(this->getHAM(stat) - 300) / 1200.f) * baseCost);

	if(cost < 0)
		cost = 0;

	return cost;
}

Reference<WeaponObject*> CreatureObjectImplementation::getWeapon() {
	if (weapon == NULL) {
		return TangibleObjectImplementation::getSlottedObject("default_weapon").castTo<WeaponObject*>();
	} else
		return weapon;
}

void CreatureObjectImplementation::setFaction(unsigned int crc) {
	faction = crc;

	if (isPlayerCreature()) {
		Reference<CreatureObject*> player = asCreatureObject();

		if (player == NULL)
			return;

		Zone* currentZone = getZone();

		if (currentZone != NULL) {
			// Notify nearby active areas of faction change
			SortedVector<ManagedReference<ActiveArea* > > activeAreas;
			currentZone->getInRangeActiveAreas(player->getPositionX(), player->getPositionY(), &activeAreas, true);

			for (int i = 0; i < activeAreas.size(); i++) {
				ActiveArea* area = activeAreas.get(i);

				if (area != NULL)
					area->notifyEnter(asCreatureObject());
			}
		}

		PlayerObject* ghost = player->getPlayerObject();

		if (ghost == NULL)
			return;

		Vector<ManagedReference<CreatureObject*> > petsToStore;

		for (int i = 0; i < ghost->getActivePetsSize(); i++) {
			ManagedReference<AiAgent*> pet = ghost->getActivePet(i);

			if (pet == NULL)
				continue;

			CreatureTemplate* creatureTemplate = pet->getCreatureTemplate();

			if (creatureTemplate != NULL) {
				String templateFaction = creatureTemplate->getFaction();

				if (!templateFaction.isEmpty() && (templateFaction.hashCode() != crc)) {
					petsToStore.add(pet.castTo<CreatureObject*>());
					player->sendSystemMessage("You're no longer the right faction for one of your pets, storing...");
					continue;
				}
			}

			Locker crossLocker(pet, player);

			pet->setFaction(crc);
		}

		StoreSpawnedChildrenTask* task = new StoreSpawnedChildrenTask(player, petsToStore);
		task->execute();
	}
}

void CreatureObjectImplementation::destroyPlayerCreatureFromDatabase(bool destroyContainedObjects) {
	if (!isPlayerCreature())
		return;

	clearBuffs(false);

	if(dataObjectComponent != NULL) {
		dataObjectComponent->notifyObjectDestroyingFromDatabase();
	}

	ZoneServer* server = getZoneServer();

	server->destroyObjectFromDatabase(getObjectID());

	asCreatureObject()->setPersistent(0);

	if (!destroyContainedObjects)
		return;

	SortedVector<ManagedReference<SceneObject*> > destroyedObjects;
	destroyedObjects.setNoDuplicateInsertPlan();

	for (int i = 0; i < getSlottedObjectsSize(); ++i) {
		ManagedReference<SceneObject*> object = getSlottedObject(i);

		if (destroyedObjects.put(object) != -1) {
			Locker locker(object);
			object->destroyObjectFromDatabase(true);
		}
	}

	for (int j = 0; j < getContainerObjectsSize(); ++j) {
		ManagedReference<SceneObject*> object = getContainerObject(j);

		if (destroyedObjects.put(object) != -1) {
			Locker locker(object);
			object->destroyObjectFromDatabase(true);
		}
	}

	//Remove all child objects from database
	for (int i = 0; i < childObjects.size(); ++i) {
		ManagedReference<SceneObject*> child = childObjects.get(i);

		if (child == NULL)
			continue;

		Locker locker(child);

		child->destroyObjectFromDatabase(true);
	}

	GuildManager* guildManager = getZoneServer()->getGuildManager();

	uint64 oid = getObjectID();
	if (guildManager->isSponsoredPlayer(oid)) {
		guildManager->removeSponsoredPlayer(oid);
	}

	if (guildManager->isCreatingGuild(oid)) {
		guildManager->removePendingGuild(oid);
	}

	if (isInGuild()) {
		ManagedReference<GuildObject*> guild = getGuildObject().get();

		Locker clocker(guild, asCreatureObject());

		guild->removeMember(oid);
	}
}

float CreatureObjectImplementation::getTemplateRadius() {
	SharedCreatureObjectTemplate* creoTempl = templateObject.castTo<SharedCreatureObjectTemplate*>();

	if (creoTempl == NULL)
		return 0;

	return creoTempl->getCollisionRadius()*getHeight();
}

bool CreatureObjectImplementation::hasEffectImmunity(uint8 effectType) {
	switch (effectType) {
	case CommandEffect::BLIND:
	case CommandEffect::DIZZY:
	case CommandEffect::INTIMIDATE:
	case CommandEffect::STUN:
	case CommandEffect::NEXTATTACKDELAY:
		if (isDroidSpecies() || isVehicleObject() || isWalkerSpecies())
			return true;
		break;
	case CommandEffect::KNOCKDOWN:
	case CommandEffect::POSTUREUP:
	case CommandEffect::POSTUREDOWN:
		if (isVehicleObject() || isWalkerSpecies())
			return true;
		break;
	default:
		return false;
	}

	return false;
}

bool CreatureObjectImplementation::hasDotImmunity(uint32 dotType) {
	switch (dotType) {
	case CreatureState::POISONED:
	case CreatureState::BLEEDING:
	case CreatureState::DISEASED:
		if (isDroidSpecies() || isVehicleObject())
			return true;
		break;
	case CreatureState::ONFIRE:
		return false;
	default:
		return false;
	}

	return false;
}

int CreatureObjectImplementation::getSpecies() {
	SharedCreatureObjectTemplate* creoData = templateObject.castTo<SharedCreatureObjectTemplate*>().get();

	if (creoData == NULL)
		return -1;

	return creoData->getSpecies();
}

int CreatureObjectImplementation::getGender() {
	SharedCreatureObjectTemplate* creoData = templateObject.castTo<SharedCreatureObjectTemplate*>().get();

	if (creoData == NULL)
		return -1;

	return creoData->getGender();
}

void CreatureObjectImplementation::updateVehiclePosition(bool sendPackets) {
	ManagedReference<SceneObject*> parent = getParent().get();

	if (parent == NULL || (!parent->isVehicleObject() && !parent->isMount()))
		return;

	CreatureObject* creo = cast<CreatureObject*>(parent.get());

	if (creo != NULL) {
		Locker clocker(creo, asCreatureObject());
		creo->setCurrentSpeed(getCurrentSpeed());
	}

	TangibleObjectImplementation::updateVehiclePosition(sendPackets);
}

CreatureObject* CreatureObjectImplementation::asCreatureObject() {
	return _this.getReferenceUnsafeStaticCast();
}

CreatureObject* CreatureObject::asCreatureObject() {
	return this;
}

bool CreatureObjectImplementation::isPlayerCreature() {
	if (templateObject == NULL)
		return false;

	return templateObject->isPlayerCreatureTemplate();
}
