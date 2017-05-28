local ObjectManager = require("managers.object.object_manager")

heraldScreenPlay = ScreenPlay:new {

		heraldList = {
			{ planet = "corellia", template = "herald_commoner", customName = "Excited Journalist", x = -5192.9, z = 21, y = -2545.55, angle = 46.5164, cell = 0, destX = 4651, destY = -5616, stringFile = "herald_corellia_imperial_strongold" },
			{ planet = "corellia", template = "herald_selonian_diplomat", x = 3256.5, z = 300, y = 5418.4, angle = -89, cell = 0, destX = -2482, destY = 2907, stringFile = "herald_corellia_afarathucave" },
			{ planet = "corellia", template = "herald_drall", x = -3774.1, z = 86, y = 3213.8, angle = 5, cell = 0, destX = 1041, destY = 4193, stringFile = "herald_corellia_drall_patriot_hideout" },
			{ planet = "corellia", template = "herald_commoner", x = -3104.7, z = 31, y = 2793.8, angle = -64, cell = 0, destX = 1413, destY = -317, stringFile = "herald_corellia_lord_nyax_cult" },
			{ planet = "corellia", template = "herald_corsec_agent", x = -202.143, z = 28, y = -4504.32, angle = 203.539, cell = 0, destX = 5290, destY = 1493, stringFile = "herald_corellia_rogue_corsec" },

			{ planet = "dantooine", template = "herald_imperial_colonel", x = -588.77, z = 3, y = 2499, angle = 264.6, cell = 0, destX = -152, destY = -444, stringFile = "herald_dantooine_kunga_stronghold" },
			{ planet = "dantooine", template = "herald_commoner", x = -643, z = 3, y = 2449, angle = 341, cell = 0, destX = 7043, destY = -4104, stringFile = "herald_dantooine_janta_stronghold" },
			{ planet = "dantooine", template = "herald_commoner", x = 1544, z = 4, y = -6415, angle = 344, cell = 0, destX = -6222, destY = 7381, stringFile = "herald_dantooine_force_crytsal_hunters_cave" },
			{ planet = "dantooine", template = "daiv_dekven", x = 1603.42, z = 4, y = -6434.54, angle = 102.9, cell = 0, destX = -6223, destY = 7363, stringFile = "herald_dantooine_mokk_stronghold" },

			{ planet = "dathomir", template = "herald_commoner", x = 611.09, z = 6, y = 3084, angle = 359.8, cell = 0, destX = 5689, destY = 1955, stringFile = "herald_dathomir_crashed_civilian_transport" },
			{ planet = "dathomir", template = "herald_escaped_nightsister_slave", x = 595.5, z = 6, y = 3045.4, angle = -5, cell = 0, destX = 2427, destY = -1700, stringFile = "herald_dathomir_nightsister_forcedlabor_camp" },
			{ planet = "dathomir", template = "herald_nightsister_rancor_tamer", x = -4157, z = 134, y = 82.6, angle = 160, cell = 0, destX = -4227, destY = -2089, stringFile = "herald_dathomir_mutant_rancor_cave" },
			{ planet = "dathomir", template = "herald_nightsister_protector", x = -3999.9, z = 118.9, y = -65.3, angle = -130, cell = 0, destX = -2451, destY = 1522, stringFile = "herald_dathomir_singing_vs_nightsister" },
			{ planet = "dathomir", template = "herald_dathomir_olof", x = -19.6, z = 1, y = 15.8, angle = -172, cell = 2665367, destX = -1217, destY = 6261, stringFile = "herald_dathomir_nsister_cave_olof" },
			{ planet = "dathomir", template = "herald_dathomir_azzenaj", x = 533.7, z = 229.5, y = 4238.5, angle = 130, cell = 0, destX = -1217, destY = 6261, stringFile = "herald_dathomir_nsister_cave_azzenaj" },
			{ planet = "dathomir", template = "herald_dathomir_leandra", x = -4082, z = 132.7, y = -210, angle = 330, cell = 0, destX = -1217, destY = 6261, stringFile = "herald_dathomir_nsister_cave_leandra" },

			{ planet = "endor", template = "herald_commoner", x = 3212.71, z = 24.5, y = -3452.25, angle = 41.7, cell = 0, destX = -1709, destY = -29, stringFile = "herald_endor_jinda_spiritualists" },
			{ planet = "endor", template = "herald_commoner", x = 3246.3, z = 24, y = -3492.1, angle = 323, cell = 0, destX = 2274, destY = 3473, stringFile = "herald_endor_korga_cave" },
			{ planet = "endor", template = "herald_commoner", customName = "Eyd Tommsin", x = -988.5, z = 73, y = 1576.8, angle = -50, cell = 0, destX = -6857, destY = 690, stringFile = "herald_endor_marauder_orphans" },

			{ planet = "lok", template = "herald_commoner", x = 429.84, z = 13, y = 5136.9, angle = 341, cell = 0, destX = -67, destY = 2650, stringFile = "herald_lok_kimogila_cult" },
			{ planet = "lok", template = "herald_commoner", x = -1873.66, z = 12, y = 3120.7, angle = 0, cell = 0, destX = -3790, destY = -3903, stringFile = "herald_lok_canyon_corsair_stronghold" },

			{ planet = "naboo", template = "herald_commoner", x = -13.9, z = 1.1, y = -9.9, angle = 71, cell = 1741472, destX = -6479, destY = -3243, stringFile = "herald_naboo_imperial_weapon_research_facility" },
			{ planet = "naboo", template = "herald_commoner", x = 1998.4, z = 12, y = 2538.1, angle = -54, cell = 0, destX = 5860, destY = -4655, stringFile = "herald_naboo_narglatch_cave" },
			{ planet = "naboo", template = "herald_commoner", x = 1573.2, z = 25, y = 2824.3, angle = -84, cell = 0, destX = 5741, destY = -1546, stringFile = "herald_naboo_veermok_cave" },
			{ planet = "naboo", template = "herald_rsf_security_guard", x = -5032.08, z = 6, y = 4091.71, angle = 115, cell = 0, destX = -264, destY = 2823, stringFile = "herald_naboo_gungan_warrior_stronghold" },
			{ planet = "naboo", template = "herald_patron", x = -5538, z = 6, y = 4365.1, angle = 103, cell = 0, destX = 2915, destY = 1151, stringFile = "herald_naboo_mauler_encampment" },
			{ planet = "naboo", template = "herald_naboo_hanna", x = -5484, z = 6, y = 4365, angle = 103, cell = 0, destX = -1507, destY = -1729, stringFile = "herald_naboo_noble_pirate_rescue_hanna" },
			{ planet = "naboo", template = "herald_naboo_ketter", x = 10.7, z = -0.9, y = 1.5, angle = -103, cell = 64, destX = -1507, destY = -1729, stringFile = "herald_naboo_pirate_pirate_rescue_ketter" },
			{ planet = "naboo", template = "herald_naboo_demitri", x = 1673, z = 12, y = 2582, angle = 0, cell = 0, destX = -1507, destY = -1729, stringFile = "herald_naboo_servant_pirate_rescue_demitri" },


			{ planet = "rori", template = "herald_commoner", x = 5352, z = 80, y = 5663, angle = 90, cell = 0, destX = 7353, destY = 110, stringFile = "herald_rori_kobola_spice_mine" },
			{ planet = "rori", template = "herald_commoner", x = 5129.3, z = 80, y = 5556, angle = 55, cell = 0, destX = 878, destY = -4917, stringFile = "herald_rori_borgle_bat_cave" },
			{ planet = "rori", template = "herald_imperial_major", x = -5273.4, z = 80.1, y = -2234, angle = -90, cell = 0, destX = -5652, destY = -5659, stringFile = "herald_rori_imperial_military_base" },
			{ planet = "rori", template = "herald_commoner", x = -5210.7, z = 80, y = -2269.6, angle = -90, cell = 0, destX = 5463, destY = 5044, stringFile = "herald_rori_cobral_hideout" },
			{ planet = "rori", template = "herald_commoner", x = -5117.3, z = 80, y = -2443, angle = 0, cell = 0, destX = -1071, destY = 4540, stringFile = "herald_rori_imperial_hyperdrive_research_facility" },

			{ planet = "talus", template = "herald_commoner", x = 378, z = 6, y = -2752, angle = 341.8, cell = 0, destX = 4197, destY = 1121, stringFile = "herald_rori_creature_village" }, -- stf labeled Rori but is actually a Talus POI
			{ planet = "talus", template = "herald_aakuan_champion", x = 4176, z = 2, y = 5109, angle = -90, cell = 0, destX = 5934, destY = 4614, stringFile = "herald_talus_aakuan_champions" },
			{ planet = "talus", template = "herald_commoner", x = 323.86, z = 6, y = -3097.8, angle = 92, cell = 0, destX = -4757, destY = -4753, stringFile = "herald_talus_berserk_khmurra" },
			{ planet = "talus", template = "herald_commoner", x = 154.4, z = 6, y = -2932.8, angle = 24.6, cell = 0, destX = 5530, destY = -4078, stringFile = "herald_talus_binyare_pirate_base" },
			{ planet = "talus", template = "herald_commoner", x = 4333.76, z = 9.9, y = 5372, angle = 75, cell = 0, destX = -4301, destY = -1435, stringFile = "herald_talus_lost_aqualish_war_party" },

			{ planet = "tatooine", template = "herald_commoner", x = 1271.2, z = 7, y = 2972.48, angle = 288.274, cell = 0, destX = 57.5, destY = -88.6, stringFile = "herald_tatooine_squill_cave" },
			{ planet = "tatooine", template = "herald_commoner", x = 145.91, z = 52, y = -5336.3, angle = 210, cell = 0, destX = -5370, destY = -4468, stringFile = "herald_tatooine_tusken_village" },
			{ planet = "tatooine", template = "herald_commoner", x = -9.5, z = 0.408, y = 0.574, angle = 94, cell = 1213345, destX = 6551, destY = -1323, stringFile = "herald_tatooine_beetle_cave" },
			{ planet = "tatooine", template = "herald_commoner", x = 3371.3, z = 5.0, y = -4520.8, angle = 118, cell = 0, destX = 5148, destY = 613, stringFile = "herald_tatooine_hutt_hideout" },
			{ planet = "tatooine", template = "herald_lorne_prestar", x = 3376.1, z = 5, y = -4786, angle = 117, cell = 0, destX = -3976, destY = 6317, stringFile = "herald_tatooine_fort_tusken" },
			{ planet = "tatooine", template = "herald_imperial_colonel", x = -1300, z = 12, y = -3507.2, angle = 100, cell = 0, destX = -1490, destY = -210, stringFile = "herald_tatooine_tusken_bunker" },
			{ planet = "tatooine", template = "herald_imperial_army_captain", x = -1307.46, z = 12, y = -3662.44, angle = 218.6, cell = 0, destX = -784, destY = -4451, stringFile = "herald_tatooine_rebel_military_base" },

			{ planet = "yavin4", template = "herald_commoner", x = -6982.5, z = 73, y = -5659.5, angle = 175, cell = 0, destX = -921, destY = -2042, stringFile = "herald_yavin_temple_of_the_blueleaf_cluster" },
			{ planet = "yavin4", template = "herald_chiss", x = -6877.3, z = 73, y = -5682.3, angle = -80, cell = 0, destX = 5152, destY = 5537, stringFile = "herald_yavin_temple_of_exarkun" },
			{ planet = "yavin4", template = "herald_commoner", x = -351.45, z = 35, y = 4856.88, angle = 178, cell = 0, destX = 482, destY = -678, stringFile = "herald_yavin_palace_of_the_woolamander" },
			{ planet = "yavin4", template = "herald_rebel_commando", x = -325, z = 35, y = 4862, angle = 40, cell = 0, destX = 4031, destY = -6281, stringFile = "herald_yavin_imperial_outpost" },
		},

		kidnappedNobles = {
			{ planet = "naboo", template = "naboo_kidnapped_noble_01", x = -31.73, z = -14, y = 74, angle = 0, cell = 5535572, respawn = 3600 },
			{ planet = "naboo", template = "naboo_kidnapped_noble_02", x = -40.97, z = -14, y = 84.07, angle = 84, cell = 5535572, respawn = 3600 },
			{ planet = "naboo", template = "naboo_kidnapped_noble_03", x = -40.21, z = -14, y = 78.5, angle = 78, cell = 5535572, respawn = 3600 },
		},

		multiDestHeraldList = {
			{ planet = "corellia", template = "herald_corellia_lock", x = -5418.26, z = 20.9986, y = -2792.36, angle = 59.9, cell = 0, dest1X = 1414, dest1Y = -317, dest1String = "Lord Nyax's Clan", dest2X = -2483, dest2Y = 2907, dest2String = "Afarathu Cave", dest3X = 4651, dest3Y = -5617, dest3String = "Imperial Stronghold", stringFile = "heraldcorellia1" },
			{ planet = "corellia", template = "herald_corellia_karin", x = -185, z = 28, y = -4460.71, angle = 0, cell = 0, dest1X = 1042, dest1Y = 4193, dest1String = "Drall Patriots Hideout", dest2X = 5291, dest2Y = 1494, dest2String = "Rogue Corsec band", stringFile = "heraldcorellia2" },
			{ planet = "lok", template = "herald_lok_talia", x = 371.717, z = 11.8618, y = 5179.1, angle = 286.408, cell = 0, dest1X = -68, dest1Y = 2650, dest1String = "Kimogila Town", dest2X = -3792, dest2Y = -3904, dest2String = "Canyon Corsair Stronghold", stringFile = "heraldlok" },
			{ planet = "lok", template = "herald_lok_andria", x = 221.864, z = 17.0919, y = 5154.99, angle = 342.34, cell = 0, dest1X = 3364, dest1Y = -4923, dest1String = "Droid Engineer's Cave", dest2X = 3703, dest2Y = 2274, dest2String = "Smuggler's Ship", stringFile = "heraldlok2" },
			{ planet = "naboo", template = "herald_naboo_vaikanna", x = -5484, z = 10, y = 4424, angle = -29, cell = 0, dest1X = -6570, dest1Y = -3240, dest1String = "The Imperial Weapons Research Facility", dest2X = 2850, dest2Y = 1084, dest2String = "Mauler Encampment", dest3X = 4771, dest3Y = -3868, dest3String = "The Gungans and Imperials", stringFile = "heraldnaboo" },
			{ planet = "naboo", template = "herald_naboo_ronin", x = 4810, z = 6.3, y = -4977, angle = -21, cell = 0, dest1X = -263, dest1Y = 2859, dest1String = "Gungan Stronghold", dest2X = 5741, dest2Y = -1546, dest2String = "Veermok Cave", stringFile = "heraldnaboo2" },
			{ planet = "tatooine", template = "herald_tatooine_viconya", x = -1001.04, z = 10, y = -3550.02, angle = 271.672, cell = 0, dest1X = -3980, dest1Y = 6311, dest1String = "Fort Tusken", dest2X = -5315, dest2Y = -4440, dest2String = "Tusken Village", stringFile = "heraldtatooine1" },
			{ planet = "tatooine", template = "herald_tatooine_errik", x = 3380.6, z = 5, y = -4498, angle = 160, cell = 0, dest1X = 5003, dest1Y = 581, dest1String = "Hutt Hideout", dest1Cost = 60, dest2X = -780, dest2Y = -4444, dest2String = "Rebel Military Base", dest2Cost = 50, dest3X = -860, dest3Y = -4780, dest3String = "Rebel Military Base", dest3Cost = 30, dest4X = -1490, dest4Y = -210, dest4String = "Tusken Bunker", dest4Cost = 30, stringFile = "heraldtatooine2" },
		}
}

