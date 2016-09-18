local ObjectManager = require("managers.object.object_manager")

heroOfTatHermitConvoHandler = {  }

--[[
1 - Started squill quest
2 - Completed squill quest
4 - Completed Altruism
8 - Completed Intellect
16 - Completed Courage
32 - Completed Honor
64 - Completed Mark of Hero
]]

function heroOfTatHermitConvoHandler:getNextConversationScreen(pConversationTemplate, pPlayer, selectedOption, pConversingNpc)
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

function heroOfTatHermitConvoHandler:getInitialScreen(pPlayer, pNpc, pConversationTemplate)
	local convoTemplate = LuaConversationTemplate(pConversationTemplate)
	if (HeroOfTatooineScreenPlay:isMissingMark(pPlayer)) then
		return convoTemplate:getScreen("you_have_returned_missing_marks")
	elseif (CreatureObject(pPlayer):hasScreenPlayState(64, "hero_of_tatooine")) then
		return convoTemplate:getScreen("hero_returns")
	elseif (CreatureObject(pPlayer):hasScreenPlayState(2, "hero_of_tatooine")) then
		return convoTemplate:getScreen("you_have_returned")
	elseif (CreatureObject(pPlayer):hasScreenPlayState(1, "hero_of_tatooine")) then
		return convoTemplate:getScreen("return_intro")
	else
		return convoTemplate:getScreen("intro")
	end
end

function heroOfTatHermitConvoHandler:runScreenHandlers(conversationTemplate, conversingPlayer, conversingNPC, selectedOption, conversationScreen)
	local pGhost = CreatureObject(conversingPlayer):getPlayerObject()

	if (pGhost == nil) then
		return conversationScreen
	end

	local screen = LuaConversationScreen(conversationScreen)

	local screenID = screen:getScreenID()

	local conversationScreen = screen:cloneScreen()
	local clonedConversation = LuaConversationScreen(conversationScreen)
	if (screenID == "go_then") then
		CreatureObject(conversingPlayer):setScreenPlayState(1, "hero_of_tatooine")
	elseif (screenID == "you_have_returned_missing_marks") then
		local pInventory = CreatureObject(conversingPlayer):getSlottedObject("inventory")

		if (pInventory == nil or SceneObject(pInventory):isContainerFullRecursive()) then
			clonedConversation:addOption("@conversation/quest_hero_of_tatooine_hermit:s_6441a2a6", "inv_full")
		else
			if (HeroOfTatooineScreenPlay:giveMissingMarks(conversingPlayer)) then
				clonedConversation:addOption("@conversation/quest_hero_of_tatooine_hermit:s_6441a2a6", "now_have_items")
			else
				clonedConversation:addOption("@conversation/quest_hero_of_tatooine_hermit:s_6441a2a6", "dont_have_room")
			end
		end
	elseif (screenID == "you_have_returned") then
		if (CreatureObject(conversingPlayer):hasScreenPlayState(2, "hero_of_tatooine") and CreatureObject(conversingPlayer):hasScreenPlayState(4, "hero_of_tatooine") and CreatureObject(conversingPlayer):hasScreenPlayState(8, "hero_of_tatooine")
			and CreatureObject(conversingPlayer):hasScreenPlayState(16, "hero_of_tatooine") and CreatureObject(conversingPlayer):hasScreenPlayState(32, "hero_of_tatooine")) then
			clonedConversation:addOption("@conversation/quest_hero_of_tatooine_hermit:s_cd2c070a", "you_bear_marks")
		end
		clonedConversation:addOption("@conversation/quest_hero_of_tatooine_hermit:s_503d468c", "which_mark")
		clonedConversation:addOption("@conversation/quest_hero_of_tatooine_hermit:s_da196589", "as_you_wish")
	elseif (screenID == "done_great_things") then

		local pInventory = CreatureObject(conversingPlayer):getSlottedObject("inventory")

		if (pInventory == nil) then
			return
		end

		if (SceneObject(pInventory):isContainerFullRecursive()) then
			CreatureObject(conversingPlayer):setScreenPlayState(16, "hero_of_tatooine_missing_marks") -- 1 - Altruism, 2 - Intellect, 4 - Courage, 8 - Honor, 16 - Ring
			CreatureObject(conversingPlayer):sendSystemMessage("@quest/hero_of_tatooine/system_messages:hero_inv_full")
		else
			local pMark = giveItem(pInventory, "object/tangible/wearables/ring/ring_mark_hero.iff", -1)

			if (pMark == nil) then
				CreatureObject(conversingPlayer):setScreenPlayState(16, "hero_of_tatooine_missing_marks") -- 1 - Altruism, 2 - Intellect, 4 - Courage, 8 - Honor, 16 - Ring
				CreatureObject(conversingPlayer):sendSystemMessage("Error creating object. Please file a bug report.")
			end
		end

		PlayerObject(pGhost):awardBadge(11)
		CreatureObject(conversingPlayer):setScreenPlayState(64, "hero_of_tatooine")
		CreatureObject(conversingNPC):doAnimation("worship")
	elseif (screenID == "return_intro") then
		local pInventory =  CreatureObject(conversingPlayer):getSlottedObject("inventory")

		if (pInventory ~= nil) then
			local pSkull = getContainerObjectByTemplate(pInventory, "object/tangible/loot/quest/hero_of_tatooine/squill_skull.iff", true)

			if (pSkull ~= nil) then
				clonedConversation:addOption("@conversation/quest_hero_of_tatooine_hermit:s_af540d0", "proven_worthy")
			end
		end
		clonedConversation:addOption("@conversation/quest_hero_of_tatooine_hermit:s_da196589", "as_you_wish")
	elseif (screenID == "proven_worthy") then
		local pInventory = CreatureObject(conversingPlayer):getSlottedObject("inventory")

		if (pInventory ~= nil) then
			local pSkull = getContainerObjectByTemplate(pInventory, "object/tangible/loot/quest/hero_of_tatooine/squill_skull.iff", true)

			if (pSkull == nil) then
				return 0
			end

			SceneObject(pSkull):destroyObjectFromWorld()
			SceneObject(pSkull):destroyObjectFromDatabase()
			CreatureObject(conversingPlayer):setScreenPlayState(2, "hero_of_tatooine")
			CreatureObject(conversingNPC):doAnimation("taken_aback")
		end
	elseif (screenID == "intro") then
		CreatureObject(conversingNPC):doAnimation("greet")
	end

	return conversationScreen
end
