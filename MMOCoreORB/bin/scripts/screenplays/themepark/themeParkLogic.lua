require("screenplays.screenplay")
local ObjectManager = require("managers.object.object_manager")

WAYPOINT_COLOR_PURPLE = 5
SIT = 1
STAND = 0
FACTIONIMPERIAL = 0xDB4ACC54
FACTIONREBEL = 0x16148850

ThemeParkLogic = ScreenPlay:new {
	numberOfActs = 1,
	npcMap = {},
	waypointMap = {},
	permissionMap = {},
	sceneObjectMap = {},
	genericGiver = false,
	className = "ThemeParkLogic",
	screenPlayState = "theme_park_general",
	distance = 1000,
	missionDescriptionStf = "",
	missionCompletionMessageStf = "",
	faction = 0
}

function ThemeParkLogic:start()
	self:spawnNpcs()
	self:spawnSceneObjects()
	self:permissionObservers()
end

function ThemeParkLogic:spawnNpcs()
	if (self.genericGiver) then
		return
	end

	for i = 1, # self.npcMap do
		local npcSpawnData = self.npcMap[i].spawnData
		if isZoneEnabled(npcSpawnData.planetName) then
			local pNpc = spawnMobile(npcSpawnData.planetName, npcSpawnData.npcTemplate, 1, npcSpawnData.x, npcSpawnData.z, npcSpawnData.y, npcSpawnData.direction, npcSpawnData.cellID)
			if pNpc ~= nil and npcSpawnData.position == SIT then
				CreatureObject(pNpc):setState(STATESITTINGONCHAIR)
			end
			if (pNpc ~= nil and npcSpawnData.mood ~= nil and npcSpawnData.mood ~= "") then
				CreatureObject(pNpc):setMoodString(npcSpawnData.mood)
			end
		end
	end
end

function ThemeParkLogic:spawnSceneObjects()
	if (self.genericGiver) then
		return
	end

	if self.sceneObjectMap ~= nil then
		for i = 1, # self.sceneObjectMap do
			local objectSpawnData = self.sceneObjectMap[i].spawnData
			if isZoneEnabled(objectSpawnData.planetName) then
				local pObject = spawnSceneObject(objectSpawnData.planetName, objectSpawnData.objectTemplate, objectSpawnData.x, objectSpawnData.z, objectSpawnData.y, objectSpawnData.cellID, objectSpawnData.dw, objectSpawnData.dx, objectSpawnData.dy, objectSpawnData.dz)
				if pObject ~= nil and self.sceneObjectMap[i].customObjectName ~= nil and self.sceneObjectMap[i].customObjectName ~= "" then
					SceneObject(pObject):setCustomObjectName(self.sceneObjectMap[i].customObjectName)
				end
				if pObject ~= nil and self.sceneObjectMap[i].objectIdLabel ~= nil and self.sceneObjectMap[i].objectIdLabel ~= "" then
					writeData(self.sceneObjectMap[i].objectIdLabel, SceneObject(pObject):getObjectID())
				end
			end
		end
	end
end

function ThemeParkLogic:permissionObservers()
	for i = 1, # self.permissionMap, 1 do
		local permission = self.permissionMap[i]
		self:setupPermissionGroups(permission)
		local pRegion = getRegion(permission.planetName, permission.regionName)
		if pRegion ~= nil then
			createObserver(ENTEREDAREA, self.className, "cellPermissionsObserver", pRegion)
		end
	end
end

function ThemeParkLogic:setupPermissionGroups(permission)
	for i = 1, #permission.permissions, 1 do
		local thisPermission = permission.permissions[i]
		for j = 1, #thisPermission.cells, 1 do
			local pCell = getSceneObject(thisPermission.cells[j])
			if pCell ~= nil then
				local cell = SceneObject(pCell)
				cell:setContainerInheritPermissionsFromParent(false)
				cell:clearContainerDefaultDenyPermission(WALKIN)
				cell:clearContainerDefaultAllowPermission(WALKIN)
				cell:setContainerAllowPermission(permission.regionName .. i, WALKIN)
				cell:setContainerDenyPermission(permission.regionName .. i, MOVEIN)
			end
		end
	end
end

function ThemeParkLogic:cellPermissionsObserver(pRegion, pCreature)
	if pRegion == nil or pCreature == nil then
		return 0
	end

	if SceneObject(pCreature):isCreatureObject() then
		for i = 1, # self.permissionMap, 1 do
			if self.permissionMap[i].regionName == SceneObject(pRegion):getObjectName() then
				self:setCellPermissions(self.permissionMap[i], pCreature)
			end
		end
	end

	return 0
end

function ThemeParkLogic:setCellPermissions(permissions, pCreature)
	ObjectManager.withCreaturePlayerObject(pCreature, function(ghost)
		for i = 1, # permissions.permissions, 1 do
			if self:hasPermission(permissions.permissions[i].conditions, pCreature) == true or ghost:hasGodMode() then
				ghost:addPermissionGroup(permissions.regionName .. i, true)
			else
				ghost:removePermissionGroup(permissions.regionName .. i, true)
			end
		end
	end)
end

function ThemeParkLogic:hasFullInventory(pPlayer)
	if (pPlayer == nil) then
		return true
	end

	local pInventory = SceneObject(pPlayer):getSlottedObject("inventory")

	if (pInventory == nil) then
		return true
	end

	return SceneObject(pInventory):isContainerFullRecursive()
end

function ThemeParkLogic:hasPermission(conditions, pCreature)
	if (pCreature == nil) then
		return false
	end

	for i = 1, # conditions, 1 do
		if conditions[i].permissionType == "faction" then
			if conditions[i].faction ~= nil then
				if self:isInFaction(conditions[i].faction, pCreature) == false then
					return false
				end
			else
				if self:isInFaction(self.faction, pCreature) == false then
					return false
				end
			end
		elseif conditions[i].permissionType == "missionState" then
			if self:hasMissionState(conditions[i].mission, conditions[i].missionState, pCreature) == false then
				return false
			end
		elseif conditions[i].permissionType == "npcState" then
			if self:hasNpcMissionState(conditions[i].npcState, conditions[i].state, pCreature) == false then
				return false
			end
		end
	end
	return true
end

function ThemeParkLogic:isInFaction(faction, pCreature)
	if pCreature == nil then
		return false
	end

	return CreatureObject(pCreature):getFaction() == faction
end

function ThemeParkLogic:isOnLeave(pPlayer)
	if (pPlayer == nil) then
		return false
	end

	return ObjectManager.withCreaturePlayerObject(pPlayer, function(player)
		return player:isOnLeave()
	end)
end

function ThemeParkLogic:isValidConvoString(stfFile, stringid)
	return getStringId(stfFile .. stringid) ~= ""
end

function ThemeParkLogic:getGlobalFaction()
	if self.faction == nil then
		return 0
	end

	return self.faction
end

function ThemeParkLogic:getMissionFaction(npcNumber, missionNumber)
	if (self.genericGiver) then
		return 0
	end

	local mission = self:getMission(npcNumber, missionNumber)
	local npcData = self:getNpcData(npcNumber)

	if (mission == nil or mission.faction == nil) and (npcData == nil or npcData.faction == nil) then
		return 0
	end

	if mission ~= nil and mission.faction ~= nil then
		return mission.faction
	else
		return npcData.faction
	end
end

function ThemeParkLogic:getNpcFaction(npcNumber)
	local npcData = self:getNpcData(npcNumber)

	if npcData == nil or npcData.faction == nil then
		return 0
	end

	return npcData.faction
end

function ThemeParkLogic:hasMissionState(mission, missionState, pCreature)
	if pCreature == nil then
		return false
	end

	local npcNumber = self:getActiveNpcNumber(pCreature)

	if npcNumber > missionState then
		return true
	else
		return false
	end
end

function ThemeParkLogic:hasNpcMissionState(npcState, state, pCreature)
	if (pCreature == nil) then
		return false
	end

	return CreatureObject(pCreature):hasScreenPlayState(state, npcState)
end

function ThemeParkLogic:getNpcNumber(pNpc)
	if (pNpc == nil) then
		return 1
	end

	if (self.genericGiver) then
		return 1
	end

	local x = CreatureObject(pNpc):getPositionX()
	local y = CreatureObject(pNpc):getPositionY()
	local z = CreatureObject(pNpc):getPositionZ()

	for i = 1, # self.npcMap do
		local npcData = self.npcMap[i].spawnData
		if (math.abs(x - npcData.x) < 0.1) and (math.abs(z - npcData.z) < 0.1) and (math.abs(y - npcData.y) < 0.1) then
			return self.npcMap[i].npcNumber
		end
	end

	return 1
