local ObjectManager = require("managers.object.object_manager")

rebelCoordinatorConvoHandler = conv_handler:new {}

function rebelCoordinatorConvoHandler:getInitialScreen(pPlayer, pNpc, pConvTemplate)
	if (pPlayer == nil or pNpc == nil) then
		return convoTemplate:getScreen("generic_response")
	end

	local convoTemplate = LuaConversationTemplate(pConvTemplate)
	local pGhost = CreatureObject(pPlayer):getPlayerObject()
	local winningFaction = tonumber(CriesOfAlderaan:getWinningFaction())

	if (TangibleObject(pPlayer):isImperial()) then
		return convoTemplate:getScreen("begin_wrong_faction")
	elseif pGhost == nil or (not PlayerObject(pGhost):hasBadge(EVENT_PROJECT_DEAD_EYE_1) and not CriesOfAlderaan.skipToThree) then
		return convoTemplate:getScreen("generic_response")
	end

	local append = "_w"

	if (winningFaction == FACTIONIMPERIAL) then
		append = "_l"
	end

	-- Has coa3 badge already
	if (PlayerObject(pGhost):hasBadge(EVENT_COA3_REBEL)) then
		return convoTemplate:getScreen("coa3_init_complete" .. append)

	-- CoA3 Conversations: Has CoA2 badge or skipToThree is true
	elseif (PlayerObject(pGhost):hasBadge(EVENT_COA2_REBEL) or CriesOfAlderaan.skipToThree) then

		if (not CriesOfAlderaan.episodeThreeEnabled) then
			return convoTemplate:getScreen("generic_response")
		end

		local state = CriesOfAlderaan:getState(pPlayer, "coa3_rebel")

		if state == 0 then
			return convoTemplate:getScreen("coa3_init" .. append)
		elseif (state == Coa3Screenplay.PRE_INFO_OFFICER) then
			return convoTemplate:getScreen("coa3_init_go_to_info" .. append)
		elseif (state == Coa3Screenplay.PRE_RETURN) then
			return convoTemplate:getScreen("coa3_init_completed_info" .. append)
		elseif (state >= Coa3Screenplay.M1_FIND_LOOKOUT and state <= Coa3Screenplay.M2_RETURNED_UNIT and Coa3Screenplay:hasDisk(pPlayer)) then
			CriesOfAlderaan:setState(pPlayer, "coa3_rebel", Coa3Screenplay.M3_TACTICAL_OFFICER)

			return convoTemplate:getScreen("coa3_init_has_disk")
		elseif (state >= Coa3Screenplay.M1_FIND_LOOKOUT and state <= Coa3Screenplay.M2_RETURNED_UNIT and not Coa3Screenplay:hasDisk(pPlayer)) then
			return convoTemplate:getScreen("coa3_init_has_lookout" .. append)
		elseif (state >= Coa3Screenplay.M3_TACTICAL_OFFICER and state <= Coa3Screenplay.M3_WAREHOUSE_DESTROYED) then
			return convoTemplate:getScreen("coa3_init_go_to_tact")
		elseif (state == Coa3Screenplay.M3_COMPLETE) then
			return convoTemplate:getScreen("coa3_init_completed_tact")
		elseif (state >= Coa3Screenplay.M4_COMMANDER) then
			return convoTemplate:getScreen("coa3_init_go_to_princess" .. append)
		end

	-- CoA2 Conversations
	else
		if (not CriesOfAlderaan.episodeTwoEnabled) then
			return convoTemplate:getScreen("generic_response")
		end

		local state = CriesOfAlderaan:getState(pPlayer, "rebel_coa2")

		if state == 0 then
			return convoTemplate:getScreen("coa2_m1_begin")
		elseif state == Coa2Screenplay.M1_REFUSED then
			return convoTemplate:getScreen("coa2_m1_refused")
		elseif state == Coa2Screenplay.M1_ACTIVE then
			return convoTemplate:getScreen("coa2_m1_active")
		elseif state == Coa2Screenplay.M1_RETURN then
			if Coa2Screenplay:hasDisk(pPlayer, "decoded") then
				return convoTemplate:getScreen("coa2_m1_finish_decoded")
			elseif Coa2Screenplay:hasDisk(pPlayer, "encoded") then
				return convoTemplate:getScreen("coa2_m1_finish_encoded")
			else
				return convoTemplate:getScreen("coa2_m1_active")
			end
		elseif state == Coa2Screenplay.M1_COMPLETE then
			if Coa2Screenplay:hasDisk(pPlayer, "decoded") then
				return convoTemplate:getScreen("coa2_m2_begin")
			else
				return convoTemplate:getScreen("coa2_m2_begin_encoded")
			end
		elseif state == Coa2Screenplay.M2_REFUSED then
			return convoTemplate:getScreen("coa2_m2_refused")
		elseif state == Coa2Screenplay.M2_ACTIVE then
			return convoTemplate:getScreen("coa2_m2_active")
		elseif state == Coa2Screenplay.M2_RETURN then
			return convoTemplate:getScreen("coa2_m2_finish")
		elseif state == Coa2Screenplay.M2_COMPLETE then
			return convoTemplate:getScreen("coa2_m3_begin")
		elseif state == Coa2Screenplay.M3_4_ACTIVE then
			return convoTemplate:getScreen("coa2_m3_active")
		elseif state > Coa2Screenplay.M3_4_ACTIVE then
			return convoTemplate:getScreen("coa2_m3_finish")
		end
	end

	return convoTemplate:getScreen("generic_response")
end

