local GoToTheater = require("quest.tasks.go_to_theater")
local ObjectManager = require("managers.object.object_manager")
local QuestManager = require("managers.quest.quest_manager")
local SpawnMobiles = require("utils.spawn_mobiles")

require("utils.helpers")

FsSad1Theater1 = GoToTheater:new {
	-- Task properties
	taskName = "FsSad1Theater1",
	-- GoToTheater properties
	minimumDistance = 800,
	maximumDistance = 1200,
	theater = {
		{ template = "object/static/structure/corellia/corl_tent_small.iff", xDiff = 1.438, zDiff = 0.392, yDiff = 1.875, heading = 0 },
		{ template = "object/static/structure/general/all_banner_generic_s01.iff", xDiff = -2.133, zDiff = -1.061, yDiff = -1.731, heading = 0 },
		{ template = "object/static/structure/general/atst_debris_01.iff", xDiff = -2.842, zDiff = 0.147, yDiff = 1.743, heading = 0 },
		{ template = "object/static/structure/general/camp_stool_tall_s01.iff", xDiff = 1.984, zDiff = 0.273, yDiff = 0.146, heading = 0 },
		{ template = "object/static/structure/general/camp_stool_tall_s01.iff", xDiff = 0.811, zDiff = 0.287, yDiff = 1.061, heading = 0 },
		{ template = "object/static/structure/general/camp_campfire_logs_smoldering_s01.iff", xDiff = 0.743, zDiff = -0.038, yDiff = -3.091, heading = 0 }
	},
	mobileList = {
		{ template = "sith_shadow_outlaw", minimumDistance = 4, maximumDistance = 12, referencePoint = 0 },
		{ template = "sith_shadow_outlaw", minimumDistance = 4, maximumDistance = 12, referencePoint = 0 },
		{ template = "sith_shadow_outlaw", minimumDistance = 4, maximumDistance = 12, referencePoint = 0 },
		{ template = "sith_shadow_outlaw", minimumDistance = 4, maximumDistance = 12, referencePoint = 0 },
		{ template = "sith_shadow_outlaw", minimumDistance = 4, maximumDistance = 12, referencePoint = 0 }
	},
	createWaypoint = false,
	despawnTime = 20 * 60 * 1000, -- 20 minutes
	activeAreaRadius = 16,
}

function FsSad1Theater1:onSuccessfulSpawn(pCreatureObject, spawnedMobileList)
	if (pCreatureObject == nil) then
		return
	end

	local playerID = SceneObject(pCreatureObject):getObjectID()
	writeData(playerID .. self.taskName .. ":killableCount", #spawnedMobileList)

	for i = 1, #spawnedMobileList, 1 do
		if (spawnedMobileList[i] ~= nil) then
			writeData(SceneObject(spawnedMobileList[i]):getObjectID() .. self.taskName .. "ownerID", playerID)
			createObserver(OBJECTDESTRUCTION, self.taskName, "notifyKilledMobile", spawnedMobileList[i])
		end
	end

	local theaterId = readData(playerID .. self.taskName .. "theaterId")
	local pTheater = getSceneObject(theaterId)

	if (pTheater ~= nil) then
		SuiRadiationSensor:setLocation(pCreatureObject, SceneObject(pTheater):getWorldPositionX(), SceneObject(pTheater):getWorldPositionY(), SceneObject(pTheater):getZoneName())
	end
end

function FsSad1Theater1:notifyKilledMobile(pVictim, pAttacker)
	local mobileID = SceneObject(pVictim):getObjectID()
	local ownerID = readData(mobileID .. self.taskName .. "ownerID")
	local numEnemies = readData(ownerID .. self.taskName .. ":killableCount")

	numEnemies = numEnemies - 1
	writeData(ownerID .. self.taskName .. ":killableCount", numEnemies)

	local pOwner = getSceneObject(ownerID)

	if (pOwner == nil) then
		self:finish()
	end

	if (numEnemies <= 0) then
		QuestManager.completeQuest(pOwner, QuestManager.quests.FS_QUESTS_SAD_TASK1)
		QuestManager.activateQuest(pOwner, QuestManager.quests.FS_QUESTS_SAD_RETURN1)
		deleteData(ownerID .. self.taskName .. ":killableCount", numEnemies)
	end

	deleteData(mobileID .. self.taskName .. "ownerID")
	return 1
end

return FsSad1Theater1
