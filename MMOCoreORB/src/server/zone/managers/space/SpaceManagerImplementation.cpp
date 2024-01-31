//
// Created by g on 12/15/17.
//

#include "server/zone/SpaceZone.h"
#include "server/zone/ZoneServer.h"
#include "server/zone/ZoneProcessServer.h"
#include "server/zone/managers/collision/CollisionManager.h"
#include "server/zone/managers/object/ObjectManager.h"
#include "server/zone/managers/space/SpaceManager.h"
#include "server/zone/managers/object/ObjectManager.h"
#include "server/zone/objects/ship/ai/ShipAiAgent.h"
#include "conf/ConfigManager.h"
#include "templates/datatables/DataTableIff.h"
#include "templates/datatables/DataTableRow.h"
#include "server/zone/managers/ship/ShipManager.h"
#include "templates/faction/Factions.h"
#include "server/zone/objects/area/space/SpaceActiveArea.h"
#include "server/zone/objects/area/areashapes/SphereAreaShape.h"
#include "server/zone/objects/area/areashapes/CuboidAreaShape.h"
#include "server/zone/objects/area/space/NebulaArea.h"
#include "server/zone/packets/jtl/CreateNebulaLightningMessage.h"

void SpaceManagerImplementation::loadLuaConfig() {
	String planetName = spaceZone->getZoneName();

	Lua* lua = new Lua();
	lua->init();

	lua->runFile("scripts/managers/space_manager.lua");

	// Get's the configuration settings object for this planet.
	LuaObject base = lua->getGlobalObject(planetName);

	if (base.isValidTable()) {
		LuaObject luaObject = base.getObjectField("spaceStations");

		if (!luaObject.isValidTable())
			return;

		for (int i = 1; i <= luaObject.getTableSize(); ++i) {
			lua_State* L = luaObject.getLuaState();
			lua_rawgeti(L, -1, i);

			LuaObject stationObject(L);

			String templateFile = stationObject.getStringField("templateFile");

			auto shipTemp = dynamic_cast<SharedShipObjectTemplate*>(TemplateManager::instance()->getTemplate(templateFile.hashCode()));

			if (shipTemp == nullptr)
				continue;

			ManagedReference<ShipAiAgent*> shipAgent = ShipManager::instance()->createAiShip(templateFile);

			if (shipAgent == nullptr)
				continue;

			Locker shipLocker(shipAgent);

			float x = stationObject.getFloatField("x");
			float y = stationObject.getFloatField("y");
			float z = stationObject.getFloatField("z");
			float ox = stationObject.getFloatField("ox");
			float oy = stationObject.getFloatField("oy");
			float oz = stationObject.getFloatField("oz");
			float ow = stationObject.getFloatField("ow");

			uint64 parentID = stationObject.getLongField("parent");

			Quaternion direction(ow, ox, oy, oz);
			direction.normalize();

			shipAgent->initializePosition(x, z, y);
			shipAgent->setDirection(direction);

			// Transfer into the zone
			spaceZone->transferObject(shipAgent, -1, true);

			shipAgent->createChildObjects();

			shipAgent->setRotationMatrix(direction);

			String faction = shipAgent->getShipFaction();

			if (faction.isEmpty() || !spaceStationMap.contains(faction)) {
				faction = "neutral";

				shipAgent->setFaction(Factions::FACTIONNEUTRAL);
				shipAgent->setOptionBit(OptionBitmask::INVULNERABLE, false);
			}

			uint64 stationID = shipAgent->getObjectID();
			Vector3 stationPosition = shipAgent->getPosition();

			spaceStationMap.get(faction).put(stationID, stationPosition);


			// info(true) << "SpaceStation Added: " << shipAgent->getDisplayedName() << " Location: " + shipAgent->getPosition().toString();

			stationObject.pop();
		}

		try {
			LuaObject travelPoints = luaObject.getObjectField("jtlTravelPoints");
			loadJTLData(&travelPoints);
			travelPoints.pop();

			LuaObject launchLocation = luaObject.getObjectField("jtlLaunchPoint");
			if (!launchLocation.isValidTable())
				return;

			jtlZoneName = launchLocation.getStringAt(1);
			float x = launchLocation.getFloatAt(2);
			float z = launchLocation.getFloatAt(3);
			float y = launchLocation.getFloatAt(4);

			jtlLaunchLocation = Vector3(x, y, z);
			launchLocation.pop();
		} catch (Exception &e) {
			error(e.getMessage());
		}
	} else {
		warning("Space configuration settings not found.");
	}
}

