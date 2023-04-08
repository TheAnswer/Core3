/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.
*/

#include "ComponentSlots.h"
#include "ShipComponentFlag.h"
#include "system/lang/UnicodeString.h"
#include "server/zone/Zone.h"
#include "server/zone/SpaceZone.h"
#include "server/zone/ZoneClientSession.h"
#include "server/zone/managers/ship/ShipManager.h"
#include "server/zone/objects/scene/SceneObject.h"
#include "server/zone/objects/ship/ShipObject.h"
#include "server/zone/objects/ship/components/ShipComponent.h"
#include "server/zone/objects/ship/components/ShipEngineComponent.h"
#include "server/zone/objects/ship/events/ShipRecoveryEvent.h"
#include "server/zone/objects/intangible/ShipControlDevice.h"
#include "server/zone/objects/tangible/TangibleObject.h"
#include "server/zone/packets/ship/ShipObjectMessage1.h"
#include "server/zone/packets/ship/ShipObjectMessage3.h"
#include "server/zone/packets/ship/ShipObjectMessage4.h"
#include "server/zone/packets/ship/ShipObjectMessage6.h"
#include "server/zone/packets/cell/UpdateCellPermissionsMessage.h"
#include "server/zone/packets/scene/SceneObjectCreateMessage.h"
#include "templates/tangible/SharedShipObjectTemplate.h"
#include "server/zone/objects/ship/ShipChassisData.h"
#include "server/zone/managers/stringid/StringIdManager.h"
#include "templates/faction/Factions.h"

void ShipObjectImplementation::initializeTransientMembers() {
	hyperspacing = false;

	if (shipRecoveryEvent == nullptr) {
		shipRecoveryEvent = new ShipRecoveryEvent(asShipObject());
	}

	TangibleObjectImplementation::initializeTransientMembers();
}

void ShipObjectImplementation::loadTemplateData(SharedObjectTemplate* templateData) {
	TangibleObjectImplementation::loadTemplateData(templateData);

	SharedShipObjectTemplate* ssot = dynamic_cast<SharedShipObjectTemplate*>(templateData);

	if (ssot != nullptr) {
		setShipName(ssot->getShipName());
		setShipType(ssot->getShipType(), false);
		setShipNameCRC(chassisDataName.hashCode(), false);
		setUniqueID(getUniqueID(), false);

		setChassisMaxHealth(ssot->getChassisHitpoints(), false);
		setCurrentChassisHealth(ssot->getChassisHitpoints(), false);

		setSlipRate(ssot->getChassisSlipRate(), false);
		setChassisSpeed(ssot->getChassisSpeed(), false);
		setChassisMaxMass(ssot->getChassisMass(), false);

		setShipFaction(ssot->getShipFaction(), false);
		setShipDifficulty(ssot->getShipDifficulty(), false);

		setConversationMessage(ssot->getConversationMessage());
		setConversationMobile(ssot->getConversationMobile());
		setConversationTemplate(ssot->getConversationTemplate());

		setHasWings(ssot->shipHasWings());
	}

	auto portal = ssot->getPortalLayout();

	if (portal == nullptr)
		totalCellNumber = 0;
	else
		totalCellNumber = portal->getCellTotalNumber();
}

void ShipObjectImplementation::sendTo(SceneObject* player, bool doClose, bool forceLoadContainer) {
	BaseMessage* msg = new SceneObjectCreateMessage(asSceneObject());
	player->sendMessage(msg);

	link(player, containmentType);

	try {
		sendBaselinesTo(player);
		sendSlottedObjectsTo(player);
		sendContainerObjectsTo(player, false);
	} catch (Exception& e) {
		error(e.getMessage());
		e.printStackTrace();
	}

	if (doClose)
		SceneObjectImplementation::close(player);
}

void ShipObjectImplementation::setShipName(const String& name, bool notifyClient) {
	chassisDataName = name;
	shipNameCRC.update(name.hashCode(), false);
}

void ShipObjectImplementation::storeShip(CreatureObject* player) {
	if (player == nullptr)
		return;

	ZoneServer* zoneServer = player->getZoneServer();

	if (zoneServer == nullptr)
		return;

	ShipControlDevice* shipControlDevice = zoneServer->getObject(getControlDeviceID()).castTo<ShipControlDevice*>();

	if (shipControlDevice == nullptr)
		return;

	shipControlDevice->storeShip(player);
}