end

function ThemeParkLogic:getNpcData(npcNumber)
	for i = 1, # self.npcMap do
		if npcNumber == self.npcMap[i].npcNumber then
			return self.npcMap[i]
		end
	end

	return nil
end

function ThemeParkLogic:getActiveNpcNumber(pConversingPlayer)
	if (pConversingPlayer == nil) then
		return 1
	end

	local npcNumber = 1
	local activeNpcNumber = 1

	if (self.genericGiver) then
		return 1
	end

	for i = 1, 10, 1 do
		if (CreatureObject(pConversingPlayer):hasScreenPlayState(npcNumber, self.screenPlayState)) then
			activeNpcNumber = npcNumber * 2
		end
		npcNumber = npcNumber * 2
	end

	return activeNpcNumber
end

function ThemeParkLogic:activeNpc(pConversingPlayer, pConversingNpc)
	if pConversingPlayer == nil then
		return -1
	end

	local npcNumber = self:getNpcNumber(pConversingNpc)
	local activeNpcNumber = self:getActiveNpcNumber(pConversingPlayer)
	return npcNumber - activeNpcNumber
end

function ThemeParkLogic:getCurrentMissionNumber(npcNumber, pConversingPlayer)
	if pConversingPlayer == nil then
		return 0
	end

	local npcData = self:getNpcData(npcNumber)
	if npcData == nil then
		return 0
	end

	if (self.genericGiver) then
		return readData(CreatureObject(pConversingPlayer):getObjectID() .. ":generic_mission_number")
	end

	local npcName = npcData.spawnData.npcTemplate
	local numberOfMissionsTotal = table.getn(npcData.missions)

	local missionsCompleted = 0
	local stateToCheck = 1
	for i = 1, numberOfMissionsTotal, 1 do
		if (CreatureObject(pConversingPlayer):hasScreenPlayState(stateToCheck, self.screenPlayState .. "_mission_" .. npcName)) then
			stateToCheck = stateToCheck * 2
			missionsCompleted = missionsCompleted + 1
		end
	end

	return missionsCompleted + 1
end

function ThemeParkLogic:missionStatus(pConversingPlayer)
	if pConversingPlayer == nil then
		return 0
	end

	return readData(CreatureObject(pConversingPlayer):getObjectID() .. ":activeMission")
end

function ThemeParkLogic:getStfFile(npcNumber)
	local npcData = self:getNpcData(npcNumber)

	if npcData == nil then
		return ""
	end

	return npcData.stfFile
end

function ThemeParkLogic:handleMissionAccept(npcNumber, missionNumber, pConversingPlayer)
	local mission = self:getMission(npcNumber, missionNumber)

	if mission == nil or pConversingPlayer == nil then
		return false
	end

	writeStringData(CreatureObject(pConversingPlayer):getObjectID() .. ":activeScreenPlay", self.className)

	if mission.missionType == "deliver" then
		return self:handleDeliverMissionAccept(mission, pConversingPlayer, missionNumber)
	elseif mission.missionType == "escort" then
		return self:handleEscortMissionAccept(mission, pConversingPlayer, missionNumber)
	elseif mission.missionType == "retrieve" then
		return self:handleRetrieveMissionAccept(mission, pConversingPlayer, missionNumber)
	elseif mission.missionType == "assassinate" then
		return self:handleAssassinateMissionAccept(mission, pConversingPlayer, missionNumber)
	elseif mission.missionType == "confiscate" then
		return self:handleConfiscateMissionAccept(mission, pConversingPlayer, missionNumber)
	elseif mission.missionType == "destroy" then
		return self:handleDestroyMissionAccept(mission, pConversingPlayer, missionNumber)
	elseif mission.missionType == "hunt" then
		return self:handleHuntMissionAccept(mission, pConversingPlayer, missionNumber)
	end
end

function ThemeParkLogic:writeData(pConversingPlayer, dataName, value)
	if pConversingPlayer == nil then
		return
	end

	writeData(CreatureObject(pConversingPlayer):getObjectID() .. dataName, value)
end

function ThemeParkLogic:getMission(npcNumber, missionNumber)
	local npcData = self:getNpcData(npcNumber)

	if (npcData == nil) then
		printf("null npcData in ThemeParkLogic:getMission for %s", self.className);
		return nil
	end

	local missions = npcData.missions

	return missions[missionNumber]
end

function ThemeParkLogic:handleDeliverMissionAccept(mission, pConversingPlayer, missionNumber)
	if (pConversingPlayer == nil) then
		return false
	end

	if self:spawnMissionNpcs(mission, pConversingPlayer) == true then
		self:giveMissionItems(mission, pConversingPlayer)
		return true
	else
		return false
	end
end

function ThemeParkLogic:handleRetrieveMissionAccept(mission, pConversingPlayer, missionNumber)
	if (pConversingPlayer == nil) then
		return false
	end

	if self:spawnMissionNpcs(mission, pConversingPlayer) == true then
		self:writeData(pConversingPlayer, ":activeMission", 1)
		return true
	else
		return false
	end
end

function ThemeParkLogic:handleEscortMissionAccept(mission, pConversingPlayer, missionNumber)
	if (pConversingPlayer == nil) then
		return false
	end

	if self:spawnMissionNpcs(mission, pConversingPlayer) == true then
		self:writeData(pConversingPlayer, ":activeMission", 1)
		return true
	else
		return false
	end
end

function ThemeParkLogic:handleDestroyMissionAccept(mission, pConversingPlayer, missionNumber)
	if (pConversingPlayer == nil) then
		return false
	end

	if self:spawnDestroyBuilding(mission, pConversingPlayer) == true then
		self:spawnMissionNpcs(mission, pConversingPlayer)
		self:writeData(pConversingPlayer, ":activeMission", 1)
		return true
	else
		return false
	end
end

function ThemeParkLogic:handleAssassinateMissionAccept(mission, pConversingPlayer, missionNumber)
	if (pConversingPlayer == nil) then
		return false
	end

	if self:spawnMissionNpcs(mission, pConversingPlayer) == true then
		self:writeData(pConversingPlayer, ":activeMission", 1)
		self:writeData(pConversingPlayer, ":killedMissionNpcs", 0)
		return true
	else
		return false
	end
end

function ThemeParkLogic:handleHuntMissionAccept(mission, pConversingPlayer, missionNumber)
	if (pConversingPlayer == nil) then
		return false
	end

	if self:startHuntMission(mission, pConversingPlayer) == true then
		self:writeData(pConversingPlayer, ":activeMission", 1)
		return true
	else
		return false
	end
end

function ThemeParkLogic:handleConfiscateMissionAccept(mission, pConversingPlayer, missionNumber)
	if (pConversingPlayer == nil) then
		return false
	end

	if self:spawnMissionNpcs(mission, pConversingPlayer) == true then
		self:writeData(pConversingPlayer, ":activeMission", 1)
		self:writeData(pConversingPlayer, ":requiredItemsLooted", 0)
		return true
	else
		return false
	end
end

function ThemeParkLogic:spawnDestroyBuilding(mission, pConversingPlayer)
	if pConversingPlayer == nil then
		return false
	end

	local npcNumber = self:getActiveNpcNumber(pConversingPlayer)
	local missionNumber = self:getCurrentMissionNumber(npcNumber, pConversingPlayer)
	local stfFile = self:getStfFile(npcNumber)

	local buildingData = mission.buildingSpawn

	if buildingData == nil then
		printf("null building data in ThemeParkLogic:spawnDestroyBuilding for %s", self.className);
		return false
	end

	local spawnDistance = self.distance

	local spawnPoint = getSpawnArea(SceneObject(pConversingPlayer):getZoneName(), SceneObject(pConversingPlayer):getWorldPositionX(), SceneObject(pConversingPlayer):getWorldPositionY(), spawnDistance, (spawnDistance/2)*3, 20, 5)
	local pBuilding = spawnBuilding(pConversingPlayer, buildingData.building.template, spawnPoint[1], spawnPoint[3], 0)

	if (pBuilding == nil) then
		return false
	end

	createObserver(OBJECTDESTRUCTION, self.className, "notifyDestroyedBuilding", pBuilding)

	local buildingCell = BuildingObject(pBuilding):getCell(buildingData.terminal.vectorCellID)

	if (buildingCell == nil) then
		return false
	end

	local pTerminal = spawnSceneObject(buildingData.building.planet, buildingData.terminal.template, buildingData.terminal.x, buildingData.terminal.z, buildingData.terminal.y, SceneObject(buildingCell):getObjectID(), 1, 0, 0, 0)

	if (pTerminal == nil) then
		return false
	end

	writeData(SceneObject(pConversingPlayer):getObjectID() .. ":destroyableBuildingID", SceneObject(pBuilding):getObjectID())
	self:updateWaypoint(pConversingPlayer, buildingData.building.planet, spawnPoint[1], spawnPoint[3], "target")
	self:spawnDestroyMissionNpcs(mission, pConversingPlayer)

	return true
