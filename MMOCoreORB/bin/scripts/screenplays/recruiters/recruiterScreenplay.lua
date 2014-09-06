local ObjectManager = require("managers.object.object_manager")
includeFile("recruiters/factionPerkData.lua")

recruiterScreenplay = Object:new {
	minimumFactionStanding = 200,

	factionHashCode = { rebel = 370444368, imperial = 3679112276 },

	errorCodes =  {
		SUCCESS = 0, INVENTORYFULL = 1,  NOTENOUGHFACTION = 2, GENERALERROR = 3, ITEMCOST = 4, INVENTORYERROR = 5,
		TEMPLATEPATHERROR = 6, GIVEERROR = 7, DATAPADFULL = 8, DATAPADERROR = 9, TOOMANYHIRELINGS = 10,
	}
}

function recruiterScreenplay:getFactionHashCode(faction)
	if (faction == "rebel") then
		return self.factionHashCode.rebel
	elseif (faction == "imperial") then
		return self.factionHashCode.imperial
	else
		return nil
	end
end

function recruiterScreenplay:getFactionFromHashCode(hash)
	if (hash == self.factionHashCode.rebel) then
		return "rebel"
	elseif (hash == self.factionHashCode.imperial) then
		return "imperial"
	else
		return nil
	end
end

function recruiterScreenplay:getRecruiterFactionHashCode(pNpc)
	local faction = self:getRecruiterFaction(pNpc)
	if (faction == "rebel") then
		return self.factionHashCode.rebel
	elseif (faction == "imperial") then
		return self.factionHashCode.imperial
	else
		return nil
	end
end

function recruiterScreenplay:getRecruiterFaction(pNpc)
	return self:getFactionFromHashCode(TangibleObject(pNpc):getFaction())
end

function recruiterScreenplay:getRecruiterEnemyFaction(pNpc)
	if (self:getRecruiterFaction(pNpc) == "rebel") then
		return self:getFactionHashCode("imperial")
	elseif (self:getRecruiterFaction(pNpc) == "imperial") then
		return self:getFactionHashCode("rebel")
	end
	return nil
end

function recruiterScreenplay:getRecruiterEnemyFactionHashCode(pNpc)
	if (self:getRecruiterFaction(pNpc) == "rebel") then
		return self.factionHashCode.imperial
	elseif (self:getRecruiterFaction(pNpc) == "imperial") then
		return self.factionHashCode.rebel
	end
	return nil
end

function recruiterScreenplay:grantBribe(pRecruiter, pPlayer, cost, factionPoints)
	ObjectManager.withCreatureAndPlayerObject(pPlayer, function(player, playerObject)
		if (player:getCashCredits() >= cost) then
			player:subtractCashCredits(cost)
			playerObject:increaseFactionStanding(self:getRecruiterFaction(pRecruiter), factionPoints)
		end
	end)
end

function recruiterScreenplay:getFactionDataTable(faction)
	if (faction == "rebel") then
		return rebelRewardData
	elseif (faction == "imperial") then
		return imperialRewardData
	else
		return nil
	end
end

function recruiterScreenplay:getMinimumFactionStanding()
	return self.minimumFactionStanding
end

function recruiterScreenplay:isWeapon(faction, strItem)
	local factionRewardData = self:getFactionDataTable(faction)
	return factionRewardData.weaponsArmor[strItem] ~= nil and factionRewardData.weaponsArmor[strItem].type == factionRewardType.weapon
end

function recruiterScreenplay:isArmor(faction, strItem)
	local factionRewardData = self:getFactionDataTable(faction)
	return factionRewardData.weaponsArmor[strItem] ~= nil and factionRewardData.weaponsArmor[strItem].type == factionRewardType.armor
end


function recruiterScreenplay:isUniform(faction, strItem)
	if faction == "rebel" then
		return false
	end
	local factionRewardData = self:getFactionDataTable(faction)
	return factionRewardData.uniforms[strItem] ~= nil
end

