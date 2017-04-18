VillageGmSui = ScreenPlay:new {
	productionServer = false
}

function VillageGmSui:showMainPage(pPlayer)
	if (pPlayer == nil) then
		return
	end

	local curPhase = VillageJediManagerTownship:getCurrentPhase()
	local phaseID = VillageJediManagerTownship:getCurrentPhaseID()
	local phaseTimeLeft = self:getPhaseDuration()

	local suiPrompt = " \\#pcontrast1 " .. "Current Phase:" .. " \\#pcontrast2 " .. curPhase .. " (id " .. phaseID .. ")\n" .. " \\#pcontrast1 " .. "Time Left: " .. " \\#pcontrast2 " .. phaseTimeLeft

	local pMaster = VillageJediManagerTownship:getMasterObject()

	if (pMaster ~= nil) then
		local playerTable = SceneObject(pMaster):getPlayersInRange(192)
		suiPrompt = suiPrompt .. "\n \\#pcontrast1 " .. "Players in Village: " .. " \\#pcontrast2 " .. #playerTable
	end

	local sui = SuiListBox.new("VillageGmSui", "mainCallback")
	sui.setTitle("Village GM Panel")
	sui.setPrompt(suiPrompt)

	sui.add("Lookup player by target", "playerLookupByTarget")
	sui.add("Lookup player by name", "playerLookupByName")
	sui.add("Lookup player by oid", "playerLookupByOID")
	sui.add("List players in village", "listOnlineVillagePlayers")

	if (not self.productionServer) then
		sui.add("Change to next phase", "changePhase")
	end

	sui.sendTo(pPlayer)
end

function VillageGmSui:mainCallback(pPlayer, pSui, eventIndex, args)
	local cancelPressed = (eventIndex == 1)

	if (cancelPressed) then
		return
	end

	local pPageData = LuaSuiBoxPage(pSui):getSuiPageData()

	if (pPageData == nil) then
		return
	end

	local suiPageData = LuaSuiPageData(pPageData)
	local menuOption =  suiPageData:getStoredData(tostring(args))

	local targetID

	if (string.find(menuOption, "%d")) then
		targetID = string.match(menuOption, '%d+')
		menuOption = string.gsub(menuOption, targetID, "")

		local pTarget = getSceneObject(targetID)

		if (pTarget == nil) then
			printLuaError("Unable to find player for VillageGmSui function " .. menuOption .. " using oid " .. targetID)
			return
		end
	end

	if (self[menuOption] == nil) then
		printLuaError("Tried to execute invalid function " .. menuOption .. " in VillageGmSui")
		return
	end


	self[menuOption](pPlayer, targetID)
end

function VillageGmSui.changePhase(pPlayer)
	if (pPlayer == nil) then
		return
	end

	local sui = SuiMessageBox.new("VillageGmSui", "changePhaseCallback")

	sui.setTitle("Village Phase Change")
	sui.setPrompt("Are you sure you want to change the village to the next phase? Doing so will reset the progress of all players in the current phase.")
	sui.setOkButtonText("Yes")
	sui.setCancelButtonText("No")

	sui.sendTo(pPlayer)
end

function VillageGmSui:changePhaseCallback(pPlayer, pSui, eventIndex, args)
	local cancelPressed = (eventIndex == 1)

	if (cancelPressed) then
		VillageGmSui:showMainPage(pPlayer)
		return
	end

	local curPhase = VillageJediManagerTownship:getCurrentPhase()
	local nextPhase = curPhase + 1

	if nextPhase == 5 then
		nextPhase = 1
	end

	CreatureObject(pPlayer):sendSystemMessage("Changing the Village from phase " .. curPhase .. " to phase " .. nextPhase .. ".")
	VillageJediManagerTownship:switchToNextPhase()
end

function VillageGmSui.playerLookupByTarget(pPlayer)
	if (pPlayer == nil) then
		return
	end

	local targetID = CreatureObject(pPlayer):getTargetID()

	local pTarget = getSceneObject(targetID)

	if (pTarget == nil or not SceneObject(pTarget):isPlayerCreature()) then
		CreatureObject(pPlayer):sendSystemMessage("Invalid target, must be a valid player.")
		VillageGmSui:showMainPage(pPlayer)
		return
	end

	VillageGmSui.playerInfo(pPlayer, targetID)
end

function VillageGmSui.playerLookupByName(pPlayer)
	if (pPlayer == nil) then
		return
	end

	local sui = SuiInputBox.new("VillageGmSui", "playerLookupByNameCallback")

	sui.setTitle("Village Player Lookup (Name)")
	sui.setPrompt("Enter the name of the player you are looking for below.")

	sui.sendTo(pPlayer)