void ShipObjectImplementation::createChildObjects() {
	auto layout = getObjectTemplate()->getPortalLayout();

	if (layout == nullptr)
		return;

	for (int i = 0; i < totalCellNumber; ++i) {

		Reference<CellObject*> newCell = getZoneServer()->createObject(0xAD431713, getPersistenceLevel()).castTo<CellObject*>();

		if (newCell == nullptr) {
			error("could not create cell");
			continue;
		}

		Locker clocker(newCell, asShipObject());

		if (!transferObject(newCell, -1, true, true)) {
			error("could not add cell");
			continue;
		}

		newCell->setCellNumber(i + 1);

		if (i != 0) {
			cellNameMap.put(layout->getCellProperty(i-1)->getName(), newCell);
		}

		cells.put(i, newCell);
	}

	ZoneServer* zoneServer = getZoneServer();

	for (int i = 0; i < templateObject->getChildObjectsSize(); ++i) {
		const ChildObject* child = templateObject->getChildObject(i);

		if (child == nullptr)
			continue;

		uint32 childHash = child->getTemplateFile().hashCode();

		ManagedReference<SceneObject *> obj = zoneServer->createObject(childHash, getPersistenceLevel());

		if (obj == nullptr)
			continue;

		Locker objLocker(obj, asSceneObject());

		Vector3 childPosition = child->getPosition();
		childObjects.put(obj);
		obj->initializePosition(childPosition.getX(), childPosition.getZ(), childPosition.getY());
		obj->setDirection(child->getDirection());

		int totalCells = getTotalCellNumber();

		try {
			if (totalCells >= child->getCellId()) {
				ManagedReference<CellObject *> cellObject = getCell(child->getCellId());

				info("Inserting into " + String::valueOf(cellObject->getObjectID()), true);

				if (cellObject != nullptr) {
					if (!cellObject->transferObject(obj, child->getContainmentType(), true)) {
						obj->destroyObjectFromDatabase(true);
						continue;
					} else {
						if (obj->getGameObjectType() == SceneObjectType::PILOTCHAIR)
							setPilotChair(obj);
					}
				} else {
					error("nullptr CELL OBJECT");
					obj->destroyObjectFromDatabase(true);
					continue;
				}
			}
		} catch (Exception &e) {
			error("unreported exception caught in void SceneObjectImplementation::createChildObjects()!");
			e.printStackTrace();
		}
	}

	updateToDatabase();
}

void ShipObjectImplementation::sendContainerObjectsTo(SceneObject* player, bool forceLoad) {
	auto creo = player->asCreatureObject();

	if (creo == nullptr) {
		return;
	}

	for (int i = 0; i < containerObjects.size(); ++i) {
		auto object = containerObjects.get(i);

		if (object == nullptr) {
			continue;
		}

		object->sendTo(creo, true);
	}

	for (int i = 0; i < cells.size(); ++i) {
		auto cell = cells.get(i);

		if (cell == nullptr || !cell->isContainerLoaded()) {
			continue;
		}

		auto perms = cell->getContainerPermissions();
		if (perms == nullptr) {
			continue;
		}

		cell->sendPermissionsTo(player->asCreatureObject(), true);

		for (int j = 0; j < cell->getContainerObjectsSize(); ++j) {
			auto object = cell->getContainerObject(j);
			if (object == nullptr) {
				continue;
			}

			object->sendTo(creo, true);
		}
	}
}

void ShipObjectImplementation::sendSlottedObjectsTo(SceneObject* player) {
	VectorMap<String, ManagedReference<SceneObject* > > slotted;
	getSlottedObjects(slotted);

	SortedVector<uint64> objects(slotted.size(), slotted.size());
	objects.setNoDuplicateInsertPlan();

	for (int i = 0; i < slotted.size(); ++i) {
		SceneObject* object = slotted.get(i);
		if (object == nullptr || objects.put(object->getObjectID()) == -1) {
			continue;
		}

		if (object == player && player->getMovementCounter() != 0) {
			continue;
		}

		if (object->isInOctTree()) {
			notifyInsert(object);
		} else {
			object->sendTo(player, true, false);
		}
	}
}

