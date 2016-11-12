-- This module handles an encounter in the quest system.
-- An encounter is an event where the player should encounter something after a while.
-- The module takes care of scheduling the event, tringgering the event, clean up etc.
-- The user supplies the module with information about what the encounter is about.
--
-- Parameters
-- taskName - need to be the name of the class inheriting the Encounter class to get the observers and events to work.
-- minimumTimeUntilEncounter - the minimum time until the encounter should happen in ms.
-- maximumTimeUntilEncounter - the maximum time until the encounter should happen in ms.
-- encounterDespawnTime - time in ms from the moment the encounter is spawned until it should be despawned.
-- customSpawnBehaviour - function that is called when the encounter has been spawned with the pCreatureObject
--                        and a list of pointers to the creature objects of the spawned objects
-- spawnObjectList - a list with one element per object that should be spawned in the encounter.
--					 It uses the format from the spawn mobile unit in utils.
--                   Each element should define a table with the following parameters:
--                    - template - the template of the mobile to spawn.
--				      - minimumDistance - the minimum distance for the mobile to spawn at from the center reference point.
--				      - maximumDistance - the maximum distance for the mobile to spawn at from the center reference point.
--				      - referencePoint - index of the spawned mobile to use as center reference point for this mobile.
--									     0 = use provided coordinates, 1 = use the spawn position of the first mobile in the mobileList, etc.
--									     This value must be lower than the current list item number.
--                    - followPlayer - boolean value indicating if the spawned object should be set to follow the player or not.

local ObjectManager = require("managers.object.object_manager")
local SpawnMobiles = require("utils.spawn_mobiles")
local Logger = require("utils.logger")

Encounter = Task:new {
	-- Task properties
	taskName = "",
	-- Encounter properties
	encounterDespawnTime = 0,
	despawnMessage = "",
	spawnObjectList = {},
	onEncounterSpawned = nil,
	onEncounterDespawned = nil,
	isEncounterFinished = nil,
	onEncounterInRange = nil,
	inRangeValue = 16,
}

-- Start the encounter.
-- @param pCreatureObject pointer to the creature object of the player.
function Encounter:taskStart(pCreatureObject)
	if not self:callFunctionIfNotNil(self.isEncounterFinished, true, pCreatureObject) then
		if self:isPlayerInPositionForEncounter(pCreatureObject) and not CreatureObject(pCreatureObject):isDead() then
			self:createEncounter(pCreatureObject)
			createEvent(self.encounterDespawnTime, self.taskName, "handleDespawnEvent", pCreatureObject, "")
		end
	end

	return true
end

-- Check if the player is online.
-- @param pCreatureObject pointer to the creature object of the player who should be checked if online.
-- @return true if the player is online.
function Encounter:isPlayerOnline(pCreatureObject)
	local pGhost = CreatureObject(pCreatureObject):getPlayerObject()
	return pGhost ~= nil and PlayerObject(pGhost):isOnline()
end

-- Check if the player is in a building or not.
-- @param pCreatureObject pointer to the creature object of the player that should be checked for being in a building or not.
-- @return true if the player is in a building.
function Encounter:isPlayerInABuilding(pCreatureObject)
	return SceneObject(pCreatureObject):getParentID() ~= 0
end

-- Check if the player is in a NPC city.
-- @param pCreatureObject pointer to the creature object of the player who should be checked for being in a NPC city.
-- return true if the player is within a NPC city.
function Encounter:isPlayerInNpcCity(pCreatureObject)
	local pCityRegion = getCityRegionAt(SceneObject(pCreatureObject):getZoneName(), SceneObject(pCreatureObject):getWorldPositionX(), SceneObject(pCreatureObject):getWorldPositionY())

	if (pCityRegion == nil) then
		return false
	end

	return CityRegion(pCityRegion):isClientRegion()
end

-- Check if the player is in a position where the encounter can be spawned.
-- @param pCreatureObject pointer to the player object of the player.
function Encounter:isPlayerInPositionForEncounter(pCreatureObject)
	if pCreatureObject == nil or SceneObject(pCreatureObject):getZoneName() == "" then
		return false
	end

	return self:isPlayerOnline(pCreatureObject) and not self:isPlayerInABuilding(pCreatureObject) and not self:isPlayerInNpcCity(pCreatureObject)
end

-- Empty handler for the handleEncounterInRange event.
-- @param pCreatureObject pointer to the player object of the player.
function Encounter:handleEncounterInRangeCheck(pCreatureObject)
	if (pCreatureObject == nil) then
		return
	end

	local spawnedObjects = SpawnMobiles.getSpawnedMobiles(pCreatureObject, self.taskName)

	if (spawnedObjects == nil or spawnedObjects[1] == nil) then
		return
	end

	-- calculateDistance function is compared against squared range for accuracy
	if (CreatureObject(spawnedObjects[1]):isInRangeWithObject(pCreatureObject, self.inRangeValue)) then
		self:callFunctionIfNotNil(self.onEncounterInRange, nil, pCreatureObject, spawnedObjects)
	else
		createEvent(1000, self.taskName, "handleEncounterInRangeCheck", pCreatureObject, "")
	end
end

-- Create the events for the encounter.
-- @param pCreatureObject pointer to the creature object of the player.
-- @param spawnedObjects list with pointers to the spawned objects.
function Encounter:createEncounterEvents(pCreatureObject, spawnedObjects)
	Logger:log("Creating encounter distance heartbeat in " .. self.taskName .. ".", LT_INFO)
	createEvent(1000, self.taskName, "handleEncounterInRangeCheck", pCreatureObject, "")
end