end

function ThemeParkLogic:spawnMissionStaticObjects(mission, pConversingPlayer, x, y)
	if (pConversingPlayer == nil) then
		return false
	end

	local playerID = SceneObject(pConversingPlayer):getObjectID()
	local npcNumber = self:getActiveNpcNumber(pConversingPlayer)
	local missionNumber = self:getCurrentMissionNumber(npcNumber, pConversingPlayer)

	local numberOfSpawns = table.getn(mission.staticObjects)

	writeData(playerID .. ":missionStaticObjects", numberOfSpawns)

	for i = 1, numberOfSpawns, 1 do
		local spawnPoint = getSpawnPoint(mission.staticObjects[i].planetName, x, y, 5, 10)
		if spawnPoint ~= nil then
			local pObject = spawnSceneObject(mission.staticObjects[i].planetName, mission.staticObjects[i].objectTemplate, spawnPoint[1], spawnPoint[2], spawnPoint[3], 0, 0, 0, 0, 0)
			writeData(playerID .. ":missionStaticObject:no" .. i, SceneObject(pObject):getObjectID())
		end
	end
end

function ThemeParkLogic:startHuntMission(mission, pConversingPlayer)
	if pConversingPlayer == nil then
		return false
	end

	self:updateWaypoint(pConversingPlayer, mission.huntTarget.planetName, mission.huntTarget.waypointX, mission.huntTarget.waypointY, "target")
	createObserver(KILLEDCREATURE, self.className, "notifyKilledHuntTarget", pConversingPlayer)

	return true
end

function ThemeParkLogic:notifyKilledHuntTarget(pAttacker, pVictim)
	if pVictim == nil or pAttacker == nil then
		return 0
	end

	local npcNumber = self:getActiveNpcNumber(pAttacker)
	local missionNumber = self:getCurrentMissionNumber(npcNumber, pAttacker)
	local mission = self:getMission(npcNumber, missionNumber)

	if mission == nil then
		return 0
	end

	if (SceneObject(pVictim):getObjectName() == mission.huntTarget.npcTemplate) then
		self:completeMission(pAttacker)
		return 1
	end
	return 0
end

function ThemeParkLogic:spawnMissionNpcs(mission, pConversingPlayer)
	if pConversingPlayer == nil then
		return false
	end

	local playerID = SceneObject(pConversingPlayer):getObjectID()

	local npcNumber = self:getActiveNpcNumber(pConversingPlayer)
	local missionNumber = self:getCurrentMissionNumber(npcNumber, pConversingPlayer)
	local stfFile = self:getStfFile(npcNumber)
	local currentMissionType = self:getMissionType(npcNumber, pConversingPlayer)
	local spawnPoints

	local numberOfSpawns = table.getn(mission.primarySpawns) + table.getn(mission.secondarySpawns)

	if (currentMissionType == "destroy") then
		local buildingID = readData(playerID .. ":destroyableBuildingID")
		local pBuilding = getSceneObject(buildingID)

		if pBuilding == nil then
			return false
		end

		spawnPoints = self:getSpawnPoints(numberOfSpawns, SceneObject(pBuilding):getWorldPositionX(), SceneObject(pBuilding):getWorldPositionY(), pConversingPlayer)
	else
		spawnPoints = self:getSpawnPoints(numberOfSpawns, SceneObject(pConversingPlayer):getWorldPositionX(), SceneObject(pConversingPlayer):getWorldPositionY(), pConversingPlayer)
	end

	if table.getn(spawnPoints) ~= numberOfSpawns then
		return false
	end

	writeData(playerID .. ":missionSpawns", numberOfSpawns)

	local mainNpcs = mission.primarySpawns
	for i = 1, table.getn(mission.primarySpawns), 1 do
		local pNpc = self:spawnNpc(mainNpcs[i], spawnPoints[i], pConversingPlayer, i)

		local planetName
		if (mainNpcs[i].planetName == "generic") then
			planetName = CreatureObject(pConversingPlayer):getZoneName()
		else
			planetName = mainNpcs[i].planetName
		end

		if pNpc ~= nil and SceneObject(pNpc):isCreatureObject() then
			AiAgent(pNpc):setNoAiAggro()
			
			if i == 1 then
				if (self:isValidConvoString(stfFile, ":npc_breech_" .. missionNumber)) then
					local pBreechArea = spawnActiveArea(planetName, "object/active_area.iff", spawnPoints[i][1], spawnPoints[i][2], spawnPoints[i][3], 32, 0)
					if pBreechArea ~= nil then
						createObserver(ENTEREDAREA, self.className, "notifyEnteredBreechArea", pBreechArea)
						writeData(SceneObject(pNpc):getObjectID() .. ":missionOwnerID", playerID)
						writeData(playerID .. ":breechNpcID", SceneObject(pNpc):getObjectID())
						writeData(playerID .. ":breechAreaID", SceneObject(pBreechArea):getObjectID())
					end
				end
				if (currentMissionType ~= "destroy") then
					self:updateWaypoint(pConversingPlayer, planetName, spawnPoints[i][1], spawnPoints[i][3], "target")
				end
				if (mission.staticObjects ~= nil and table.getn(mission.staticObjects) > 0) then
					self:spawnMissionStaticObjects(mission, pConversingPlayer, spawnPoints[i][1], spawnPoints[i][3])
				end
			end
			if mission.missionType == "assassinate" then
				createObserver(OBJECTDESTRUCTION, self.className, "notifyDefeatedTarget", pNpc)
				createObserver(DEFENDERADDED, self.className, "notifyTriggeredBreechAggro", pNpc)
				writeData(CreatureObject(pNpc):getObjectID() .. ":missionOwnerID", playerID)
			elseif mission.missionType == "confiscate" then
				createObserver(OBJECTDESTRUCTION, self.className, "notifyDefeatedTargetWithLoot", pNpc)
				createObserver(DEFENDERADDED, self.className, "notifyTriggeredBreechAggro", pNpc)
				writeData(CreatureObject(pNpc):getObjectID() .. ":missionOwnerID", playerID)
			elseif mission.missionType == "escort" then
				CreatureObject(pNpc):setPvpStatusBitmask(0)
				self:normalizeNpc(pNpc, 16, 3000)
				writeData(CreatureObject(pNpc):getObjectID() .. ":missionOwnerID", playerID)
			elseif mission.missionType == "retrieve" or mission.missionType == "deliver" then
				CreatureObject(pNpc):setPvpStatusBitmask(0)
			end
		end
	end

	local secondaryNpcs = mission.secondarySpawns
	for i = 1 + table.getn(mission.primarySpawns), numberOfSpawns, 1 do
		local secondaryNpc = secondaryNpcs[i - table.getn(mission.primarySpawns)]
		local pNpc = self:spawnNpc(secondaryNpc, spawnPoints[i], pConversingPlayer, i)

		if pNpc ~= nil and SceneObject(pNpc):isCreatureObject() then
			writeData(CreatureObject(pNpc):getObjectID() .. ":missionOwnerID", playerID)
			createObserver(DEFENDERADDED, self.className, "notifyTriggeredBreechAggro", pNpc)
			if (secondaryNpc.dead ~= nil and secondaryNpc.dead == "true") then
				CreatureObject(pNpc):setPosture(14)
			end
		end
	end
	return true
end

function ThemeParkLogic:normalizeNpc(pNpc, level, ham)
	if (pNpc == nil) then
		return
	end

	AiAgent(pNpc):setLevel(level)
	ObjectManager.withCreatureObject(pNpc, function(npc)
		for i = 0, 8, 1 do
			if (i % 3 == 0) then
				npc:setHAM(i, ham)
				npc:setBaseHAM(i, ham)
				npc:setMaxHAM(i, ham)
			else
				npc:setHAM(i, ham / 100)
				npc:setBaseHAM(i, ham / 100)
				npc:setMaxHAM(i, ham / 100)
			end
		end
	end)
end