void ShipObjectImplementation::updateCraftingValues(CraftingValues* values, bool firstUpdate) {
	for (int i = 0; i < values->getTotalExperimentalAttributes(); ++i) {
		String attribute;
		float min;
		float max;
		float current;

		attribute = values->getAttribute(i);
		min = values->getMinValue(attribute);
		max = values->getMaxValue(attribute);
		current = values->getCurrentValue(attribute);

		//info("Attribute: " + attribute + " min: " + String::valueOf(min) + " max: " + String::valueOf(max) + " current: " + String::valueOf(current), true);
	}

	TangibleObjectImplementation::updateCraftingValues(values, firstUpdate);
}

uint16 ShipObjectImplementation::getUniqueID() {
	uint32 hash = UnsignedLong::hashCode(getObjectID());
	uint16 id = (uint16) (hash ^ (hash >> 16));

	//info("uniqueId: 0x" + String::hexvalueOf(id), true);

	return id;
}

void ShipObjectImplementation::sendBaselinesTo(SceneObject* player) {
	bool sendSelf = player->isASubChildOf(asShipObject());

	if (sendSelf) {
		ShipObjectMessage1* ship1 = new ShipObjectMessage1(_this.getReferenceUnsafeStaticCast());
		player->sendMessage(ship1);
	}

	ShipObjectMessage3* ship3 = new ShipObjectMessage3(_this.getReferenceUnsafeStaticCast());
	player->sendMessage(ship3);

	if (sendSelf) {
		ShipObjectMessage4* ship4 = new ShipObjectMessage4(_this.getReferenceUnsafeStaticCast());
		player->sendMessage(ship4);
	}

	ShipObjectMessage6* ship6 = new ShipObjectMessage6(_this.getReferenceUnsafeStaticCast());
	player->sendMessage(ship6);

	if (player->isPlayerCreature()) {
		auto creature = player->asCreatureObject();

		if (creature != nullptr) {
			sendPvpStatusTo(creature);
		}
	}
}

ShipObject* ShipObjectImplementation::asShipObject() {
	return _this.getReferenceUnsafeStaticCast();
}

ShipObject* ShipObject::asShipObject() {
	return this;
}

void ShipObjectImplementation::install(CreatureObject* player, SceneObject* sceno, int slot, bool notifyClient) {
	if (getComponentObject(slot) != nullptr) {
		return uninstall(player, slot, notifyClient);
	}

	ManagedReference<ShipComponent*> shipComponent = dynamic_cast<ShipComponent*>(sceno);
	if (shipComponent == nullptr) {
		return;
	}

	Locker lock(shipComponent);
	shipComponent->destroyObjectFromWorld(true);

	components.put(slot, shipComponent);

	shipComponent->install(player, asShipObject(), slot, notifyClient);
}

void ShipObjectImplementation::uninstall(CreatureObject* player, int slot, bool notifyClient) {
	ManagedReference<ShipComponent*> shipComponent = getComponentObject(slot);
	if (shipComponent == nullptr) {
		return;
	}

	Locker lock(shipComponent);
	shipComponent->destroyObjectFromWorld(true);

	if (player != nullptr) {
		ManagedReference<SceneObject*> inventory = player->getSlottedObject("inventory");
		if (inventory == nullptr || !inventory->transferObject(shipComponent, -1, notifyClient, true)) {
			return;
		}

		if (notifyClient) {
			shipComponent->sendTo(player, true);
		}
	}

	shipComponent->uninstall(player, asShipObject(), slot, notifyClient);

	components.drop(slot);
}

String ShipObjectImplementation::getParkingLocation() {
	if (parkingLocation == "") {
		ManagedReference<CreatureObject*> creo = owner.get();
		if (creo != nullptr) {
			parkingLocation = creo->getCityRegion().get()->getRegionDisplayedName();
		}
	}
	return parkingLocation;
}