function rebelCoordinatorConvoHandler:runScreenHandlers(pConvTemplate, pPlayer, pNpc, selectedOption, pConvScreen)
	if (pPlayer == nil) then
		return
	end

	local screen = LuaConversationScreen(pConvScreen)
	local screenID = screen:getScreenID()

	local faction = CreatureObject(pNpc):getFaction()
	local stateKey = Coa3Screenplay:getStateKey(faction)
	local state = CriesOfAlderaan:getState(pPlayer, stateKey)
	local playerID = SceneObject(pPlayer):getObjectID()
	local winningFaction = tonumber(CriesOfAlderaan:getWinningFaction())

	local append = "_w"

	if (winningFaction == FACTIONIMPERIAL) then
		append = "_l"
	end

	-- CoA2
	if screenID == "coa2_m1_begin_no" then
		writeScreenPlayData(pPlayer, "rebel_coa2", "state", Coa2Screenplay.M1_REFUSED)
	elseif screenID == "coa2_m1_begin_yes" or screenID == "coa2_m1_refused_yes" then
		Coa2Screenplay:startMissionOne(pPlayer, pNpc, "rebel")
	elseif screenID == "coa2_m1_active_abort" then
		writeScreenPlayData(pPlayer, "rebel_coa2", "state", Coa2Screenplay.M1_REFUSED)
		Coa2Screenplay:cleanupMission(pPlayer)
	elseif screenID == "coa2_m1_active_restart" then
		Coa2Screenplay:cleanupMission(pPlayer)
		Coa2Screenplay:startMissionOne(pPlayer, pNpc, "rebel")
	elseif screenID == "coa2_m1_finish_encoded" then
		Coa2Screenplay:finishMissionOne(pPlayer, "rebel")
	elseif screenID == "coa2_m1_finish_decoded" then
		writeScreenPlayData(pPlayer, "rebel_coa2", "state", Coa2Screenplay.M1_COMPLETE)
		Coa2Screenplay:cleanupMission(pPlayer)
	elseif screenID == "coa2_m2_begin_no" then
		writeScreenPlayData(pPlayer, "rebel_coa2", "state", Coa2Screenplay.M2_REFUSED)
	elseif screenID == "coa2_m2_begin_yes" or screenID == "coa2_m2_refused_yes" then
		Coa2Screenplay:startMissionTwo(pPlayer, pNpc, "rebel")
	elseif screenID == "coa2_m2_active_abort" then
		writeScreenPlayData(pPlayer, "rebel_coa2", "state", Coa2Screenplay.M2_REFUSED)
		Coa2Screenplay:cleanupMission(pPlayer)
	elseif screenID == "coa2_m2_active_restart" then
		Coa2Screenplay:cleanupMission(pPlayer)
		Coa2Screenplay:startMissionTwo(pPlayer, pNpc, "rebel")
	elseif screenID == "coa2_m2_finish" then
		writeScreenPlayData(pPlayer, "rebel_coa2", "state", Coa2Screenplay.M2_COMPLETE)
		Coa2Screenplay:cleanupMission(pPlayer)
	elseif screenID == "coa2_m2_finish_yes" then
		Coa2Screenplay:startMissionThree(pPlayer, pNpc, "rebel")
	elseif screenID == "coa2_m3_begin_yes" then
		Coa2Screenplay:startMissionThree(pPlayer, pNpc, "rebel")
	elseif screenID == "coa2_m3_active_abort" then
		writeScreenPlayData(pPlayer, "rebel_coa2", "state", Coa2Screenplay.M2_COMPLETE)
		Coa2Screenplay:cleanupMission(pPlayer)
	elseif screenID == "coa2_m3_active_restart" then
		Coa2Screenplay:cleanupMission(pPlayer)
		Coa2Screenplay:startMissionThree(pPlayer, pNpc, "rebel")

	-- CoA3
	elseif (screenID == ("coa3_give_you_report" .. append)) then
		CriesOfAlderaan:setState(pPlayer, "coa3_rebel", Coa3Screenplay.PRE_INFO_OFFICER)
	elseif (screenID == ("coa3_pose_as_them" .. append) or screenID == ("coa3_uploaded_lookout_location" .. append)) then
		CriesOfAlderaan:setState(pPlayer, "coa3_rebel", Coa3Screenplay.M1_FIND_LOOKOUT)
		Coa3Screenplay:setupMission(pPlayer, pNpc, "rebel", Coa3Screenplay.LOOKOUT_MISSION)
	elseif (screenID == ("coa3_ran_into_trouble")) then
		Coa3Screenplay:cleanUpCaravan(pPlayer)
		Coa3Screenplay:abortMission(pPlayer, Coa3Screenplay.LOOKOUT_MISSION, 0)

		deleteData(playerID .. ":CoA3:lookoutConvoFlow:")
		deleteData(playerID .. ":CoA3:lookoutTracker:")
		CriesOfAlderaan:setState(pPlayer, "coa3_rebel", Coa3Screenplay.M1_FIND_LOOKOUT)

		Coa3Screenplay:setupMission(pPlayer, pNpc, "rebel", Coa3Screenplay.LOOKOUT_MISSION)
	elseif (screenID == ("coa3_come_back_later")) then
		Coa3Screenplay:abortMission(pPlayer, Coa3Screenplay.LOOKOUT_MISSION, 0)
	elseif (screenID == ("coa3_big_impression") or screenID == ("coa3_thought_was_correct")) then
		CriesOfAlderaan:setState(pPlayer, "coa3_rebel", Coa3Screenplay.M4_COMMANDER)

		Coa3Screenplay:setupCommanderMission(pPlayer, "rebel")
	end

	return pConvScreen
end
