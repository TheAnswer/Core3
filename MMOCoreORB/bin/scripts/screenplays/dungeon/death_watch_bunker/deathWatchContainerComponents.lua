local ObjectManager = require("managers.object.object_manager")

deathWatchJetpackCraftingDroid = { }

function deathWatchJetpackCraftingDroid:canAddObject(pDroid, pIngredient, slot)
	if pIngredient == nil then
		return TRANSFERCANTADD
	end

	if slot ~= -1 then
		return TRANSFERCANTADD
	end

	local pParent = DeathWatchBunkerScreenPlay:getObjOwner(pIngredient)

	if (pParent == nil or not SceneObject(pParent):isPlayerCreature()) then
		return TRANSFERCANTADD
	end

	local number = readData(SceneObject(pDroid):getObjectID() .. ":dwb:craftingterminal")
	local statusPrefix = "dwb:craftingTerminal" .. number .. ":"

	return ObjectManager.withCreatureObject(pParent, function(creature)
		local template = SceneObject(pIngredient):getTemplateObjectPath()
		local userid = readData(statusPrefix .. "user")

		if userid ~= 0 and userid ~= creature:getObjectID() then
			creature:sendSystemMessage("@dungeon/death_watch:same_user_only")
			return TRANSFERCANTADD
		end

		if creature:hasSkill(DeathWatchBunkerScreenPlay.terminalSkills[8]) == false then
			creature:sendSystemMessage(DeathWatchBunkerScreenPlay.terminalSkillMessage[4])
			return TRANSFERCANTADD
		end

		local droidId = SceneObject(pDroid):getObjectID()

		local hasMineral = readData(statusPrefix .. "alummineral")
		local hasJetpackBase = readData(statusPrefix .. "jetpackbase")
		local hasJetpackStabilizer = readData(statusPrefix .. "jetpackstabilizer")
		local hasDuctedFan = readData(statusPrefix .. "ductedfan")
		local hasInjectorTank = readData(statusPrefix .. "injectortank")
		local hasDispersionUnit = readData(statusPrefix .. "dispersionunit")

		if template == DeathWatchBunkerScreenPlay.bunkerItems.jetPackBase then
			if hasJetpackBase == 0 then
				return TRANSFERCANADD
			else
				creature:sendSystemMessage("@dungeon/death_watch:already_has_component")
			end
		elseif template == DeathWatchBunkerScreenPlay.bunkerItems.jetPackStabilizer then
			if hasJetpackStabilizer == 0 then
				return TRANSFERCANADD
			else
				creature:sendSystemMessage("@dungeon/death_watch:already_has_component")
			end
		elseif template == DeathWatchBunkerScreenPlay.bunkerItems.ductedFan then
			if hasDuctedFan == 0 then
				return TRANSFERCANADD
			else
				creature:sendSystemMessage("@dungeon/death_watch:already_has_component")
			end
		elseif template == DeathWatchBunkerScreenPlay.bunkerItems.injectorTank then
			if hasInjectorTank == 0 then
				return TRANSFERCANADD
			else
				creature:sendSystemMessage("@dungeon/death_watch:already_has_component")
			end
		elseif template == DeathWatchBunkerScreenPlay.bunkerItems.dispersionUnit then
			if hasDispersionUnit == 0 then
				return TRANSFERCANADD
			else
				creature:sendSystemMessage("@dungeon/death_watch:already_has_component")
			end
		elseif template == DeathWatchBunkerScreenPlay.bunkerItems.alumMineral then
			if hasMineral == 0 then
				return TRANSFERCANADD
			else
				creature:sendSystemMessage("@dungeon/death_watch:already_has_component")
			end
		end
		return TRANSFERCANTADD
	end)
end

