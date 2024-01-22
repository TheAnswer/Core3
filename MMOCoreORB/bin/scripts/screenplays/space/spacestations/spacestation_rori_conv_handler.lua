local Logger = require("utils.logger")
require("utils.helpers")

SpacestationRoriConvoHandler = Object:new {}

function SpacestationRoriConvoHandler:getInitialScreen(pPlayer, pNpc, pConvTemplate)
	local convoTemplate = LuaConversationTemplate(pConvTemplate)

	return convoTemplate:getScreen("spacestation_rori_greeting")
end

function SpacestationRoriConvoHandler:getNextConversationScreen(pConvTemplate, pPlayer, selectedOption)
	if (pPlayer == nil or pConvTemplate == nil) then
		return
	end

	local pConvoSession = CreatureObject(pPlayer):getConversationSession()
	local pConversation = LuaConversationTemplate(pConvTemplate)

	if (pConversation == nil)  then
		return
	end

	local pLastScreen
	local pNextScreen

	if (pConvoSession ~= nil) then
		pLastScreen = LuaConversationSession(pConvoSession):getLastConversationScreen()
	end

	if (pLastScreen ~= nil) then
		local pOptionLink = LuaConversationScreen(pLastScreen):getOptionLink(selectedOption)

		if (pOptionLink ~= nil) then
			pNextScreen = pConversation:getScreen(pOptionLink)
		end
	end

	if (pNextScreen == nil) then
		pNextScreen = pConversation:getScreen("spacestation_rori_greeting")
	end

	return pNextScreen
end

function SpacestationRoriConvoHandler:runScreenHandlers(pConvTemplate, pPlayer, pNpc, selectedOption, pConvScreen)
	if (pPlayer == nil or pConvScreen == nil) then
		return
	end

	local screen = LuaConversationScreen(pConvScreen)
	local screenID = screen:getScreenID()

	local pScreenClone = screen:cloneScreen()
	local pClonedConvo = LuaConversationScreen(pScreenClone)

	pClonedConvo:setDialogTextTU(CreatureObject(pPlayer):getFirstName())

	local pShip = SceneObject(pPlayer):getRootParent()

	if (pShip == nil or not SceneObject(pShip):isShipObject()) then
		return
	end

	-- Only show request repairs if the player has enough credits for the smallest repair
	if (screenID == "spacestation_rori_greeting" and SpaceStationScreenPlay:hasCreditsForRepair(pPlayer, pShip, 0.25)) then
		pClonedConvo:addOption("@conversation/station_rori:s_373", "spacestation_rori_repair") -- I need repairs.
	end

	if (screenID == "spacestation_rori_land_narmle_complete") then
		createEvent(1000, "SpaceStationScreenPlay", "landShip", pPlayer, "narmle")
	elseif (screenID == "spacestation_rori_land_restuss_complete") then
		createEvent(1000, "SpaceStationScreenPlay", "landShip", pPlayer, "restuss")
	end

	-- Handle Repair Options
	if (screenID == "spacestation_rori_repair" or screenID == "spacestation_rori_repair2") then
		-- We already checked if player has enough credits for at least 25% repair, add that option
		pClonedConvo:addOption("@conversation/station_rori:s_377", "repair_small") -- Just a little.

		if (SpaceStationScreenPlay:hasCreditsForRepair(pPlayer, pShip, 0.50)) then
			pClonedConvo:addOption("@conversation/station_rori:s_389", "repair_half") -- Repair half the damage.
		end
		if (SpaceStationScreenPlay:hasCreditsForRepair(pPlayer, pShip, 0.75)) then
			pClonedConvo:addOption("@conversation/station_rori:s_401", "repair_most") -- Repair most of the damage.
		end
		if (SpaceStationScreenPlay:hasCreditsForRepair(pPlayer, pShip, 1.00)) then
			pClonedConvo:addOption("@conversation/station_rori:s_413", "repair_full") -- Fully repair my ship.
		end

		pClonedConvo:addOption("@conversation/station_rori:s_429", "spacestation_rori_repair_unsure") -- Are you sure you can repair my ship?
	end

	-- Repair selection
	if (screenID == "repair_small") then
		pClonedConvo:setDialogTextDI(SpaceStationScreenPlay:getRepairCost(pShip, 0.25))
	elseif (screenID == "repair_half") then
		pClonedConvo:setDialogTextDI(SpaceStationScreenPlay:getRepairCost(pShip, 0.50))
	elseif (screenID == "repair_most") then
		pClonedConvo:setDialogTextDI(SpaceStationScreenPlay:getRepairCost(pShip, 0.75))
	elseif (screenID == "repair_full") then
		pClonedConvo:setDialogTextDI(SpaceStationScreenPlay:getRepairCost(pShip, 1.0))
	end

	-- Handle Repairing Ship
	if (screenID == "accept_repair_25") then
		SpaceStationScreenPlay:repairShip(pPlayer, pShip, 0.25, pNpc)
	elseif (screenID == "accept_repair_50") then
		SpaceStationScreenPlay:repairShip(pPlayer, pShip, 0.50, pNpc)
	elseif (screenID == "accept_repair_75") then
		SpaceStationScreenPlay:repairShip(pPlayer, pShip, 0.75, pNpc)
	elseif (screenID == "accept_repair_full") then
		SpaceStationScreenPlay:repairShip(pPlayer, pShip, 1.0, pNpc)
	end

	return pScreenClone
end