registerScreenPlay("heraldScreenPlay", true)

function heraldScreenPlay:start()
	self:spawnMobiles()
end

function heraldScreenPlay:spawnMobiles()
	local mobiles = self.heraldList
	for i = 1, #mobiles, 1 do
		if isZoneEnabled(mobiles[i].planet) then
			local pSpawn = spawnMobile(mobiles[i].planet, mobiles[i].template, 1, mobiles[i].x, mobiles[i].z, mobiles[i].y, mobiles[i].angle, mobiles[i].cell)
			if (pSpawn ~= nil) then
				if (mobiles[i].customName ~= nil) then
					CreatureObject(pSpawn):setCustomObjectName(mobiles[i].customName)
				end
				writeData(SceneObject(pSpawn):getObjectID() .. ":heraldID", i)
			end
		end
	end

	mobiles = self.kidnappedNobles
	for i = 1, #mobiles, 1 do
		if isZoneEnabled(mobiles[i].planet) then
			local pSpawn = spawnMobile(mobiles[i].planet, mobiles[i].template, mobiles[i].respawn, mobiles[i].x, mobiles[i].z, mobiles[i].y, mobiles[i].angle, mobiles[i].cell)
		end
	end

	mobiles = self.multiDestHeraldList
	for i = 1, #mobiles, 1 do
		if isZoneEnabled(mobiles[i].planet) then
			local pSpawn = spawnMobile(mobiles[i].planet, mobiles[i].template, 1, mobiles[i].x, mobiles[i].z, mobiles[i].y, mobiles[i].angle, mobiles[i].cell)
			if pSpawn ~= nil then
				writeData(SceneObject(pSpawn):getObjectID() .. ":multiDestHeraldID", i)
			end
		end
	end
