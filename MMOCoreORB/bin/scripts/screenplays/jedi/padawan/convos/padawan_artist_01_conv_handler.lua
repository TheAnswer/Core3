padawan_artist_01_conv_handler = conv_handler:new {}

function padawan_artist_01_conv_handler:getInitialScreen(pPlayer, pNpc, pConvTemplate)
	local convoTemplate = LuaConversationTemplate(pConvTemplate)
	local giverTrialNum = JediTrials:getTrialNumByName(pPlayer, "architect")
	local trialState = JediTrials:getTrialStateName(pPlayer, giverTrialNum)
	local trialOwnerID = readData(SceneObject(pNpc):getObjectID() .. ":ownerID")
	local playerID = SceneObject(pPlayer):getObjectID()

	if (trialOwnerID ~= playerID) then
		return convoTemplate:getScreen("not_quest_owner")
	end

	if (CreatureObject(pPlayer):hasScreenPlayState(1, trialState)) then
		return convoTemplate:getScreen("completed_quest")
	elseif (readData(playerID .. ":JediTrials:spokeToTarget") == 1) then
		return convoTemplate:getScreen("intro_in_progress")
	elseif (readData(playerID .. ":JediTrials:acceptedTask") == 1) then
		return convoTemplate:getScreen("intro_spoke_to_target")
	else
		return convoTemplate:getScreen("intro")
	end

end

function padawan_artist_01_conv_handler:runScreenHandlers(pConvTemplate, pPlayer, pNpc, selectedOption, pConvScreen)
	local screen = LuaConversationScreen(pConvScreen)
	local screenID = screen:getScreenID()
	local pConvScreen = screen:cloneScreen()
	local clonedConversation = LuaConversationScreen(pConvScreen)
	local playerID = SceneObject(pPlayer):getObjectID()

	if (screenID == "true_colors" or screenID == "forget_i_asked") then
		PadawanTrials:failTrial(pPlayer)
	elseif (screenID == "not_first_time" or screenID == "spoke_hastily") then
		PadawanTrials:passTrial(pPlayer)
	elseif (screenID == "meet_assistant") then
		writeData(playerID .. ":JediTrials:acceptedTask", 1)
		writeData(SceneObject(pNpc):getObjectID() .. ":destroyNpcOnExit", 1)
		PadawanTrials:createSecondLocation(pPlayer)
	end

	return pConvScreen
end
