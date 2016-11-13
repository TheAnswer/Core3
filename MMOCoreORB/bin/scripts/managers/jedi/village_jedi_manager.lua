JediManager = require("managers.jedi.jedi_manager")
local Logger = require("utils.logger")
local QuestManager = require("managers.quest.quest_manager")

jediManagerName = "VillageJediManager"

NOTINABUILDING = 0

NUMBEROFTREESTOMASTER = 6

VillageJediManager = JediManager:new {
	screenplayName = jediManagerName,
	jediManagerName = jediManagerName,
	jediProgressionType = VILLAGEJEDIPROGRESSION,
	startingEvent = nil,
}

-- Handling of the useItem event.
-- @param pSceneObject pointer to the item object.
-- @param itemType the type of item that is used.
-- @param pPlayer pointer to the creature object that used the item.
function VillageJediManager:useItem(pSceneObject, itemType, pPlayer)
	if (pSceneObject == nil or pPlayer == nil) then
		return
	end

	Logger:log("useItem called with item type " .. itemType, LT_INFO)
	if itemType == ITEMHOLOCRON then
		VillageJediManagerHolocron.useHolocron(pSceneObject, pPlayer)
	end
	if itemType == ITEMWAYPOINTDATAPAD then
		SithShadowEncounter:useWaypointDatapad(pSceneObject, pPlayer)
	end
	if itemType == ITEMTHEATERDATAPAD then
		SithShadowIntroTheater:useTheaterDatapad(pSceneObject, pPlayer)
	end
end

-- Handling of the checkForceStatus command.
-- @param pPlayer pointer to the creature object of the player who performed the command
function VillageJediManager:checkForceStatusCommand(pPlayer)
	if (pPlayer == nil) then
		return
	end

	Glowing:checkForceStatusCommand(pPlayer)
end

-- Handling of the onPlayerLoggedIn event. The progression of the player will be checked and observers will be registered.
-- @param pPlayer pointer to the creature object of the player who logged in.
function VillageJediManager:onPlayerLoggedIn(pPlayer)
	if (pPlayer == nil) then
		return
	end

	Glowing:onPlayerLoggedIn(pPlayer)

	if (FsIntro:isOnIntro(pPlayer)) then
		FsIntro:onLoggedIn(pPlayer)
	end

	if (FsOutro:isOnOutro(pPlayer)) then
		FsOutro:onLoggedIn(pPlayer)
	end

	if (QuestManager.hasActiveQuest(pPlayer, QuestManager.quests.FS_PHASE_2_CRAFT_DEFENSES_MAIN) and not QuestManager.hasCompletedQuest(pPlayer, QuestManager.quests.FS_PHASE_2_CRAFT_DEFENSES_MAIN) and not VillageCommunityCrafting:isOnActiveCrafterList(pPlayer)) then
		QuestManager.resetQuest(pPlayer, QuestManager.quests.FS_PHASE_2_CRAFT_DEFENSES_MAIN)
		QuestManager.resetQuest(pPlayer, QuestManager.quests.FS_PHASE_2_CRAFT_DEFENSES_01)
		QuestManager.resetQuest(pPlayer, QuestManager.quests.FS_PHASE_2_CRAFT_DEFENSES_02)
	end

	if (QuestManager.hasActiveQuest(pPlayer, QuestManager.quests.FS_PHASE_3_CRAFT_SHIELDS_MAIN) and not QuestManager.hasCompletedQuest(pPlayer, QuestManager.quests.FS_PHASE_3_CRAFT_SHIELDS_MAIN) and not VillageCommunityCrafting:isOnActiveCrafterList(pPlayer)) then
		QuestManager.resetQuest(pPlayer, QuestManager.quests.FS_PHASE_3_CRAFT_SHIELDS_MAIN)
		QuestManager.resetQuest(pPlayer, QuestManager.quests.FS_PHASE_3_CRAFT_SHIELDS_01)
		QuestManager.resetQuest(pPlayer, QuestManager.quests.FS_PHASE_3_CRAFT_SHIELDS_02)
	end

	if (not VillageCommunityCrafting:isOnActiveCrafterList(pPlayer)) then
		VillageCommunityCrafting:removeSchematics(pPlayer, 2)
		VillageCommunityCrafting:removeSchematics(pPlayer, 3)
	end

	-- Any quests below are run from township because they are not a standard task
	local currentPhase = VillageJediManagerTownship:getCurrentPhase()
	if (currentPhase ~= 1) then
		if (QuestManager.hasActiveQuest(pPlayer, QuestManager.quests.FS_MEDIC_PUZZLE_QUEST_01) or
			QuestManager.hasActiveQuest(pPlayer, QuestManager.quests.FS_MEDIC_PUZZLE_QUEST_02) or
			QuestManager.hasActiveQuest(pPlayer, QuestManager.quests.FS_MEDIC_PUZZLE_QUEST_03)) then
			FsMedicPuzzle:doPhaseChange(pPlayer)
		end
		if (QuestManager.hasActiveQuest(pPlayer, QuestManager.quests.FS_CRAFT_PUZZLE_QUEST_00) and not
			QuestManager.hasCompletedQuest(pPlayer, QuestManager.quests.FS_CRAFT_PUZZLE_QUEST_00)) then
			FsCrafting1:doPhaseChangeFail(pPlayer)
		end
	end
	if (currentPhase ~= 2) then
		if (QuestManager.hasActiveQuest(pPlayer, QuestManager.quests.FS_QUESTS_SAD_TASKS)) then
			FsSad:doPhaseChangeFail(pPlayer)
		end
	end
	if (currentPhase ~= 3) then
		if (QuestManager.hasActiveQuest(pPlayer, QuestManager.quests.FS_QUESTS_SAD2_TASKS)) then
			FsSad2:doPhaseChangeFail(pPlayer)
		end
	end
	if (currentPhase ~= 4) then
		FsVillageDefense:doPhaseChangeFail(pPlayer)
	end
