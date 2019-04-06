#include "server/zone/objects/intangible/PetControlDevice.h"
#include "server/zone/objects/intangible/PetControlObserver.h"
#include "server/zone/objects/intangible/tasks/EnqueuePetCommand.h"
#include "server/zone/managers/creature/PetManager.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/creature/ai/AiAgent.h"
#include "server/zone/objects/creature/ai/Creature.h"
#include "server/zone/objects/creature/ai/DroidObject.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/objects/player/sui/listbox/SuiListBox.h"
#include "server/zone/objects/player/sui/messagebox/SuiMessageBox.h"
#include "server/zone/objects/player/sui/callbacks/MountGrowthArrestSuiCallback.h"
#include "server/zone/objects/player/sui/callbacks/PetFixSuiCallback.h"
#include "server/zone/ZoneServer.h"
#include "server/zone/Zone.h"
#include "tasks/CallPetTask.h"
#include "server/zone/objects/region/CityRegion.h"
#include "server/zone/objects/player/sessions/TradeSession.h"
#include "server/zone/managers/player/PlayerManager.h"
#include "server/zone/objects/creature/events/DroidSkillModTask.h"
#include "server/zone/objects/creature/events/DroidPowerTask.h"
#include "server/zone/objects/tangible/weapon/WeaponObject.h"
#include "server/zone/objects/building/BuildingObject.h"
#include "server/zone/managers/stringid/StringIdManager.h"
#include "tasks/StorePetTask.h"
#include "server/chat/ChatManager.h"
#include "server/zone/objects/player/FactionStatus.h"
#include "server/zone/managers/frs/FrsManager.h"

void PetControlDeviceImplementation::callObject(CreatureObject* player) {
	if (player->isInCombat() || player->isDead() || player->isIncapacitated() || player->getPendingTask("tame_pet") != NULL) {
		player->sendSystemMessage("@pet/pet_menu:cant_call"); // You cannot call this pet right now.
		return;
	}

	if (player->isRidingMount()) {
		player->sendSystemMessage("@pet/pet_menu:mounted_call_warning"); // You cannot call a pet while mounted or riding a vehicle.
		return;
	}

	if (player->getParent() != NULL) {
		ManagedReference<SceneObject*> strongRef = player->getRootParent();
		ManagedReference<BuildingObject*> building = NULL;

		if (strongRef != NULL)
			building = strongRef.castTo<BuildingObject*>();

		if (building == NULL || building->isPrivateStructure()) {
			player->sendSystemMessage("@pet/pet_menu:private_house"); // You cannot call pets in a private building.
			return;
		}
	}

	if (!isASubChildOf(player))
		return;

	ManagedReference<TangibleObject*> controlledObject = this->controlledObject.get();

	if (controlledObject == NULL || !controlledObject->isAiAgent())
		return;

	ManagedReference<AiAgent*> pet = cast<AiAgent*>(controlledObject.get());
	ManagedReference<PlayerObject*> ghost = player->getPlayerObject();

	if (ghost->hasActivePet(pet))
		return;

	FrsManager* frsManager = server->getZoneServer()->getFrsManager();

	if (frsManager->isFrsEnabled() && frsManager->isPlayerInEnclave(player)) {
		player->sendSystemMessage("@pet/pet_menu:cant_call"); //  You cannot call this pet right now.
		return;
	}

	if (vitality <= 0) {
		player->sendSystemMessage("@pet/pet_menu:dead_pet"); // This pet is dead. Select DESTROY from the radial menu to delete this pet control device.
		return;
	}

	if (!pet->getCooldownTimerMap()->isPast("call_cooldown")) {
		if (petType == PetManager::DROIDPET)
			player->sendSystemMessage("@pet/droid_modules:droid_maint_on_maint_run"); //You cannot call that droid. It is currently on a maintenance run.
		else
			player->sendSystemMessage("@pet/pet_menu:cant_call"); // cant call pet right now
		return;
	}
	assert(pet->isLockedByCurrentThread());

	unsigned int petFaction = pet->getFaction();

	if (petFaction != 0) {
		if (player->getFaction() == 0) {
			StringIdChatParameter message("@faction_perk:prose_be_declared"); // You must be declared to a faction to use %TT.
			message.setTT(pet->getDisplayedName());
			player->sendSystemMessage(message);
			return;
		}

		if (player->getFaction() != petFaction || player->getFactionStatus() == FactionStatus::ONLEAVE) {
			StringIdChatParameter message("@faction_perk:prose_be_declared_faction"); // You must be a declared %TO to use %TT.
			message.setTO(pet->getFactionString());
			message.setTT(pet->getDisplayedName());
			player->sendSystemMessage(message);
			return;
		}
	}

	if(player->getPendingTask("call_pet") != NULL) {
		StringIdChatParameter waitTime("pet/pet_menu", "call_delay_finish_pet"); // Already calling a Pet: Call will be finished in %DI seconds.
		AtomicTime nextExecution;
		Core::getTaskManager()->getNextExecutionTime(player->getPendingTask("call_pet"), nextExecution);
		int timeLeft = (nextExecution.getMiliTime() / 1000) - System::getTime();
		waitTime.setDI(timeLeft);

		player->sendSystemMessage(waitTime);
		return;
	}

	if (!growPet(player))
		return;

	if (petType == PetManager::CREATUREPET && !isValidPet(pet)) {
		ManagedReference<SuiMessageBox*> box = new SuiMessageBox(player,SuiWindowType::PET_FIX_DIALOG);
		box->setCallback(new PetFixSuiCallback(player->getZoneServer(), _this.getReferenceUnsafeStaticCast()));
		box->setPromptText("@bio_engineer:pet_sui_text");
		box->setPromptTitle("@bio_engineer:pet_sui_title");
		box->setOkButton(true,"@bio_engineer:pet_sui_fix_stats");
		box->setCancelButton(true,"@bio_engineer:pet_sui_abort");
		box->setOtherButton(true,"@bio_engineer:pet_sui_fix_level");
		box->setUsingObject(_this.getReferenceUnsafeStaticCast());
		player->getPlayerObject()->addSuiBox(box);
		player->sendMessage(box->generateMessage());
		return;
	}

	int currentlySpawned = 0;
	int spawnedLevel = 0;
	int maxPets = 1;
	int maxLevelofPets = 10;
	int level = pet->getLevel();

	if (pet->getCreatureTemplate() == NULL) {
		player->sendSystemMessage("Invalid creature to spawn!"); // Old npc without a npc template?
		return;
	}

	if (petType == PetManager::CREATUREPET) {
		ManagedReference<Creature*> creaturePet = cast<Creature*>(pet.get());
		if (creaturePet == NULL)
			return;

		bool ch = player->hasSkill("outdoors_creaturehandler_novice");

		if (ch) {
			maxPets = player->getSkillMod("keep_creature");
			maxLevelofPets = player->getSkillMod("tame_level");
		}

		if (creaturePet->getAdultLevel() > maxLevelofPets) {
			player->sendSystemMessage("@pet/pet_menu:control_exceeded"); // Calling this pet would exceed your Control Level ability.
			return;
		}

		if (creaturePet->isVicious() && (player->getSkillMod("tame_aggro") <= 0 || !ch)) {
			player->sendSystemMessage("@pet/pet_menu:lack_skill"); // You lack the skill to call a pet of this type.
			return;
		}

	} else if (petType == PetManager::FACTIONPET){
		maxPets = 3;
	}

	for (int i = 0; i < ghost->getActivePetsSize(); ++i) {
		ManagedReference<AiAgent*> object = ghost->getActivePet(i);

		if (object != NULL) {
			if (object->isCreature() && petType == PetManager::CREATUREPET) {
				CreatureTemplate* activePetTemplate = object->getCreatureTemplate();

				if (activePetTemplate == NULL || activePetTemplate->getTemplateName() == "at_st")
					continue;

				if (++currentlySpawned >= maxPets) {
					player->sendSystemMessage("@pet/pet_menu:at_max"); // You already have the maximum number of pets of this type that you can call.
					return;
				}

				spawnedLevel += object->getLevel();

				if ((spawnedLevel + level) > maxLevelofPets) {
					player->sendSystemMessage("@pet/pet_menu:control_exceeded"); // Calling this pet would exceed your Control Level ability.
					return;
				}
			} else if (object->isNonPlayerCreatureObject() && petType == PetManager::FACTIONPET) {
				if (++currentlySpawned >= maxPets) {
					player->sendSystemMessage("@pet/pet_menu:at_max"); // You already have the maximum number of pets of this type that you can call.
					return;
				}
			} else if (object->isCreature() && petType == PetManager::FACTIONPET) {
				CreatureTemplate* activePetTemplate = object->getCreatureTemplate();
				CreatureTemplate* callingPetTemplate = pet->getCreatureTemplate();

				if (activePetTemplate == NULL || callingPetTemplate == NULL || activePetTemplate->getTemplateName() != "at_st")
					continue;

				if (++currentlySpawned >= maxPets || (activePetTemplate->getTemplateName() == "at_st" && callingPetTemplate->getTemplateName() == "at_st")) {
					player->sendSystemMessage("@pet/pet_menu:at_max"); // You already have the maximum number of pets of this type that you can call.
					return;
				}
			} else if (object->isDroidObject() && petType == PetManager::DROIDPET) {
				if (++currentlySpawned >= maxPets) {
					player->sendSystemMessage("@pet/pet_menu:at_max"); // You already have the maximum number of pets of this type that you can call.
					return;
				}
			}

		}
	}

	ManagedReference<TradeSession*> tradeContainer = player->getActiveSession(SessionFacadeType::TRADE).castTo<TradeSession*>();

	if (tradeContainer != NULL) {
		server->getZoneServer()->getPlayerManager()->handleAbortTradeMessage(player);
	}

	if (player->getCurrentCamp() == NULL && player->getCityRegion() == NULL) {

		Reference<CallPetTask*> callPet = new CallPetTask(_this.getReferenceUnsafeStaticCast(), player, "call_pet");

		StringIdChatParameter message("pet/pet_menu", "call_pet_delay"); // Calling pet in %DI seconds. Combat will terminate pet call.
		message.setDI(15);
		player->sendSystemMessage(message);

		player->addPendingTask("call_pet", callPet, 15 * 1000);

		if (petControlObserver == NULL) {
			petControlObserver = new PetControlObserver(_this.getReferenceUnsafeStaticCast());
			petControlObserver->deploy();
		}

		player->registerObserver(ObserverEventType::STARTCOMBAT, petControlObserver);

	} else { // Player is in a city or camp, spawn pet immediately

		if( player->getCooldownTimerMap() == NULL )
			return;

		// Check cooldown
		if( !player->getCooldownTimerMap()->isPast("petCallOrStoreCooldown") ){
			player->sendSystemMessage("@pet/pet_menu:cant_call_1sec"); //"You cannot CALL for 1 second."
			return;
		}

		spawnObject(player);

		// Set cooldown
		player->getCooldownTimerMap()->updateToCurrentAndAddMili("petCallOrStoreCooldown", 1000); // 1 sec
	}

	EnqueuePetCommand* enqueueCommand = new EnqueuePetCommand(pet, String("petFollow").toLowerCase().hashCode(), String::valueOf(player->getObjectID()), player->getObjectID(), 1);
	enqueueCommand->execute();
}

