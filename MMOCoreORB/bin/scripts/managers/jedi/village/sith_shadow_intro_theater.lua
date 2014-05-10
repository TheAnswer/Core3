local GoToTheater = require("quest.tasks.go_to_theater")
local ObjectManager = require("managers.object.object_manager")
local QuestManager = require("managers.quest.quest_manager")
local SpawnMobiles = require("utils.spawn_mobiles")
require("utils.helpers")

SithShadowIntroTheater = GoToTheater:new {
	-- Task properties
	taskName = "SithShadowIntroTheater",
	-- GoToTheater properties
	minimumDistance = 1024,
	maximumDistance = 1536,
	theater = "object/building/poi/anywhere_fs_intro_camp.iff",
	waypointDescription = "@quest/force_sensitive/intro:theater_sum",
	mobileList = {
		{ template = "sith_shadow", minimumDistance = 12, maximumDistance = 24, referencePoint = 0 },
		{ template = "sith_shadow", minimumDistance = 12, maximumDistance = 24, referencePoint = 0 }
	},
	despawnTime = 2 * 60* 60* 1000, -- 2 hours
	activeAreaRadius = 64,
	onFailedSpawn = nil,
	onSuccessfulSpawn = nil,
	onEnteredActiveArea = nil
}

-- Check if the sith shadow is the first one spawned for the player.
-- @param pSithShadow pointer to the sith shadow.
-- @param pCreatureObject pointer to the creature object of the player.
-- @return true if the sith shadow is the first one spawned for the player.
function SithShadowIntroTheater:isTheFirstSithShadowOfThePlayer(pSithShadow, pCreatureObject)
	local spawnedSithShadows = self:getSpawnedMobileList(pCreatureObject)

	if spawnedSithShadows ~= nil then
		return ObjectManager.withCreatureObject(spawnedSithShadows[1], function(sithShadowInList)
			return ObjectManager.withCreatureObject(pSithShadow, function(sithShadow)
				return sithShadowInList:getObjectID() == sithShadow:getObjectID()
			end) == true
		end) == true
	else
		return false
	end
end

-- Create the waypoint datapad as loot on the sith shadow.
-- @param pSithShadow pointer to the creature object of the sith shadow.
function SithShadowIntroTheater:addWaypointDatapadAsLoot(pSithShadow)
	ObjectManager.withInventoryPointer(pSithShadow, function(pInventory)
		createLoot(pInventory, "sith_shadow_intro_theater_datapad", 0, true)
	end)
end

-- Event handler for the LOOTCREATURE event on one of the sith shadows.
-- @param pLootedCreature pointer to the sith shadow creature that is being looted.
-- @param pLooter pointer to the creature object of the looter.
-- @param nothing unused variable for the default footprint of event handlers.
-- @return 1 if the correct player looted the creature to remove the observer, 0 otherwise to keep the observer.
function SithShadowIntroTheater:onLoot(pLootedCreature, pLooter, nothing)
	Logger:log("Looting the sith shadow.", LT_INFO)
	if self:isTheFirstSithShadowOfThePlayer(pLootedCreature, pLooter) then
		self:addWaypointDatapadAsLoot(pLootedCreature)
		QuestManager.completeQuest(pLooter, QuestManager.quests.FS_THEATER_CAMP)
		return 1
	end

	return 0
end

-- Event handler for the enter active area event.
-- The event will cause all spawned Sith Shadows to attack the player.
-- @param pCreatureObject pointer to the creature object of the player.
-- @param spawnedSithShadowsList list with pointers to the spawned sith shadows.
function SithShadowIntroTheater:onEnteredActiveArea(pCreatureObject, spawnedSithShadowsList)
	foreach(spawnedSithShadowsList, function(pMobile)
		ObjectManager.withCreatureAiAgent(pMobile, function(mobile)
			mobile:setFollowObject(pCreatureObject)
		end)
	end)
end

-- Event handler for the successful spawn event.
-- The event will activate the FS_THEATER_CAMP quest for the player.
-- @param pCreatureObject pointer to the creature object of the player.
-- @param spawnedSithShadowsList list with pointers to the spawned sith shadows.
function SithShadowIntroTheater:onSuccessfulSpawn(pCreatureObject, spawnedSithShadowsList)
	QuestManager.activateQuest(pCreatureObject, QuestManager.quests.FS_THEATER_CAMP)
	createObserver(LOOTCREATURE, self.taskName, "onLoot", spawnedSithShadowsList[1])
end

return SithShadowIntroTheater