function deathWatchJetpackCraftingDroid:transferObject(pDroid, pIngredient, slot)
	if (pIngredient == nil) then
		return TRANSFERFAIL
	end

	if slot ~= -1 then
		return TRANSFERFAIL
	end

	local pParent = DeathWatchBunkerScreenPlay:getObjOwner(pIngredient)

	local template = SceneObject(pIngredient):getTemplateObjectPath()

	local number = readData(SceneObject(pDroid):getObjectID() .. ":dwb:craftingterminal")
	local statusPrefix = "dwb:craftingTerminal" .. number .. ":"

	if template == DeathWatchBunkerScreenPlay.bunkerItems.jetPackBase then
		writeData(statusPrefix .. "jetpackbase", 1)
	elseif template == DeathWatchBunkerScreenPlay.bunkerItems.jetPackStabilizer then
		writeData(statusPrefix .. "jetpackstabilizer", 1)
	elseif template == DeathWatchBunkerScreenPlay.bunkerItems.ductedFan then
		writeData(statusPrefix .. "ductedfan", 1)
	elseif template == DeathWatchBunkerScreenPlay.bunkerItems.injectorTank then
		writeData(statusPrefix .. "injectortank", 1)
	elseif template == DeathWatchBunkerScreenPlay.bunkerItems.dispersionUnit then
		writeData(statusPrefix .. "dispersionunit", 1)
	elseif template == DeathWatchBunkerScreenPlay.bunkerItems.alumMineral then
		writeData(statusPrefix .. "alummineral", 1)
	end

	createEvent(100, "DeathWatchBunkerScreenPlay", "destroyIngredient", pIngredient)

	local hasMineral = readData(statusPrefix .. "alummineral")
	local hasJetpackBase = readData(statusPrefix .. "jetpackbase")
	local hasJetpackStabilizer = readData(statusPrefix .. "jetpackstabilizer")
	local hasDuctedFan = readData(statusPrefix .. "ductedfan")
	local hasInjectorTank = readData(statusPrefix .. "injectortank")
	local hasDispersionUnit = readData(statusPrefix .. "dispersionunit")

	local userid = readData(statusPrefix .. "user")
	if userid == 0 then
		writeData(statusPrefix .. "user", CreatureObject(pParent):getObjectID())
		writeData(CreatureObject(pParent):getObjectID() .. ":dwb:terminal", number)
	end

	if (hasMineral == 1 and hasJetpackBase == 1 and hasJetpackStabilizer == 1 and hasDuctedFan == 1 and hasInjectorTank == 1 and hasDispersionUnit == 1) then
		CreatureObject(pParent):sendSystemMessage("@dungeon/death_watch:starting_to_craft")
		writeData(statusPrefix .. "currentlycrafting", 1)
		writeData(statusPrefix .. "targetitemindex", 1)

		local spawn = deathWatchSpecialSpawns["jetpackattack1"]
		spawnMobile("endor", spawn[1], spawn[2], spawn[3], spawn[4], spawn[5], spawn[6], spawn[7])
		local spawn = deathWatchSpecialSpawns["jetpackattack2"]
		spawnMobile("endor", spawn[1], spawn[2], spawn[3], spawn[4], spawn[5], spawn[6], spawn[7])
		local spawn = deathWatchSpecialSpawns["jetpackattack3"]
		spawnMobile("endor", spawn[1], spawn[2], spawn[3], spawn[4], spawn[5], spawn[6], spawn[7])
		local spawn = deathWatchSpecialSpawns["jetpackattack4"]
		spawnMobile("endor", spawn[1], spawn[2], spawn[3], spawn[4], spawn[5], spawn[6], spawn[7])

		local spawn = deathWatchSpecialSpawns["droidengineerattack1"]
		spawnMobile("endor", spawn[1], spawn[2], spawn[3], spawn[4], spawn[5], spawn[6], spawn[7])
		local spawn = deathWatchSpecialSpawns["droidengineerattack2"]
		spawnMobile("endor", spawn[1], spawn[2], spawn[3], spawn[4], spawn[5], spawn[6], spawn[7])
		local spawn = deathWatchSpecialSpawns["droidengineerattack3"]
		spawnMobile("endor", spawn[1], spawn[2], spawn[3], spawn[4], spawn[5], spawn[6], spawn[7])
		local spawn = deathWatchSpecialSpawns["droidengineerattack4"]
		spawnMobile("endor", spawn[1], spawn[2], spawn[3], spawn[4], spawn[5], spawn[6], spawn[7])

		DeathWatchBunkerScreenPlay:startCraftingProcess(pParent, pDroid)
	else
		CreatureObject(pParent):sendSystemMessage("@dungeon/death_watch:items_still_needed")
		if hasJetpackBase == 0 then
			CreatureObject(pParent):sendSystemMessage("@dungeon/death_watch:jetpack_base")
		end

		if hasJetpackStabilizer == 0 then
			CreatureObject(pParent):sendSystemMessage("@dungeon/death_watch:stabilizer")
		end

		if hasDuctedFan == 0 then
			CreatureObject(pParent):sendSystemMessage("@dungeon/death_watch:ducted_fan")
		end

		if hasInjectorTank == 0 then
			CreatureObject(pParent):sendSystemMessage("@dungeon/death_watch:fuel_injector")
		end

		if hasDispersionUnit == 0 then
			CreatureObject(pParent):sendSystemMessage("@dungeon/death_watch:dispersion")
		end

		if hasMineral == 0 then
			CreatureObject(pParent):sendSystemMessage("@dungeon/death_watch:alum_mineral")
		end
	end

	return TRANSFERSUCCESS