function ThemeParkLogic:spawnDestroyMissionNpcs(mission, pConversingPlayer)
	if pConversingPlayer == nil then
		return
	end

	local playerID = CreatureObject(pConversingPlayer):getObjectID()

	local npcNumber = self:getActiveNpcNumber(pConversingPlayer)
	local missionNumber = self:getCurrentMissionNumber(npcNumber, pConversingPlayer)
	local stfFile = self:getStfFile(npcNumber)
	local currentMissionType = self:getMissionType(npcNumber, pConversingPlayer)

	local buildingData = mission.buildingSpawn
	local childNpcs = buildingData.childNpcs
	local numberOfChildNpcs = table.getn(childNpcs)

	local buildingID = readData(playerID .. ":destroyableBuildingID")
	local pBuilding = getSceneObject(buildingID)

	if pBuilding == nil then
		return
	end

	for i = 1, numberOfChildNpcs, 1 do
		local targetCellObject = SceneObject(BuildingObject(pBuilding):getCell(childNpcs[i].vectorCellID))
		local pNpc = spawnMobile(buildingData.building.planet, childNpcs[i].npcTemplate, 0, childNpcs[i].x, childNpcs[i].z, childNpcs[i].y, getRandomNumber(360) - 180, targetCellObject:getObjectID())

		if (pNpc ~= nil) then
			local npcName = self:getNpcName(childNpcs[i].npcName)

			CreatureObject(pNpc):setCustomObjectName(npcName)
			if i == 1 and self:isValidConvoString(stfFile, ":npc_breech_" .. missionNumber) then
				local pBreechArea = spawnActiveArea(buildingData.building.planet, "object/active_area.iff", childNpcs[i].x, childNpcs[i].z, childNpcs[i].y, 20, childNpcs[i].vectorCellID)
				if pBreechArea ~= nil then
					createObserver(ENTEREDAREA, self.className, "notifyEnteredBreechArea", pBreechArea)
					writeData(playerID .. ":breechNpcID", CreatureObject(pNpc):getObjectID())
					writeData(playerID .. ":breechAreaID", SceneObject(pBreechArea):getObjectID())
				end
			end
		end
	end
end

function ThemeParkLogic:notifyDefeatedTargetWithLoot(pVictim, pAttacker)
	if pVictim == nil or pAttacker == nil or not SceneObject(pVictim):isCreatureObject() or not SceneObject(pAttacker):isCreatureObject() then
		return 0
	end

	local victimID = CreatureObject(pVictim):getObjectID()
	local attackerID = CreatureObject(pAttacker):getObjectID()

	local ownerID = readData(victimID .. ":missionOwnerID")
	local pOwner = getCreatureObject(ownerID)

	if (pOwner == nil) then
		self:clearInventory(pVictim)
		return 1
	end

	if self:killedByCorrectPlayer(victimID, attackerID) == false and (self:isGroupedWith(pOwner, pAttacker) == false or self:isInQuestRangeOf(pOwner, pVictim) == false) then
		self:clearInventory(pVictim)
		self:failMission(pOwner)
		return 1
	end

	CreatureObject(pVictim):setLootRights(pOwner)

	local pInventory = CreatureObject(pVictim):getSlottedObject("inventory")
	if pInventory == nil then
		return 1
	end

	local inventory = LuaSceneObject(pInventory)

	local numberOfItems = inventory:getContainerObjectsSize()
	local activeNpcNumber = self:getActiveNpcNumber(pOwner)
	local requiredItems = self:getRequiredItem(activeNpcNumber, pOwner)

	for j = 1, # requiredItems, 1 do
		for i = 0, numberOfItems - 1, 1 do
			local pItem = inventory:getContainerObject(i)

			if pItem ~= nil then
				local item = SceneObject(pItem)
				if requiredItems[j].itemTemplate == item:getTemplateObjectPath() and (requiredItems[j].itemName == item:getCustomObjectName() or requiredItems[j].itemName == item:getDisplayedName()) then
					createObserver(ITEMLOOTED, self.className, "notifyItemLooted", pItem)
					writeData(item:getObjectID() .. ":missionOwnerID", ownerID)
					break
				end
			end
		end
	end

	return 1
end

function ThemeParkLogic:clearInventory(pCreature)
	if (pCreature == nil) then
		return
	end

	local pInventory = CreatureObject(pCreature):getSlottedObject("inventory")

	if pInventory == nil then
		return
	end

	local numberOfItems = SceneObject(pInventory):getContainerObjectsSize()
	for i = 0, numberOfItems - 1, 1 do
		local pItem = SceneObject(pInventory):getContainerObject(0)
		if (pItem ~= nil) then
			SceneObject(pItem):destroyObjectFromWorld()
			SceneObject(pItem):destroyObjectFromDatabase()
		end
	end
end

function ThemeParkLogic:notifyItemLooted(pItem, pLooter)
	if pItem == nil or pLooter == nil or not SceneObject(pLooter):isCreatureObject() then
		return 0
	end

	local itemID = SceneObject(pItem):getObjectID()
	local looterID = SceneObject(pLooter):getObjectID()

	if self:lootedByCorrectPlayer(itemID, looterID) == true then
		local currentLootCount = readData(looterID .. ":requiredItemsLooted") + 1
		writeData(looterID, ":requiredItemsLooted", currentLootCount)

		if currentLootCount == self:getMissionLootCount(pLooter) then
			self:completeMission(pLooter)
			return 1
		end
	end

	return 0
end

function ThemeParkLogic:lootedByCorrectPlayer(itemID, looterID)
	return readData(itemID .. ":missionOwnerID") == looterID
end

function ThemeParkLogic:getMissionLootCount(pLooter)
	if (pLooter == nil) then
		return 0
	end

	local npcNumber = self:getActiveNpcNumber(pLooter)
	local missionNumber = self:getCurrentMissionNumber(npcNumber, pLooter)
	local mission = self:getMission(npcNumber, missionNumber)

	if mission ~= nil and mission.missionType == "confiscate" then
		return table.getn(mission.itemSpawns)
	else
		return 0
	end
end

function ThemeParkLogic:getMissionPreReq(pPlayer)
	if (self.genericGiver or pPlayer == nil) then
		return 0
	end

	local npcNumber = self:getActiveNpcNumber(pPlayer)
	local missionNumber = self:getCurrentMissionNumber(npcNumber, pPlayer)
	local mission = self:getMission(npcNumber, missionNumber)

	if mission == nil then
		return 0
	end

	local preReq = mission.preReq

	if preReq == nil or preReq == "" then
		return 0
	else
		return preReq
	end
end

function ThemeParkLogic:notifyEnteredBreechArea(pActiveArea, pPlayer)
	if pActiveArea == nil or pPlayer == nil or not SceneObject(pActiveArea):isActiveArea() or not SceneObject(pPlayer):isCreatureObject() then
		return 0
	end

	local playerID = CreatureObject(pPlayer):getObjectID()
	local breechNpcID = readData(playerID .. ":breechNpcID")
	local breechAreaID = readData(playerID .. ":breechAreaID")

	if (readData(playerID .. ":breechTriggered") == 1) then
		return 0
	end

	if (SceneObject(pActiveArea):getObjectID() == breechAreaID and breechNpcID ~= nil and breechNpcID ~= 0) then
		local npcNumber = self:getActiveNpcNumber(pPlayer)
		local missionNumber = self:getCurrentMissionNumber(npcNumber, pPlayer)
		local stfFile = self:getStfFile(npcNumber)
		local pNpc = getSceneObject(breechNpcID)

		if pNpc ~= nil then
			spatialChat(pNpc, stfFile .. ":npc_breech_" .. missionNumber)
		end

		writeData(playerID .. ":breechNpcID", 0)
		writeData(playerID .. ":breechTriggered", 1)
		SceneObject(pActiveArea):destroyObjectFromWorld()
		self:setNpcDefender(pPlayer)
		return 1
	end

	return 0
end

function ThemeParkLogic:notifyTriggeredBreechAggro(pNpc, pPlayer)
	if pNpc == nil or pPlayer == nil or not SceneObject(pNpc):isCreatureObject() or not SceneObject(pPlayer):isCreatureObject() then
		return 0
	end

	local playerID = CreatureObject(pPlayer):getObjectID()
	local breechNpcID = readData(playerID .. ":breechNpcID")
	local missionOwnerID = readData(CreatureObject(pNpc):getObjectID() .. ":missionOwnerID")
	local pBreechNpc

	if (breechNpcID == nil or breechNpcID == 0) then
		return 0
	end

	if (CreatureObject(pNpc):getObjectID() ~= breechNpcID) then
		pBreechNpc = getSceneObject(breechNpcID)
	else
		pBreechNpc = pNpc
	end

	if (pBreechNpc == nil) then
		return 0
	end

	if (missionOwnerID == playerID and readData(playerID .. ":breechTriggered") ~= 1) then
		local npcNumber = self:getActiveNpcNumber(pPlayer)
		local missionNumber = self:getCurrentMissionNumber(npcNumber, pPlayer)
		local stfFile = self:getStfFile(npcNumber)
		spatialChat(pBreechNpc, stfFile .. ":npc_breech_" .. missionNumber)
		writeData(playerID .. ":breechNpcID", 0)
		writeData(playerID .. ":breechTriggered", 1)
	end

	return 0