int PetControlDeviceImplementation::handleObjectMenuSelect(CreatureObject* player, byte selectedID) {
	//Logger::info("selected call");

	Reference<TangibleObject*> strongRef = controlledObject.get();

	if (strongRef == NULL)
		return 1;

	AiAgent* pet = cast<AiAgent*>(strongRef.get());

	Reference<PlayerObject*> ghost = player->getPlayerObject();

	if (selectedID == 44) {
		if (pet == NULL) {
			error("null controlled object in pet control device");
			return 1;
		} else {
			Reference<AiAgent*> petReference = pet;
			Reference<CreatureObject*> playerReference = player;
			Reference<PetControlDevice*> thisReference = _this.getReferenceUnsafeStaticCast();

			Core::getTaskManager()->executeTask([thisReference, petReference, playerReference] () {
				Locker locker(playerReference);

				Locker crossLocker(petReference, playerReference);

				Locker controlLocker(thisReference);

				thisReference->callObject(playerReference);
			}, "ControlDeviceCallLambda");
		}
	} else if (selectedID == 59) {
		if (pet == NULL) {
			error("null controlled object in pet control device");
			return 1;
		} else {
			if (status == 1 && !ghost->hasActivePet(pet)) {
				Reference<AiAgent*> petReference = pet;
				Reference<CreatureObject*> playerReference = player;
				Reference<PetControlDevice*> thisReference = _this.getReferenceUnsafeStaticCast();

				Core::getTaskManager()->executeTask([thisReference, petReference, playerReference] () {
					Locker locker(playerReference);

					Locker crossLocker(petReference, playerReference);

					Locker controlLocker(thisReference);

					thisReference->callObject(playerReference);
				}, "ControlDeviceCallLambda2");
			} else {
				Reference<AiAgent*> petReference = pet;
				Reference<CreatureObject*> playerReference = player;
				Reference<PetControlDevice*> thisReference = _this.getReferenceUnsafeStaticCast();

				Core::getTaskManager()->executeTask([thisReference, petReference, playerReference] () {
					Locker locker(playerReference);

					Locker crossLocker(petReference, playerReference);

					Locker controlLocker(thisReference);

					thisReference->storeObject(playerReference);
				}, "ControlDeviceStoreLambda");
			}
		}
	}

	return 0;
}

