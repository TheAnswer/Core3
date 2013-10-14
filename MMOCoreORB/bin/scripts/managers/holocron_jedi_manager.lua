package.path = package.path .. ";scripts/managers/?.lua"
JediManager = require("jedi_manager")

jediManagerName = "HolocronJediManager"

NUMBEROFPROFESSIONSTOMASTER = 6
MAXIMUMNUMBEROFPROFESSIONSTOSHOWWITHHOLOCRON = NUMBEROFPROFESSIONSTOMASTER - 2

HolocronJediManager = JediManager:new {
	screenplayName = jediManagerName,
	jediManagerName = jediManagerName,
	jediProgressionType = HOLOCRONJEDIPROGRESSION,
	startingEvent = nil,
}

-- Return a list of all professions and their badge number that are available for the hologrind
-- @return a list of professions and their badge numbers.
function HolocronJediManager:getGrindableProfessionList()
	local grindableProfessions = {
		-- String Id, badge number, profession name
		--{ "pilot_rebel_navy_corellia", 	PILOT_REBEL_NAVY_CORELLIA },
		--{ "pilot_imperial_navy_corellia", 	PILOT_IMPERIAL_NAVY_CORELLIA },
		--{ "pilot_neutral_corellia", 		PILOT_CORELLIA },
		--{ "pilot_rebel_navy_tatooine", 	PILOT_REBEL_NAVY_TATOOINE },
		--{ "pilot_imperial_navy_naboo", 	PILOT_IMPERIAL_NAVY_NABOO },
		{ "crafting_architect_master", 		CRAFTING_ARCHITECT_MASTER  },
		{ "crafting_armorsmith_master", 	CRAFTING_ARMORSMITH_MASTER  },
		{ "crafting_artisan_master", 		CRAFTING_ARTISAN_MASTER  },
		{ "outdoors_bio_engineer_master", 	OUTDOORS_BIOENGINEER_MASTER  },
		{ "combat_bountyhunter_master", 	COMBAT_BOUNTYHUNTER_MASTER  },
		{ "combat_brawler_master", 		COMBAT_BRAWLER_MASTER  },
		{ "combat_carbine_master", 		COMBAT_CARBINE_MASTER  },
		{ "crafting_chef_master", 		CRAFTING_CHEF_MASTER  },
		{ "science_combatmedic_master", 	SCIENCE_COMBATMEDIC_MASTER  },
		{ "combat_commando_master", 		COMBAT_COMMANDO_MASTER  },
		--{ "outdoors_creaturehandler_master", 	OUTDOORS_CREATUREHANDLER_MASTER  },
		{ "social_dancer_master", 		SOCIAL_DANCER_MASTER  },
		{ "science_doctor_master", 		SCIENCE_DOCTOR_MASTER  },
		{ "crafting_droidengineer_master", 	CRAFTING_DROIDENGINEER_MASTER  },
		{ "social_entertainer_master", 		SOCIAL_ENTERTAINER_MASTER  },
		{ "combat_1hsword_master", 		COMBAT_1HSWORD_MASTER  },
		{ "social_imagedesigner_master", 	SOCIAL_IMAGEDESIGNER_MASTER  },
		{ "combat_marksman_master", 		COMBAT_MARKSMAN_MASTER  },
		{ "science_medic_master", 		SCIENCE_MEDIC_MASTER  },
		{ "crafting_merchant_master", 		CRAFTING_MERCHANT_MASTER  },
		{ "social_musician_master", 		SOCIAL_MUSICIAN_MASTER  },
		{ "combat_polearm_master", 		COMBAT_POLEARM_MASTER  },
		{ "combat_pistol_master", 		COMBAT_PISTOL_MASTER  },
		--{ "social_politician_master", 	SOCIAL_POLITICIAN_MASTER  },
		{ "outdoors_ranger_master", 		OUTDOORS_RANGER_MASTER  },
		{ "combat_rifleman_master", 		COMBAT_RIFLEMAN_MASTER  },
		{ "outdoors_scout_master", 		OUTDOORS_SCOUT_MASTER  },
		--{ "crafting_shipwright", 		CRAFTING_SHIPWRIGHT },
		{ "combat_smuggler_master", 		COMBAT_SMUGGLER_MASTER  },
		{ "outdoors_squadleader_master", 	OUTDOORS_SQUADLEADER_MASTER  },
		{ "combat_2hsword_master", 		COMBAT_2HSWORD_MASTER  },
		{ "crafting_tailor_master", 		CRAFTING_TAILOR_MASTER  },
		{ "crafting_weaponsmith_master", 	CRAFTING_WEAPONSMITH_MASTER  },
		--{ "pilot_neutral_naboo", 		PILOT_NEUTRAL_NABOO },
		--{ "pilot_neutral_tatooine", 		PILOT_TATOOINE },
		--{ "pilot_imperial_navy_tatooine", 	PILOT_IMPERIAL_NAVY_TATOOINE },
		{ "combat_unarmed_master", 		COMBAT_UNARMED_MASTER  },
		--{ "pilot_rebel_navy_naboo", 		PILOT_REBEL_NAVY_NABOO }
	}
	return grindableProfessions
