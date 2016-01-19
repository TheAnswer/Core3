local ObjectManager = require("managers.object.object_manager")
local VillageJediManagerCommon = require("managers.jedi.village.village_jedi_manager_common")
local QuestManager = require("managers.quest.quest_manager")
local FsSad2Theater1 = require("managers.jedi.village.phase3.fs_sad_2_theaters.fs_sad_2_theater_1")
local FsSad2Theater2 = require("managers.jedi.village.phase3.fs_sad_2_theaters.fs_sad_2_theater_2")
local FsSad2Theater3 = require("managers.jedi.village.phase3.fs_sad_2_theaters.fs_sad_2_theater_3")
local FsSad2Theater4 = require("managers.jedi.village.phase3.fs_sad_2_theaters.fs_sad_2_theater_4")
local FsSad2Theater5 = require("managers.jedi.village.phase3.fs_sad_2_theaters.fs_sad_2_theater_5")
local FsSad2Theater6 = require("managers.jedi.village.phase3.fs_sad_2_theaters.fs_sad_2_theater_6")
local FsSad2Theater7 = require("managers.jedi.village.phase3.fs_sad_2_theaters.fs_sad_2_theater_7")
local FsSad2Theater8 = require("managers.jedi.village.phase3.fs_sad_2_theaters.fs_sad_2_theater_8")

FsSad2 = Object:new {
	--timePeriod = 24 * 60 * 60, -- 24 Hours
	timePeriod = 1 * 60 * 60, -- 1 hour for testing
	tasksPerPeriod = 3,
	theaterTable = { FsSad2Theater1, FsSad2Theater2, FsSad2Theater3, FsSad2Theater4, FsSad2Theater5, FsSad2Theater6, FsSad2Theater7, FsSad2Theater8 }
}

function FsSad2:hasExceededLimit(pPlayer)
	local lastTimestamp = self:getLastTimestamp(pPlayer)
	local tasksSinceLast = self:getTasksSinceLastTimestamp(pPlayer)

	if ((os.difftime(os.time(), lastTimestamp) < self.timePeriod) and tasksSinceLast >= self.tasksPerPeriod) then
		return true
	end

	return false
end

function FsSad2:updateLastTimestamp(pPlayer)
	writeScreenPlayData(pPlayer, "VillageJediProgression", "FsSad2LastLimitReset", os.time())
end

function FsSad2:getLastTimestamp(pPlayer)
	local lastTimestamp = readScreenPlayData(pPlayer, "VillageJediProgression", "FsSad2LastLimitReset")

	if (lastTimestamp == "") then
		self:updateLastTimestamp(pPlayer)
		return os.time()
	end

	return tonumber(lastTimestamp)
end

function FsSad2:setTasksSinceLastTimestamp(pPlayer, count)
	writeScreenPlayData(pPlayer, "VillageJediProgression", "FsSad2TasksSinceLastTimestamp", count)
end

function FsSad2:getTasksSinceLastTimestamp(pPlayer)
	local count = readScreenPlayData(pPlayer, "VillageJediProgression", "FsSad2TasksSinceLastTimestamp")

	if (count == "") then
		count = 0
		self:setTasksSinceLastTimestamp(pPlayer, count)
	end

	return tonumber(count)
end

function FsSad2:acceptNextTask(pPlayer)
	local lastTimestamp = self:getLastTimestamp(pPlayer)

	if (os.difftime(os.time(), lastTimestamp) > self.timePeriod) then
		self:updateLastTimestamp(pPlayer)
		self:setTasksSinceLastTimestamp(pPlayer, 1)
	else
		local tasksSinceLast = self:getTasksSinceLastTimestamp(pPlayer)
		self:setTasksSinceLastTimestamp(pPlayer, tasksSinceLast + 1)
	end

	if (not QuestManager.hasCompletedQuest(pPlayer, QuestManager.quests.FS_QUESTS_SAD2_TASKS) and not QuestManager.hasActiveQuest(pPlayer, QuestManager.quests.FS_QUESTS_SAD2_TASKS)) then
		QuestManager.activateQuest(pPlayer, QuestManager.quests.FS_QUESTS_SAD2_TASKS)
		VillageJediManagerCommon.setActiveQuestThisPhase(pPlayer)

		if (not SuiRadiationSensor:hasSensor(pPlayer)) then
			SuiRadiationSensor:giveSensor(pPlayer)
		end

		self:startTask(pPlayer, QuestManager.quests.FS_QUESTS_SAD2_TASK1, 1)
	else
		for i = 1, 8, 1 do
			local questID = getPlayerQuestID("fs_quests_sad2_return" .. i)

			if QuestManager.hasActiveQuest(pPlayer, questID) then
				QuestManager.completeQuest(pPlayer, questID)
				self:finishOldTask(pPlayer, i)

				if (i == 8) then
					self:completeSeries(pPlayer, questID)
				else
					local newQuestID = getPlayerQuestID("fs_quests_sad2_task" .. i + 1)
					self:startTask(pPlayer, newQuestID, i + 1)
				end
			else
			end
		end
	end
