local ObjectManager = require("managers.object.object_manager")

heroOfTatRanchersWifeConvoHandler = {  }

function heroOfTatRanchersWifeConvoHandler:getNextConversationScreen(pConversationTemplate, pPlayer, selectedOption, pConversingNpc)
	local pConversationSession = CreatureObject(pPlayer):getConversationSession()
	local pLastConversationScreen = nil
	if (pConversationSession ~= nil) then
		local conversationSession = LuaConversationSession(pConversationSession)
		pLastConversationScreen = conversationSession:getLastConversationScreen()
	end
	local conversationTemplate = LuaConversationTemplate(pConversationTemplate)
	if (pLastConversationScreen ~= nil) then
		local lastConversationScreen = LuaConversationScreen(pLastConversationScreen)
		local optionLink = lastConversationScreen:getOptionLink(selectedOption)
		return conversationTemplate:getScreen(optionLink)
	end
	return self:getInitialScreen(pPlayer, pConversingNpc, pConversationTemplate)
end

function heroOfTatRanchersWifeConvoHandler:getInitialScreen(pPlayer, pNpc, pConversationTemplate)
	local convoTemplate = LuaConversationTemplate(pConversationTemplate)

	local inProgressID = readData("hero_of_tat:ranch_player_id")
	if (inProgressID ~= 0 and inProgressID ~= player:getObjectID()) then
		return convoTemplate:getScreen("intro_otherinprogress")
	elseif (CreatureObject(pPlayer):hasScreenPlayState(2, "hero_of_tatooine_honor")) then
		return convoTemplate:getScreen("intro_noquest")
	elseif (not CreatureObject(pPlayer):hasScreenPlayState(2, "hero_of_tatooine_honor") and CreatureObject(pPlayer):hasScreenPlayState(1, "hero_of_tatooine_honor") and readData(CreatureObject(pPlayer):getObjectID() .. ":hero_of_tat_honor:accepted") == 1) then
		if (readData(CreatureObject(pPlayer):getObjectID() .. ":hero_of_tat_honor:distracting_wife") == 1) then
			return convoTemplate:getScreen("intro_distract")
		else
			return convoTemplate:getScreen("intro_hasquest")
		end
	elseif (readData(CreatureObject(pPlayer):getObjectID() .. ":hero_of_tat_honor:spoke_to_wife") == 1) then
		return convoTemplate:getScreen("intro_leftprior")
	else
		return convoTemplate:getScreen("intro")
	end
end

function heroOfTatRanchersWifeConvoHandler:runScreenHandlers(conversationTemplate, conversingPlayer, conversingNPC, selectedOption, conversationScreen)
	local screen = LuaConversationScreen(conversationScreen)

	local screenID = screen:getScreenID()

	local conversationScreen = screen:cloneScreen()
	local clonedConversation = LuaConversationScreen(conversationScreen)

	if (screenID == "leave_now") then
		writeData(CreatureObject(conversingPlayer):getObjectID() .. ":hero_of_tat_honor:spoke_to_wife", 1)
	elseif (screenID == "thanks_intercom") then
		local inProgressID = readData("hero_of_tat:ranch_player_id")
		if (inProgressID ~= 0 and inProgressID ~= SceneObject(conversingPlayer):getObjectID()) then
			clonedConversation:setDialogTextStringId("@conversation/quest_hero_of_tatooine_wife:s_c74cdffb")
		else
			HeroOfTatooineScreenPlay:doHonorStart(conversingPlayer)
		end
	elseif (screenID == "so_nice_ahh") then
		HeroOfTatooineScreenPlay:doHonorFail(conversingPlayer)
	elseif (screenID == "cant_believe_this") then
		HeroOfTatooineScreenPlay:doHonorSuccess(conversingPlayer)
		HeroOfTatooineScreenPlay:doSuccessHonorPhase(conversingPlayer)
	end

	return conversationScreen
end
