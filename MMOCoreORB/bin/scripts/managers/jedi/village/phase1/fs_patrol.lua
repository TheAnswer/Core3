local Patrol = require("quest.tasks.patrol")
local ObjectManager = require("managers.object.object_manager")
local QuestManager = require("managers.quest.quest_manager")
local VillageJediManagerCommon = require("managers.jedi.village.village_jedi_manager_common")
local VillageJediManagerTownship = require("managers.jedi.village.village_jedi_manager_township")
require("utils.helpers")

FsPatrol = Patrol:new {
	-- Task properties
	taskName = "FsPatrol",
	-- Patrol properties
	waypointName = "@quest/force_sensitive/fs_patrol:patrol_point",
	numPoints = 8,
	areaSize = 48,
	originX = 5313,
	originY = -4161,
	forceSpawn = true,
	enemyList = { "sith_shadow_mercenary", "sith_shadow_thug", "sith_shadow_pirate", "sith_shadow_outlaw" }
}

function FsPatrol:spawnEnemies(pPlayer, numEnemies, x, y)
	writeData(SceneObject(pPlayer):getObjectID() .. self.taskName .. "numEnemies", numEnemies)
	local playerID = SceneObject(pPlayer):getObjectID()

	for i = 1, numEnemies, 1 do
		local npcType

		if (numEnemies < 3) then
			npcType = getRandomNumber(1,2)
		else
			npcType = getRandomNumber(3,4)
		end

		local offsetX = getRandomNumber(-5, 5)
		local offsetY = getRandomNumber(-5, 5)

		local spawnX = x + offsetX
		local spawnY = y + offsetY
		local spawnZ = getTerrainHeight(pPlayer, spawnX, spawnY)

		local pMobile = spawnMobile(SceneObject(pPlayer):getZoneName(), self.enemyList[npcType], 0, spawnX, spawnZ, spawnY, 0, 0)

		if (pMobile ~= nil) then
			createEvent(600 * 1000, self.taskName, "destroyMobile", pMobile)
			writeData(SceneObject(pMobile):getObjectID() .. self.taskName .. "ownerID", playerID)
			if (numEnemies <= 3) then
				createObserver(OBJECTDESTRUCTION, self.taskName, "notifyKilledGoodTarget", pMobile)
			else
				createObserver(OBJECTDESTRUCTION, self.taskName, "notifyKilledBadTarget", pMobile)
			end
		end
	end
end

function FsPatrol:destroyMobile(pMobile)
	if (pMobile == nil) then
		return
	end

	if (CreatureObject(pMobile):isInCombat()) then
		createEvent(60 * 1000, self.taskName, "destroyMobile", pMobile)
		return
	end

	deleteData(SceneObject(pMobile):getObjectID() .. self.taskName .. "ownerID")
	SceneObject(pMobile):destroyObjectFromWorld()
end

function FsPatrol:notifyKilledGoodTarget(pVictim, pAttacker)
	local ownerID = readData(SceneObject(pVictim):getObjectID() .. self.taskName .. "ownerID")

	local numEnemies = readData(ownerID .. self.taskName .. "numEnemies")

	numEnemies = numEnemies - 1

	if (numEnemies <= 0) then
		writeData(ownerID .. ":completedCurrentPoint", 1)
	end

	deleteData(SceneObject(pVictim):getObjectID() .. self.taskName .. "ownerID")
	writeData(ownerID .. self.taskName .. "numEnemies", numEnemies)
	return 1
end

function FsPatrol:notifyKilledBadTarget(pVictim, pAttacker)
	local ownerID = readData(SceneObject(pVictim):getObjectID() .. self.taskName .. "ownerID")
	if (SceneObject(pAttacker):getObjectID() == ownerID) then
		CreatureObject(pAttacker):sendSystemMessage("@quest/force_sensitive/fs_patrol:failed_killed_dont_kill_npc")
		self:failPatrol(pAttacker)
	end

	deleteData(SceneObject(pVictim):getObjectID() .. self.taskName .. "ownerID")
	return 1
