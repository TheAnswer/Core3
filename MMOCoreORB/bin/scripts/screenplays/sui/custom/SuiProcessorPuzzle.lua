local ObjectManager = require("managers.object.object_manager")

SuiProcessorPuzzle = {
	playerButtons = { "top.triangles.player.right.1", "top.triangles.player.right.2", "top.triangles.player.right.3",
		"top.triangles.player.left.2", "top.triangles.player.left.3", "top.triangles.player.left.1" },

	serverButtons = {"top.triangles.server.right.1", "top.triangles.server.right.2", "top.triangles.server.right.3",
		"top.triangles.server.left.2", "top.triangles.server.left.3", "top.triangles.server.left.1"}
}

function SuiProcessorPuzzle:openPuzzle(pCreatureObject, pPuzzle)
	local sui = SuiCalibrationGame4.new("SuiProcessorPuzzle", "defaultCallback")
	local playerID = SceneObject(pCreatureObject):getObjectID()

	sui.setTargetNetworkId(SceneObject(pPuzzle):getObjectID())
	sui.setForceCloseDistance(0)

	local goal = { 0, 0, 0, 0, 0, 0 }
	local current = { 0, 0, 0, 0, 0, 0 }
	local tries = 10

	local rand = -1
	local lastRand = -1

	local mixed = false

	while (not mixed) do
		-- Start over if not mixed enough
		for i = 1, #goal, 1 do
			goal[i] = 0
		end

		for i = 1, #goal, 1 do
			repeat
				rand = getRandomNumber(0, 5)
			until (rand ~= lastRand)

			lastRand = rand

			goal = self:toggleButton(goal, rand)
		end

		for i = 1, #goal, 1 do
			if (goal[i] ~= current[i]) then
				mixed = true
			end
		end
	end

	for i = 1, #goal, 1 do
		writeData(playerID .. ":processorPuzzle:goal" .. i, goal[i])
		writeData(playerID .. ":processorPuzzle:current" .. i, current[i])

		if (goal[i] == 1) then
			sui.setProperty(self.serverButtons[i], "Color", "#000000")
		end

		sui.setProperty(self.serverButtons[i], "IsCancelButton", "false")
		sui.setProperty(self.playerButtons[i], "IsCancelButton", "false")

		sui.subscribeToEvent(SuiEventType.SET_onButton, self.playerButtons[i], "SuiProcessorPuzzle:defaultCallback")
	end

	sui.setProperty("bg.mmc.close", "IsCancelButton", "true")

	writeData(playerID .. ":processorPuzzle:tries", tries)
	writeData(playerID .. ":processorPuzzle:maxTries", tries)

	sui.setTitle("@quest/force_sensitive/fs_crafting:sui_config_title")
	sui.setDescription("@quest/force_sensitive/fs_crafting:sui_config_description")
	sui.setAttemptsDesc("@quest/force_sensitive/fs_crafting:sui_attempts_remaining" .. " 100%")

	local pageId = sui.sendTo(pCreatureObject)
	writeData(playerID .. ":processorPuzzle:Pid", pageId)
end

function SuiProcessorPuzzle:toggleButton(goalTable, button)
	local temp1 = -1
	local temp2 = -1

	if (button == 0) then
		temp1 = 3
		temp2 = 4
	elseif (button == 1) then
		temp1 = 4
		temp2 = 5
	elseif (button == 2) then
		temp1 = 3
		temp2 = 5
	elseif (button == 3) then
		temp1 = 0
		temp2 = 2
	elseif (button == 4) then
		temp1 = 0
		temp2 = 1
	elseif (button == 5) then
		temp1 = 1
		temp2 = 2
	end

	if(goalTable[button] == 0) then
		goalTable[button] = 1
	else
		goalTable[button] = 0
	end

	if(goalTable[temp1] == 0) then
		goalTable[temp1] = 1
	else
		goalTable[temp1] = 0
	end

	if(goalTable[temp2] == 0) then
		goalTable[temp2] = 1
	else
		goalTable[temp2] = 0
	end

	return goalTable
end

function SuiProcessorPuzzle:noCallback(pPlayer, pSui, eventIndex)
end