end

-- Handling of the onPlayerCreated event.
-- Hologrind professions will be generated for the player.
-- @param pCreatureObject pointer to the creature object of the created player.
function HolocronJediManager:onPlayerCreated(pCreatureObject)
	local skillList = HolocronJediManager.getGrindableProfessionList()
	HolocronJediManager.withCreaturePlayerObject(pCreatureObject, function(playerObject)
		for i = 1, NUMBEROFPROFESSIONSTOMASTER, 1 do
			local numberOfSkillsInList = table.getn(skillList)
			local skillNumber = math.random(1, numberOfSkillsInList)
			playerObject:addHologrindProfession(skillList[skillNumber][2])
			table.remove(skillList, skillNumber)
		end
	end)
end

-- Check and count the number of mastered hologrind professions.
-- @param pCreatureObject pointer to the creature object of the player which should get its number of mastered professions counted.
-- @return the number of mastered hologrind professions.
function HolocronJediManager.getNumberOfMasteredProfessions(pCreatureObject)
	return HolocronJediManager.withCreaturePlayerObject(pCreatureObject, function(playerObject)
		local professions = playerObject:getHologrindProfessions()
		local masteredNumberOfProfessions = 0
		for i = 1, table.getn(professions), 1 do
			if playerObject:hasBadge(professions[i]) then
				masteredNumberOfProfessions = masteredNumberOfProfessions + 1
			end
		end
		return masteredNumberOfProfessions
	end)
end

-- Check if the player is jedi.
-- @param pCreatureObject pointer to the creature object of the player to check if he is jedi.
-- @return returns if the player is jedi or not.
function HolocronJediManager.isJedi(pCreatureObject)
	return HolocronJediManager.withCreaturePlayerObject(pCreatureObject, function(playerObject)
		return playerObject:isJedi()
	end)
end

-- Sui window ok pressed callback function.
function HolocronJediManager:notifyOkPressed()
	-- Do nothing.
end

-- Send a sui window to the player about unlocking jedi and award jedi status and force sensitive skill.
-- @param pCreatureObject pointer to the creature object of the player who unlocked jedi.
function HolocronJediManager.sendSuiWindow(pCreatureObject)
	local suiManager = LuaSuiManager()
	suiManager:sendMessageBox(pCreatureObject, pCreatureObject, "@quest/force_sensitive/intro:force_sensitive", "Perhaps you should meditate somewhere alone...", "@ok", "HolocronJediManager", "notifyOkPressed")
end

-- Award skill and jedi status to the player.
-- @param pCreatureObject pointer to the creature object of the player who unlocked jedi.
function HolocronJediManager.awardJediStatusAndSkill(pCreatureObject)
	HolocronJediManager.withCreaturePlayerObject(pCreatureObject, function(playerObject)
		awardSkill(pCreatureObject, "force_title_jedi_novice")
		playerObject:setJediState(1)
	end)