end

function VillageGmSui:playerLookupByNameCallback(pPlayer, pSui, eventIndex, args)
	local cancelPressed = (eventIndex == 1)

	if (cancelPressed) then
		VillageGmSui:showMainPage(pPlayer)
		return
	end

	if (args == nil or args == "") then
		CreatureObject(pPlayer):sendSystemMessage("Invalid string entered, please try again.")
		VillageGmSui.playerLookupByName(pPlayer)
		return
	end

	local pTarget = getPlayerByName(args)

	if (pTarget == nil) then
		CreatureObject(pPlayer):sendSystemMessage("Unable to find player using the string: " .. args .. ".")
		VillageGmSui.playerLookupByName(pPlayer)
		return
	end

	local targetID = SceneObject(pTarget):getObjectID()
	VillageGmSui.playerInfo(pPlayer, targetID)
end

function VillageGmSui.playerLookupByOID(pPlayer)
	if (pPlayer == nil) then
		return
	end

	local sui = SuiInputBox.new("VillageGmSui", "playerLookupByOIDCallback")

	sui.setTitle("Village Player Lookup (Object ID)")
	sui.setPrompt("Enter the object id of the player you are looking for below.")

	sui.sendTo(pPlayer)
end

function VillageGmSui:playerLookupByOIDCallback(pPlayer, pSui, eventIndex, args)
	local cancelPressed = (eventIndex == 1)

	if (cancelPressed) then
		VillageGmSui:showMainPage(pPlayer)
		return
	end

	if (args == nil or args == "") then
		CreatureObject(pPlayer):sendSystemMessage("Invalid oid entered, please try again.")
		VillageGmSui.playerLookupByOID(pPlayer)
		return
	end

	local pTarget = getSceneObject(args)

	if (pTarget == nil) then
		CreatureObject(pPlayer):sendSystemMessage("Unable to find player using the oid: " .. args .. ".")
		VillageGmSui.playerLookupByOID(pPlayer)
		return
	end

	VillageGmSui.playerInfo(pPlayer, tonumber(args))
end

function VillageGmSui.listOnlineVillagePlayers(pPlayer)
	if (pPlayer == nil) then
		return
	end

	local pMaster = VillageJediManagerTownship:getMasterObject()

	if (pMaster == nil) then
		printLuaError("Null village master object in VillageGmSui:listOnlineVillagePlayers")
		return
	end

	local playerTable = SceneObject(pMaster):getPlayersInRange(192)

	local sui = SuiListBox.new("VillageGmSui", "mainCallback")
	sui.setTitle("Village GM Panel")
	sui.setPrompt("These are the online players within 192 meters of the center of the Village:")

	for i = 1, #playerTable, 1 do
		if (playerTable[i] ~= nil) then
			sui.add(SceneObject(playerTable[i]):getCustomObjectName(), "playerInfo" .. SceneObject(playerTable[i]):getObjectID())
		end
	end

	sui.sendTo(pPlayer)
end

