local QuestManager = require("managers.quest.quest_manager")

local Syren = {}
Syren.act1 = {}
Syren.act1.IMPERIAL_CRC = QuestManager.questCRC.QUEST_C_STORY1_1_IMP
Syren.act1.REBEL_CRC = QuestManager.questCRC.QUEST_C_STORY1_1_REB
Syren.act1.LOOK_FOR_PILOT = 0
Syren.act1.CRASH_SITE_FOUND = 1
Syren.act1.FOUND_DATAPAD = 2
Syren.act1.TALK_TO_CONTACT = 3
Syren.act1.TALK_TO_DOCTOR = 4
Syren.act1.TALK_TO_MOXXAR = 5
Syren.act1.SECOND_DATAPAD = 6
Syren.act1.RETURN_TO_CONTACT = 7
Syren.act1.SPICE_FOUND = 8

SecretsOfTheSyren = ScreenPlay:new {
    SCREEN_PLAY_NAME = "secrets_of_the_syren",

	className = "SecretsOfTheSyren",
	staticNpcs = {
		{"syren_veega_madish", "corellia", -5203, 21, -2572, 180, 0},
		--{"syren_tyla_jinn", "naboo", -5181, 21, 4258, 50, 0},
		{"syren_jevan_monsul", "tatooine", 7, -0.9, -5, 50, 1028647},
        {"syren_edvar_vang", "rori", -22, 0, -10, 0, 4635424}
        --{"MERCENARIES on Rori"}
	},
}

registerScreenPlay("SecretsOfTheSyren", false)

function SecretsOfTheSyren:start()
	self:spawnStaticNpcs()
    self:spawnActiveAreas(Syren.act1.REBEL_CRC)
    self:spawnActiveAreas(Syren.act1.IMPERIAL_CRC)
end

function SecretsOfTheSyren:spawnStaticNpcs()
	for i = 1, # self.staticNpcs do
		local npc = self.staticNpcs[i]
		if isZoneEnabled(npc[2]) then
			spawnMobile(npc[2], npc[1], 0, npc[3], npc[4], npc[5], npc[6], npc[7])
		end
	end
end

function SecretsOfTheSyren:spawnActiveAreas(questCrc)
    local pSyrenTasks = getQuestTasks(questCrc)

    if pSyrenTasks == nil then
        printLuaError("SecretsOfTheSyren: Could not retrieve quest tasks to set up active areas.")
        return
    end

    local syrenTasks = LuaQuestTasks(pSyrenTasks)

    for taskIndex = 0, syrenTasks:getNumberOfTasks() - 1 do
        local pQuestTask = syrenTasks:getTask(taskIndex)

        if pQuestTask == nil then
            printLuaError("SecretsOfTheSyren: Could not retrieve quest task to set up active areas.")
            return
        end

        local questTask = LuaQuestTask(pQuestTask)

        if questTask:getAttachScript() == "quest.task.ground.go_to_location" then
            if isZoneEnabled(questTask:getPlanetName()) then
                local cellid = 0
                if questTask:getPlanetName() == "tatooine" then
                    cellid = 1498879
                end
                local pQuestArea = spawnActiveArea(questTask:getPlanetName(), "object/active_area.iff", questTask:getLocationX(), questTask:getLocationY(), questTask:getLocationZ(), questTask:getRadius(), cellid)

                if pQuestArea == nil then
                    return false
                end

                ActiveArea(pQuestArea):setNoBuildArea(true)
                ActiveArea(pQuestArea):setNoSpawnArea(true)

                createObserver(ENTEREDAREA, self.className, "notifyEnteredQuestArea", pQuestArea)
                local questAreaID = SceneObject(pQuestArea):getObjectID()
                writeData(questAreaID .. ":questCrc", questCrc)
                writeData(questAreaID .. ":taskIndex", taskIndex)
            else
                printf(questTask:getPlanetName() .. "is not loaded")
            end
        end
    end
end

function SecretsOfTheSyren:notifyEnteredQuestArea(pActiveArea, pPlayer)
    if (pActiveArea == nil) or (pPlayer == nil) or (not SceneObject(pActiveArea):isActiveArea()) or
        (not SceneObject(pPlayer):isPlayerCreature()) then
        return 0
    end

    local activeAreaID = SceneObject(pActiveArea):getObjectID()
    local questCrc = readData(activeAreaID .. ":questCrc")
    local taskIndex = readData(activeAreaID .. ":taskIndex")
    local creature = LuaCreatureObject(pPlayer)
    local pGhost = creature:getPlayerObject()
    if pGhost ~= nil then
        local ghost = LuaPlayerObject(pGhost)
        if self:activeTask(ghost, questCrc, taskIndex) then
            ghost:completeJournalQuestTask(questCrc, taskIndex, true)
            ghost:activateJournalQuestTask(questCrc, taskIndex + 1, true)
            if taskIndex == Syren.act1.CRASH_SITE_FOUND then
                ghost:activateJournalQuestTask(questCrc, Syren.act1.FOUND_DATAPAD, true)
                ghost:completeJournalQuestTask(questCrc, Syren.act1.FOUND_DATAPAD, true)
                ghost:activateJournalQuestTask(questCrc, Syren.act1.SPICE_FOUND, true)
                ghost:completeJournalQuestTask(questCrc, Syren.act1.SPICE_FOUND, true)
                ghost:activateJournalQuestTask(questCrc, Syren.act1.TALK_TO_CONTACT, true)
                self:giveItems(pPlayer, taskIndex)
                self:returnToContactWaypoint(questCrc, pPlayer, ghost)
            elseif taskIndex == Syren.act1.TALK_TO_MOXXAR then
                ghost:activateJournalQuestTask(questCrc, Syren.act1.SECOND_DATAPAD, true)
                ghost:completeJournalQuestTask(questCrc, Syren.act1.SECOND_DATAPAD, true)
                ghost:activateJournalQuestTask(questCrc, Syren.act1.RETURN_TO_CONTACT, true)
                self:giveItems(pPlayer, taskIndex)
                self:returnToContactWaypoint(questCrc, pPlayer, ghost)
            end
        end
    end
    return 0