-- Set all spawned objects to follow an object.
-- @param spawnedObjects list with pointers to the spawned objects.
-- @param objectToFollow pointer to the object to follow.
function Encounter:setSpawnedObjectsToFollow(spawnedObjects, objectToFollow)
	if spawnedObjects ~= nil then
		for i = 1, #spawnedObjects, 1 do
			if self.spawnObjectList[i]["setNotAttackable"] then
				CreatureObject(spawnedObjects[i]):setPvpStatusBitmask(0)
			end

			if self.spawnObjectList[i]["followPlayer"] then
				if self.spawnObjectList[i]["setNotAttackable"] then
					AiAgent(spawnedObjects[i]):setAiTemplate("follow")
				else
					AiAgent(spawnedObjects[i]):setAiTemplate("stationarynoleash")
				end
				AiAgent(spawnedObjects[i]):setFollowObject(objectToFollow)
			end
		end
	end
end

-- Create the encounter, spawn objects, setup observers and events.
-- @param pCreatureObject pointer to the creature object of the player.
function Encounter:createEncounter(pCreatureObject)
	Logger:log("Spawning mobiles in encounter " .. self.taskName .. ".", LT_INFO)
	local spawnedObjects = SpawnMobiles.spawnMobiles(pCreatureObject, self.taskName, self.spawnObjectList, true)

	if spawnedObjects ~= nil then
		Logger:log("Set spawned mobiles to follow in encounter " .. self.taskName .. ".", LT_INFO)
		self:setSpawnedObjectsToFollow(spawnedObjects, pCreatureObject)
		self:createEncounterEvents(pCreatureObject, spawnedObjects)
		self:callFunctionIfNotNil(self.onEncounterSpawned, nil, pCreatureObject, spawnedObjects)
	end
end

-- Despawn encounter
-- Function to call from the despawn event.
-- @param pCreatureObject pointer to the creature object of the player.
function Encounter:handleDespawnEvent(pCreatureObject)
	if (pCreatureObject == nil) then
		return
	end

	local spawnedObjects = SpawnMobiles.getSpawnedMobiles(pCreatureObject, self.taskName)

	if (spawnedObjects == nil) then
		self:doDespawn(pCreatureObject)
		return
	end

	for i = 1, #spawnedObjects, 1 do
		if spawnedObjects[i] ~= nil and AiAgent(spawnedObjects[i]):isInCombat() then
			createEvent(self.encounterDespawnTime, self.taskName, "handleDespawnEvent", pCreatureObject, "")
			return
		end
	end

	if (self.despawnMessage ~= "") then
		CreatureObject(pCreatureObject):sendSystemMessage(self.despawnMessage)
	end

	local runAway = false

	local mobX, mobY

	for i = 1, #spawnedObjects, 1 do
		if spawnedObjects[i] ~= nil and self.spawnObjectList[i]["runOnDespawn"] then
			CreatureObject(spawnedObjects[i]):setPvpStatusBitmask(0)
			AiAgent(spawnedObjects[i]):setAiTemplate("follow")
			runAway = true
			mobX = SceneObject(spawnedObjects[i]):getPositionX()
			mobY = SceneObject(spawnedObjects[i]):getPositionY()
		end
	end

	if (not runAway) then
		self:doDespawn(pCreatureObject)
		return
	end

	local playerX = SceneObject(pCreatureObject):getPositionX()
	local playerY = SceneObject(pCreatureObject):getPositionY()
	local newX, newY

	if (playerX < mobX) then
		newX = playerX - getRandomNumber(20,40)
	else
		newX = playerX + getRandomNumber(20,40)
	end

	if (playerY < mobY) then
		newY = playerY - getRandomNumber(20,40)
	else
		newY = playerY + getRandomNumber(20,40)
	end

	local newZ = getTerrainHeight(pCreatureObject, newX, newY)

	for i = 1, #spawnedObjects, 1 do
		if (spawnedObjects[i] ~= nil) then
			local objectID = SceneObject(spawnedObjects[i]):getObjectID()
			writeData(objectID .. ":encounterNewX", newX)
			writeData(objectID .. ":encounterNewY", newY)
			writeData(objectID .. ":encounterNewZ", newZ)
			createEvent(1000, self.taskName, "doRunAway", spawnedObjects[i], "")
		end
	end

	createEvent(9000, self.taskName, "doDespawn", pCreatureObject, "")
end

function Encounter:doRunAway(pAiAgent)
	if pAiAgent == nil or not SceneObject(pAiAgent):isAiAgent() then
		return
	end

	local objectID = SceneObject(pAiAgent):getObjectID()
	local newX = readData(objectID .. ":encounterNewX")
	local newY = readData(objectID .. ":encounterNewY")
	local newZ = readData(objectID .. ":encounterNewZ")

	AiAgent(pAiAgent):setFollowObject(nil)
	AiAgent(pAiAgent):setAiTemplate("manualescort")

	AiAgent(pAiAgent):stopWaiting()
	AiAgent(pAiAgent):setWait(0)
	AiAgent(pAiAgent):setNextPosition(newX, newZ, newY, 0)
	AiAgent(pAiAgent):executeBehavior()

	deleteData(objectID .. ":encounterNewX")
	deleteData(objectID .. ":encounterNewY")
	deleteData(objectID .. ":encounterNewZ")
end

function Encounter:doDespawn(pCreatureObject)
	if (pCreatureObject == nil) then
		return
	end

	Logger:log("Despawning mobiles in encounter " .. self.taskName .. ".", LT_INFO)

	SpawnMobiles.despawnMobiles(pCreatureObject, self.taskName)
	self:callFunctionIfNotNil(self.onEncounterDespawned, nil, pCreatureObject)

	self:finish(pCreatureObject)
end

return Encounter
