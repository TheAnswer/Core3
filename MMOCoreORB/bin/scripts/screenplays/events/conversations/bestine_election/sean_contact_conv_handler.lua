local ObjectManager = require("managers.object.object_manager")

seanContactConvoHandler = Object:new {}

function seanContactConvoHandler:hasHistoryDisk(pPlayer)
	if (pPlayer ~= nil) then
		if (getQuestStatus("bestine_election:electionWinner") == "Sean") then
			local pInventory = CreatureObject(pPlayer):getSlottedObject("inventory")
			if (pInventory ~= nil) then
				if (getContainerObjectByTemplate(pInventory, "object/tangible/loot/quest/sean_history_disk.iff", true) ~= nil) then
					return true
				end
			end
		end
	end
	return false
end

function seanContactConvoHandler:destroyHDisk(pPlayer)
	if (pPlayer ~= nil) then
		local playerID = CreatureObject(pPlayer):getObjectID()
		local pPlayerObj = CreatureObject(pPlayer):getPlayerObject()
		if (pPlayerObj ~= nil) then
			local pInventory = CreatureObject(pPlayer):getSlottedObject("inventory")
			if pInventory ~= nil then
				local hDisk = getContainerObjectByTemplate(pInventory, "object/tangible/loot/quest/sean_history_disk.iff" , true)
				if (hDisk ~= nil) then
					SceneObject(hDisk):destroyObjectFromWorld()
					SceneObject(hDisk):destroyObjectFromDatabase()

					local wayID = readData(playerID .. ":bestine_election:contactWaypointID")
					PlayerObject(pPlayerObj):removeWaypoint(wayID, true)
					deleteData(playerID .. ":bestine_election:contactWaypointID")
					CreatureObject(pPlayer):removeScreenPlayState(16, "bestine_history_quest")
					CreatureObject(pPlayer):setScreenPlayState(32, "bestine_history_quest")
				end
			end
		end
	end
end


function seanContactConvoHandler:getInitialScreen(pPlayer, npc, pConversationTemplate)
	local convoTemplate = LuaConversationTemplate(pConversationTemplate)

	if self:hasHistoryDisk(pPlayer) then
		return convoTemplate:getScreen("sean_inOffice_hasItem")
	elseif (getQuestStatus("bestine_election:electionWinner") == "Sean")then
		return convoTemplate:getScreen("sean_inOffice_noItem")
	end
	return convoTemplate:getScreen("sean_notinOffice")
end

function seanContactConvoHandler:runScreenHandlers(conversationTemplate, conversingPlayer, conversingNPC, selectedOption, conversationScreen)
	local screen = LuaConversationScreen(conversationScreen)
	local screenID = screen:getScreenID()
	local conversationScreen = screen:cloneScreen()


	if (screenID == "seanContact_itemDestroyed") then
		self:destroyHDisk(conversingPlayer)
	end

	return conversationScreen
end

function seanContactConvoHandler:getNextConversationScreen(pConversationTemplate, pPlayer, selectedOption, pConversingNpc)
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