void PetControlDeviceImplementation::spawnObject(CreatureObject* player) {
	ZoneServer* zoneServer = getZoneServer();

	ManagedReference<TangibleObject*> controlledObject = this->controlledObject.get();

	if (controlledObject == NULL)
		return;

	assert(controlledObject->isLockedByCurrentThread());

	if (!isASubChildOf(player))
		return;

	ManagedReference<TradeSession*> tradeContainer = player->getActiveSession(SessionFacadeType::TRADE).castTo<TradeSession*>();

	if (tradeContainer != NULL) {
		server->getZoneServer()->getPlayerManager()->handleAbortTradeMessage(player);
	}

	controlledObject->initializePosition(player->getPositionX(), player->getPositionZ(), player->getPositionY());
	ManagedReference<CreatureObject*> creature = NULL;

	if (controlledObject->isCreatureObject()) {
		creature = cast<CreatureObject*>(controlledObject.get());
		creature->setCreatureLink(player);
		creature->setControlDevice(_this.getReferenceUnsafeStaticCast());
		creature->setFaction(player->getFaction());
		creature->setObjectMenuComponent("PetMenuComponent");

		if (creature->getHueValue() >= 0)
			creature->setHue(creature->getHueValue());

		if (player->getPvpStatusBitmask() & CreatureFlag::PLAYER)
			creature->setPvpStatusBitmask(player->getPvpStatusBitmask() - CreatureFlag::PLAYER, true);
		else
			creature->setPvpStatusBitmask(player->getPvpStatusBitmask(), true);

		if (trainedAsMount && (creature->getOptionsBitmask() ^ 0x1000)) {
			creature->setOptionBit(0x1000);
		}
	}

	Zone* zone = player->getZone();

	if (zone == NULL)
		return;

	ManagedReference<CellObject*> parent = player->getParent().get().castTo<CellObject*>();

	if (parent != NULL)
		parent->transferObject(controlledObject, -1, true);
	else
		zone->transferObject(controlledObject, -1, true);

	updateStatus(1);

	if (petControlObserver != NULL)
		player->dropObserver(ObserverEventType::STARTCOMBAT, petControlObserver);

	AiAgent* pet = cast<AiAgent*>(creature.get());

	if (pet == NULL)
		return;

	ManagedReference<PlayerObject*> ghost = player->getPlayerObject();
	ghost->addToActivePets(pet);

	bool isDroid = false;
	if (pet->isDroidObject()) {
		DroidObject* droid = cast<DroidObject*>(pet);
		isDroid = true;
		if( droid == NULL )
			return;

		// Sanity check that there aren't outstanding power/skill mod tasks
		droid->removePendingTask( "droid_power" );
		droid->removePendingTask( "droid_skill_mod" );
		droid->initDroidModules();
		droid->onCall();
		droid->loadSkillMods(player);
		// Submit new power task
		Reference<Task*> droidPowerTask = new DroidPowerTask( droid );
		droid->addPendingTask("droid_power", droidPowerTask, 120000); // 2 min
		// Submit new skill mod task
		Reference<Task*> droidSkillModTask = new DroidSkillModTask( droid, player );
		droid->addPendingTask("droid_skill_mod", droidSkillModTask, 3000); // 3 sec
	}

	pet->setHomeLocation(player->getPositionX(), player->getPositionZ(), player->getPositionY(), parent);
	pet->setNextStepPosition(player->getPositionX(), player->getPositionZ(), player->getPositionY(), parent);
	pet->clearPatrolPoints();
	if (petType == PetManager::CREATUREPET) {
		pet->setCreatureBitmask(CreatureFlag::PET);
	}
	if (petType == PetManager::DROIDPET) {
		pet->setCreatureBitmask(CreatureFlag::DROID_PET);
	}
	if (petType == PetManager::FACTIONPET) {
		pet->setCreatureBitmask(CreatureFlag::FACTION_PET);
		/** dont know if npc faction pets trained via converse instead of radial
		if (pet->isNonPlayerCreatureObject() && pet->getDiet() != CreatureFlag::NONE) // show converse to npcs that eat food i.e. not atst
			pet->setOptionBit(OptionBitmask::CONVERSE,true);
		**/
	}
	pet->activateLoad("");
	pet->activateRecovery();
	// Not training any commands
	trainingCommand = 0;
	clearPatrolPoints();
}

void PetControlDeviceImplementation::cancelSpawnObject(CreatureObject* player) {
	Reference<Task*> petTask = player->getPendingTask("call_pet");

	if(petTask != NULL) {
		petTask->cancel();
		player->removePendingTask("call_pet");
	}

	if (petControlObserver != NULL)
		player->dropObserver(ObserverEventType::STARTCOMBAT, petControlObserver);
}

void PetControlDeviceImplementation::storeObject(CreatureObject* player, bool force) {
	ManagedReference<TangibleObject*> controlledObject = this->controlledObject.get();

	if (controlledObject == NULL || !controlledObject->isAiAgent())
		return;

	ManagedReference<AiAgent*> pet = cast<AiAgent*>(controlledObject.get());

	assert(pet->isLockedByCurrentThread());

	if (!force && (pet->isInCombat() || player->isInCombat() || player->isDead()))
		return;

	if (player->isRidingMount() && player->getParent() == pet) {

		if (!force && !player->checkCooldownRecovery("mount_dismount"))
			return;

		player->executeObjectControllerAction(STRING_HASHCODE("dismount"));

		if (player->isRidingMount())
			return;
	}

	if (player->getCooldownTimerMap() == NULL)
		return;

	// Check cooldown
	if (!player->getCooldownTimerMap()->isPast("petCallOrStoreCooldown") && !force) {
		player->sendSystemMessage("@pet/pet_menu:cant_store_1sec"); //"You cannot STORE for 1 second."
		return;
	}

	// Not training any commands
	trainingCommand = 0;

	//clear friend list
	friends.removeAll();

	Reference<StorePetTask*> task = new StorePetTask(player, pet);

	// Store non-faction pets immediately.  Store faction pets after 60sec delay.
	if (petType != PetManager::FACTIONPET || force || player->getPlayerObject()->isPrivileged()) {
		task->execute();
	}
	else {
		if (pet->getPendingTask("store_pet") == NULL) {
			player->sendSystemMessage( "Storing pet in 60 seconds");
			pet->addPendingTask("store_pet", task, 60 * 1000);
		}
		else {
			AtomicTime nextExecution;
			Core::getTaskManager()->getNextExecutionTime(pet->getPendingTask("store_pet"), nextExecution);
			int timeLeft = (nextExecution.getMiliTime() / 1000) - System::getTime();
			player->sendSystemMessage( "Pet will store in " + String::valueOf(timeLeft) + " seconds." );
			return;
		}

	}

	// Set cooldown
	player->getCooldownTimerMap()->updateToCurrentAndAddMili("petCallOrStoreCooldown", 1000); // 1 sec
}