function recruiterScreenplay:isHireling(faction, strItem)
	local factionRewardData = self:getFactionDataTable(faction)
	return factionRewardData.hirelings[strItem] ~= nil
end

function recruiterScreenplay:isFurniture(faction, strItem)
	local factionRewardData = self:getFactionDataTable(faction)
	return factionRewardData.furniture[strItem] ~= nil
end

function recruiterScreenplay:isContainer(faction, strItem)
	local factionRewardData = self:getFactionDataTable(faction)
	return factionRewardData.furniture[strItem] ~= nil and factionRewardData.furniture[strItem].type == factionRewardType.container
end

function recruiterScreenplay:isTerminal(faction, strItem)
	local factionRewardData = self:getFactionDataTable(faction)
	return factionRewardData.furniture[strItem] ~= nil and factionRewardData.furniture[strItem].type == factionRewardType.terminal
end

function recruiterScreenplay:isInstallation(faction, strItem)
	local factionRewardData = self:getFactionDataTable(faction)
	return factionRewardData.installations[strItem] ~= nil and factionRewardData.installations[strItem].type == factionRewardType.installation
end

function recruiterScreenplay:getWeaponsArmorOptions(faction, gcwDiscount, smugglerDiscount)
	local optionsTable = { }
	local factionRewardData = self:getFactionDataTable(faction)
	for k,v in pairs(factionRewardData.weaponsArmorList) do
		if ( factionRewardData.weaponsArmor[v] ~= nil and 	factionRewardData.weaponsArmor[v].display ~= nil and factionRewardData.weaponsArmor[v].cost ~= nil ) then
			table.insert(optionsTable, self:generateSuiString(factionRewardData.weaponsArmor[v].display, math.ceil(factionRewardData.weaponsArmor[v].cost * gcwDiscount * smugglerDiscount)))
		end
	end
	return optionsTable
end

function recruiterScreenplay:getFurnitureOptions(faction, gcwDiscount, smugglerDiscount)
	local optionsTable = { }
	local factionRewardData = self:getFactionDataTable(faction)
	for k,v in pairs(factionRewardData.furnitureList) do
		if ( factionRewardData.furniture[v] ~= nil and factionRewardData.furniture[v].display ~= nil and factionRewardData.furniture[v].cost ~= nil ) then
			table.insert(optionsTable, self:generateSuiString(factionRewardData.furniture[v].display, math.ceil(factionRewardData.furniture[v].cost * gcwDiscount * smugglerDiscount)))
		end
	end
	return optionsTable
end

function recruiterScreenplay:getInstallationsOptions(faction, gcwDiscount, smugglerDiscount)
	local optionsTable = { }
	local factionRewardData = self:getFactionDataTable(faction)
	for k,v in pairs(factionRewardData.installationsList) do
		if ( factionRewardData.installations[v] ~= nil and factionRewardData.installations[v].display ~= nil and factionRewardData.installations[v].cost ~= nil ) then
			table.insert(optionsTable, self:generateSuiString(factionRewardData.installations[v].display, math.ceil(factionRewardData.installations[v].cost * gcwDiscount * smugglerDiscount)))
		end
	end
	return optionsTable
end

function recruiterScreenplay:getHirelingsOptions(faction, gcwDiscount, smugglerDiscount)
	local optionsTable = { }
	local factionRewardData = self:getFactionDataTable(faction)
	for k,v in pairs(factionRewardData.hirelingList) do
		if ( factionRewardData.hirelings[v] ~= nil and factionRewardData.hirelings[v].display ~= nil and factionRewardData.hirelings[v].cost ~= nil ) then
			table.insert(optionsTable, self:generateSuiString(factionRewardData.hirelings[v].display, math.ceil(factionRewardData.hirelings[v].cost * gcwDiscount * smugglerDiscount)))
		end
	end
	return optionsTable
end