end

function FsSad2:completeSeries(pPlayer, questID)
	QuestManager.completeQuest(pPlayer, QuestManager.quests.FS_QUESTS_SAD2_TASKS)
	QuestManager.completeQuest(pPlayer, QuestManager.quests.FS_QUESTS_SAD2_FINISH)
	VillageJediManagerCommon.setCompletedQuestThisPhase(pPlayer)
	VillageJediManagerCommon.unlockBranch(pPlayer, "force_sensitive_enhanced_reflexes_ranged_defense")
	SuiRadiationSensor:removeSensor(pPlayer)

	local pInventory = SceneObject(pPlayer):getSlottedObject("inventory")

	if (pInventory ~= nil) then
		local pPowerup = giveItem(pInventory, "object/tangible/skill_buff/skill_buff_ranged_defense.iff", -1, true)

		if (pPowerup == nil) then
			CreatureObject(pPlayer):sendSystemMessage("Error: Unable to generate item.")
		end
	end
end

function FsSad2:doPhaseChangeFail(pPlayer)
	if (not QuestManager.hasActiveQuest(pPlayer, QuestManager.quests.FS_QUESTS_SAD2_TASKS) or QuestManager.hasCompletedQuest(pPlayer, QuestManager.quests.FS_QUESTS_SAD2_FINISH)) then
		return
	end

	CreatureObject(pPlayer):sendSystemMessage("@quest/quest_journal/fs_quests_sad:wrong_phase")

	if QuestManager.hasCompletedQuest(pPlayer, QuestManager.quests.FS_QUESTS_SAD2_TASKS) or QuestManager.hasActiveQuest(pPlayer, QuestManager.quests.FS_QUESTS_SAD2_TASKS) then
		QuestManager.resetQuest(pPlayer, QuestManager.quests.FS_QUESTS_SAD2_TASKS)
	end

	for i = 1, 8, 1 do
		local questName = "fs_quests_sad2_task" .. i
		local questID = getPlayerQuestID(questName)

		if QuestManager.hasCompletedQuest(pPlayer, questID) or QuestManager.hasActiveQuest(pPlayer, questID) then
			QuestManager.resetQuest(pPlayer, questID)
		end
	end
	
	SuiRadiationSensor:removeSensor(pPlayer)
end

function FsSad2:finishOldTask(pPlayer, questNum)
	local FsSad2Theater = self.theaterTable[questNum]

	FsSad2Theater:finish(pPlayer)
end

function FsSad2:startTask(pPlayer, questID, questNum)
	QuestManager.activateQuest(pPlayer, questID)

	local FsSad2Theater = self.theaterTable[questNum]

	FsSad2Theater:start(pPlayer)
end

function FsSad2:hasActiveReturnTask(pPlayer)
	for i = 1, 8, 1 do
		local questID = getPlayerQuestID("fs_quests_sad2_return" .. i)

		if QuestManager.hasActiveQuest(pPlayer, questID) then
			return true
		end
	end

	return false
end

function FsSad2:hasActiveNonReturnTask(pPlayer)
	for i = 1, 8, 1 do
		local questID = getPlayerQuestID("fs_quests_sad2_task" .. i)

		if QuestManager.hasActiveQuest(pPlayer, questID) then
			return true
		end
	end

	return false
end

function FsSad2:hasFullInventory(pPlayer)
	local pInventory = SceneObject(pPlayer):getSlottedObject("inventory")

	if (pInventory == nil) then
		return true
	end

	return SceneObject(pInventory):isContainerFullRecursive()
end

return FsSad2