bool PetControlDeviceImplementation::growPet(CreatureObject* player, bool force, bool adult) {
	if (petType != PetManager::CREATUREPET)
		return true;

	if (growthStage <= 0 || growthStage >= 10)
		return true;

	ManagedReference<TangibleObject*> controlledObject = this->controlledObject.get();

	if (controlledObject == NULL || !controlledObject->isCreature())
		return true;
	ManagedReference<Creature*> pet = cast<Creature*>(controlledObject.get());

	Reference<CreatureTemplate*> creatureTemplate = pet->getCreatureTemplate();

	if (creatureTemplate == NULL)
		return true;

	PetManager* petManager = pet->getZoneServer()->getPetManager();
	if (petManager == NULL)
		return true;

	Time currentTime;
	uint32 timeDelta = currentTime.getTime() - lastGrowth.getTime();
	int stagesToGrow = timeDelta / 43200; // 12 hour

	if (adult)
		stagesToGrow = 10;

	if (stagesToGrow == 0 && !force)
		return true;

	int newStage = growthStage + stagesToGrow;
	if (newStage > 10)
		newStage = 10;

	float newLevel = ((float)pet->getAdultLevel() / 10.0) * (float)newStage;
	if (newLevel < 1)
		newLevel = 1;

	float newHeight = creatureTemplate->getScale() * (0.46 + ((float)newStage * 0.06));

	short preEligibility = petManager->checkMountEligibility(_this.getReferenceUnsafeStaticCast());
	short postEligibility = petManager->checkMountEligibility(_this.getReferenceUnsafeStaticCast(), newHeight);

	assert(pet->isLockedByCurrentThread());

	if (preEligibility == PetManager::CANBEMOUNTTRAINED && postEligibility == PetManager::TOOLARGE && !force) {
		if (isTrainedAsMount()) {
			arrestGrowth();
			return true;
		}

		PlayerObject* ghost = player->getPlayerObject();

		if (ghost == NULL){
			return true;
		}

		ManagedReference<SuiListBox*> box = new SuiListBox(player, SuiWindowType::MOUNT_GROWTH_ARREST);
		box->setPromptTitle("@pet/pet_menu:mount_growth_title"); // Pet Growth Arrest
		box->setPromptText("@pet/pet_menu:mount_growth_prompt"); // Your pet could be trained as a mount, but is about to grow too large to serve as one. If you ever plan on training this pet as a mount you must arrest it's growth now. Stop pet's growth?
		box->setUsingObject(_this.getReferenceUnsafeStaticCast());
		box->setCancelButton(true, "@cancel");
		box->setOkButton(true, "@yes");
		box->setOtherButton(true, "@no");
		box->setCallback(new MountGrowthArrestSuiCallback(player->getZoneServer(), _this.getReferenceUnsafeStaticCast()));

		ghost->addSuiBox(box);
		player->sendMessage(box->generateMessage());
		return false;
	}

	if (adult)
		pet->setHeight(newHeight, true);
	else
		pet->setHeight(newHeight, false);

	pet->setPetLevel(newLevel);

	growthStage = newStage;
	lastGrowth.updateToCurrentTime();

	setVitality(getVitality());

	return true;
}

void PetControlDeviceImplementation::arrestGrowth() {
	if (petType != PetManager::CREATUREPET)
		return;

	if (growthStage <= 0 || growthStage >= 10)
		return;

	ManagedReference<TangibleObject*> controlledObject = this->controlledObject.get();

	if (controlledObject == NULL || !controlledObject->isCreature())
		return;

	ManagedReference<Creature*> pet = cast<Creature*>(controlledObject.get());

	Reference<CreatureTemplate*> creatureTemplate = pet->getCreatureTemplate();

	if (creatureTemplate == NULL)
		return;

	PetManager* petManager = pet->getZoneServer()->getPetManager();
	if (petManager == NULL)
		return;

	int newStage = growthStage;

	for (int i = (growthStage + 1); i < 11; i++) {
		float newHeight = creatureTemplate->getScale() * (0.46 + ((float)i * 0.06));

		short mountEligibility = petManager->checkMountEligibility(_this.getReferenceUnsafeStaticCast(), newHeight);

		if (mountEligibility == PetManager::TOOLARGE)
			break;
		else if (mountEligibility == PetManager::CANBEMOUNTTRAINED)
			newStage = i;
	}

	if (newStage > growthStage) {
		float newLevel = ((float)pet->getAdultLevel() / 10.0) * (float)newStage;
		if (newLevel < 1)
			newLevel = 1;

		float newHeight = creatureTemplate->getScale() * (0.46 + ((float)newStage * 0.06));

		pet->setHeight(newHeight, false);
		pet->setPetLevel(newLevel);
	}

	setVitality(getVitality());

	growthStage = 10;
	lastGrowth.updateToCurrentTime();
}

void PetControlDeviceImplementation::destroyObjectFromDatabase(bool destroyContainedObjects) {
	ManagedReference<TangibleObject*> controlledObject = this->controlledObject.get();

	if (controlledObject != NULL) {
		Locker locker(controlledObject);

		ManagedReference<CreatureObject*> object = controlledObject->getSlottedObject("rider").castTo<CreatureObject*>();

		if (object != NULL) {
			Locker clocker(object, controlledObject);

			object->executeObjectControllerAction(STRING_HASHCODE("dismount"));

			object = controlledObject->getSlottedObject("rider").castTo<CreatureObject*>();

			if (object != NULL) {
				controlledObject->removeObject(object, NULL, true);

				Zone* zone = getZone();

				if (zone != NULL)
					zone->transferObject(object, -1, false);
			}
		}

		controlledObject->destroyObjectFromDatabase(true);
	}

	IntangibleObjectImplementation::destroyObjectFromDatabase(destroyContainedObjects);
}