end

function heraldScreenPlay:createLoc(pPlayer, heraldNum)
	local pGhost = CreatureObject(pPlayer):getPlayerObject()

	if (pGhost == nil) then
		return
	end

	local heraldData = self.heraldList[heraldNum]
	local stfFile = "@spawning/static_npc/" .. heraldData.stringFile
	local playerID = CreatureObject(pPlayer):getObjectID()

	local waypointID = PlayerObject(pGhost):addWaypoint(heraldData.planet, stfFile .. ":waypoint_name_1", stfFile .. ":waypoint_description_1", heraldData.destX, heraldData.destY, WAYPOINTPURPLE, true, true, 0, 0)
	writeData(playerID .. ":herald" .. heraldNum, waypointID)

	local pArea = spawnActiveArea(heraldData.planet, "object/active_area.iff", heraldData.destX, heraldData.z, heraldData.destY, 10, 0)
	if (pArea ~= nil) then
		local areaID = ActiveArea(pArea):getObjectID()
		createObserver(ENTEREDAREA, "heraldScreenPlay", "notifyEnteredHeraldArea", pArea)
		writeData(areaID .. ":ownerID", playerID)
		writeData(areaID .. ":heraldNum", heraldNum)
		writeData(playerID .. ":heraldArea" .. heraldNum, areaID)
	end