end

function FsPatrol:onPlayerKilled(pPlayer)
	local completedCount = tonumber(QuestManager.getStoredVillageValue(pPlayer, "FsPatrolCompletedCount"))
	local playerID = SceneObject(pPlayer):getObjectID()

	if (completedCount >= 0 and completedCount < 20 and readData(playerID .. ":failedPatrol") ~= 1) then
		CreatureObject(pPlayer):sendSystemMessage("@quest/force_sensitive/fs_patrol:failed_death")
		self:failPatrol(pPlayer)
	end
end

function FsPatrol:onEnteredActiveArea(pPlayer, pActiveArea)
	if (pPlayer == nil) then
		return 0
	end

	local playerID = SceneObject(pPlayer):getObjectID()

	if (readData(playerID .. ":completedCurrentPoint") == -1) then
		CreatureObject(pPlayer):sendSystemMessage("@quest/force_sensitive/fs_patrol:failed_previous_point")
		self:failPatrol(pPlayer)
		deleteData(playerID .. ":completedCurrentPoint")
		return 1
	end

	local spawnChance = getRandomNumber(1,100)

	if (spawnChance >= 70) then
		writeData(playerID .. ":completedCurrentPoint", -1)
		local numEnemies = getRandomNumber(1,3)
		CreatureObject(pPlayer):sendSystemMessage("@quest/force_sensitive/fs_patrol:small_group" .. numEnemies)
		self:spawnEnemies(pPlayer, numEnemies, SceneObject(pActiveArea):getWorldPositionX(), SceneObject(pActiveArea):getWorldPositionY())
	elseif (spawnChance >= 60 and spawnChance < 70) then
		writeData(playerID .. ":completedCurrentPoint", 1)
		local numEnemies = getRandomNumber(5,8)
		CreatureObject(pPlayer):sendSystemMessage("@quest/force_sensitive/fs_patrol:large_group" .. numEnemies)
		self:spawnEnemies(pPlayer, numEnemies, SceneObject(pActiveArea):getWorldPositionX(), SceneObject(pActiveArea):getWorldPositionY())
	else
		writeData(playerID .. ":completedCurrentPoint", 1)
		CreatureObject(pPlayer):sendSystemMessage("@quest/force_sensitive/fs_patrol:no_objective")
	end

	return 1
end

function FsPatrol:resetFsPatrol(pCreature)
	local playerID = SceneObject(pCreature):getObjectID()
	deleteData(playerID .. ":patrolWaypointsReached")
	deleteData(playerID .. ":failedPatrol")
	deleteData(playerID .. "completedCurrentPoint")
	self:waypointCleanup(pCreature)
	self:setupPatrolPoints(pCreature)
end

function FsPatrol:completeFsPatrol(pCreature)
	local playerID = SceneObject(pCreature):getObjectID()
	deleteData(playerID .. "completedCurrentPoint")
	deleteData(playerID .. ":patrolWaypointsReached")
	self:finish(pCreature)
end

function FsPatrol:onLoggedIn(pCreatureObject)
	if (not self:hasTaskStarted(pCreatureObject)) then
		return 1
	end

	if (VillageJediManagerTownship:getCurrentPhase() ~= 1) then
		self:doPhaseChangeFail(pCreatureObject)
	else
		CreatureObject(pCreatureObject):sendSystemMessage("@fs_quest_village:combat_quest_failed_timeout");
		self:failPatrol(pCreatureObject)
	end

	return 1
end

function FsPatrol:doPhaseChangeFail(pCreatureObject)
	if (not self:hasTaskStarted(pCreatureObject)) then
		return
	end

	CreatureObject(pCreatureObject):sendSystemMessage("@fs_quest_village:combat_quest_failed_timeout");
	self:finish(pCreatureObject)
end

return FsPatrol