end

function deathWatchJetpackCraftingDroid:removeObject(droid, ingredient, slot)
	return TRANSFERFAIL
end

deathWatchCraftingDroid = { }

function deathWatchCraftingDroid:canAddObject(pDroid, pIngredient, slot)
	if pIngredient == nil then
		return TRANSFERCANTADD
	end

	if slot ~= -1 then
		return TRANSFERCANTADD
	end

	local pParent = DeathWatchBunkerScreenPlay:getObjOwner(pIngredient)

	if (pParent == nil or not SceneObject(pParent):isPlayerCreature()) then
		return TRANSFERCANTADD
	end

	local terminalId = SceneObject(pDroid):getObjectID()

	local number = readData(terminalId .. ":dwb:craftingterminal")
	local statusPrefix = "dwb:craftingTerminal" .. number .. ":"

	if (number == 0) then
		CreatureObject(pParent):sendSystemMessage("Error determining crafting terminal ID. Please file a bug report.")
	end

	local template = SceneObject(pIngredient):getTemplateObjectPath()

	local userid = readData(statusPrefix .. "user")

	if userid ~= 0 and userid ~= CreatureObject(pParent):getObjectID() then
		CreatureObject(pParent):sendSystemMessage("@dungeon/death_watch:same_user_only")
		return TRANSFERCANTADD
	end

	if CreatureObject(pParent):hasSkill(DeathWatchBunkerScreenPlay.terminalSkills[number + 4]) == false then
		CreatureObject(pParent):sendSystemMessage(DeathWatchBunkerScreenPlay.terminalSkillMessage[number])
		return TRANSFERCANTADD
	end

	local hasMineral = readData(statusPrefix .. "alummineral")
	local hasBL = readData(statusPrefix .. "binary")
	local hasPLC = readData(statusPrefix .. "protective")
	local hasArmorPart = readData(statusPrefix .. "bharmorpart")

	if template == DeathWatchBunkerScreenPlay.bunkerItems.binaryLiquid then
		if hasBL == 0 then
			return TRANSFERCANADD
		else
			CreatureObject(pParent):sendSystemMessage("@dungeon/death_watch:already_has_component")
		end
	elseif template == DeathWatchBunkerScreenPlay.bunkerItems.protectiveLiquid then
		if hasPLC == 0 then
			return TRANSFERCANADD
		else
			CreatureObject(pParent):sendSystemMessage("@dungeon/death_watch:already_has_component")
		end
	elseif template == DeathWatchBunkerScreenPlay.bunkerItems.alumMineral then
		if hasMineral == 0 then
			return TRANSFERCANADD
		else
			CreatureObject(pParent):sendSystemMessage("@dungeon/death_watch:already_has_component")
		end
	else
		local table = DeathWatchBunkerScreenPlay.requiredDoorItems[number + 4]
		for i,v in ipairs(table) do
			if template == v then
				if hasArmorPart == 0 then
					return TRANSFERCANADD
				else
					CreatureObject(pParent):sendSystemMessage("@dungeon/death_watch:making_something_else")
				end
			end
		end
	end
	return TRANSFERCANTADD
end