function recruiterScreenplay:getUniformsOptions(faction, gcwDiscount, smugglerDiscount)
	local optionsTable = { }
	local factionRewardData = self:getFactionDataTable(faction)
	for k,v in pairs(factionRewardData.uniformList) do
		if ( factionRewardData.uniforms[v] ~= nil and factionRewardData.uniforms[v].display ~= nil and factionRewardData.uniforms[v].cost ~= nil ) then
			table.insert(optionsTable, self:generateSuiString(factionRewardData.imperialUniforms[v].display, math.ceil(factionRewardData.imperialUniforms[v].cost * gcwDiscount * smugglerDiscount)))
		end
	end
end

function recruiterScreenplay:generateSuiString(item, cost)
	return getStringId(item) .. " (Cost: " .. cost .. ")"
end

function recruiterScreenplay:getItemCost(faction, itemString)
	local factionRewardData = self:getFactionDataTable(faction)
	if self:isWeapon(faction, itemString) or self:isArmor(faction, itemString)  and factionRewardData.weaponsArmor[itemString] ~= nil and factionRewardData.weaponsArmor[itemString].cost ~= nil then
		return factionRewardData.weaponsArmor[itemString].cost
	elseif self:isUniform(faction, itemString) and factionRewardData.uniforms[itemString].cost ~= nil then
		return factionRewardData.uniforms[itemString].cost
	elseif self:isFurniture(faction, itemString) and factionRewardData.furniture[itemString].cost ~= nil then
		return factionRewardData.furniture[itemString].cost
	elseif self:isInstallation(faction, itemString) and factionRewardData.installations[itemString].cost ~= nil then
		return factionRewardData.installations[itemString].cost
	elseif self:isHireling(faction, itemString) and factionRewardData.hirelings[itemString].cost ~= nil then
		return factionRewardData.hirelings[itemString].cost
	end
	return nil
end

function recruiterScreenplay:getTemplatePath(faction, itemString)
	local factionRewardData = self:getFactionDataTable(faction)
	if self:isWeapon(faction, itemString) or self:isArmor(faction, itemString) then
		return factionRewardData.weaponsArmor[itemString].item
	elseif self:isUniform(faction, itemString) then
		return factionRewardData.uniforms[itemString].item
	elseif self:isFurniture(faction, itemString) then
		return factionRewardData.furniture[itemString].item
	elseif self:isInstallation(faction, itemString) then
		return factionRewardData.installations[itemString].item
	elseif self:isHireling(faction, itemString) then
		return factionRewardData.hirelings[itemString].item
	end
	return nil
end


function recruiterScreenplay:getGeneratedObjectTemplate(faction, itemString)
	local factionRewardData = self:getFactionDataTable(faction)
	if self:isInstallation(faction, itemString) and factionRewardData.installations[itemString].generatedObjectTemplate ~= nil then
		return factionRewardData.installations[itemString].generatedObjectTemplate
	end
	return nil
end

function recruiterScreenplay:getControlledObjectTemplate(faction, itemString)
	local factionRewardData = self:getFactionDataTable(faction)
	if self:isHireling(faction, itemString) and factionRewardData.hirelings[itemString].controlledObjectTemplate ~= nil then
		return factionRewardData.hirelings[itemString].controlledObjectTemplate
	end
	return nil
end

function recruiterScreenplay:getBonusItems(faction, itemString)
	local factionRewardData = self:getFactionDataTable(faction)
	if self:isInstallation(faction, itemString) and factionRewardData.installations[itemString].bonus ~= nil then
		return factionRewardData.installations[itemString].bonus
	end
	return nil
end

function recruiterScreenplay:getBonusItemCount(faction, itemString)
	local factionRewardData = self:getFactionDataTable(faction)
	if self:isInstallation(faction, itemString) and factionRewardData.installations[itemString].bonus ~= nil then
		return #factionRewardData.installations[itemString].bonus
	end
	return 0
end