int ShipObjectImplementation::notifyObjectInsertedToChild(SceneObject* object, SceneObject* child, SceneObject* oldParent) {
	ManagedReference<SpaceZone*> zone = getSpaceZone();

	Locker* _locker = nullptr;

	if (zone != nullptr)
		_locker = new Locker(zone);

	try {
		if (object->getCloseObjects() != nullptr)
			object->addInRangeObject(object, false);

		if (child->isCellObject()) {
			bool runInRange = true;

			if ((oldParent == nullptr || !oldParent->isCellObject()) || oldParent == child) {

				if (oldParent == nullptr || (oldParent != nullptr && dynamic_cast<SpaceZone*>(oldParent) == nullptr && !oldParent->isCellObject())) {
					//notifyObjectInsertedToZone(object);
					runInRange = false;
				}

				if (!object->isPlayerCreature()) {
					broadcastDestroy(object, true);
					broadcastObject(object, false);
				}
			}

			if (runInRange) {
				ManagedReference<CellObject*> cell = cast<CellObject*>(child);

				if (cell != nullptr) {
					for (int j = 0; j < cell->getContainerObjectsSize(); ++j) {
						ManagedReference<SceneObject*> cobj = cell->getContainerObject(j);

						if (cobj != object) {

							if (cobj->getCloseObjects() != nullptr) {
								if (!cobj->getCloseObjects()->contains(object)) {
									cobj->addInRangeObject(object, false);
									object->sendTo(cobj, true, false);
								}
							} else
								cobj->notifyInsert(object);

							if (object->getCloseObjects() != nullptr) {
								if (!object->getCloseObjects()->contains(cobj.get())) {
									object->addInRangeObject(cobj.get(), false);
									cobj->sendTo(object, true, false);//sendTo because notifyInsert doesnt send objects with parent
								} else {
									if (object->getClient() != nullptr && cobj->isCreatureObject()) {
										object->sendMessage(cobj->link(cell->getObjectID(), -1));
									}
								}
							} else {
								object->notifyInsert(cobj.get());
							}

						}
					}
				}
			}
		}

	} catch (Exception& e) {
		error(e.getMessage());
		e.printStackTrace();
	}

	if (zone != nullptr) {
		delete _locker;

		if (object->isTangibleObject()) {
			TangibleObject* tano = object->asTangibleObject();
		   // zone->updateActiveAreas(tano);
		}
	}
	//info("Added: " + object->getDisplayedName(), true);
	//TangibleObjectImplementation::notifyObjectInsertedToChild(object, child, oldParent);

	return 0;
}

int ShipObjectImplementation::notifyObjectRemovedFromChild(SceneObject* object, SceneObject* child) {
	TangibleObjectImplementation::notifyObjectRemovedFromChild(object, child);
	//info("Removed: " + object->getDisplayedName(), true);
	/*SceneObject* parent = sceneObject->getParent();
	Zone* zone = sceneObject->getZone();

	if (!parent->isCellObject())
		return;

	if (building != parent->getParent()) {
		error("removing from wrong building object");
		return;
	}

	sceneObject->broadcastMessage(sceneObject->link((uint64)0, (uint32)0xFFFFFFFF), true, false);*/

	//parent->removeObject(sceneObject, false);


	//remove(object);

	//		building->removeNotifiedSentObject(sceneObject);


	return 0;
}

void ShipObjectImplementation::damageArmor(float damage, DeltaMessage* delta) {
	// Assume damage > total armor
	float armor0 = getCurrentArmorMap()->get(Components::ARMOR0);
	float armor1 = getCurrentArmorMap()->get(Components::ARMOR1);
	float half = damage * 0.5f;
	armor0 -= damage;
	bool hasArmor0 = armor0 != 0;
	bool hasArmor1 = armor1 != 0;
	float remaining = 0;
	armor0 -= half;

	if (armor0 < 0) {
		remaining = -armor0;
		armor0 = 0;
	}

	armor1 -= remaining + half;

	if (hasArmor0) {
		setComponentArmor(Components::ARMOR0, armor0, delta);
	}

	if (hasArmor1) {
		setComponentArmor(Components::ARMOR1, armor1, delta);
	}
}