end

function heraldScreenPlay:notifyEnteredHeraldArea(pActiveArea, pPlayer)
	if (pActiveArea == nil or pPlayer == nil) then
		return 0
	end

	if (not SceneObject(pPlayer):isPlayerCreature()) then
		return 0
	end

	local areaID = SceneObject(pActiveArea):getObjectID()

	local ownerID = readData(areaID .. ":ownerID")
	local heraldNum = readData(areaID .. ":heraldNum")
	if (ownerID == CreatureObject(pPlayer):getObjectID()) then
		CreatureObject(pPlayer):sendSystemMessage("@theme_park/messages:go_message")
		self:cleanUp(pPlayer, heraldNum)
		return 1
	end

	return 0
end

function heraldScreenPlay:cleanUp(pPlayer, heraldNum)
	local pGhost = CreatureObject(pPlayer):getPlayerObject()

	if (pGhost == nil) then
		return
	end

	local playerID = CreatureObject(pPlayer):getObjectID()
	local waypointID = readData(playerID .. ":herald" .. heraldNum)
	local areaID = readData(playerID .. ":heraldArea" .. heraldNum)
	PlayerObject(pGhost):removeWaypoint(waypointID, true)
	local pArea = getSceneObject(areaID)
	if (pArea ~= nil) then
		deleteData(SceneObject(pArea):getObjectID() .. ":ownerID")
		deleteData(SceneObject(pArea):getObjectID() .. ":heraldNum")

		SceneObject(pArea):destroyObjectFromWorld()
	end

	writeData(playerID .. ":heraldArea" .. heraldNum, 0)
	writeData(playerID .. ":herald" .. heraldNum, 0)