function VillageGmSui.playerInfo(pPlayer, targetID)
	local pTarget = getCreatureObject(targetID)

	if (pTarget == nil) then
		return
	end

	local pGhost = CreatureObject(pTarget):getPlayerObject()

	if (pGhost == nil) then
		return
	end

	local promptBuf = " \\#pcontrast1 " .. "Player name:" .. " \\#pcontrast2 " .. SceneObject(pTarget):getCustomObjectName() .. "\n"
	promptBuf = promptBuf .. " \\#pcontrast1 " .. "Object ID:" .. " \\#pcontrast2 " .. targetID .. "\n"
	promptBuf = promptBuf .. " \\#pcontrast1 " .. "Jedi State:" .. " \\#pcontrast2 " .. PlayerObject(pGhost):getJediState() .. "\n"
	promptBuf = promptBuf .. " \\#pcontrast1 " .. "Progression:" .. " \\#pcontrast2 "

	if (VillageJediManagerCommon.hasJediProgressionScreenPlayState(pTarget, VILLAGE_JEDI_PROGRESSION_COMPLETED_PADAWAN_TRIALS)) then
		promptBuf = promptBuf.. "Padawan Trials Completed\n"
	elseif (VillageJediManagerCommon.hasJediProgressionScreenPlayState(pTarget, VILLAGE_JEDI_PROGRESSION_DEFEATED_MELLIACHAE)) then
		if (JediTrials:isOnPadawanTrials(pTarget)) then
			promptBuf = promptBuf .. "Padawan Trials (" .. JediTrials:getTrialsCompleted(pTarget) .. " completed)\n"
		else
			promptBuf = promptBuf .. "Mellichae (Defeated)\n"
		end
	elseif (VillageJediManagerCommon.hasJediProgressionScreenPlayState(pTarget, VILLAGE_JEDI_PROGRESSION_ACCEPTED_MELLICHAE)) then
		promptBuf = promptBuf .. "Mellichae\n"
	elseif (FsOutro:isOnOutro(pTarget)) then
		local curStep = FsIntro:getCurrentStep(pTarget)

		if (curStep == OLDMANWAIT) then
			promptBuf = promptBuf .. "Outro (Waiting for Old Man)\n"
		elseif (curStep == OLDMANMEET) then
			promptBuf = promptBuf .. "Outro (Old Man Visit)\n"
		end
	elseif (VillageJediManagerCommon.hasJediProgressionScreenPlayState(pTarget, VILLAGE_JEDI_PROGRESSION_HAS_VILLAGE_ACCESS)) then
		promptBuf = promptBuf .. "Village Phase Quests\n"
	elseif (FsIntro:isOnIntro(pTarget)) then
		local curStep = FsIntro:getCurrentStep(pTarget)

		if (curStep == FsIntro.OLDMANWAIT) then
			promptBuf = promptBuf .. "Intro (Waiting for Old Man)\n"
			local timeTilVisit = readScreenPlayData(pTarget, "VillageJediProgression", "FsIntroDelay") - os.time()

			if (not PlayerObject(pGhost):isOnline()) then
				promptBuf = promptBuf .. " \\#pcontrast1 " .. "Time until visit:" .. " \\#pcontrast2 Player Offline\n"
			elseif (timeTilVisit > 0) then
				promptBuf = promptBuf .. " \\#pcontrast1 " .. "Time until visit:" .. " \\#pcontrast2 " .. VillageGmSui:getTimeString(timeTilVisit) .. "\n"
			else
				promptBuf = promptBuf .. " \\#pcontrast1 " .. "Time until visit:" .. " \\#pcontrast2 Soon\n"
			end
			local totalVisits = tonumber(readScreenPlayData(pTarget, "VillageJediProgression", "FsIntroOldManVisits"))

			if (totalVisits == nil) then totalVisits = 0 end
			promptBuf = promptBuf .. " \\#pcontrast1 " .. "Old Man Visits So Far:" .. " \\#pcontrast2 " .. totalVisits .. "\n"
		elseif (curStep == FsIntro.OLDMANMEET) then
			promptBuf = promptBuf .. "Intro (Old Man Visit)\n"
		elseif (curStep == FsIntro.SITHWAIT) then
			promptBuf = promptBuf .. "Intro (Waiting for Sith Attack)\n"
			local timeTilAttack = readScreenPlayData(pTarget, "VillageJediProgression", "FsIntroDelay") - os.time()

			if (not PlayerObject(pGhost):isOnline()) then
				promptBuf = promptBuf .. " \\#pcontrast1 " .. "Time until attack:" .. " \\#pcontrast2 Player Offline\n"
			elseif (timeTilAttack > 0) then
				promptBuf = promptBuf .. " \\#pcontrast1 " .. "Time until attack:" .. " \\#pcontrast2 " .. VillageGmSui:getTimeString(timeTilAttack) .. "\n"
			else
				promptBuf = promptBuf .. " \\#pcontrast1 " .. "Time until attack:" .. " \\#pcontrast2 Soon\n"
			end
		elseif (curStep == FsIntro.SITHATTACK) then
			promptBuf = promptBuf .. "Intro (Sith Attack)\n"
		elseif (curStep == FsIntro.USEDATAPADONE) then
			promptBuf = promptBuf .. "Intro (First Datapad Looted)\n"
		elseif (curStep == FsIntro.SITHTHEATER) then
			promptBuf = promptBuf .. "Intro (Sith Camp)\n"
		elseif (curStep == FsIntro.USEDATAPADTWO) then
			promptBuf = promptBuf .. "Intro (Second Datapad Looted)\n"
		elseif (curStep == FsIntro.VILLAGE) then
			promptBuf = promptBuf .. "Intro (Sent to Village)\n"
		end
	elseif (Glowing:isGlowing(pTarget)) then
		promptBuf = promptBuf .. "Glowing\n"
	else
		promptBuf = promptBuf .. "Not Glowing\n"
	end

	if (VillageJediManagerCommon.hasActiveQuestThisPhase(pTarget)) then
		promptBuf = promptBuf .. " \\#pcontrast1 " .. "Has Active Quest This Phase:" .. " \\#pcontrast2 YES\n"
	else
		promptBuf = promptBuf .. " \\#pcontrast1 " .. "Has Active Quest This Phase:" .. " \\#pcontrast2 NO\n"
	end

	if (VillageJediManagerCommon.hasCompletedQuestThisPhase(pTarget)) then
		promptBuf = promptBuf .. " \\#pcontrast1 " .. "Has Completed Quest This Phase:" .. " \\#pcontrast2 YES\n"
	else
		promptBuf = promptBuf .. " \\#pcontrast1 " .. "Has Completed Quest This Phase:" .. " \\#pcontrast2 NO\n"
	end

	if (VillageJediManagerCommon.hasJediProgressionScreenPlayState(pTarget, VILLAGE_JEDI_PROGRESSION_HAS_VILLAGE_ACCESS) or VillageJediManagerCommon.getUnlockedBranchCount(pTarget) > 0) then
		promptBuf = promptBuf .. " \\#pcontrast1 " .. "Unlocked Branches:" .. " \\#pcontrast2 " .. VillageJediManagerCommon.getUnlockedBranchCount(pTarget) .. "\n"
	end

	local sui = SuiListBox.new("VillageGmSui", "mainCallback")
	sui.setTitle("Village GM Panel")
	sui.setPrompt(promptBuf)

	sui.add("FS Branch Management", "branchManagement" .. targetID)

	if (VillageJediManagerCommon.hasActiveQuestThisPhase(pTarget)) then
		sui.add("Reset Active Quest This Phase", "resetActiveQuest" .. targetID)
	end

	if (VillageJediManagerCommon.hasCompletedQuestThisPhase(pTarget)) then
		sui.add("Reset Completed Quest This Phase", "resetCompletedQuest" .. targetID)
	end

	sui.sendTo(pPlayer)