void ShipObjectImplementation::doRecovery(int mselapsed) {
	if (getSpaceZone() == nullptr)
		return;

	float deltaTime = mselapsed / 1000.0f;
	bool reschedule = false;

	ShipObject* ship = _this.getReferenceUnsafeStaticCast();

	if (ship == nullptr)
		return;

	//info(true) << "ShipObjectImplementation::doRecovery -- called";

	// Ship is locked
	Locker lock(ship);

	CreatureObject* strongOwner = owner.get();

	for (const auto& entry : components) {
		int slot = entry.getKey();
		ShipComponent* component = entry.getValue();

		if (component == nullptr)
			continue;

		// Component crosslocked to ship
		Locker clock(component, ship);

		switch (slot) {
			case Components::SHIELD0:
			case Components::SHIELD1:
				break;
			case Components::REACTOR: {
				break;
			}
			case Components::ENGINE:
				break;
			case Components::CAPACITOR: {
				float amount = getCapacitorRechargeRate() * deltaTime;
				float currentEnergy = getCapacitorEnergy();
				float capMax = getCapacitorMaxEnergy();

				setCapacitorEnergy(Math::min(currentEnergy + amount, capMax), true);

				if (getCapacitorEnergy() < capMax) {
					reschedule = true;
				}

				break;
			}
			case Components::BOOSTER: {
				if (isBoosterActive()) {
					int time = boostTimer.miliDifference() / 1000.f;

					// TODO: Fix these values when component data loading is functional
					float compEfficiency = 1.0f; //getComponentEnergyEfficiency(Components::BOOSTER);
					float rate = 150.0f; //getBoosterConsumptionRate();

					float consumedAmount = (rate * time) * compEfficiency;
					float currentEnergy = Math::max(0.0f, getBoosterEnergy() - consumedAmount);

					//info(true) << "Time: " << time << " Consumed Amount: " << consumedAmount << " Current Energy: " << currentEnergy << " Consumption Rate: " << rate << " Component Efficiency = " << compEfficiency;

					if (currentEnergy > 0) {
						setBoosterEnergy(currentEnergy, true);
					} else {
						ship->removeComponentFlag(Components::BOOSTER, ShipComponentFlag::DISABLED, true);

						if (strongOwner != nullptr) {
							StringIdChatParameter param;
							param.setStringId("@space/space_interaction:booster_energy_depleted");
							strongOwner->sendSystemMessage(param);
						}

						ship->restartBooster();
					}

					reschedule = true;
				} else {
					float amount = getBoosterRechargeRate() * deltaTime;
					float cur = getBoosterEnergy();
					float boosterMax = getBoosterMaxEnergy();

					setBoosterEnergy(Math::min(cur + amount, boosterMax), true);
				}
			}
		}
	}

	// Update current max speed
	float calculateSpeed = getActualSpeed();

	if (getCurrentSpeed() != calculateSpeed) {
		DeltaMessage* message6 = new DeltaMessage(ship->getObjectID(), 'SHIP', 6);
		setCurrentSpeed(calculateSpeed, true, message6);
		message6->close();
		broadcastMessage(message6, true);
	}

	scheduleRecovery();
}

void ShipObjectImplementation::scheduleRecovery() {
	if (shipRecoveryEvent != nullptr && !shipRecoveryEvent->isScheduled())
		shipRecoveryEvent->schedule(1000);
}

void ShipObjectImplementation::cancelRecovery() {
	if (shipRecoveryEvent != nullptr && shipRecoveryEvent->isScheduled())
		shipRecoveryEvent->cancel();
}

bool ShipObject::isShipObject() {
	return true;
}

bool ShipObjectImplementation::isShipObject() {
	return true;
}

void ShipObjectImplementation::addComponentFlag(uint32 slot, uint32 bit, bool notifyClient) {
	if (slot == -1)
		return;

	ShipComponent* component = getComponentObject(slot);

	if (component == nullptr)
		return;

	Locker lock(component);

	if (notifyClient) {
		DeltaMessage* message = new DeltaMessage(getObjectID(), 'SHIP', 3);

		setComponentOptions(slot, bit, message);
		message->close();

		broadcastMessage(message, true);
	}
}