function recruiterScreenplay:sendPurchaseSui(pNpc, pPlayer, screenID)
	local faction = self:getRecruiterFaction(pNpc)
	local gcwDiscount = getGCWDiscount(pPlayer)
	local smugglerDiscount = self:getSmugglerDiscount(pPlayer)
	writeStringData(CreatureObject(pPlayer):getObjectID() .. ":faction_purchase", screenID)
	local suiManager = LuaSuiManager()
	local options = { }
	if screenID == "fp_furniture" then
		options = self:getFurnitureOptions(faction, gcwDiscount, smugglerDiscount)
	elseif screenID == "fp_weapons_armor" then
		options = self:getWeaponsArmorOptions(faction, gcwDiscount, smugglerDiscount)
	elseif screenID == "fp_installations" then
		options = self:getInstallationsOptions(faction, gcwDiscount, smugglerDiscount)
	elseif screenID == "fp_uniforms" then
		options = self:getUniformsOptions(faction, gcwDiscount, smugglerDiscount)
	elseif screenID == "fp_hirelings" then
		options = self:getHirelingsOptions(faction, gcwDiscount, smugglerDiscount)
	end

	suiManager:sendListBox(pNpc, pPlayer, "@faction_recruiter:faction_purchase", "@faction_recruiter:select_item_purchase", 2, "@cancel", "", "@ok", "recruiterScreenplay", "handleSuiPurchase", options)
end

function recruiterScreenplay:handleSuiPurchase(pCreature, pSui, cancelPressed, arg0)
	if (cancelPressed) then
		deleteStringData(CreatureObject(pCreature):getObjectID() .. ":faction_purchase")
		return
	end
	local purchaseCategory = readStringData(CreatureObject(pCreature):getObjectID() .. ":faction_purchase")
	local purchaseIndex = arg0 + 1
	local faction = self:getFactionFromHashCode(CreatureObject(pCreature):getFaction())
	local dataTable = self:getFactionDataTable(faction)
	local itemListTable = self:getItemListTable(faction, purchaseCategory)
	local itemString = itemListTable[purchaseIndex]
	deleteStringData(CreatureObject(pCreature):getObjectID() .. ":faction_purchase")

	local awardResult = nil

	if (self:isHireling(faction, itemString)) then
		awardResult = self:awardData(pCreature, faction, itemString)
	else
		awardResult = self:awardItem(pCreature, faction, itemString)
	end

	if (awardResult == self.errorCodes.SUCCESS) then
		local messageString
		if (self:isHireling(faction, itemString)) then
			messageString = LuaStringIdChatParameter("@faction_recruiter:hireling_purchase_complete")
		else
			messageString = LuaStringIdChatParameter("@faction_recruiter:item_purchase_complete")
		end

		if self:isArmor(faction, itemString) or self:isUniform(faction, itemString) then
			messageString:setTT("@wearables_name:" .. itemString)
		elseif self:isWeapon(faction, itemString) then
			messageString:setTT("@weapon_name:" .. itemString)
		elseif self:isFurniture(faction, itemString) and self:isContainer(faction, itemString) then
			messageString:setTT("container_name",itemString)
		elseif self:isFurniture(faction, itemString) and not self:isContainer(faction, itemString) then
			if self:isTerminal(faction, itemString) then
				itemString = "frn_data_terminal"
			end
			messageString:setTT("@frn_n:"  .. itemString)
		elseif self:isInstallation(faction, itemString) then
			messageString:setTT("@deed:" .. itemString)
		elseif self:isHireling(faction, itemString) then
			messageString:setTT("@mob/creature_names:" .. itemString)
		end
		CreatureObject(pCreature):sendSystemMessage(messageString:_getObject())
	elseif (awardResult == self.errorCodes.INVENTORYFULL) then
		CreatureObject(pCreature):sendSystemMessage("@dispenser:inventory_full") -- Your inventory is full. You must make some room before you can purchase.
	elseif (awardResult == self.errorCodes.DATAPADFULL) then
		CreatureObject(pCreature):sendSystemMessage("@faction_recruiter:datapad_full") -- Your datapad is full. You must first free some space.
	elseif (awardResult == self.errorCodes.TOOMANYHIRELINGS) then
		CreatureObject(pCreature):sendSystemMessage("@faction_recruiter:too_many_hirelings") -- You already have too much under your command.
	elseif ( awardResult == self.errorCodes.ITEMCOST ) then
		CreatureObject(pCreature):sendSystemMessage("Error determining cost of item. Please post a bug report regarding the item you attempted to purchase.")
	elseif ( awardResult == self.errorCodes.INVENTORYERROR or awardResult == self.DATAPADERROR) then
		CreatureObject(pCreature):sendSystemMessage("Error finding location to put item. Please post a report.")
	elseif (awardResult == self.errorCodes.TEMPLATEPATHERROR) then
		CreatureObject(pCreature):sendSystemMessage("Error determining data for item. Please post a bug report regarding the item you attempted to purchase..")
	end