end

function SecretsOfTheSyren:giveItems(pPlayer, taskIndex)
    creature = LuaCreatureObject(pPlayer)
	local pInventory = creature:getSlottedObject("inventory")
	if pInventory == nil then
        printLuaError("Unable to giveItems in SecretsOfTheSyren to player " .. creature:getObjectID())
		return
	end

    if taskIndex == Syren.act1.CRASH_SITE_FOUND then
	    local pItem = giveItem(pInventory, "object/tangible/mission/quest_item/syren1_locked_data.iff", -1, true)
        if pItem == nil then
            printLuaError("Unable to give syren1_locked_data in SecretsOfTheSyren to player " .. creature:getObjectID())
        end
	    local pItem = giveItem(pInventory, "object/tangible/mission/quest_item/syren1_spice.iff", -1, true)
        if pItem == nil then
            printLuaError("Unable to give syren1_spice in SecretsOfTheSyren to player " .. creature:getObjectID())
        end
    elseif taskIndex == Syren.act1.TALK_TO_MOXXAR then
	    local pItem = giveItem(pInventory, "object/tangible/mission/quest_item/syren1_warning.iff", -1, true)
        if pItem == nil then
            printLuaError("Unable to give syren1_warning in SecretsOfTheSyren to player " .. creature:getObjectID())
        end
    end
end

function SecretsOfTheSyren:returnToContactWaypoint(questCrC, pPlayer, ghost)
    if questCrc == Syren.act1.REBEL_CRC then
        self:updateWaypoint(pPlayer, ghost, "corellia", "Return to your contact", -5203, -2572)
    end -- Add other factions
end

function SecretsOfTheSyren:updateWaypoint(pPlayer, ghost, planet, text, x, y)
    local playerID = SceneObject(pPlayer):getObjectID()
    local previousWaypointID = readData(playerID .. ":syrent:waypointID")
    if previousWaypointID ~= nil then
        ghost:removeWaypoint(previousWaypointID, true)
    end
    local waypointID = ghost:addWaypoint(planet, text, "", x, y, WAYPOINTPURPLE, true, true, WAYPOINTQUESTTASK, 0)
    writeData(playerID .. ":syrent:waypointID", waypointID)
end

function SecretsOfTheSyren:activeTask(ghost, questCrc, taskIndex)
    return ghost:isJournalQuestActive(questCrc) and ghost:isJournalQuestTaskActive(questCrc, taskIndex) and
               not ghost:isJournalQuestComplete(questCrc) and not ghost:isJournalQuestTaskComplete(questCrc, taskIndex)
end

function SecretsOfTheSyren:accept_quest_looking_for_pilot(pPlayer, questCrc)
    if pPlayer ~= nil and SceneObject(pPlayer):isPlayerCreature() then
        local creature = LuaCreatureObject(pPlayer)
        local pGhost = creature:getPlayerObject()
        if pGhost ~= nil then
            local ghost = LuaPlayerObject(pGhost)
            ghost:activateJournalQuest(questCrc, true)
            ghost:activateJournalQuestTask(questCrc, Syren.act1.LOOK_FOR_PILOT, true)
            self:updateWaypoint(pPlayer, ghost, "rori", "Search for the pilot", 0, 2012)
        end
    end
end

function SecretsOfTheSyren:accept_quest_talk_to_doctor(pPlayer, questCrc)
    if pPlayer ~= nil and SceneObject(pPlayer):isPlayerCreature() then
        local creature = LuaCreatureObject(pPlayer)
        local pGhost = creature:getPlayerObject()
        if pGhost ~= nil then
            local ghost = LuaPlayerObject(pGhost)
            ghost:completeJournalQuestTask(questCrc, Syren.act1.TALK_TO_CONTACT, true)
            ghost:activateJournalQuestTask(questCrc, Syren.act1.TALK_TO_DOCTOR, true)
            self:updateWaypoint(pPlayer, ghost, "rori", "Talk to Dr. Vang", -5108, -2202)
        end
    end
end

function SecretsOfTheSyren:accept_quest_looking_for_moxxar(pPlayer, questCrc)
    if pPlayer ~= nil and SceneObject(pPlayer):isPlayerCreature() then
        local creature = LuaCreatureObject(pPlayer)
        local pGhost = creature:getPlayerObject()
        if pGhost ~= nil then
            local ghost = LuaPlayerObject(pGhost)
            ghost:completeJournalQuestTask(questCrc, Syren.act1.TALK_TO_DOCTOR, true)
            ghost:activateJournalQuestTask(questCrc, Syren.act1.TALK_TO_MOXXAR, true)
            self:updateWaypoint(pPlayer, ghost, "tatooine", "Talk to Moxxar Krieg", 3292, -4542)
        end
    end
end

function SecretsOfTheSyren:completeAct1(pPlayer, questCrc)
    if pPlayer ~= nil and SceneObject(pPlayer):isPlayerCreature() then
        local creature = LuaCreatureObject(pPlayer)
        local pGhost = creature:getPlayerObject()
        if pGhost ~= nil then
            local ghost = LuaPlayerObject(pGhost)
            ghost:completeJournalQuestTask(questCrc, Syren.act1.RETURN_TO_CONTACT, true)
            ghost:completeJournalQuest(questCrc, true)
        end
    end
end

return Syren