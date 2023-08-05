BartenderConversationHandler = conv_handler:new {}

function BartenderConversationHandler:runScreenHandlers(pConvTemplate, pPlayer, pNpc, selectedOption, pConvScreen)
	local screen = LuaConversationScreen(pConvScreen)

	local convoTemplate = LuaConversationTemplate(pConvTemplate)
	local pConvScreen = screen:cloneScreen()
	local clonedConversation = LuaConversationScreen(pConvScreen)

	local screenID = screen:getScreenID()
	local playerID = SceneObject(pPlayer):getObjectID()
	local npcID = SceneObject(pNpc):getObjectID()

	if (screenID == "opt_rumor") then
		local hasRumors = readData(npcID .. ":BartenderPlayerRumors:")

		-- Chance to have no rumor
		if (getRandomNumber(100) < 50) then
			return convoTemplate:getScreen("query_buy")
		elseif (hasRumors == 1) then
			local playerName = readStringData(npcID .. ":BartenderRumorPlayer:")
			local rumorString = readStringData(npcID .. ":BartenderRumorMessage:")

			clonedConversation:setDialogTextStringId("@bartender:prose_rumor" .. getRandomNumber(1,4))
			clonedConversation:setDialogTextTT(playerName)
			clonedConversation:setDialogTextTO(rumorString)

			deleteData(npcID .. ":BartenderPlayerRumors:")
			deleteStringData(npcID .. ":BartenderRumorPlayer:")
			deleteStringData(npcID .. ":BartenderRumorMessage:")
		else
			clonedConversation:setDialogTextStringId("@bartender:monthly_fiction_" .. getRandomNumber(1, 9))
		end
	elseif (screenID == "opt_buy" or screenID == "opt_yes") then
		-- Purchase Drink SUI
		local zoneName = CreatureObject(pPlayer):getZoneName()
		local drinksTable = BartendersScreenPlay.drinkOptions[zoneName]

		local pSui = SuiListBox.new("BartendersScreenPlay", "purchaseDrinkCallback")
		pSui.setTitle("@bartender:sui_title")
		pSui.setPrompt("@bartender:sui_prompt")

		for i = 1, #drinksTable, 1 do
			local drinkName = drinksTable[i][1]
			local drinkCost = drinksTable[i][2]
			local messageString = "@food_name:" .. drinkName

			pSui.add("[" .. drinkCost .. "] " .. getStringId(messageString), "")
		end

		pSui.sendTo(pPlayer)
	end

	return pConvScreen
end

function BartenderConversationHandler:getInitialScreen(pPlayer, pNpc, pConvTemplate)
	if (pPlayer == nil or pNpc == nil or pConvTemplate == nil) then
		return
	end

	local convoTemplate = LuaConversationTemplate(pConvTemplate)
	local npcID = SceneObject(pNpc):getObjectID()

	local rumorBartender = readData(npcID .. ":BartenderRumorFlag:")

	if (rumorBartender == 1) then
		return convoTemplate:getScreen("bartender_rumor_start")
	end

	return convoTemplate:getScreen("bartender_start")
end