void SpaceManagerImplementation::initialize() {
	VectorMap<uint64, Vector3> stationMap;
	stationMap.setNoDuplicateInsertPlan();
	stationMap.setNullValue(Vector3::ZERO);

	spaceStationMap.put("rebel", stationMap);
	spaceStationMap.put("neutral", stationMap);
	spaceStationMap.put("imperial", stationMap);

	info(true) << "loading space manager " << spaceZone->getZoneName();

	loadLuaConfig();
	loadNebulaAreas();
}

void SpaceManagerImplementation::loadJTLData(LuaObject* luaObject) {
	if (!luaObject->isValidTable())
		return;

	for (int i = 1; i <= luaObject->getTableSize(); ++i) {
		lua_State* L = luaObject->getLuaState();
		lua_rawgeti(L, -1, i);

		LuaObject location(L);

		String locationName = location.getStringAt(1);
		float x = location.getFloatAt(2);
		float z = location.getFloatAt(3);
		float y = location.getFloatAt(4);

		jtlTravelDestinations.put(locationName, Vector3(x, y, z));
		location.pop();
	}
}

void SpaceManagerImplementation::initializeTransientMembers() {
	ManagedObjectImplementation::initializeTransientMembers();
}

void SpaceManagerImplementation::finalize() {
	spaceZone = nullptr;
	server = nullptr;
}

void SpaceManagerImplementation::start() {
}

void SpaceManagerImplementation::loadNebulaAreas() {
	auto zoneServer = spaceZone->getZoneServer();

	if (zoneServer == nullptr)
		return;

	TemplateManager* templateManager = TemplateManager::instance();

	if (templateManager == nullptr)
		return;

	String zoneName = spaceZone->getZoneName();

	// No Nebulas in space_heavy1
	if (zoneName == "space_heavy1")
		return;

	IffStream* iffStream = templateManager->openIffFile("datatables/space/nebula/" + zoneName + ".iff");

	if (iffStream == nullptr) {
		info(true) << "Nebula table for " << zoneName << " could not be found.";
		return;
	}

	DataTableIff dtiff;
	dtiff.readObject(iffStream);

	// Declare variables
	String name, ambientSound, lightningAppearance, lightningSound, lightningSoundLoop, lightningHitEffectClient, lightningHitEffectServer;
	float x, z, y, radius, density, lightningDamageMin, lightningDamageMax, lightningFrequency, lightningDurationMax;
	StringBuffer nebulaName;

	for (int i = 0; i < dtiff.getTotalRows(); ++i) {
		DataTableRow* row = dtiff.getRow(i);

		row->getCell(0)->getValue(name);
		row->getCell(1)->getValue(x);
		row->getCell(2)->getValue(z);
		row->getCell(3)->getValue(y);
		row->getCell(4)->getValue(radius);
		row->getCell(5)->getValue(density);

		row->getCell(23)->getValue(ambientSound);
		row->getCell(29)->getValue(lightningFrequency);
		row->getCell(30)->getValue(lightningDurationMax);
		row->getCell(31)->getValue(lightningDamageMin);
		row->getCell(32)->getValue(lightningDamageMax);
		row->getCell(33)->getValue(lightningAppearance);
		row->getCell(42)->getValue(lightningSound);
		row->getCell(43)->getValue(lightningSoundLoop);
		row->getCell(44)->getValue(lightningHitEffectClient);
		row->getCell(45)->getValue(lightningHitEffectServer);

		// Create new nebula area
		ManagedReference<NebulaArea*> nebulaArea = (zoneServer->createObject(STRING_HASHCODE("object/nebula_area.iff"), 0)).castTo<NebulaArea*>();

		if (nebulaArea == nullptr)
			continue;

		Locker areaLock(nebulaArea);

		// Create sphere active area shape
		ManagedReference<SphereAreaShape*> sphereArea = new SphereAreaShape();

		if (sphereArea == nullptr)
			continue;

		Locker shapeLocker(sphereArea, nebulaArea);

		sphereArea->setAreaCenter(x, z, y);
		sphereArea->setRadius(radius);

		shapeLocker.release();

		nebulaName << zoneName << "_nebula_" << name;
		nebulaArea->setAreaName(nebulaName.toString());
		nebulaName.deleteAll();

		nebulaArea->setAreaShape(sphereArea);

		nebulaArea->setNebulaDensity(density);
		nebulaArea->setAmbientSound(ambientSound);
		nebulaArea->setLightningFrequency(lightningFrequency);
		nebulaArea->setLightningDurationMax(lightningDurationMax);
		nebulaArea->setLightningDamageMin(lightningDamageMin);
		nebulaArea->setLightningDamageMax(lightningDamageMax);
		nebulaArea->setLightningAppearance(lightningAppearance);
		nebulaArea->setLightningSound(lightningSound);
		nebulaArea->setLightningSoundLoop(lightningSoundLoop);
		nebulaArea->setLightningHitEffectClient(lightningHitEffectClient);
		nebulaArea->setLightningHitEffectServer(lightningHitEffectServer);

		nebulaArea->setNebulaID(i + 1);
		nebulaArea->setRadius(radius);

		/*
		StringBuffer debugMsg;
		debugMsg << zoneName << "_nebula_" << name << " - X = " << x << " Z = " << z << " Y = " << y << " Radius = " << radius;
		debugMsg << "Ambient Sound: " << ambientSound << " Lightning Frequencey: " << lightningFrequency << " lightningDurationMax: " << lightningDurationMax;
		debugMsg << " Lightning Damage Min: " << lightningDamageMin << " Lightning Damage Max: " << lightningDamageMax << " Lightning Appearance: " << lightningAppearance;
		debugMsg << " Lightning Sound: " << lightningSound << " Lightning Sound Loop: " << lightningSoundLoop << " Lightning Hit Effect Client: " << lightningHitEffectClient << " Lightning Hit Effect Server: " << lightningHitEffectServer;
		info(true) << debugMsg.toString();
		*/

		// Initialize nebula position
		nebulaArea->initializePosition(x, z, y);

		spaceZone->transferObject(nebulaArea, -1, true);
	}

	delete iffStream;
}