void ShipObjectImplementation::removeComponentFlag(uint32 slot, uint32 bit, bool notifyClient) {
	if (slot == -1)
		return;

	ShipComponent* component = getComponentObject(slot);

	if (component == nullptr)
		return;

	Locker lock(component);

	if (notifyClient) {
		DeltaMessage* message = new DeltaMessage(getObjectID(), 'SHIP', 3);

		setComponentOptions(slot, 0, message);
		message->close();

		broadcastMessage(message, true);
	}
}

void ShipObjectImplementation::restartBooster() {
	float boosterMax = getBoosterMaxSpeed();
	float deceleration = getShipDecelerationRate();
	float boostTime = 0.0f;

	if (deceleration > 0.0f) {
		boostTime = boosterMax / deceleration;
	}

	boostTime = Math::clamp(2.0f, boostTime, 10.0f);

	boostTimer.updateToCurrentTime();
	boostTimer.addMiliTime(boostTime * 1000);
}

void ShipObjectImplementation::setHyperspaceDelay() {
	int delay = HYPERSPACE_DELAY;

	ManagedReference<CreatureObject*> player = owner.get();

	if (player != nullptr && player->isPlayerCreature()) {
		PlayerObject* ghost = player->getPlayerObject();

		if (ghost != nullptr && ghost->hasAbility("space_navigator"))
			delay = NAVIGATOR_DELAY;
	}

	hyperspaceTime.updateToCurrentTime();
	hyperspaceTime.addMiliTime(delay * 1000);
}

int ShipObjectImplementation::getHyperspaceDelay() {
	Time now;
	return (now.miliDifference() - hyperspaceTime.miliDifference()) / 1000;
}

float ShipObjectImplementation::getActualSpeed() {
	const auto componentMap = getShipComponentMap();

	float componentActual = 0.f;

	if (componentMap->get(Components::ENGINE) != 0) {
		float efficiency = getComponentEfficiencyMap()->get(Components::ENGINE);
		componentActual += maxSpeed * efficiency;
	}

	if (componentMap->get(Components::BOOSTER) != 0 && isBoosterActive()) {
		float efficiency = getComponentEfficiencyMap()->get(Components::BOOSTER);
		componentActual += boosterMaxSpeed * efficiency;
	}

	float chassisActual = 1.f;

	if (chassisSpeed != 0) {
		chassisActual = chassisSpeed;
	}

	if (hasShipWings() && (getOptionsBitmask() & OptionBitmask::WINGS_OPEN)) {
		const auto chassisData = ShipManager::instance()->getChassisData(getShipName());

		if (chassisData != nullptr) {
			chassisActual *= chassisData->getWingOpenSpeed();
		}
	}

	return componentActual * chassisActual;
}

bool ShipObjectImplementation::checkInConvoRange(SceneObject* targetObject) {
	if (targetObject == nullptr)
		return false;

	int sqDistance = getWorldPosition().squaredDistanceTo(targetObject->getWorldPosition());

	if (targetObject->isSpaceStationObject()) {
		int maxSqDistance = SPACESTATION_COMM_MAX_DISTANCE * SPACESTATION_COMM_MAX_DISTANCE;

		if (sqDistance < maxSqDistance)
			return true;
	}

	return false;
}

float ShipObjectImplementation::calculateCurrentMass() {
	float currentMass = 0;

	for (int i = 0; i < componentMass.size(); ++i) {
		auto value = componentMass.getValueAt(i);

		if (value > 0.f) {
			currentMass += value;
		}
	}

	return currentMass;
}

float ShipObjectImplementation::calculateCurrentEnergyCost() {
	float energyCost = 0;

	for (int i = 0; i < componentEnergyCost.size(); ++i) {
		auto value = componentEnergyCost.getValueAt(i);

		if (value > 0.f) {
			energyCost += value;
		}
	}

	return energyCost;
}

void ShipObjectImplementation::setShipFaction(uint32 value, bool notifyClient) {
	TangibleObjectImplementation::setFaction(value);
	String faction = "";

	if (value == Factions::FACTIONREBEL) {
		faction = "rebel";
	} else if (value == Factions::FACTIONIMPERIAL) {
		faction = "imperial";
	}

	setShipFaction(faction, notifyClient);
}