void PetControlDeviceImplementation::destroyObjectFromWorld(bool sendSelfDestroy) {
	if (petType == PetManager::CREATUREPET) {
		ManagedReference<CreatureObject*> player = getParentRecursively(SceneObjectType::PLAYERCREATURE).castTo<CreatureObject*>();

		if (player != NULL) {
			player->sendSystemMessage("@pet/pet_menu:pet_released"); // You release your pet back into the wild
		}
	}

	IntangibleObjectImplementation::destroyObjectFromWorld(sendSelfDestroy);
}

int PetControlDeviceImplementation::canBeDestroyed(CreatureObject* player) {
	ManagedReference<AiAgent*> controlledObject = cast<AiAgent*>(this->controlledObject.get().get());

	if (controlledObject != NULL) {
		ManagedReference<PlayerObject*> ghost = player->getPlayerObject();
		if (ghost->hasActivePet(controlledObject))
			return 1;
	}

	return IntangibleObjectImplementation::canBeDestroyed(player);
}

bool PetControlDeviceImplementation::canBeTradedTo(CreatureObject* player, CreatureObject* receiver, int numberInTrade) {
	ManagedReference<SceneObject*> datapad = receiver->getSlottedObject("datapad");

	if (datapad == NULL)
		return false;

	if (petType == PetManager::FACTIONPET) {
		player->sendSystemMessage("@pet/pet_menu:bad_type"); // You cannot trade a pet of that type. Transfer failed.
		return false;
	} else if (petType == PetManager::DROIDPET) {
		ManagedReference<PlayerManager*> playerManager = player->getZoneServer()->getPlayerManager();

		int droidsInDatapad = numberInTrade;
		int maxStoredDroids = playerManager->getBaseStoredDroids();

		for (int i = 0; i < datapad->getContainerObjectsSize(); i++) {
			Reference<SceneObject*> obj =  datapad->getContainerObject(i).castTo<SceneObject*>();

			if (obj != NULL && obj->isPetControlDevice() ){
				Reference<PetControlDevice*> petDevice = cast<PetControlDevice*>(obj.get());
				if( petDevice != NULL && petDevice->getPetType() == PetManager::DROIDPET){
					droidsInDatapad++;
				}
			}
		}

		if( droidsInDatapad >= maxStoredDroids){
			player->sendSystemMessage("That person has too many droids in their datapad");
			return false;
		}

		return true;

	} else if (petType == PetManager::CREATUREPET) {
		ManagedReference<TangibleObject*> controlledObject = this->controlledObject.get();

		if (controlledObject == NULL || !controlledObject->isCreature())
			return false;

		ManagedReference<Creature*> pet = cast<Creature*>(controlledObject.get());

		ManagedReference<PlayerManager*> playerManager = player->getZoneServer()->getPlayerManager();

		int numberStored = numberInTrade;
		int maxStoredPets = playerManager->getBaseStoredCreaturePets();
		int maxLevelofPets = 10;
		int level = pet->getAdultLevel();
		bool ch = receiver->hasSkill("outdoors_creaturehandler_novice");

		if (ch) {
			maxStoredPets += receiver->getSkillMod("stored_pets");
			maxLevelofPets = receiver->getSkillMod("tame_level");
		}

		if (level > maxLevelofPets) {
			player->sendSystemMessage("@pet/pet_menu:no_chance"); // That person has no chance of controlling this creature. Transfer failed.
			receiver->sendSystemMessage("@pet/pet_menu:cannot_control"); // You have no chance of controlling that creature.
			return false;
		}

		if (pet->isVicious() && (receiver->getSkillMod("tame_aggro") <= 0 || !ch)) {
			player->sendSystemMessage("@pet/pet_menu:no_chance"); // That person has no chance of controlling this creature. Transfer failed.
			receiver->sendSystemMessage("@pet/pet_menu:cannot_control"); // You have no chance of controlling that creature.
			return false;
		}

		if (numberStored >= maxStoredPets) {
			player->sendSystemMessage("@pet/pet_menu:targ_too_many_stored"); // That person has too many stored pets. Transfer failed.
			receiver->sendSystemMessage("@pet/pet_menu:sys_too_many_stored"); // There are too many pets stored in this container. Release some of them to make room for more.
			return false;
		}

		for (int i = 0; i < datapad->getContainerObjectsSize(); ++i) {
			ManagedReference<SceneObject*> object = datapad->getContainerObject(i);

			if (object != NULL && object->isPetControlDevice()) {
				PetControlDevice* device = cast<PetControlDevice*>( object.get());

				if (device->getPetType() == PetManager::CREATUREPET) {
					if (++numberStored >= maxStoredPets) {
						player->sendSystemMessage("@pet/pet_menu:targ_too_many_stored"); // That person has too many stored pets. Transfer failed.
						receiver->sendSystemMessage("@pet/pet_menu:sys_too_many_stored"); // There are too many pets stored in this container. Release some of them to make room for more.
						return false;
					}

				}
			}
		}

		return true;
	}

	return false;
}