end

function VillageGmSui.resetActiveQuest(pPlayer, targetID)
	local pTarget = getSceneObject(targetID)

	if (pTarget == nil) then
		return
	end

	local sui = SuiMessageBox.new("VillageGmSui", "resetActiveQuestCallback")

	sui.setTitle("Reset Active Quest")
	sui.setPrompt("Are you sure you want to reset the player's active quest status? This could potentially allow a player to have two active quests at the same time.")
	sui.setOkButtonText("Yes")
	sui.setCancelButtonText("No")
	sui.setTargetNetworkId(targetID)

	sui.sendTo(pPlayer)
end

function VillageGmSui:resetActiveQuestCallback(pPlayer, pSui, eventIndex, args)
	local pPageData = LuaSuiBoxPage(pSui):getSuiPageData()

	if (pPageData == nil) then
		return
	end

	local suiPageData = LuaSuiPageData(pPageData)

	local targetID = suiPageData:getTargetNetworkId()

	local pTarget = getSceneObject(targetID)

	if (pTarget == nil) then
		return
	end

	local cancelPressed = (eventIndex == 1)

	if (cancelPressed) then
		VillageGmSui.playerInfo(pPlayer, targetID)
		return
	end

	local phaseID = VillageJediManagerTownship:getCurrentPhaseID()
	VillageJediManagerCommon.removeFromActiveQuestList(pTarget)
	removeQuestStatus(targetID .. ":village:lastActiveQuest")

	CreatureObject(pPlayer):sendSystemMessage("Player has been removed from this phase's active quest list.")

	VillageGmSui.playerInfo(pPlayer, targetID)
end

function VillageGmSui.resetCompletedQuest(pPlayer, targetID)
	local pTarget = getSceneObject(targetID)

	if (pTarget == nil) then
		return
	end

	local sui = SuiMessageBox.new("VillageGmSui", "resetCompletedQuestCallback")

	sui.setTitle("Reset Completed Quest")
	sui.setPrompt("Are you sure you want to reset the player's completed quest status? This will allow the player to complete a second quest this phase.")
	sui.setOkButtonText("Yes")
	sui.setCancelButtonText("No")
	sui.setTargetNetworkId(targetID)

	sui.sendTo(pPlayer)
end