function SuiProcessorPuzzle:defaultCallback(pPlayer, pSui, eventIndex, ...)
	local cancelPressed = (eventIndex == 1)

	local args = {...}
	local playerID = SceneObject(pPlayer):getObjectID()
	local pageId = readData(playerID .. ":processorPuzzle:Pid")

	if (pageId == 0) then
		return
	end

	local pPageData = LuaSuiBoxPage(pSui):getSuiPageData()

	if (pPageData == nil) then
		printf("Error in SuiProcessorPuzzle:defaultCallback, pageData is nil.\n")
		return
	end

	local suiPageData = LuaSuiPageData(pPageData)

	if (cancelPressed) then
		CreatureObject(pPlayer):sendSystemMessage("@quest/force_sensitive/fs_crafting:phase1_msg_calibration_aborted")
		self:doDataCleanup(pPlayer)
		return
	end

	local puzzleID = suiPageData:getTargetNetworkId()
	local pPuzzle = getSceneObject(puzzleID)

	if (pPuzzle == nil) then
		printf("Error in SuiProcessorPuzzle:defaultCallback, pPuzzle nil.\n")
		return
	end

	local pInventory = CreatureObject(pPlayer):getSlottedObject("inventory")

	if pInventory == nil or SceneObject(pPuzzle):getParentID() ~= SceneObject(pInventory):getObjectID() then
		return
	end

	ObjectManager.withCreaturePlayerObject(pPlayer, function(playerObject)
		playerObject:addSuiBox(pSui)
	end)

	local buttonPressed = eventIndex - 2
	local goal = { }
	local current = { }
	local tries = readData(playerID .. ":processorPuzzle:tries")
	local maxTries = readData(playerID .. ":processorPuzzle:maxTries")

	for i = 1, 6, 1 do
		goal[i] = readData(playerID .. ":processorPuzzle:goal" .. i)
		current[i] = readData(playerID .. ":processorPuzzle:current" .. i)
		suiPageData:setProperty(self.playerButtons[i], "Color", "#FFFFFF")
	end

	current = self:toggleButton(current, buttonPressed)
	local wonPuzzle = true

	for i = 1, 6, 1 do
		if (current[i] == 1) then
			suiPageData:setProperty(self.playerButtons[i], "Color", "#000000")
		end

		if (current[i] ~= goal[i]) then
			wonPuzzle = false
		end
	end

	tries = tries - 1
	local integrity = (tries / maxTries) * 100

	if (wonPuzzle or tries <= 0) then
		for i = 1, 6, 1 do
			suiPageData:setProperty(self.playerButtons[i], "GetsInput", "false")
			suiPageData:subscribeToEvent(SuiEventType.SET_onButton, self.playerButtons[i], "SuiProcessorPuzzle:noCallback")
		end

		if (wonPuzzle) then
			LuaFsCraftingComponentObject(pPuzzle):setStatus(1)
			suiPageData:setProperty("top.description.desc", "Text", "@quest/force_sensitive/fs_crafting:sui_calibration_success")
		else
			LuaFsCraftingComponentObject(pPuzzle):setStatus(-1)
			suiPageData:setProperty("top.description.desc", "Text", "@quest/force_sensitive/fs_crafting:sui_calibration_failure")
			suiPageData:setProperty("top.description.attempts", "Text", "@quest/force_sensitive/fs_crafting:sui_attempts_remaining" .. " " .. integrity .. "%")
		end

		self:doDataCleanup(pPlayer)
	else
		suiPageData:setProperty("top.description.attempts", "Text", "@quest/force_sensitive/fs_crafting:sui_attempts_remaining" .. " " .. integrity .. "%")
		writeData(playerID .. ":processorPuzzle:tries", tries)

		for i = 1, 6, 1 do
			writeData(playerID .. ":processorPuzzle:current" .. i, current[i])
		end
	end

	suiPageData:sendUpdateTo(pPlayer)
end

function SuiProcessorPuzzle:doDataCleanup(pPlayer)
	local playerID = SceneObject(pPlayer):getObjectID()

	deleteData(playerID .. ":processorPuzzle:tries")
	deleteData(playerID .. ":processorPuzzle:maxTries")

	for i = 1, 6, 1 do
		deleteData(playerID .. ":processorPuzzle:goal" .. i)
		deleteData(playerID .. ":processorPuzzle:current" .. i)
	end

	deleteData(playerID .. ":processorPuzzle:Pid")
end
