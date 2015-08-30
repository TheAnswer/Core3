local ObjectManager = require("managers.object.object_manager")

rebelSympathizerConvoHandler = Object:new {}

function rebelSympathizerConvoHandler:getInitialScreen(pPlayer, npc, pConversationTemplate)
	local convoTemplate = LuaConversationTemplate(pConversationTemplate)

	local npcID = SceneObject(npc):getObjectID()
	local playersNpcID = readData(SceneObject(pPlayer):getObjectID() .. ":coaNpcID")

	if npcID == playersNpcID then
		local state = tonumber(readScreenPlayData(pPlayer, "rebel_coa2", "state"))

		if state == 3 then
			return convoTemplate:getScreen("m1_sympathizer_complete")
		else
			return convoTemplate:getScreen("m1_sympathizer_greeting")
		end
	else
		return convoTemplate:getScreen("m1_sympathizer_invalid")
	end
end

function rebelSympathizerConvoHandler:runScreenHandlers(conversationTemplate, conversingPlayer, conversingNPC, selectedOption, conversationScreen)
	local screen = LuaConversationScreen(conversationScreen)
	local screenID = screen:getScreenID()

	if screenID == "m1_sympathizer_greeting" then
		Coa2Screenplay:progressMissionOne(conversingPlayer, "rebel")
	end

	return conversationScreen
end

function rebelSympathizerConvoHandler:getNextConversationScreen(pConversationTemplate, pPlayer, selectedOption, pConversingNpc)
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