end

function ThemeParkLogic:setNpcDefender(pPlayer)
	if (pPlayer == nil) then
		return
	end

	local playerID = CreatureObject(pPlayer):getObjectID()
	local npcNumber = self:getActiveNpcNumber(pPlayer)
	local missionNumber = self:getCurrentMissionNumber(npcNumber, pPlayer)
	local mission = self:getMission(npcNumber, missionNumber)

	if mission == nil then
		return
	end

	local currentMissionType = self:getMissionType(npcNumber, pPlayer)

	local numberOfSpawns = readData(playerID .. ":missionSpawns")
	for i = 1, numberOfSpawns, 1 do
		local objectID = readData(playerID .. ":missionSpawn:no" .. i)
		if readData(objectID .. ":missionOwnerID") == playerID then
			local pNpc = getSceneObject(objectID)
			if pNpc ~= nil and SceneObject(pNpc):isAiAgent() then
				if (i <= table.getn(mission.primarySpawns)) then
					if currentMissionType == "assassinate" or currentMissionType == "confiscate" or currentMissionType == "destroy" then
						AiAgent(pNpc):setDefender(pPlayer)
					end
				elseif i > table.getn(mission.primarySpawns) then
					if (mission.secondarySpawns[i - table.getn(mission.primarySpawns)].dead == nil or mission.secondarySpawns[i - table.getn(mission.primarySpawns)].dead ~= "true") then
						AiAgent(pNpc):setDefender(pPlayer)
					end
				end
			end
		end
	end
end

function ThemeParkLogic:notifyDefeatedTarget(pVictim, pAttacker)
	if pVictim == nil or pAttacker == nil or not SceneObject(pVictim):isCreatureObject() or not SceneObject(pAttacker):isCreatureObject() then
		return 0
	end

	local victimID = CreatureObject(pVictim):getObjectID()
	local attackerID = CreatureObject(pAttacker):getObjectID()

	local ownerID = readData(victimID .. ":missionOwnerID")
	local pOwner = getCreatureObject(ownerID)

	if pOwner == nil then
		return 0
	end

	if self:killedByCorrectPlayer(victimID, attackerID) == false and (self:isGroupedWith(pOwner, pAttacker) == false or self:isInQuestRangeOf(pOwner, pVictim) == false) then
		self:failMission(pOwner)
		return 1
	else
		local currentKillCount = readData(ownerID .. ":killedMissionNpcs") + 1
		writeData(ownerID .. ":killedMissionNpcs", currentKillCount)

		if currentKillCount == self:getMissionKillCount(pOwner) then
			self:completeMission(pOwner)
		end
	end

	return 1
end

function ThemeParkLogic:notifyDestroyedBuilding(pBuilding, pBuilding2)
	if pBuilding == nil or not SceneObject(pBuilding):isBuildingObject() then
		return 1
	end

	local ownerID = BuildingObject(pBuilding):getOwnerID()
	local pPlayer = getCreatureObject(ownerID)

	if (pPlayer == nil) then
		return 1
	end

	self:completeMission(pPlayer)
	return 1
end

function ThemeParkLogic:getMissionKillCount(pAttacker)
	if (pAttacker == nil) then
		return 0
	end

	local npcNumber = self:getActiveNpcNumber(pAttacker)
	local missionNumber = self:getCurrentMissionNumber(npcNumber, pAttacker)
	local mission = self:getMission(npcNumber, missionNumber)

	if mission ~= nil and mission.missionType == "assassinate" then
		return table.getn(mission.primarySpawns)
	else
		return 0
	end
end

function ThemeParkLogic:killedByCorrectPlayer(victimID, attackerID)
	return readData(victimID .. ":missionOwnerID") == attackerID
end

function ThemeParkLogic:isGroupedWith(pPlayer1, pPlayer2)
	if (pPlayer1 == nil or pPlayer2 == nil) then
		return false
	end

	return CreatureObject(pPlayer1):isGroupedWith(pPlayer2)
end

function ThemeParkLogic:isInQuestRangeOf(pCreature1, pCreature2)
	if (pCreature1 == nil or pCreature2 == nil) then
		return false
	end

	return CreatureObject(pCreature1):isInRangeWithObject(pCreature2, 128)
end

function ThemeParkLogic:spawnNpc(npcTemplate, position, pConversingPlayer, spawnNumber)
	if pConversingPlayer == nil then
		return nil
	end

	local planetName
	if (npcTemplate.planetName == "generic") then
		planetName = CreatureObject(pConversingPlayer):getZoneName()
	else
		planetName = npcTemplate.planetName
	end

	local pNpc = spawnMobile(planetName, npcTemplate.npcTemplate, 0, position[1], position[2], position[3], getRandomNumber(360) - 180, position[4])

	if pNpc ~= nil and SceneObject(pNpc):isCreatureObject() then
		local npcName = self:getNpcName(npcTemplate.npcName)
		CreatureObject(pNpc):setCustomObjectName(npcName)
		writeData(CreatureObject(pConversingPlayer):getObjectID() .. ":missionSpawn:no" .. spawnNumber, CreatureObject(pNpc):getObjectID())
	end

	return pNpc
end

function ThemeParkLogic:getNpcName(name)
	if name ~= "random" then
		return name
	end
	return makeCreatureName(true)
end

function ThemeParkLogic:giveMissionItems(mission, pConversingPlayer)
	if pConversingPlayer == nil then
		return
	end

	local playerID = SceneObject(pConversingPlayer):getObjectID()
	local activeNpcNumber = self:getActiveNpcNumber(pConversingPlayer)
	local currentMissionType = self:getMissionType(activeNpcNumber, pConversingPlayer)

	if (currentMissionType == "retrieve") then
		writeData(playerID .. ":activeMission", 2)
	else
		writeData(playerID .. ":activeMission", 1)
	end

	local itemsToGive = mission.itemSpawns

	local pInventory = CreatureObject(pConversingPlayer):getSlottedObject("inventory")
	if pInventory == nil then
		return
	end

	writeData(playerID .. ":missionItems", table.getn(itemsToGive))

	for i = 1, table.getn(itemsToGive), 1 do
		local pInvItem = getContainerObjectByTemplate(pInventory, itemsToGive[i].itemTemplate, false)

		if (pInvItem == nil) then
			local pItem = giveItem(pInventory, itemsToGive[i].itemTemplate, -1)

			if pItem ~= nil then
				SceneObject(pItem):setCustomObjectName(itemsToGive[i].itemName)
				writeData(playerID .. ":missionItem:no" .. i, SceneObject(pItem):getObjectID())
			else
				printf("Unable to giveItem " .. itemsToGive[i].itemTemplate .. " in ThemeParkLogic:giveMissionItems for %s", self.className);
			end
		else
			writeData(playerID .. ":missionItem:no" .. i, SceneObject(pInvItem):getObjectID())
		end
	end
end

function ThemeParkLogic:getMissionDescription(pConversingPlayer, direction)
	if (pConversingPlayer == nil) then
		return ""
	end

	local activeNpcNumber = self:getActiveNpcNumber(pConversingPlayer)
	local missionNumber = self:getCurrentMissionNumber(activeNpcNumber, pConversingPlayer)

	local curMission = self:getMission(activeNpcNumber, missionNumber)

	local npcNumber = 1
	while (npcNumber < activeNpcNumber) do
		missionNumber = missionNumber + table.getn(self:getNpcData(npcNumber).missions)
		npcNumber = npcNumber * 2
	end
	if curMission ~= nil and curMission.missionDescription ~= "" and curMission.missionDescription ~= nil and direction == "target" then
		CreatureObject(pConversingPlayer):sendSystemMessage(curMission.missionDescription)
		return curMission.missionDescription
	elseif self.missionDescriptionStf == "" then
		local stfFile = self:getStfFile(activeNpcNumber)
		if not self:isValidConvoString(stfFile, ":waypoint_description_" .. missionNumber) or not self:isValidConvoString(stfFile, ":waypoint_name_" .. missionNumber) or not self:isValidConvoString(stfFile, ":return_waypoint_name_" .. missionNumber) then
			return self:getDefaultWaypointName(pConversingPlayer, direction)
		else
			if direction == "target" then
				CreatureObject(pConversingPlayer):sendSystemMessage(stfFile .. ":waypoint_description_" .. missionNumber)
				return stfFile .. ":waypoint_name_" .. missionNumber
			else
				return stfFile .. ":return_waypoint_name_" .. missionNumber
			end
		end
	else
		if direction == "target" then
			local message = self.missionDescriptionStf .. missionNumber
			CreatureObject(pConversingPlayer):sendSystemMessage(message)
			return message
		else
			return self.missionDescriptionStf .. "return"
		end
	end