end

function VillageJediManager:onPlayerLoggedOut(pPlayer)
	if (pPlayer == nil) then
		return
	end

	if (FsIntro:isOnIntro(pPlayer)) then
		FsIntro:onLoggedOut(pPlayer)
	end

	if (FsOutro:isOnOutro(pPlayer)) then
		FsOutro:onLoggedOut(pPlayer)
	end
end

--Check for force skill prerequisites
function VillageJediManager:canLearnSkill(pPlayer, skillName)
	if string.find(skillName, "force_sensitive") ~= nil then
		local index = string.find(skillName, "0")
		if index ~= nil then
			local skillNameFinal = string.sub(skillName, 1, string.len(skillName) - 3)
			if CreatureObject(pPlayer):getScreenPlayState("VillageUnlockScreenPlay:" .. skillNameFinal) < 2 then
				return false
			end
		end
	end

	if skillName == "force_title_jedi_rank_01" and CreatureObject(pPlayer):getForceSensitiveSkillCount(false) < 24 then
		return false
	end

	if skillName == "force_title_jedi_rank_03" and not CreatureObject(pPlayer):villageKnightPrereqsMet("") then
		return false
	end

	return true
end

--Check to ensure force skill prerequisites are maintained
function VillageJediManager:canSurrenderSkill(pPlayer, skillName)
	if skillName == "force_title_jedi_novice" and CreatureObject(pPlayer):getForceSensitiveSkillCount(true) > 0 then
		return false
	end

	if string.find(skillName, "force_sensitive_") and CreatureObject(pPlayer):hasSkill("force_title_jedi_rank_01") and CreatureObject(pPlayer):getForceSensitiveSkillCount(false) <= 24 then
		return false
	end

	if string.find(skillName, "force_discipline_") and CreatureObject(pPlayer):hasSkill("force_title_jedi_rank_03") and not CreatureObject(pPlayer):villageKnightPrereqsMet(skillName) then
		return false
	end

	return true
end

-- Handling of the onFSTreesCompleted event.
-- @param pPlayer pointer to the creature object of the player
function VillageJediManager:onFSTreeCompleted(pPlayer, branch)
	if (pPlayer == nil) then
		return
	end

	-- Remove the "_04" from the end of the skill...
	local branchSub = string.sub(branch, 0, (string.len(branch) - 3))

	CreatureObject(pPlayer):setScreenPlayState(4, "VillageUnlockScreenPlay:" .. branchSub)

	if (ExperienceConverter:getMasteredBranches(pPlayer) >= NUMBEROFTREESTOMASTER) then
		VillageJediManagerCommon.setJediProgressionScreenPlayState(pPlayer, VILLAGE_JEDI_PROGRESSION_COMPLETED_VILLAGE)
		FsOutro:startOldMan(pPlayer)
	end
end

registerScreenPlay("VillageJediManager", true)

return VillageJediManager