end

function recruiterScreenplay:awardItem(pPlayer, faction, itemString)
	return ObjectManager.withCreatureAndPlayerObject(pPlayer, function(player, playerObject)
		local pInventory = SceneObject(pPlayer):getSlottedObject("inventory")

		local factionStanding = playerObject:getFactionStanding(faction)

		local itemCost = self:getItemCost(faction, itemString)

		if itemCost == nil then
			return self.errorCodes.ITEMCOST
		elseif ( pInventory == nil  ) then
			return self.errorCodes.INVENTORYERROR
		end
		
		itemCost  = math.ceil(itemCost *  getGCWDiscount(pPlayer) * self:getSmugglerDiscount(pPlayer))

		if (factionStanding  < (itemCost + 200)) then
			return self.errorCodes.NOTENOUGHFACTION
		end

		local slotsremaining = SceneObject(pInventory):getContainerVolumeLimit() - SceneObject(pInventory):getContainerObjectsSize()

		local bonusItemCount = self:getBonusItemCount(faction, itemString)

		if (slotsremaining < (1 + bonusItemCount)) then
			return self.errorCodes.INVENTORYFULL
		end

		local transferResult =  self:transferItem(player, pInventory, faction, itemString)

		if(transferResult ~= self.errorCodes.SUCCESS) then
			return transferResult
		end

		playerObject:decreaseFactionStanding(faction, itemCost)

		if bonusItemCount then
			local bonusItems = self:getBonusItems(faction, itemString)
			if bonusItems ~= nil then
				for k, v in pairs(bonusItems) do
					transferResult = self:transferItem(pPlayer, pInventory, faction, v)
					if(transferResult ~= self.errorCodes.SUCCESS) then
						return transferResult
					end
				end
			end
		end

		return self.errorCodes.SUCCESS
	end)
end

function recruiterScreenplay:awardData(pPlayer, faction, itemString)
	return ObjectManager.withCreatureAndPlayerObject(pPlayer, function(player, playerObject)
		local pDatapad = SceneObject(pPlayer):getSlottedObject("datapad")

		local factionStanding = playerObject:getFactionStanding(faction)
		local itemCost = self:getItemCost(faction, itemString)

		if pDatapad == nil then
			return self.errorCodes.DATAPADERROR
		elseif itemCost == nil then
			return self.errorCodes.ITEMCOST
		elseif factionStanding  < (itemCost + 200) then
			return self.errorCodes.NOTENOUGHFACTION
		end

		itemCost  = math.ceil(itemCost *  getGCWDiscount(pPlayer) * self:getSmugglerDiscount(pPlayer))
		local slotsRemaining = SceneObject(pDatapad):getContainerVolumeLimit() - SceneObject(pDatapad):getContainerObjectsSize()
		local bonusItemCount = self:getBonusItemCount(faction, itemString)

		if (slotsRemaining < (1 + bonusItemCount)) then
			return self.errorCodes.DATAPADFULL
		end

		local transferResult = self:transferData(pPlayer, pDatapad, faction, itemString)

		if(transferResult ~= self.errorCodes.SUCCESS) then
			return transferResult
		end

		playerObject:decreaseFactionStanding(faction, itemCost)

		if bonusItemCount then
			local bonusItems = self:getBonusItems(faction, itemString)
			if bonusItems ~= nil then
				for k, v in pairs(bonusItems) do
					transferResult = self:transferData(pPlayer, pDatapad, faction, v)
					if (transferResult ~= self.errorCodes.SUCCESS) then
						return transferResult
					end
				end
			end
		end
		return self.errorCodes.SUCCESS
	end)