end

function ThemeParkLogic:getDefaultWaypointName(pConversingPlayer, direction)
	if (pConversingPlayer == nil) then
		return ""
	end

	if direction == "target" then
		local activeNpcNumber = self:getActiveNpcNumber(pConversingPlayer)
		local currentMissionType = self:getMissionType(activeNpcNumber, pConversingPlayer)
		local currentMissionNumber = self:getCurrentMissionNumber(activeNpcNumber, pConversingPlayer)
		local mission = self:getMission(activeNpcNumber, currentMissionNumber)

		if mission == nil then
			return ""
		end

		local mainNpc = mission.primarySpawns
		local mainNpcName = self:getNpcName(mainNpc[1].npcName)
		local missionItem = mission.itemSpawns

		if currentMissionType == "deliver" then
			local missionItemName = missionItem[1].itemName
			return "Deliver " .. missionItemName
		elseif currentMissionType == "hunt" then
			return "Hunt " .. mission.huntTarget.npcName
		elseif currentMissionType == "escort" then
			return "Escort " .. mainNpcName
		elseif currentMissionType == "retrieve" then
			local missionItemName = missionItem[1].itemName
			return "Retrieve " .. missionItemName
		elseif currentMissionType == "assassinate" then
			return "Kill " .. mainNpcName
		elseif currentMissionType == "confiscate" then
			local missionItemName = missionItem[1].itemName
			return "Confiscate " .. missionItemName
		else
			return ""
		end
	else
		return "Return to the mission giver."
	end
end

function ThemeParkLogic:createEscortReturnArea(pNpc, pPlayer)
	if (pNpc == nil or pPlayer == nil) then
		return
	end

	local playerID = SceneObject(pPlayer):getObjectID()
	local npcNumber = self:getActiveNpcNumber(pPlayer)
	local missionNumber = self:getCurrentMissionNumber(npcNumber, pPlayer)
	local stfFile = self:getStfFile(npcNumber)

	local npcData = self:getNpcData(npcNumber)

	if npcData == nil then
		return
	end

	local escortAreaID = readData(playerID .. ":escortAreaID")

	if (self:isValidConvoString(stfFile, ":npc_dropoff_" .. missionNumber) and (escortAreaID == nil or escortAreaID == 0)) then
		local pEscortArea
		if (npcData.spawnData.cellID == 0) then
			pEscortArea = spawnActiveArea(npcData.spawnData.planetName, "object/active_area.iff", npcData.spawnData.x, npcData.spawnData.z, npcData.spawnData.y, 10, 0)
		else
			if (self.genericGiver) then
				local giverId = readData(playerID ..":genericGiverID")
				local pGiver = getSceneObject(giverId)
				if (pGiver == nil) then
					printf("Error in ThemeParkLogic:createEscortReturnArea(), unable to find generic quest giver.")
					return
				end
				pEscortArea = spawnActiveArea(CreatureObject(pGiver):getZoneName(), "object/active_area.iff", SceneObject(pGiver):getWorldPositionX(), 0, SceneObject(pGiver):getWorldPositionY(), 10, 0)
			else
				pEscortArea = spawnActiveArea(npcData.spawnData.planetName, "object/active_area.iff", npcData.worldPosition.x, 0, npcData.worldPosition.y, 10, 0)
			end
		end

		if pEscortArea == nil then return end

		local areaID = SceneObject(pEscortArea):getObjectID()
		createObserver(ENTEREDAREA, self.className, "notifyEnteredEscortArea", pEscortArea)
		writeData(areaID .. ":escortNpcID", CreatureObject(pNpc):getObjectID())
		writeData(playerID .. ":escortAreaID", areaID)
	end
end

function ThemeParkLogic:notifyEnteredEscortArea(pActiveArea, pCreature)
	if pActiveArea == nil or pCreature == nil or not SceneObject(pActiveArea):isActiveArea() or not SceneObject(pCreature):isCreatureObject() then
		return 0
	end

	local areaID = SceneObject(pActiveArea):getObjectID()
	local objectID = CreatureObject(pCreature):getObjectID()
	local escortNpcID = readData(areaID .. ":escortNpcID")

	if (objectID == escortNpcID) then
		CreatureObject(pCreature):setOptionsBitmask(128)
		local ownerID = readData(escortNpcID .. ":missionOwnerID")
		local pPlayer = getCreatureObject(ownerID)

		if (pPlayer == nil) then
			return 0
		end

		local npcNumber = self:getActiveNpcNumber(pPlayer)
		local missionNumber = self:getCurrentMissionNumber(npcNumber, pPlayer)
		local stfFile = self:getStfFile(npcNumber)
		spatialChat(pCreature, stfFile .. ":npc_dropoff_" .. missionNumber)
		deleteData(areaID .. ":escortNpcID")
		writeData(CreatureObject(pPlayer):getObjectID() .. ":escortAreaID", 0)
		SceneObject(pActiveArea):destroyObjectFromWorld()
		return 1
	end

	return 0
end

function ThemeParkLogic:updateWaypoint(pConversingPlayer, planetName, x, y, direction)
	ObjectManager.withCreaturePlayerObject(pConversingPlayer, function(ghost)
		ghost:addWaypoint(planetName, self:getMissionDescription(pConversingPlayer, direction), "", x, y, WAYPOINT_COLOR_PURPLE, true, true, WAYPOINTTHEMEPARK, 0)
	end)
end

function ThemeParkLogic:getSpawnPoints(numberOfSpawns, x, y, pConversingPlayer)
	if (pConversingPlayer == nil) then
		return {}
	end

	local spawnPoints = {}

	if (numberOfSpawns == 0) then
		return spawnPoints
	end

	local firstSpawnPoint

	local activeNpcNumber = self:getActiveNpcNumber(pConversingPlayer)
	local currentMissionType = self:getMissionType(activeNpcNumber, pConversingPlayer)
	local currentMissionNumber = self:getCurrentMissionNumber(activeNpcNumber, pConversingPlayer)

	local spawnDistance = self.distance

	local zoneName = SceneObject(pConversingPlayer):getZoneName()

	if currentMissionType == "destroy" then
		firstSpawnPoint = getSpawnPoint(zoneName, x, y, 15, 25, true)
	else
		firstSpawnPoint = getSpawnPoint(zoneName, x, y, spawnDistance, (spawnDistance/2)*3)
	end

	if firstSpawnPoint ~= nil then
		table.insert(spawnPoints, firstSpawnPoint)
		for i = 2, numberOfSpawns, 1 do
			local nextSpawnPoint
			if currentMissionType == "destroy" then
				nextSpawnPoint = getSpawnPoint(zoneName, firstSpawnPoint[1], firstSpawnPoint[3], 10, 20, true)
			else
				nextSpawnPoint = getSpawnPoint(zoneName, firstSpawnPoint[1], firstSpawnPoint[3], 5, 15)
			end
			if nextSpawnPoint ~= nil then
				table.insert(spawnPoints, nextSpawnPoint)
			end
		end
	end

	return spawnPoints
end

function ThemeParkLogic:hasEliteCombatProfession(pPlayer)
	if (pPlayer == nil) then
		return false
	end

	return CreatureObject(pPlayer):getLevel() > 6
end

function ThemeParkLogic:requiresEliteCombatProfession()
	return self.requiresEliteCombat ~= nil and self.requiresEliteCombat == true
end

function ThemeParkLogic:hasRequiredItem(pConversingPlayer)
	if (pConversingPlayer == nil) then
		return false
	end

	local pInventory = CreatureObject(pConversingPlayer):getSlottedObject("inventory")
	if pInventory == nil then
		return false
	end

	local itemID = readData(CreatureObject(pConversingPlayer):getObjectID() .. ":missionItem:no1")
	local pItem = SceneObject(pInventory):getContainerObjectById(itemID)

	return pItem ~= nil
end

function ThemeParkLogic:doPreReqItemCheck(pPlayer, preReq)
	if (pPlayer == nil) then
		return false
	end

	local itemIff = preReq.itemTemplate
	local pInventory = CreatureObject(pPlayer):getSlottedObject("inventory")
	if pInventory == nil then
		return false
	end

	local pItem = getContainerObjectByTemplate(pInventory, itemIff, true)
	if pItem == nil then
		return false
	end

	if preReq.destroy ~= nil and preReq.destroy == true then
		SceneObject(pItem):destroyObjectFromWorld()
		SceneObject(pItem):destroyObjectFromDatabase()
	end

	writeData(CreatureObject(pPlayer):getObjectID() .. ":hasPreReqItem", 1)
	return true