Vector3 SpaceManagerImplementation::getJtlLaunchLocationss() {
	return jtlLaunchLocation;
}

Vector3 SpaceManagerImplementation::getClosestSpaceStationPosition(const Vector3& shipPosition, const String& shipFaction) {
	uint64 objectID = getClosestSpaceStationObjectID(shipPosition, shipFaction);

	if (objectID == 0) {
		return shipPosition;
	}

	return spaceStationMap.get(shipFaction).get(objectID);
}

uint64 SpaceManagerImplementation::getClosestSpaceStationObjectID(const Vector3& shipPosition, const String& shipFaction) {
	uint64 stationObjectID = 0;
	float stationDistance = FLT_MAX;

	for (int i = 0; i < spaceStationMap.size(); ++i) {
		const auto& stationKey = spaceStationMap.elementAt(i).getKey();
		const auto& stationMap = spaceStationMap.elementAt(i).getValue();

		if (shipFaction != stationKey) {
			continue;
		}

		for (int ii = 0; ii < stationMap.size(); ++ii) {
			const auto& entryPosition = stationMap.elementAt(ii).getValue();
			float sqrDistance = shipPosition.squaredDistanceTo(entryPosition);

			if (stationDistance > sqrDistance) {
				stationDistance = sqrDistance;
				stationObjectID = stationMap.elementAt(ii).getKey();
			}
		}
	}

	return stationObjectID;
}

void SpaceManagerImplementation::broadcastNebulaLightning(ShipObject* ship, const Vector3& nebulaCenter, uint16 lightningCount, int nebulaID, int startMili, int endMili, const Vector3& startPoint, const Vector3& endPoint) {
	if (ship == nullptr)
		return;

	CloseObjectsVector* closeObjects = ship->getCloseObjects();
	SortedVector<ManagedReference<TreeEntry*>>closeObjectsCopy(200, 50);

	if (closeObjects != nullptr) {
		closeObjects->safeCopyReceiversTo(closeObjectsCopy, CloseObjectsVector::PLAYERTYPE);
	} else {
		spaceZone->getInRangeObjects(nebulaCenter.getX(), nebulaCenter.getZ(), nebulaCenter.getY(), spaceZone->getZoneObjectRange(), &closeObjectsCopy, true);
	}

	try {
		for (int i = 0; i < closeObjectsCopy.size(); ++i) {
			auto playerEntry = static_cast<SceneObject*>(closeObjectsCopy.getUnsafe(i).get());

			if (playerEntry == nullptr || !playerEntry->isPlayerCreature()) {
				continue;
			}

			uint32 syncStamp = playerEntry->getSyncStamp();

			auto lightningMessage = new CreateNebulaLightningMessage(lightningCount, nebulaID, startMili + syncStamp, endMili + syncStamp, startPoint, endPoint);
			playerEntry->sendMessage(lightningMessage);
		}
	} catch (...) {
		throw;
	}
}