void PetControlDeviceImplementation::fillAttributeList(AttributeListMessage* alm, CreatureObject* object) {
	SceneObjectImplementation::fillAttributeList(alm, object);

	if (petType == PetManager::DROIDPET) {
		alm->insertAttribute("creature_vitality", String::valueOf(vitality) + "/" + String::valueOf(maxVitality));

		ManagedReference<DroidObject*> droid = this->controlledObject.get().castTo<DroidObject*>();

		if (droid != NULL) {
			alm->insertAttribute("challenge_level", droid->getLevel());
			alm->insertAttribute("creature_health", droid->getBaseHAM(0));
			alm->insertAttribute("creature_action", droid->getBaseHAM(3));
			alm->insertAttribute("creature_mind", droid->getBaseHAM(6));

			droid->fillAttributeList(alm, object);
		}
	} else {
		ManagedReference<AiAgent*> pet = cast<AiAgent*>(this->controlledObject.get().get());

		if (pet != NULL) {
			alm->insertAttribute("challenge_level", pet->getLevel());

			if (petType == PetManager::CREATUREPET)
				alm->insertAttribute("creature_vitality", String::valueOf(vitality) + "/" + String::valueOf(maxVitality));

			alm->insertAttribute("creature_health", pet->getBaseHAM(0));
			alm->insertAttribute("creature_action", pet->getBaseHAM(3));
			alm->insertAttribute("creature_mind", pet->getBaseHAM(6));

			int armor = pet->getArmor();
			if (armor == 0)
				alm->insertAttribute("armor_rating", "None");
			else if (armor == 1)
				alm->insertAttribute("armor_rating", "Light");
			else if (armor == 2)
				alm->insertAttribute("armor_rating", "Medium");
			else if (armor == 3)
				alm->insertAttribute("armor_rating", "Heavy");

			if (pet->getKinetic() < 0)
				alm->insertAttribute("dna_comp_armor_kinetic", "Vulnerable");
			else
				alm->insertAttribute("dna_comp_armor_kinetic", pet->getKinetic());

			if (pet->getEnergy() < 0)
				alm->insertAttribute("dna_comp_armor_energy", "Vulnerable");
			else
				alm->insertAttribute("dna_comp_armor_energy", pet->getEnergy());

			if (pet->getBlast() < 0)
				alm->insertAttribute("dna_comp_armor_blast", "Vulnerable");
			else
				alm->insertAttribute("dna_comp_armor_blast", pet->getBlast());

			if (pet->getHeat() < 0)
				alm->insertAttribute("dna_comp_armor_heat", "Vulnerable");
			else
				alm->insertAttribute("dna_comp_armor_heat", pet->getHeat());

			if (pet->getCold() < 0)
				alm->insertAttribute("dna_comp_armor_cold", "Vulnerable");
			else
				alm->insertAttribute("dna_comp_armor_cold", pet->getCold());

			if (pet->getElectricity() < 0)
				alm->insertAttribute("dna_comp_armor_electric", "Vulnerable");
			else
				alm->insertAttribute("dna_comp_armor_electric", pet->getElectricity());

			if (pet->getAcid() < 0)
				alm->insertAttribute("dna_comp_armor_acid", "Vulnerable");
			else
				alm->insertAttribute("dna_comp_armor_acid", pet->getAcid());

			if (pet->getStun() < 0)
				alm->insertAttribute("dna_comp_armor_stun", "Vulnerable");
			else
				alm->insertAttribute("dna_comp_armor_stun", pet->getStun());

			if (pet->getLightSaber() < 0)
				alm->insertAttribute("dna_comp_armor_saber", "Vulnerable");
			else
				alm->insertAttribute("dna_comp_armor_saber", pet->getLightSaber());

			ManagedReference<WeaponObject*> weapon = pet->getWeapon();
			if (weapon != NULL){
				StringBuffer displayValue;
				displayValue << Math::getPrecision(weapon->getAttackSpeed(), 2);
				alm->insertAttribute("creature_attack", displayValue);
			}
			StringBuffer displayValue;
			displayValue << Math::getPrecision(pet->getChanceHit(), 2);
			alm->insertAttribute("creature_tohit", displayValue);

			alm->insertAttribute("creature_damage", String::valueOf(pet->getDamageMin()) + " - " + String::valueOf(pet->getDamageMax()));

			if (petType == PetManager::CREATUREPET) {
				CreatureAttackMap* attMap = pet->getAttackMap();

				if (attMap != NULL && attMap->size() > 0) {
					String cmd = attMap->getCommand(0);
					if (cmd.isEmpty()) {
						alm->insertAttribute("spec_atk_1", " ---");
					} else {
						String str = StringIdManager::instance()->getStringId(("@combat_effects:" + cmd).hashCode()).toString();
						alm->insertAttribute("spec_atk_1", str);
					}
				} else {
					alm->insertAttribute("spec_atk_1", " ---");
				}

				if (attMap != NULL && attMap->size() > 1) {
					String cmd = attMap->getCommand(1);
					if (cmd.isEmpty()) {
						alm->insertAttribute("spec_atk_1", " ---");
					} else {
						String str = StringIdManager::instance()->getStringId(("@combat_effects:" + cmd).hashCode()).toString();
						alm->insertAttribute("spec_atk_2", str);
					}
				} else {
					alm->insertAttribute("spec_atk_2", " ---");
				}

				// TODO set this up to check for the actual ranged weapon
				if (pet->hasRangedWeapon())
					alm->insertAttribute("dna_comp_ranged_attack", "Yes");
				else
					alm->insertAttribute("dna_comp_ranged_attack", "No");
			}
		}
	}

	// Trained Commands
	if ( trainedCommands.size() > 0) {
		alm->insertAttribute("pet_command", "" );
	}

	if( trainedCommands.contains(PetManager::STAY) ){
		alm->insertAttribute("pet_command_1", trainedCommands.get(PetManager::STAY) );
	}

	if( trainedCommands.contains(PetManager::FOLLOW) ){
		alm->insertAttribute("pet_command_0", trainedCommands.get(PetManager::FOLLOW) );
	}

	if( trainedCommands.contains(PetManager::STORE) ){
		alm->insertAttribute("pet_command_11", trainedCommands.get(PetManager::STORE) );
	}

	if( trainedCommands.contains(PetManager::ATTACK) ){
		alm->insertAttribute("pet_command_4", trainedCommands.get(PetManager::ATTACK) );
	}

	if( trainedCommands.contains(PetManager::GUARD) ){
		alm->insertAttribute("pet_command_2", trainedCommands.get(PetManager::GUARD) );
	}

	if( trainedCommands.contains(PetManager::FRIEND) ){
		alm->insertAttribute("pet_command_3", trainedCommands.get(PetManager::FRIEND) );
	}

	if( trainedCommands.contains(PetManager::FOLLOWOTHER) ){
		alm->insertAttribute("pet_command_17", trainedCommands.get(PetManager::FOLLOWOTHER) );
	}

	if( trainedCommands.contains(PetManager::TRICK1) ){
		alm->insertAttribute("pet_command_12", trainedCommands.get(PetManager::TRICK1) );
	}

	if( trainedCommands.contains(PetManager::TRICK2) ){
		alm->insertAttribute("pet_command_13", trainedCommands.get(PetManager::TRICK2) );
	}

	if( trainedCommands.contains(PetManager::PATROL) ){
		alm->insertAttribute("pet_command_5", trainedCommands.get(PetManager::PATROL) );
	}

	if( trainedCommands.contains(PetManager::GETPATROLPOINT) ){
		alm->insertAttribute("pet_command_6", trainedCommands.get(PetManager::GETPATROLPOINT) );
	}

	if( trainedCommands.contains(PetManager::CLEARPATROLPOINTS) ){
		alm->insertAttribute("pet_command_7", trainedCommands.get(PetManager::CLEARPATROLPOINTS) );
	}

	if( trainedCommands.contains(PetManager::FORMATION1) ){
		alm->insertAttribute("pet_command_8", trainedCommands.get(PetManager::FORMATION1) );
	}

	if( trainedCommands.contains(PetManager::FORMATION2) ){
		alm->insertAttribute("pet_command_9", trainedCommands.get(PetManager::FORMATION2) );
	}

	if( trainedCommands.contains(PetManager::SPECIAL_ATTACK1) ){
		alm->insertAttribute("pet_command_18", trainedCommands.get(PetManager::SPECIAL_ATTACK1) );
	}

	if( trainedCommands.contains(PetManager::SPECIAL_ATTACK2) ){
		alm->insertAttribute("pet_command_19", trainedCommands.get(PetManager::SPECIAL_ATTACK2) );
	}

	if( trainedCommands.contains(PetManager::RANGED_ATTACK) ){
		alm->insertAttribute("pet_command_20", trainedCommands.get(PetManager::RANGED_ATTACK) );
	}

	if( trainedCommands.contains(PetManager::GROUP) ){
		alm->insertAttribute("pet_command_16", trainedCommands.get(PetManager::GROUP) );
	}

	if( trainedCommands.contains(PetManager::RECHARGEOTHER) ){
		alm->insertAttribute("@pet/pet_menu:menu_recharge_other", trainedCommands.get(PetManager::RECHARGEOTHER) );
	}

	if( trainedCommands.contains(PetManager::TRANSFER) ){
		alm->insertAttribute("pet_command_10", trainedCommands.get(PetManager::TRANSFER) );
	}

	if( trainedCommands.contains(PetManager::REPAIR) ){
		alm->insertAttribute("pet_command_13", trainedCommands.get(PetManager::REPAIR) ); // Droid Repair really was listed as Trick 2
	}
	if( trainedCommands.contains(PetManager::THROWTRAP) ){
		alm->insertAttribute("pet_command_22", trainedCommands.get(PetManager::THROWTRAP) );
	}
	if( trainedCommands.contains(PetManager::HARVEST) ){
		alm->insertAttribute("pet_command_21", trainedCommands.get(PetManager::HARVEST) );
	}
}