end

function ThemeParkLogic:hasLootedRequiredItem(activeNpcNumber, pConversingPlayer)
	if (pConversingPlayer == nil) then
		return false
	end

	local pInventory = CreatureObject(pConversingPlayer):getSlottedObject("inventory")

	if pInventory == nil then
		return false
	end

	local numberOfItems = SceneObject(pInventory):getContainerObjectsSize()
	local requiredItems = self:getRequiredItem(activeNpcNumber, pConversingPlayer)

	local unmatchedItems = 0
	local itemsToDestroy = {}
	for j = 1, # requiredItems, 1 do
		unmatchedItems = unmatchedItems + 1
		for i = 0, numberOfItems - 1, 1 do
			local pItem = SceneObject(pInventory):getContainerObject(i)

			if pItem ~= nil then
				local item = LuaSceneObject(pItem)
				if requiredItems[j].itemTemplate == item:getTemplateObjectPath() and (requiredItems[j].itemName == item:getCustomObjectName() or requiredItems[j].itemName == item:getDisplayedName()) then
					table.insert(itemsToDestroy, item)
					unmatchedItems = unmatchedItems - 1
					break
				end
			end
		end
	end

	if unmatchedItems == 0 then
		for i = 1, # itemsToDestroy, 1 do
			itemsToDestroy[i]:destroyObjectFromWorld()
			itemsToDestroy[i]:destroyObjectFromDatabase()
		end
		return true
	else
		return false
	end
end

function ThemeParkLogic:getRequiredItem(activeNpcNumber, pConversingPlayer)
	if (pConversingPlayer == nil) then
		return {}
	end

	local missionNumber = self:getCurrentMissionNumber(activeNpcNumber, pConversingPlayer)
	local mission = self:getMission(activeNpcNumber, missionNumber)

	if mission == nil then
		return {}
	end

	return mission.itemSpawns
end

function ThemeParkLogic:getNpcWorldPosition(npcNumber)
	local worldPosition = { x = 0, y = 0 }

	for i = 1, # self.npcMap do
		local npcSpawnNumber = self.npcMap[i].npcNumber
		if npcNumber == npcSpawnNumber then
			if self.npcMap[i].spawnData.cellID == 0 then
				worldPosition.x = self.npcMap[i].spawnData.x
				worldPosition.y = self.npcMap[i].spawnData.y
			else
				worldPosition.x = self.npcMap[i].worldPosition.x
				worldPosition.y = self.npcMap[i].worldPosition.y
			end
		end
	end

	return worldPosition
end

function ThemeParkLogic:completeMission(pConversingPlayer)
	if (pConversingPlayer == nil) then
		return
	end

	local playerID = SceneObject(pConversingPlayer):getObjectID()
	local npcNumber = self:getActiveNpcNumber(pConversingPlayer)
	local missionNumber = self:getCurrentMissionNumber(npcNumber, pConversingPlayer)
	local stfFile = self:getStfFile(npcNumber)

	if self.missionCompletionMessageStf ~= "" then
		CreatureObject(pConversingPlayer):sendSystemMessage(self.missionCompletionMessageStf)
	elseif self:isValidConvoString(stfFile, ":return_waypoint_description_" .. missionNumber) then
		CreatureObject(pConversingPlayer):sendSystemMessage(stfFile .. ":return_waypoint_description_" .. missionNumber)
	else
		CreatureObject(pConversingPlayer):sendSystemMessage("@theme_park/messages:static_completion_message")
	end

	if (self.genericGiver) then
		local giverId = readData(CreatureObject(pConversingPlayer):getObjectID() ..":genericGiverID")
		local pGiver = getSceneObject(giverId)
		if (pGiver == nil) then
			printf("Error in ThemeParkLogic:completeMission(), unable to find generic quest giver.")
			return
		end
		self:updateWaypoint(pConversingPlayer, SceneObject(pGiver):getZoneName(), SceneObject(pGiver):getWorldPositionX(), SceneObject(pGiver):getWorldPositionY(), "return")
	else
		local npcData = self:getNpcData(npcNumber)
		if npcData == nil then
			return
		end
		local worldPosition = self:getNpcWorldPosition(npcNumber)
		self:updateWaypoint(pConversingPlayer, npcData.spawnData.planetName, worldPosition.x, worldPosition.y, "return")
	end

	writeData(playerID .. ":activeMission", 2)
	writeData(playerID .. ":destroyableBuildingID", 0)
end

function ThemeParkLogic:failMission(pConversingPlayer)
	if (pConversingPlayer == nil) then
		return
	end

	local npcNumber = self:getActiveNpcNumber(pConversingPlayer)
	local missionNumber = self:getCurrentMissionNumber(npcNumber, pConversingPlayer)
	local stfFile = self:getStfFile(npcNumber)

	CreatureObject(pConversingPlayer):sendSystemMessage("@theme_park/messages:generic_fail_message")

	if (self.genericGiver) then
		local giverId = readData(CreatureObject(pConversingPlayer):getObjectID() ..":genericGiverID")
		local pGiver = getSceneObject(giverId)
		if (pGiver == nil) then
			printf("Error in ThemeParkLogic:completeMission(), unable to find generic quest giver.")
			return
		end
		self:updateWaypoint(pConversingPlayer, SceneObject(pGiver):getZoneName(), SceneObject(pGiver):getWorldPositionX(), SceneObject(pGiver):getWorldPositionY(), "return")
	else
		local npcData = self:getNpcData(npcNumber)
		if npcData == nil then
			return
		end
		local worldPosition = self:getNpcWorldPosition(npcNumber)
		self:updateWaypoint(pConversingPlayer, npcData.spawnData.planetName, worldPosition.x, worldPosition.y, "return")
	end

	writeData(CreatureObject(pConversingPlayer):getObjectID() .. ":activeMission", -1)
end

function ThemeParkLogic:handleMissionReward(pConversingPlayer)
	if (pConversingPlayer == nil) then
		return
	end

	local npcNumber = self:getActiveNpcNumber(pConversingPlayer)
	local missionNumber = self:getCurrentMissionNumber(npcNumber, pConversingPlayer)
	local mission = self:getMission(npcNumber, missionNumber)

	if mission == nil then
		return
	end

	local rewards = mission.rewards

	for i = 1, # rewards, 1 do
		local reward = rewards[i]
		if reward.rewardType == "credits" then
			self:giveCredits(pConversingPlayer, reward.amount)
		elseif reward.rewardType == "faction" then
			self:giveFaction(pConversingPlayer, reward.faction, reward.amount)
		elseif reward.rewardType == "loot" then
			self:giveLoot(pConversingPlayer, reward.lootGroup)
		elseif reward.rewardType == "loot_set" then
			self:giveLootSet(pConversingPlayer, reward.lootGroup, reward.setSize)
		elseif reward.rewardType == "badge" then
			self:giveBadge(pConversingPlayer, reward.badge)
		elseif reward.rewardType == "permission" then
			self:givePermission(pConversingPlayer, reward.permissionGroup)
		elseif reward.rewardType == "item" then
			self:giveItemReward(pConversingPlayer, reward.itemTemplate)
		end
	end
end

function ThemeParkLogic:givePermission(pConversingPlayer, permissionGroup)
	ObjectManager.withCreaturePlayerObject(pConversingPlayer, function(ghost)
		ghost:addPermissionGroup(permissionGroup, true)
	end)
end

function ThemeParkLogic:giveBadge(pConversingPlayer, badge)
	ObjectManager.withCreaturePlayerObject(pConversingPlayer, function(ghost)
		ghost:awardBadge(badge)
	end)
end

function ThemeParkLogic:giveLoot(pConversingPlayer, lootGroup)
	if (pConversingPlayer == nil) then
		return
	end

	local pInventory = CreatureObject(pConversingPlayer):getSlottedObject("inventory")

	if pInventory == nil then
		return
	end

	createLoot(pInventory, lootGroup, 0, true)
	CreatureObject(pConversingPlayer):sendSystemMessage("@theme_park/messages:theme_park_reward")
end

function ThemeParkLogic:giveLootSet(pConversingPlayer, lootGroup, setSize)
	if (pConversingPlayer == nil) then
		return
	end

	local pInventory = CreatureObject(pConversingPlayer):getSlottedObject("inventory")

	if pInventory == nil then
		return
	end

	createLootSet(pInventory, lootGroup, 0, true, setSize)
	CreatureObject(pConversingPlayer):sendSystemMessage("@theme_park/messages:theme_park_reward")