end

function heraldScreenPlay:giveMultiDestWaypoint(pPlayer, heraldNum, locNum)
	local pGhost = CreatureObject(pPlayer):getPlayerObject()

	if (pGhost == nil) then
		return
	end

	local heraldData = self.multiDestHeraldList[heraldNum]
	local stfFile = "@conversation/" .. heraldData.stringFile
	local x, y, destString, destCost

	if locNum == 1 then
		x = heraldData.dest1X
		y = heraldData.dest1Y
		destString = heraldData.dest1String
		destCost = heraldData.dest1Cost
	elseif locNum == 2 then
		x = heraldData.dest2X
		y = heraldData.dest2Y
		destString = heraldData.dest2String
		destCost = heraldData.dest2Cost
	elseif locNum == 3 then
		x = heraldData.dest3X
		y = heraldData.dest3Y
		destString = heraldData.dest3String
		destCost = heraldData.dest3Cost
	elseif locNum == 4 then
		x = heraldData.dest4X
		y = heraldData.dest4Y
		destString = heraldData.dest4String
		destCost = heraldData.dest4Cost
	end

	if destCost ~= nil and destCost > 0 then
		CreatureObject(pPlayer):subtractCashCredits(destCost)
	end

	local pWaypoint = PlayerObject(pGhost):getWaypointAt(x, y, heraldData.planet)

	if pWaypoint ~= nil then
		local waypoint = LuaWaypointObject(pWaypoint)

		if not waypoint:isActive() then
			waypoint:setActive(1)
			PlayerObject(pGhost):updateWaypoint(SceneObject(pWaypoint):getObjectID())
		end
	else
		local wpName
		if string.find(destString, ":") ~= nil then
			wpName = stfFile .. destString
		else
			wpName = destString
		end

		PlayerObject(pGhost):addWaypoint(heraldData.planet, wpName, "", x, y, WAYPOINTBLUE, true, true, 0)
	end