end

function recruiterScreenplay:transferData(player, pDatapad, faction, itemString)
	local pItem
	local templatePath = self:getTemplatePath(faction, itemString)

	if templatePath == nil then
		return self.errorCodes.TEMPLATEPATHERROR
	end

	local genPath = self:getControlledObjectTemplate(faction, itemString)

	if genPath == nil then
		return self.errorCodes.TEMPLATEPATHERROR
	end

	if (self:isHireling(faction, itemString)) then
		if (checkTooManyHirelings(pDatapad)) then
			return self.errorCodes.TOOMANYHIRELINGS
		end

		pItem = giveControlDevice(pDatapad, templatePath, genPath, -1, true)
	else
		pItem = giveControlDevice(pDatapad, templatePath, genPath, -1, false)
	end

	if pItem ~= nil then
		if SceneObject(pItem) == nil then
			return self.errorCodes.GIVEERROR
		end
		SceneObject(pItem):sendTo(player)
	else
		return self.errorCodes.GIVEERROR
	end

	return self.errorCodes.SUCCESS
end

function recruiterScreenplay:transferItem(player, pInventory, faction, itemString)
	local pItem
	local templatePath = self:getTemplatePath(faction, itemString)

	if templatePath == nil then
		return self.errorCodes.TEMPLATEPATHERROR
	end

	pItem = giveItem(pInventory, templatePath, -1)

	if (pItem == nil) then
		return self.errorCodes.GIVEERROR
	end

	if SceneObject(pItem) == nil then
		return self.GIVEERROR
	end

	if (self:isInstallation(faction, itemString)) then
		SceneObject(pItem):setObjectName("deed", itemString)
		local deed = LuaDeed(pItem)
		local genPath = self:getGeneratedObjectTemplate(faction, itemString)

		if genPath == nil then
			return self.errorCodes.TEMPLATEPATHERROR
		end

		deed:setGeneratedObjectTemplate(genPath)

		if TangibleObject(pItem) ~= nil then
			TangibleObject(pItem):setFaction(faction)
		end
	end

	return self.errorCodes.SUCCESS
end


function recruiterScreenplay:getItemListTable(faction, screenID)
	local dataTable = self:getFactionDataTable(faction)
	if screenID == "fp_furniture" then
		return dataTable.furnitureList
	elseif screenID == "fp_weapons_armor" then
		return dataTable.weaponsArmorList
	elseif screenID == "fp_installations" then
		return dataTable.installationsList
	elseif screenID == "fp_uniforms" then
		return dataTable.uniformsList
	elseif screenID == "fp_hirelings" then
		return dataTable.hirelingList
	end
end

function recruiterScreenplay:getSmugglerDiscount(pPlayer)
	if CreatureObject(pPlayer):hasSkill("combat_smuggler_master") then
		return .75
	elseif CreatureObject(pPlayer):hasSkill("combat_smuggler_underworld_01") then
		return .90
	end
	return 1.0
end

function recruiterScreenplay:handleGoOnLeave(pPlayer)
	ObjectManager.withCreaturePlayerObject(pPlayer, function(playerObject)
		playerObject:setFactionStatus(0)
	end)
end

function recruiterScreenplay:handleGoCovert(pPlayer)
	ObjectManager.withCreaturePlayerObject(pPlayer, function(playerObject)
		playerObject:setFactionStatus(1)
	end)
end

function recruiterScreenplay:handleGoOvert(pPlayer)
	ObjectManager.withCreaturePlayerObject(pPlayer, function(playerObject)
		playerObject:setFactionStatus(2)
	end)
end

function recruiterScreenplay:handleResign(pPlayer)
	ObjectManager.withCreatureAndPlayerObject(pPlayer, function(player, playerObject)
		player:setFactionRank(0)
		player:setFaction(0)
		playerObject:setFactionStatus(0)
	end)
end