end

function ThemeParkLogic:giveCredits(pConversingPlayer, amount)
	if (pConversingPlayer == nil) then
		return
	end

	CreatureObject(pConversingPlayer):addCashCredits(amount, true)
	CreatureObject(pConversingPlayer):sendSystemMessageWithDI("@theme_park/messages:theme_park_credits_pp", amount)
end

function ThemeParkLogic:giveFaction(pConversingPlayer, faction, points)
	ObjectManager.withCreaturePlayerObject(pConversingPlayer, function(playerObject)
		playerObject:increaseFactionStanding(faction, points)
	end)
end

function ThemeParkLogic:giveItemReward(pConversingPlayer, itemTemplate)
	if (pConversingPlayer == nil) then
		return
	end

	local pInventory = CreatureObject(pConversingPlayer):getSlottedObject("inventory")

	if pInventory == nil then
		return
	end

	local pItem = giveItem(pInventory, itemTemplate, -1)

	if (pItem == nil) then
		CreatureObject(pConversingPlayer):sendSystemMessage("Error: Unable to generate quest reward.")
	else
		CreatureObject(pConversingPlayer):sendSystemMessage("@theme_park/messages:theme_park_reward")
	end
end

function ThemeParkLogic:giveItem(pConversingPlayer, itemList)
	if (pConversingPlayer == nil) then
		return
	end

	local pInventory = CreatureObject(pConversingPlayer):getSlottedObject("inventory")
	if pInventory == nil then
		return
	end

	for currentItem = 1, # itemList, 1 do
		local thisItem = itemList[currentItem]
		local itemTemplate = thisItem.itemTemplate
		local numberOfItemsOfThisType = getRandomNumber(thisItem.minimum, thisItem.maximum)

		for i = 1, numberOfItemsOfThisType, 1 do
			local pItem = giveItem(pInventory, itemTemplate, -1)
		end
	end
end

function ThemeParkLogic:cleanUpMission(pConversingPlayer)
	ObjectManager.withCreatureAndPlayerObject(pConversingPlayer, function(creature, playerObject)
		local playerID = creature:getObjectID()
		playerObject:removeWaypointBySpecialType(WAYPOINTTHEMEPARK)
		local npcNumber = self:getActiveNpcNumber(pConversingPlayer)
		local currentMissionType = self:getMissionType(npcNumber, pConversingPlayer)

		if (currentMissionType == "destroy") then
			local buildingID = readData(playerID .. ":destroyableBuildingID")
			if (buildingID ~= 0) then
				dropObserver(OBJECTDESTRUCTION, getSceneObject(buildingID))
				destroyBuilding(buildingID)
			end
			writeData(playerID .. ":destroyableBuildingID", 0)
		end

		local numberOfObjects = readData(playerID .. ":missionStaticObjects")
		for i = 1, numberOfObjects, 1 do
			local objectID = readData(playerID .. ":missionStaticObject:no" .. i)
			local pObj = getSceneObject(objectID)
			if pObj ~= nil then
				SceneObject(pObj):destroyObjectFromWorld()
			end
		end

		local numberOfSpawns = readData(playerID .. ":missionSpawns")
		for i = 1, numberOfSpawns, 1 do
			local objectID = readData(playerID .. ":missionSpawn:no" .. i)
			local pNpc = getSceneObject(objectID)
			if pNpc ~= nil then
				SceneObject(pNpc):destroyObjectFromWorld()
			end
		end
	end)
end

function ThemeParkLogic:removeDeliverItem(pConversingPlayer)
	if (pConversingPlayer == nil) then
		return
	end

	local pInventory = CreatureObject(pConversingPlayer):getSlottedObject("inventory")

	if pInventory == nil then
		return false
	end

	local playerID = CreatureObject(pConversingPlayer):getObjectID()
	local numberOfItems = readData(playerID .. ":missionItems")
	for i = 1, numberOfItems, 1 do
		local itemID = readData(playerID .. ":missionItem:no" .. i)

		local pItem = SceneObject(pInventory):getContainerObjectById(itemID)
		if pItem ~= nil then
			SceneObject(pItem):destroyObjectFromWorld()
			SceneObject(pItem):destroyObjectFromDatabase()
		end
	end
end

function ThemeParkLogic:goToNextMission(pConversingPlayer)
	if (pConversingPlayer == nil) then
		return
	end

	local npcNumber = self:getActiveNpcNumber(pConversingPlayer)
	local missionNumber = self:getCurrentMissionNumber(npcNumber, pConversingPlayer)
	local npcData = self:getNpcData(npcNumber)

	if npcData == nil then
		return
	end

	local npcName = npcData.spawnData.npcTemplate

	local playerID = SceneObject(pConversingPlayer):getObjectID()
	writeData(playerID .. ":activeMission", 0)
	writeData(playerID .. ":hasPreReqItem", 0)
	writeData(playerID .. ":breechTriggered", 0)
	writeStringData(playerID .. ":activeScreenPlay", "")
	CreatureObject(pConversingPlayer):setScreenPlayState(math.pow(2, missionNumber - 1), self.screenPlayState .. "_mission_" .. npcName)

	if missionNumber == table.getn(npcData.missions) then
		CreatureObject(pConversingPlayer):setScreenPlayState(npcNumber, self.screenPlayState)
	end
end

function ThemeParkLogic:followPlayer(pConversingNpc, pConversingPlayer)
	if (pConversingNpc == nil) then
		return
	end

	ObjectManager.withCreatureAndPlayerObject(pConversingPlayer, function(playerCreo, player)
		local npc = AiAgent(pConversingNpc)
		npc:setFollowObject(pConversingPlayer)
		if (playerCreo:getFaction() == FACTIONREBEL or playerCreo:getFaction() == FACTIONIMPERIAL) and not player:isOnLeave() then
			local npcCreo = LuaCreatureObject(pConversingNpc)
			npcCreo:setFaction(playerCreo:getFaction())
			if player:isOvert() then
				npcCreo:setPvpStatusBitmask(5)
			elseif player:isCovert() then
				npcCreo:setPvpStatusBitmask(1)
			end
		end
		npc:setAiTemplate("follow")
	end)
end

function ThemeParkLogic:getMissionType(activeNpcNumber, pConversingPlayer)
	if pConversingPlayer == nil then
		return ""
	end

	local missionNumber = self:getCurrentMissionNumber(activeNpcNumber, pConversingPlayer)

	if missionNumber == 0 then
		return ""
	end

	local mission = self:getMission(activeNpcNumber, missionNumber)

	if mission == nil then
		return ""
	end

	return mission.missionType
end

function ThemeParkLogic:escortedNpcCloseEnough(pConversingPlayer)
	if pConversingPlayer == nil then
		return false
	end
	local objectID = readData(CreatureObject(pConversingPlayer):getObjectID() .. ":missionSpawn:no1")
	local pNpc = getSceneObject(objectID)

	return pNpc ~= nil and SceneObject(pConversingPlayer):getDistanceTo(pNpc) < 64
end

function ThemeParkLogic:resetThemePark(pConversingPlayer)
	if (pConversingPlayer == nil) then
		return
	end

	-- reset currnt missions
	self:resetCurrentMission(pConversingPlayer)
	-- wipe all missions out
	ObjectManager.withCreatureObject(pConversingPlayer, function(creature)
		-- clear the root state
		local state = creature:getScreenPlayState(self.screenPlayState)
		creature:removeScreenPlayState(state, self.screenPlayState)
		-- clear all missions
		for i = 1, #self.npcMap do
			local name = self.npcMap[i].spawnData.npcTemplate
			local npcState = creature:getScreenPlayState(self.screenPlayState .. "_mission_" .. name)
			creature:removeScreenPlayState(npcState, self.screenPlayState .. "_mission_" .. name)
		end
	end)
end

function ThemeParkLogic:resetCurrentMission(pConversingPlayer)
	if (pConversingPlayer == nil) then
		return
	end

	local playerID = SceneObject(pConversingPlayer):getObjectID()

	writeData(playerID .. ":activeMission", 0)
	writeData(playerID .. ":breechNpcID", 0)
	writeData(playerID .. ":breechTriggered", 0)
	writeData(playerID .. ":breechAreaID", 0)
	writeData(playerID .. ":escortAreaID", 0)
	writeData(playerID .. ":hasPreReqItem", 0)
	writeData(playerID .. ":genericGiverID", 0)
	writeStringData(playerID .. ":activeScreenPlay", "")

	self:cleanUpMission(pConversingPlayer)
end