function VillageGmSui:resetCompletedQuestCallback(pPlayer, pSui, eventIndex, args)
	local pPageData = LuaSuiBoxPage(pSui):getSuiPageData()

	if (pPageData == nil) then
		return
	end

	local suiPageData = LuaSuiPageData(pPageData)

	local targetID = suiPageData:getTargetNetworkId()

	local pTarget = getSceneObject(targetID)

	if (pTarget == nil) then
		return
	end

	local cancelPressed = (eventIndex == 1)

	if (cancelPressed) then
		VillageGmSui.playerInfo(pPlayer, targetID)
		return
	end

	local phaseID = VillageJediManagerTownship:getCurrentPhaseID()
	removeQuestStatus(targetID .. ":village:lastCompletedQuest")

	CreatureObject(pPlayer):sendSystemMessage("Player has had their completed quest status for this phase reset.")

	VillageGmSui.playerInfo(pPlayer, targetID)
end

function VillageGmSui.branchManagement(pPlayer, targetID)
	local pTarget = getSceneObject(targetID)

	if (pTarget == nil) then
		return
	end

	local sui = SuiListBox.new("VillageGmSui", "branchManagementCallback")
	sui.setTitle("Village GM Panel")
	sui.setPrompt("The statuses of " .. SceneObject(pTarget):getCustomObjectName() .. "'s force sensitive branches are listed below. Branches can be locked and unlocked from this window.")

	for i = 1, #VillageJediManagerCommon.forceSensitiveBranches, 1 do
		local rowString = getStringId("@quest/force_sensitive/utils:" .. VillageJediManagerCommon.forceSensitiveBranches[i])

		if (VillageJediManagerCommon.hasUnlockedBranch(pTarget, VillageJediManagerCommon.forceSensitiveBranches[i])) then
			rowString = rowString .. " \\#pcontrast1 (UNLOCKED)"
		else
			rowString = rowString .. " \\#pcontrast2 (Locked)"
		end

		sui.add(rowString, VillageJediManagerCommon.forceSensitiveBranches[i] .. targetID)
	end

	sui.setOkButtonText("Lock/Unlock")
	sui.sendTo(pPlayer)
end

function VillageGmSui:branchManagementCallback(pPlayer, pSui, eventIndex, args)
	local cancelPressed = (eventIndex == 1)

	if (cancelPressed) then
		return
	end

	local pPageData = LuaSuiBoxPage(pSui):getSuiPageData()

	if (pPageData == nil) then
		return
	end

	local suiPageData = LuaSuiPageData(pPageData)
	local menuOption = suiPageData:getStoredData(tostring(args))

	local targetID = string.match(menuOption, '%d+')
	local branchName = string.gsub(menuOption, targetID, "")

	local pTarget = getSceneObject(targetID)

	if (pTarget == nil) then
		printLuaError("Unable to find player in VillageGmSui:branchManagementCallback using oid " .. targetID)
		return
	end

	if (VillageJediManagerCommon.hasUnlockedBranch(pTarget, branchName)) then
		CreatureObject(pPlayer):sendSystemMessage(SceneObject(pTarget):getCustomObjectName() .. "'s " .. getStringId("@quest/force_sensitive/utils:" .. branchName) .. " has been LOCKED.")
		CreatureObject(pTarget):sendSystemMessage("Your unlock of the branch " .. getStringId("@quest/force_sensitive/utils:" .. branchName) .. " has been removed by a GM.")
		CreatureObject(pTarget):removeScreenPlayState(2, "VillageUnlockScreenPlay:" .. branchName)
	else
		CreatureObject(pPlayer):sendSystemMessage(SceneObject(pTarget):getCustomObjectName() .. "'s " .. getStringId("@quest/force_sensitive/utils:" .. branchName) .. " has been UNLOCKED.")
		VillageJediManagerCommon.unlockBranch(pTarget, branchName)
	end

	VillageGmSui.branchManagement(pPlayer, targetID)
end

function VillageGmSui:getPhaseDuration()
	local eventID = getServerEventID("VillagePhaseChange")

	if (eventID == nil) then
		return
	end

	return self:getTimeString(getServerEventTimeLeft(eventID))
end

function VillageGmSui:getTimeString(miliTime)
	local timeLeft = miliTime / 1000
	local daysLeft = math.floor(timeLeft / (24 * 60 * 60))
	local hoursLeft = math.floor((timeLeft / 3600) % 24)
	local minutesLeft = math.floor((timeLeft / 60) % 60)
	local secondsLeft = math.floor(timeLeft % 60)

	return daysLeft .. "d " .. hoursLeft .. "h " .. minutesLeft .. "m " .. secondsLeft .. "s"
end
