local Logger = require("utils.logger")
require("utils.helpers")

SpacestationNabooConvoHandler = Object:new {}

function SpacestationNabooConvoHandler:getInitialScreen(pPlayer, pNpc, pConvTemplate)
	local convoTemplate = LuaConversationTemplate(pConvTemplate)

	return convoTemplate:getScreen("spacestation_naboo_greeting")
end

function SpacestationNabooConvoHandler:getNextConversationScreen(pConvTemplate, pPlayer, selectedOption)
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
		pNextScreen = pConversation:getScreen("spacestation_naboo_greeting")
	end

	return pNextScreen
end

function SpacestationNabooConvoHandler:runScreenHandlers(pConvTemplate, pPlayer, pNpc, selectedOption, pConvScreen)
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
	if (screenID == "spacestation_naboo_greeting" and SpaceStationScreenPlay:hasCreditsForRepair(pPlayer, pShip, 0.25)) then
		pClonedConvo:addOption("@conversation/station_naboo:s_c358d041", "spacestation_naboo_repair") -- (Request Repairs)
	end

	if (screenID == "spacestation_naboo_land_theed_complete") then
		createEvent(1000, "SpaceStationScreenPlay", "landShip", pPlayer, "theed")
	elseif (screenID == "spacestation_naboo_land_kaadara_complete") then
		createEvent(1000, "SpaceStationScreenPlay", "landShip", pPlayer, "kaadara")
	elseif (screenID == "spacestation_naboo_land_keren_complete") then
		createEvent(1000, "SpaceStationScreenPlay", "landShip", pPlayer, "keren")
	elseif (screenID == "spacestation_naboo_land_moenia_complete") then
		createEvent(1000, "SpaceStationScreenPlay", "landShip", pPlayer, "moenia")
	end

	-- Handle Repair Options
	if (screenID == "spacestation_naboo_repair") then
		-- We already checked if player has enough credits for at least 25% repair, add that option
		pClonedConvo:addOption("@conversation/station_naboo:s_90ee7f5d", "repair_small") -- Just a little.

		if (SpaceStationScreenPlay:hasCreditsForRepair(pPlayer, pShip, 0.50)) then
			pClonedConvo:addOption("@conversation/station_naboo:s_4468336c", "repair_half") -- Half of it.
		end
		if (SpaceStationScreenPlay:hasCreditsForRepair(pPlayer, pShip, 0.75)) then
			pClonedConvo:addOption("@conversation/station_naboo:s_14897567", "repair_most") -- Most of it.
		end
		if (SpaceStationScreenPlay:hasCreditsForRepair(pPlayer, pShip, 1.00)) then
			pClonedConvo:addOption("@conversation/station_naboo:s_70876928", "repair_full") -- All of it.
		end

		pClonedConvo:addOption("@conversation/station_naboo:s_169df3bb", "spacestation_naboo_repair_deny") -- Never mind.
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