void PetControlDeviceImplementation::setDefaultCommands() {
	trainedCommands.put(PetManager::STAY, "stay");
	trainedCommands.put(PetManager::FOLLOW, "follow");
	trainedCommands.put(PetManager::STORE, "store");

	ManagedReference<DroidObject*> droid = this->controlledObject.get().castTo<DroidObject*>();
	if (droid != NULL) {
		if (droid->isCombatDroid()) {
			trainedCommands.put(PetManager::ATTACK, "attack");
			trainedCommands.put(PetManager::GUARD, "guard");
		}
	} else {
		trainedCommands.put(PetManager::ATTACK, "attack");
		trainedCommands.put(PetManager::GUARD, "guard");
	}

	trainedCommands.put(PetManager::FRIEND, "friend");
	trainedCommands.put(PetManager::FOLLOWOTHER, "followother");
	trainedCommands.put(PetManager::PATROL, "patrol");
	trainedCommands.put(PetManager::GETPATROLPOINT, "getpatrolpoint");
	trainedCommands.put(PetManager::CLEARPATROLPOINTS, "clearpatrolpoints");
	trainedCommands.put(PetManager::FORMATION1, "formation1");
	trainedCommands.put(PetManager::FORMATION2, "formation2");

	if (droid != NULL) {
		int species = droid->getSpecies();

		if (droid->isCombatDroid() && (species == DroidObject::PROBOT || species == DroidObject::DZ70))
			trainedCommands.put(PetManager::RANGED_ATTACK, "ranged attack");
	} else {
		trainedCommands.put(PetManager::RANGED_ATTACK, "ranged attack");
	}

	trainedCommands.put(PetManager::GROUP, "group");
}

void PetControlDeviceImplementation::setTrainingCommand(unsigned int commandID) {

	// we set to 0 to flag completion so skip all this then.
	if (commandID == 0) {
		trainingCommand = 0;
		return;
	}

	ManagedReference<TangibleObject*> controlledObject = this->controlledObject.get();
	if (controlledObject == NULL || !controlledObject->isAiAgent())
		return;

	AiAgent* pet = cast<AiAgent*>(controlledObject.get());
	if (pet == NULL)
		return;

	ManagedReference<CreatureObject*> owner = pet->getLinkedCreature().get();
	if (owner == NULL || !owner->isPlayerCreature())
		return;

	if (petType == PetManager::DROIDPET) {
		ManagedReference<DroidObject*> droid = this->controlledObject.get().castTo<DroidObject*>();
		if (droid == NULL)
			return;

		// Check power on droids
		if (!droid->hasPower()) {
			droid->showFlyText("npc_reaction/flytext","low_power", 204, 0, 0);  // "*Low Power*"
			return;
		}

		if (((commandID == PetManager::ATTACK || commandID == PetManager::GUARD) && !droid->isCombatDroid()) ||
			(commandID == PetManager::RANGED_ATTACK && (!droid->isCombatDroid() || !droid->hasRangedWeapon())) ||
			(commandID == PetManager::RECHARGEOTHER && !droid->isPowerDroid()) ||
			(commandID == PetManager::TRANSFER) ||
			(commandID == PetManager::SPECIAL_ATTACK1) ||
			(commandID == PetManager::SPECIAL_ATTACK2) ||
			(commandID == PetManager::TRICK1) ||
			(commandID == PetManager::TRICK2)){
			return;
		}
	}
	else if (petType == PetManager::CREATUREPET) {
		if (((commandID == PetManager::ATTACK || commandID == PetManager::FOLLOW || commandID == PetManager::STORE) && !owner->hasSkill("outdoors_creaturehandler_novice") ) ||
			(commandID == PetManager::STAY && !owner->hasSkill("outdoors_creaturehandler_training_01")) ||
			(commandID == PetManager::GUARD && !owner->hasSkill("outdoors_creaturehandler_training_02")) ||
			(commandID == PetManager::FRIEND && !owner->hasSkill("outdoors_creaturehandler_support_03")) ||
			((commandID == PetManager::PATROL || commandID == PetManager::GETPATROLPOINT || commandID == PetManager::CLEARPATROLPOINTS) && !owner->hasSkill("outdoors_creaturehandler_training_03")) ||
			((commandID == PetManager::FORMATION1 || commandID == PetManager::FORMATION2) && !owner->hasSkill("outdoors_creaturehandler_training_04")) ||
			(commandID == PetManager::TRANSFER && !owner->hasSkill("outdoors_creaturehandler_master")) ||
			(commandID == PetManager::TRICK1 && !owner->hasSkill("outdoors_creaturehandler_healing_01")) ||
			(commandID == PetManager::TRICK2 && !owner->hasSkill("outdoors_creaturehandler_healing_03")) ||
			(commandID == PetManager::GROUP && !owner->hasSkill("outdoors_creaturehandler_support_01")) ||
			(commandID == PetManager::SPECIAL_ATTACK1 && (!owner->hasSkill("outdoors_creaturehandler_taming_03") || !pet->hasSpecialAttack(1))) ||
			(commandID == PetManager::SPECIAL_ATTACK2 && (!owner->hasSkill("outdoors_creaturehandler_taming_04") || !pet->hasSpecialAttack(2))) ||
			(commandID == PetManager::RANGED_ATTACK && (!owner->hasSkill("outdoors_creaturehandler_master") || !pet->hasRangedWeapon())) ||
			(commandID == PetManager::FOLLOWOTHER && !owner->hasSkill("outdoors_creaturehandler_support_02")) ||
			(commandID == PetManager::RECHARGEOTHER))
				return;
	}
	else if (petType == PetManager::FACTIONPET) {
		if (commandID == PetManager::RECHARGEOTHER ||
			(commandID == PetManager::TRANSFER) ||
			(commandID == PetManager::SPECIAL_ATTACK1) ||
			(commandID == PetManager::SPECIAL_ATTACK2) ||
			(commandID == PetManager::TRICK1) ||
			(commandID == PetManager::TRICK2))
				return;
	}

	/** Check for converse and if so, get its personalityStf**/
	if (pet->getOptionsBitmask() & OptionBitmask::CONVERSE) {
		String stf = pet->getPersonalityStf();
		StringBuffer message;
		message << stf << ":start_convo_4";
		StringIdChatParameter chat;
		chat.setStringId(message.toString());
		pet->getZoneServer()->getChatManager()->broadcastChatMessage(pet, chat, 0, 0, 0);
	} else {
		pet->showFlyText("npc_reaction/flytext","alert", 204, 0, 0);  // "?"
	}

	trainingCommand = commandID;
}