end

HeraldConvoHandler = conv_handler:new {
	themePark = nil
}

function HeraldConvoHandler:setThemePark(themeParkNew)
	self.themePark = themeParkNew
end

function HeraldConvoHandler:runScreenHandlers(pConvTemplate, pPlayer, pNpc, selectedOption, pConvScreen)
	local screen = LuaConversationScreen(pConvScreen)
	local screenID = screen:getScreenID()

	local pConvScreen = screen:cloneScreen()

	if screenID == "init" then
		pConvScreen = self:handleScreenInit(pConvTemplate, pPlayer, pNpc, selectedOption, pConvScreen)
	elseif screenID == "npc_1_1" then
		pConvScreen = self:handleScreenNpc1(pConvTemplate, pPlayer, pNpc, selectedOption, pConvScreen)
	elseif screenID == "npc_2_1" then
		pConvScreen = self:handleScreenNpc2(pConvTemplate, pPlayer, pNpc, selectedOption, pConvScreen)
	elseif screenID == "npc_3_1" then
		pConvScreen = self:handleScreenNpc3(pConvTemplate, pPlayer, pNpc, selectedOption, pConvScreen)
	elseif screenID == "npc_4_1" then
		pConvScreen = self:handleScreenNpc4(pConvTemplate, pPlayer, pNpc, selectedOption, pConvScreen)
	elseif screenID == "npc_work_1" then
		pConvScreen = self:handleScreenNpcWork(pConvTemplate, pPlayer, pNpc, selectedOption, pConvScreen)
	elseif screenID == "npc_reset" then
		pConvScreen = self:handleScreenNpcReset(pConvTemplate, pPlayer, pNpc, selectedOption, pConvScreen)
	elseif screenID == "npc_backtowork_1" then
		pConvScreen = self:handleScreenNpcBackToWork(pConvTemplate, pPlayer, pNpc, selectedOption, pConvScreen)
	end
	return pConvScreen