end

-- Check if the player has mastered all hologrind professions and send sui window and award skills.
-- @param pCreatureObject pointer to the creature object of the player to check the jedi progression on.
function HolocronJediManager.checkIfProgressedToJedi(pCreatureObject)
	if HolocronJediManager.getNumberOfMasteredProfessions(pCreatureObject) >= NUMBEROFPROFESSIONSTOMASTER and 
	   not HolocronJediManager.isJedi(pCreatureObject) then
		HolocronJediManager.sendSuiWindow(pCreatureObject)
		HolocronJediManager.awardJediStatusAndSkill(pCreatureObject)
	end
end

function HolocronJediManager:badgeAwardedEventHandler(pCreatureObject, pCreatureObject2, badgeNumber)
	HolocronJediManager.checkIfProgressedToJedi(pCreatureObject)

	return 0
end

-- Register observer on the player for observing badge awards.
-- @param pCreatureObject pointer to the creature object of the player to register observers on.
function HolocronJediManager.registerObservers(pCreatureObject)
	createObserver(BADGEAWARDED, "HolocronJediManager", "badgeAwardedEventHandler", pCreatureObject)
end

-- Handling of the onPlayerLoggedIn event. The progression of the player will be checked and observers will be registered.
-- @param pCreatureObject pointer to the creature object of the player who logged in.
function HolocronJediManager:onPlayerLoggedIn(pCreatureObject)
	HolocronJediManager.checkIfProgressedToJedi(pCreatureObject)
	HolocronJediManager.registerObservers(pCreatureObject)
end

-- Get the profession name from the badge number.
-- @param badgeNumber the badge number to find the profession name for.
-- @return the profession name associated with the badge number, Unknown profession returned if the badge number isn't found.
function HolocronJediManager.getProfessionStringIdFromBadgeNumber(badgeNumber)
	local skillList = HolocronJediManager.getGrindableProfessionList()
	for i = 1, table.getn(skillList), 1 do
		if skillList[i][2] == badgeNumber then
			return skillList[i][1]
		end
	end
	return "Unknown profession"
end

-- Find out and send the response from the holocron to the player
-- @param pCreatureObject pointer to the creature object of the player who used the holocron.
function HolocronJediManager.sendHolocronMessage(pCreatureObject)
	if HolocronJediManager.getNumberOfMasteredProfessions(pCreatureObject) >= MAXIMUMNUMBEROFPROFESSIONSTOSHOWWITHHOLOCRON then
		HolocronJediManager.withCreatureObject(pCreatureObject, function(creatureObject)
			-- The Holocron is quiet. The ancients' knowledge of the Force will no longer assist you
  			-- on your journey. You must continue seeking on your own.
			creatureObject:sendSystemMessage("@jedi_spam:holocron_quiet")
		end)
	else
		HolocronJediManager.withCreatureAndPlayerObject(pCreatureObject, function(creatureObject, playerObject)
			local professions = playerObject:getHologrindProfessions()
			for i = 1, table.getn(professions), 1 do			
				if not playerObject:hasBadge(professions[i]) then
					local professionText = HolocronJediManager.getProfessionStringIdFromBadgeNumber(professions[i])
					creatureObject:sendSystemMessageWithTO("@jedi_spam:holocron_light_information", "@skl_n:" .. professionText)
					return
				end
			end
		end)
	end
end

-- Handling of the useHolocron event.
-- @param pSceneObject pointer to the holocron object.
-- @param pCreatureObject pointer to the creature object that used the holocron.
function HolocronJediManager:useHolocron(pSceneObject, pCreatureObject)
	HolocronJediManager.sendHolocronMessage(pCreatureObject)
	sceneObject = LuaSceneObject(pSceneObject)
	sceneObject:destroyObjectFromWorld()
end

registerScreenPlay("HolocronJediManager", true)

return HolocronJediManager