function deathWatchCraftingDroid:transferObject(pDroid, pIngredient, slot)
	if (pIngredient == nil) then
		return TRANSFERFAIL
	end

	if slot ~= -1 then
		return TRANSFERFAIL
	end

	local pParent = DeathWatchBunkerScreenPlay:getObjOwner(pIngredient)
	local template = SceneObject(pIngredient):getTemplateObjectPath()

	local terminalId = SceneObject(pDroid):getObjectID()

	local number = readData(terminalId .. ":dwb:craftingterminal")
	local statusPrefix = "dwb:craftingTerminal" .. number .. ":"

	if template == DeathWatchBunkerScreenPlay.bunkerItems.binaryLiquid then
		writeData(statusPrefix .. "binary", 1)
	elseif template == DeathWatchBunkerScreenPlay.bunkerItems.protectiveLiquid then
		writeData(statusPrefix .. "protective", 1)
	elseif template == DeathWatchBunkerScreenPlay.bunkerItems.alumMineral then
		writeData(statusPrefix .. "alummineral", 1)
	else
		local table = DeathWatchBunkerScreenPlay.requiredDoorItems[number + 4]
		for i,v in ipairs(table) do
			if template == v then
				writeData(statusPrefix .. "bharmorpart", 1)
				writeData(statusPrefix .. "targetitemindex", i)
			end
		end
		if (readData(statusPrefix .. "bharmorpart") ~= 1) then
			return TRANSFERFAIL
		end
	end

	createEvent(100, "DeathWatchBunkerScreenPlay", "destroyIngredient", pIngredient)

	local hasMineral = readData(statusPrefix .. "alummineral")
	local hasBL = readData(statusPrefix .. "binary")
	local hasPLC = readData(statusPrefix .. "protective")
	local hasArmorPart = readData(statusPrefix .. "bharmorpart")

	local userid = readData(statusPrefix .. "user")
	if userid == 0 then
		local playerID = CreatureObject(pParent):getObjectID()
		writeData(statusPrefix .. "user", playerID)
		writeData(playerID .. ":dwb:currentlycrafting", 1)
		writeData(CreatureObject(pParent):getObjectID() .. ":dwb:terminal", number)
	end

	if (hasMineral == 1 and hasBL == 1 and hasPLC == 1 and hasArmorPart == 1) then
		CreatureObject(pParent):sendSystemMessage("@dungeon/death_watch:starting_to_craft")
		writeData(statusPrefix .. "currentlycrafting", 1)
		if number == 1 then
			local spawn = deathWatchSpecialSpawns["armorattack1"]
			spawnMobile("endor", spawn[1], spawn[2], spawn[3], spawn[4], spawn[5], spawn[6], spawn[7])
			local spawn = deathWatchSpecialSpawns["armorattack2"]
			spawnMobile("endor", spawn[1], spawn[2], spawn[3], spawn[4], spawn[5], spawn[6], spawn[7])
			local spawn = deathWatchSpecialSpawns["armorattack3"]
			spawnMobile("endor", spawn[1], spawn[2], spawn[3], spawn[4], spawn[5], spawn[6], spawn[7])
			local spawn = deathWatchSpecialSpawns["armorattack4"]
			spawnMobile("endor", spawn[1], spawn[2], spawn[3], spawn[4], spawn[5], spawn[6], spawn[7])
		elseif number == 2 then
			local spawn = deathWatchSpecialSpawns["droidengineerattack1"]
			spawnMobile("endor", spawn[1], spawn[2], spawn[3], spawn[4], spawn[5], spawn[6], spawn[7])
			local spawn = deathWatchSpecialSpawns["droidengineerattack2"]
			spawnMobile("endor", spawn[1], spawn[2], spawn[3], spawn[4], spawn[5], spawn[6], spawn[7])
			local spawn = deathWatchSpecialSpawns["droidengineerattack3"]
			spawnMobile("endor", spawn[1], spawn[2], spawn[3], spawn[4], spawn[5], spawn[6], spawn[7])
			local spawn = deathWatchSpecialSpawns["droidengineerattack4"]
			spawnMobile("endor", spawn[1], spawn[2], spawn[3], spawn[4], spawn[5], spawn[6], spawn[7])
		elseif number == 3 then
			local spawn = deathWatchSpecialSpawns["tailorattack1"]
			spawnMobile("endor", spawn[1], spawn[2], spawn[3], spawn[4], spawn[5], spawn[6], spawn[7])
			local spawn = deathWatchSpecialSpawns["tailorattack2"]
			spawnMobile("endor", spawn[1], spawn[2], spawn[3], spawn[4], spawn[5], spawn[6], spawn[7])
			local spawn = deathWatchSpecialSpawns["tailorattack3"]
			spawnMobile("endor", spawn[1], spawn[2], spawn[3], spawn[4], spawn[5], spawn[6], spawn[7])
			local spawn = deathWatchSpecialSpawns["tailorattack4"]
			spawnMobile("endor", spawn[1], spawn[2], spawn[3], spawn[4], spawn[5], spawn[6], spawn[7])
		end

		DeathWatchBunkerScreenPlay:startCraftingProcess(pParent, pDroid)
	else
		ObjectManager.withCreatureObject(pParent, function(creature)
			creature:sendSystemMessage("@dungeon/death_watch:items_still_needed")
			if hasBL == 0 then
				creature:sendSystemMessage("@dungeon/death_watch:binary_liquid")
			end

			if hasPLC == 0 then
				creature:sendSystemMessage("@dungeon/death_watch:emulsifier")
			end

			if hasArmorPart == 0 then
				creature:sendSystemMessage(DeathWatchBunkerScreenPlay.partStrings[number])
			end

			if hasMineral == 0 then
				creature:sendSystemMessage("@dungeon/death_watch:alum_mineral")
			end
		end)
	end

	return TRANSFERSUCCESS
end

function deathWatchCraftingDroid:removeObject(droid, ingredient, slot)
	return TRANSFERFAIL
end