end

function HeraldConvoHandler:handleScreenNpc1(pConvTemplate, pPlayer, pNpc, selectedOption, pConvScreen)
	local clonedScreen = LuaConversationScreen(pConvScreen)

	local heraldNumber = readData(SceneObject(pNpc):getObjectID() .. ":heraldID")
	local stfFile = "@spawning/static_npc/" .. self.themePark.heraldList[heraldNumber].stringFile

	clonedScreen:setDialogTextStringId(stfFile .. ":npc_1_1")
	clonedScreen:removeAllOptions()

	clonedScreen:addOption(stfFile .. ":player_1_1", "npc_2_1")
	clonedScreen:addOption(stfFile .. ":player_2_1", "npc_3_1")
	clonedScreen:addOption(stfFile .. ":player_3_1", "npc_4_1")

	return pConvScreen
end

function HeraldConvoHandler:handleScreenNpc2(pConvTemplate, pPlayer, pNpc, selectedOption, pConvScreen)
	local clonedScreen = LuaConversationScreen(pConvScreen)

	local heraldNumber = readData(SceneObject(pNpc):getObjectID() .. ":heraldID")
	local heraldData = self.themePark.heraldList[heraldNumber]
	local stfFile = "@spawning/static_npc/" .. heraldData.stringFile
	heraldScreenPlay:createLoc(pPlayer, heraldNumber)

	clonedScreen:setDialogTextStringId(stfFile .. ":npc_2_1")

	return pConvScreen
end

function HeraldConvoHandler:handleScreenNpc3(pConvTemplate, pPlayer, pNpc, selectedOption, pConvScreen)
	local clonedScreen = LuaConversationScreen(pConvScreen)

	local heraldNumber = readData(SceneObject(pNpc):getObjectID() .. ":heraldID")
	local stfFile = "@spawning/static_npc/" .. self.themePark.heraldList[heraldNumber].stringFile

	clonedScreen:setDialogTextStringId(stfFile .. ":npc_3_1")

	return pConvScreen
end

function HeraldConvoHandler:handleScreenNpc4(pConvTemplate, pPlayer, pNpc, selectedOption, pConvScreen)
	local clonedScreen = LuaConversationScreen(pConvScreen)

	local heraldNumber = readData(SceneObject(pNpc):getObjectID() .. ":heraldID")
	local stfFile = "@spawning/static_npc/" .. self.themePark.heraldList[heraldNumber].stringFile

	clonedScreen:setDialogTextStringId(stfFile .. ":npc_4_1")
	clonedScreen:removeAllOptions()

	clonedScreen:addOption(stfFile .. ":player_1_1", "npc_2_1")
	clonedScreen:addOption(stfFile .. ":player_2_1", "npc_3_1")

	return pConvScreen
end