void PetControlDeviceImplementation::trainAsMount(CreatureObject* player) {
	if (isTrainedAsMount() || !player->hasSkill("outdoors_creaturehandler_support_04"))
		return;

	PetManager* petManager = player->getZoneServer()->getPetManager();
	if (petManager == NULL)
		return;

	if (petManager->checkMountEligibility(_this.getReferenceUnsafeStaticCast()) != PetManager::CANBEMOUNTTRAINED)
		return;

	ManagedReference<TangibleObject*> controlledObject = this->controlledObject.get();
	if (controlledObject == NULL || !controlledObject->isAiAgent())
		return;

	AiAgent* pet = cast<AiAgent*>(controlledObject.get());
	if (pet == NULL)
		return;

	assert(pet->isLockedByCurrentThread());

	trainedAsMount = true;
	pet->setOptionBit(0x1000);
}

void PetControlDeviceImplementation::resetNamingCommands() {
	if (namingCommands.isEmpty())
		return;

	namingCommands.removeAll();
}

void PetControlDeviceImplementation::addPatrolPoint(PatrolPoint& point) {
	patrolPoints.add(point);
}

PatrolPoint PetControlDeviceImplementation::getPatrolPoint(int idx) {
	return patrolPoints.get(idx);
}

bool PetControlDeviceImplementation::isValidPet(AiAgent* pet) {
	PetDeed* deed = pet->getPetDeed();

	if (deed != NULL) {
		// time to calculate!
		int calculatedLevel =  deed->calculatePetLevel();

		if (pet->getTemplateLevel() >= (calculatedLevel * 0.85)) {
			return true;
		} else {
			return false;
		}
	}

	return true;
}

void PetControlDeviceImplementation::setVitality(int vit) {
	vitality = vit;

	if (petType == PetManager::CREATUREPET || petType == PetManager::DROIDPET) {
		ManagedReference<CreatureObject*> pet = this->controlledObject.get().castTo<CreatureObject*>();
		if (controlledObject == NULL)
			return;

		float hamPenaltyModifier = 0;
		if (vitality <= 75 && vitality > 50) {
			hamPenaltyModifier = 0.25f;
		}
		else if (vitality <= 50 && vitality > 25) {
			hamPenaltyModifier = 0.5f;
		}
		else if (vitality <= 25) {
			hamPenaltyModifier = 0.75f;
		}

		Reference<PetControlDevice*> petControlDevice = _this.getReferenceUnsafeStaticCast();

		float vitalityMindPenalty = this->vitalityMindPenalty;
		float vitalityActionPenalty = this->vitalityActionPenalty;
		float vitalityHealthPenalty	= this->vitalityHealthPenalty;

		Core::getTaskManager()->executeTask([pet, petControlDevice, hamPenaltyModifier, vitalityMindPenalty, vitalityActionPenalty, vitalityHealthPenalty] () {
			Locker locker(pet);

			Locker clocker(petControlDevice, pet);

			int newVitalityHealthPenalty = pet->getBaseHAM(0) * hamPenaltyModifier;
			int newVitalityActionPenalty = pet->getBaseHAM(3) * hamPenaltyModifier;
			int newVitalityMindPenalty = pet->getBaseHAM(6) * hamPenaltyModifier;

			if (newVitalityHealthPenalty != vitalityHealthPenalty) {
				int change = vitalityHealthPenalty - newVitalityHealthPenalty;
				petControlDevice->setVitalityHealthPenalty(newVitalityHealthPenalty);
			}

			if (newVitalityActionPenalty != vitalityActionPenalty) {
				int change = vitalityActionPenalty - newVitalityActionPenalty;
				pet->setMaxHAM(3, pet->getMaxHAM(3) + change, true);
				petControlDevice->setVitalityActionPenalty(newVitalityActionPenalty);
			}

			if (newVitalityMindPenalty != vitalityMindPenalty) {
				int change = vitalityMindPenalty - newVitalityMindPenalty;
				pet->setMaxHAM(6, pet->getMaxHAM(6) + change, true);
				petControlDevice->setVitalityMindPenalty(newVitalityMindPenalty);
			}
		}, "PetSetVitalityLambda");
	}
}