function HeraldConvoHandler:handleScreenNpcWork(pConvTemplate, pPlayer, pNpc, selectedOption, pConvScreen)
	local clonedScreen = LuaConversationScreen(pConvScreen)

	local heraldNumber = readData(SceneObject(pNpc):getObjectID() .. ":heraldID")
	local stfFile = "@spawning/static_npc/" .. self.themePark.heraldList[heraldNumber].stringFile

	clonedScreen:setDialogTextStringId(stfFile .. ":npc_work_1")

	clonedScreen:removeAllOptions()

	clonedScreen:addOption(stfFile .. ":player_sorry", "npc_backtowork_1")
	clonedScreen:addOption(stfFile .. ":player_reset", "npc_reset")

	return pConvScreen
end

function HeraldConvoHandler:handleScreenNpcBackToWork(pConvTemplate, pPlayer, pNpc, selectedOption, pConvScreen)
	local clonedScreen = LuaConversationScreen(pConvScreen)

	local heraldNumber = readData(SceneObject(pNpc):getObjectID() .. ":heraldID")
	local stfFile = "@spawning/static_npc/" .. self.themePark.heraldList[heraldNumber].stringFile

	heraldScreenPlay:cleanUp(pPlayer, heraldNumber)
	heraldScreenPlay:createLoc(pPlayer, heraldNumber)

	clonedScreen:setDialogTextStringId(stfFile .. ":npc_backtowork_1")

	return pConvScreen
end

function HeraldConvoHandler:handleScreenNpcReset(pConvTemplate, pPlayer, pNpc, selectedOption, pConvScreen)
	local clonedScreen = LuaConversationScreen(pConvScreen)

	local heraldNumber = readData(SceneObject(pNpc):getObjectID() .. ":heraldID")
	local stfFile = "@spawning/static_npc/" .. self.themePark.heraldList[heraldNumber].stringFile

	heraldScreenPlay:cleanUp(pPlayer, heraldNumber)

	clonedScreen:setDialogTextStringId(stfFile .. ":npc_reset")

	return pConvScreen
end


function HeraldConvoHandler:handleScreenInit(pConvTemplate, pPlayer, pNpc, selectedOption, pConvScreen)
	local convoTemplate = LuaConversationTemplate(pConvTemplate)
	local nextScreenName
	local heraldNumber = readData(SceneObject(pNpc):getObjectID() .. ":heraldID")
	local activeHerald = readData(CreatureObject(pPlayer):getObjectID() .. ":herald" .. heraldNumber)
	if activeHerald ~= 0 then
		nextScreenName = "npc_work_1"
	else
		nextScreenName = "npc_1_1"
	end
	return self:runScreenHandlers(pConvTemplate, pPlayer, pNpc, selectedOption, convoTemplate:getScreen(nextScreenName))
end

herald_conv_handler = HeraldConvoHandler:new {
	themePark = heraldScreenPlay
}

MultiDestHeraldConvoHandler = conv_handler:new {
	themePark = nil
}

function MultiDestHeraldConvoHandler:runScreenHandlers(pConvTemplate, pPlayer, pNpc, selectedOption, pConvScreen)
	local screen = LuaConversationScreen(pConvScreen)
	local screenID = screen:getScreenID()

	if screenID == "loc1" or screenID == "loc1a" then
		self:handleScreenLoc(pPlayer, pNpc, 1)
	elseif screenID == "loc2" then
		self:handleScreenLoc(pPlayer, pNpc, 2)
	elseif screenID == "loc3" then
		self:handleScreenLoc(pPlayer, pNpc, 3)
	elseif screenID == "loc4" then
		self:handleScreenLoc(pPlayer, pNpc, 4)
	end

	return pConvScreen
end

function MultiDestHeraldConvoHandler:handleScreenLoc(pPlayer, pNpc, locNum)
	local heraldNumber = readData(SceneObject(pNpc):getObjectID() .. ":multiDestHeraldID")
	heraldScreenPlay:giveMultiDestWaypoint(pPlayer, heraldNumber, locNum)
end

multi_dest_herald_conv_handler = MultiDestHeraldConvoHandler:new {
	themePark = heraldScreenPlay
}
