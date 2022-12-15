NabooKaadaraScreenPlay = CityScreenPlay:new {
	numberOfActs = 1,

	screenplayName = "NabooKaadaraScreenPlay",

	planet = "naboo",

	gcwMobs = {
		--spawn garrison outside city in the outskirts
		{"stormtrooper", "rebel_trooper", 0, 0.3, 7.7, 0, 1259939, "", ""},
		{"stormtrooper", "rebel_trooper", 0, 0.3, 6.2, 0, 1259939, "", ""},
		{"stormtrooper", "rebel_trooper", 3.7, 0.3, -2.0, 0, 1259940, "", ""},
		{"stormtrooper_rifleman", "rebel_commando", -3.7, -3.3, 4.9, 180, 1259942, "", ""},
		{"stormtrooper_medic", "rebel_medic", 7.8, -6.8, -5.8, -65, 1259943, "", ""},
		{"stormtrooper_colonel", "rebel_colonel", 3.5, -6.8, -4.8, 0, 1259943, "", "", true},
		{"stormtrooper_sniper", "rebel_scout", -3.7, -13.8, -18.5, 90, 1259944, "", ""},
		{"stormtrooper_colonel", "rebel_major", -0.9, -13.7, -4.7, -45, 1259945, "", ""},
		{"stormtrooper", "rebel_trooper", -5.1, -13.7, 7.5, 180, 1259945, "", ""},
		{"stormtrooper", "rebel_trooper", -1.9, -13.7, 7.5, 180, 1259945, "", ""},
		{"stormtrooper_medic", "rebel_medic", -5.3, -13.8, 13.5, -30, 1259950, "", ""},
		{"stormtrooper_squad_leader", "rebel_general", 14.7, -13.8, 5.2, 0, 1260045, "", "", true},
		{"stormtrooper", "rebel_trooper", 14.7, -13.8, 8.0, 0, 1260045, "", ""},
		{"stormtrooper", "rebel_trooper", 14.7, -13.8, 9.8, 0, 1260045, "", ""},
		{"imperial_army_captain", "rebel_army_captain", 15.1, -11.5, -20.4, 0, 1260046, "", "", true},
		{"stormtrooper", "rebel_trooper", 3.7, 0.1, 2.1, -90, 1260051, "", ""},
		{"stormtrooper", "rebel_trooper", 0, 0.1, -1.9, 0, 1260050, "", ""},
		{"stormtrooper", "rebel_trooper", 0, 0.1, -1.9, 0, 1260057, "", ""},
		{"imperial_colonel", "rebel_lieutenant_colonel", 5.3, 0.1, 4.5, -150, 1260058, "", ""},
		{"stormtrooper", "rebel_trooper", 5384.5, -197.2, 6455, -97, 0, "", ""},
		{"stormtrooper", "rebel_trooper", 5384.5, -197.2, 6458, -97, 0, "", ""},
		{"stormtrooper", "rebel_trooper", 5381.5, -197.2, 6454.5, -97, 0, "", "", true},
		{"stormtrooper", "rebel_trooper", 5381.5, -197.2, 6457.5, -97, 0, "", ""},
		{"stormtrooper", "rebel_trooper", 5378.5, -197.2, 6454, -97, 0, "", ""},
		{"stormtrooper", "rebel_trooper", 5378.5, -197.2, 6457, -97, 0, "", ""},
		{"stormtrooper", "rebel_trooper", 5375.5, -197.2, 6453.5, -97, 0, "", ""},
		{"stormtrooper", "rebel_trooper", 5375.5, -197.2, 6456.5, -97, 0, "", "", true},

		--faction spawns inside the actual city limits
		{"stormtrooper", "rebel_trooper", 5290.5, -192, 6582.2, -90, 0, "", "", true},
		{"stormtrooper", "rebel_trooper", 5294.5, -192, 6582.2, -90, 0, "", ""},
		{"stormtrooper", "rebel_trooper", 5298.5, -192, 6582.2, -90, 0, "", ""},
		{"stormtrooper", "rebel_trooper", getRandomNumber(20) + 5189.5, -192, getRandomNumber(28) + 6624.5, getRandomNumber(360), 0, "", ""},
		{"stormtrooper_medic", "rebel_medic", getRandomNumber(20) + 5189.5, -192, getRandomNumber(28) + 6624.5, getRandomNumber(360), 0, "", ""},
		{"stormtrooper_rifleman", "rebel_gungan_bomber", getRandomNumber(20) + 5189.5, -192, getRandomNumber(28) + 6624.5, getRandomNumber(360), 0, "", ""},
		{"stormtrooper", "rebel_trooper", getRandomNumber(20) + 5189.5, -192, getRandomNumber(20) + 6624.5, getRandomNumber(360), 0, "", ""},
		{"stormtrooper", "rebel_trooper", 29.9, 0.3, -2, -79, 1741439, "", ""},
		{"stormtrooper", "rebel_trooper", 18.6, 0.3, 9.5, -90, 1741439, "", "", true},
		{"stormtrooper", "rebel_trooper", -7.2, 0.3, -3.2, -111, 1741443, "", ""},
		{"stormtrooper", "rebel_trooper", -18.4, 0.3, -6.6, -101, 1741443, "", ""},
		{"naboo_police", "naboo_police", 5095.7, -192, 6744.4, 136, 0, "", "", true},
		{"naboo_police", "naboo_police", 5105.6, -192, 6733.9, 136, 0, "", "", true},
		{"stormtrooper", "rebel_trooper", 5137.7, -192, 6692.9, 136, 0, "", ""},
		{"stormtrooper", "rebel_trooper", 5148.6, -192, 6682.4, 136, 0, "", "", true},
		{"stormtrooper", "rebel_trooper", 5023.0, -192, 6714.8, -40, 0, "", ""},
		{"naboo_police_chief", "naboo_police_chief", 5011.7, -192, 6728.2, -40, 0, "", "", true},
		{"naboo_police_officer", "naboo_police_officer", 4985.0, -192, 6763.0, 88, 0, "", "", true},
	},

	patrolNpcs = {"businessman_patrol", "commoner_fat_patrol", "commoner_old_patrol", "commoner_patrol", "commoner_naboo_patrol", "noble_patrol", "official_patrol", "scientist_patrol"},

	patrolMobiles = {
		--{patrolPoints, template, x, z, y, direction, cell, mood, combatPatrol},

		--Droids
		{"surgical_1", "surgical_droid_21b", 4.5, -5.5, -13.2, -144, 1741516, "", false},
		{"r3_1", "r3", 5189, -192, 6691, 0, 0, "", false},

		--NPCs
		{"npc_1", "patrolNpc", 5236, -192, 6759, 87, 0, "", false},
		{"npc_2", "patrolNpc", 5180.9, -192, 6782.7, 45, 0, "", false},
	},

	patrolPoints = {
		--table_name = {{x, z, y, cell, delayAtNextPoint}}
		surgical_1 = {{4.5, -5.5, -13.2, 1741516, false}, {-13.5, -5.0, -10.7, 1741516, false}, {-7.1, -5.5, -9.4, 1741516, false}, {-3.1, -5.5, -3.0, 1741516, false}, {6.2, -5.5, -3.9, 1741516, true}, {6.3, -5.5, -3.1, 1741516, false}, {12.4, -5.0, -15.8, 1741516, false}},
		r3_1 = {{5189, -192, 6691, 0, false}, {5179, -192, 6671, 0, false}, {5189, -192, 6653, 0, false}, {5200, -192, 6655, 0, false}, {5205, -192, 6693, 0, false}},

		npc_1 = {{5236, -192, 6759, 0, true}, {5255, -192, 6755, 0, true}, {5242, -192, 6721, 0, true}, {5230, -192, 6719, 0, true}, {5224,  -192, 6749, 0, true}},
		npc_2 = {{5180.9, -192, 6782.7, 0, true}, {5160, -192, 6758, 0, true}, {5163, -192, 6769, 0, true}, {5182, -192, 6789, 0, true}},
	},

	stationaryCommoners = {"commoner", "commoner_fat", "commoner_naboo", "commoner_old"},
	stationaryNpcs = {"agriculturalist", "artisan", "bodyguard", "bothan_diplomat", "bounty_hunter", "businessman", "commoner_technician", "contractor", "entertainer", "explorer", "farmer", "farmer_rancher", "fringer",
				"gambler", "info_broker", "medic", "mercenary", "miner", "naboo_nomad", "noble", "official", "patron_ishitib", "pilot", "rancher", "scientist", "slicer", "traveller"},

	--{respawn, x, z, y, direction, cell, mood}
	stationaryMobiles = {
		{1, 5040.3, -183.3, 6736.1, 132, 0, ""},
		{1, 5371.9, -191.4, 6736.9, 141, 0, "conversation"},
		{1, 5373.5, -191.4, 6737.0, -132, 0, "conversation"},
		{1, 5372.5, -191.4, 6735.6, 12, 0, "conversation"},
		{1, 5377.1, -192, 6632.5, 159, 0, "conversation"},
		{1, 5377.9, -192, 6630.5, -31, 0, "conversation"},
		{1, 5325.5, -192, 6607.4, -151, 0, ""},
		{1, 5332.8, -192, 6602.2, 127, 0, "conversation"},
		{1, 5334.6, -192, 6600.7, -45, 0, "conversation"},
		{1, 5284.8, -192, 6591.8, -111, 0, ""},
		{1, 5299.3, -191.4, 6757.1, -80, 0, ""},
		{1, 5298.0, -191.4, 6757.1, 100, 0, ""},
		{1, 5080.9, -191.7, 6714.0, -155, 0, ""},
		{1, 5208.8, -192, 6644.8, 0, 0, "conversation"},
		{1, 5208.8, -192, 6646.3, 180, 0, "conversation"},
	},

	mobiles = {
		--Hotel
		{"info_broker",60,-2.1,1.0,12.6,90,1741477, "happy"},
		{"patron_devaronian",60,2.1,1.0,-4.5,-24,1741477, "entertained"},
		{"entertainer",60,1.2,1.0,-1.9,78,1741477, "happy"},
		{"chiss_female",60,2.6,1.0,-0.5,-115,1741477, "npc_standing_drinking"},
		{"contractor",60,4.0,1.0,4.3,-165,1741477, "npc_accusing"},
		{"bartender", 1, 19.5, 1.6, 12.6, 180, 1741478, ""},

		--cantina
		{"bartender", 1, 2.9, -0.9, 3.4, 2, 64, "neutral"},

		--starport interior
		{"chassis_dealer",60,6.3,0.6,65.8,-60,1741539, "neutral"},

		--commoners
		{"medic", 1, -3.5, -0.1, -3.3, 0, 1741514, ""},
		{"medic", 1, -5.2, 0.1, 5.2, -90, 1741514, ""},
		{"scientist", 1, -13.2, 0.1, 15.3, 90, 1741517, ""},
		{"commoner_tatooine", 120, -23.6, 1.6, -4.2, -172, 1741483, ""},
		{"commoner_naboo", 120, -24.5, 1.6, -7.3, 17, 1741483, ""},

		--trainers
		{"trainer_architect", 1, 11, 1.133, -14.5, 0, 1741470, ""},
		{"trainer_armorsmith", 1, -15, 1.1, 0, 221, 1741469, ""},
		{"trainer_artisan", 1, -0.031573, 1.13306, -14.1498, 0, 1741510, ""},
		{"trainer_artisan", 1, 5182, -192, 6686, 138, 0, ""},
		{"trainer_brawler", 1, 5174.04, -192, 6639.92, 4, 0, ""},
		{"trainer_brawler", 0, -11.887, 1.13306, -11.5344, 1, 1741462, ""},
		{"trainer_combatmedic", 1, 5080, -192, 6655, -49, 0, ""},
		{"trainer_combatmedic", 0, -16.368, 0.26, 11.0637, 172, 1741443, ""},
		{"trainer_creaturehandler", 1, 5181, -192, 6806, 0, 0, ""},
		{"trainer_dancer", 1, 17.6312, 2.12874, 53.7646, 1, 4105618, ""},
		{"trainer_doctor", 1, 5021, -192, 6787, 2, 0, ""},
		{"trainer_doctor", 1, -24.4979, 0.26, -4.7603, 187, 1741443, ""},
		{"trainer_doctor", 1, 21.4466, 0.26, -2.96069, 37, 1741439, ""},
		{"trainer_droidengineer", 1, 5191, -192, 6695, 143, 0, ""},
		{"trainer_droidengineer", 1, -10.8596, 1.13306, -14.4116, 1, 1741472, ""},
		{"trainer_entertainer", 1, 5209.69, -192, 6661.65, 257, 0, ""},
		{"trainer_imagedesigner", 1, -21.662, 2.12878, 75.0346, 166, 4105619, ""},
		{"trainer_marksman", 1, 0, 1.13306, -13, 0, 1741461, ""},
		{"trainer_marksman", 0, 5182, -192, 6632, 0, 0, ""},
		{"trainer_medic", 1, 5211, -192, 6694, -62, 0, ""},
		{"trainer_medic", 1, -16.43, 0.26, 0.33, 173, 1741443, ""},
		{"trainer_medic", 1, 15.4875, 0.26, 6.2929, 20, 1741439, ""},
		{"trainer_merchant", 1, 12, 1.13306, 6, 180, 1741468, ""},
		{"trainer_musician", 1, 21.8, 2.1, 76.3, 180, 4105618, ""},
		{"theater_manager", 1, 21.7817, 2.12808, 63.5, 1, 4105618, ""},
		{"trainer_politician", 1, 5279, -192, 6591, 0, 0, ""},
		{"trainer_ranger", 1, 5279, -192, 6594, 180, 0, ""},
		{"trainer_scout", 1, 5192.38, -192, 6614.09, 5, 0, ""},
		{"trainer_scout", 1, -10.8899, 1.13306, 4.89649, 204, 1741459, ""},
		{"trainer_weaponsmith", 1, -2.5, 1.13306, -8.4, 91, 1741471, ""},
		{"trainer_shipwright",1,5.1,0.6,66.6,119,1741539, "neutral"},

		--misc
		{"informant_npc_lvl_1", 1, 5042, -188, 6734, 0, 0, ""},
		{"informant_npc_lvl_1", 1, 5199, -192, 6546, 0, 0, ""},
		{"informant_npc_lvl_2", 1, 5410, -192, 6672, 225, 0, ""},
		{"informant_npc_lvl_2", 1, 5075, -192, 6784, 0, 0, ""},
		{"informant_npc_lvl_2", 1, 5261, -192, 6793, 90, 0, ""},
		{"informant_npc_lvl_2", 1, 5084, -192, 6593, 225, 0, ""},
		{"informant_npc_lvl_3", 1, 4999, -192, 6822, 270, 0, ""},
		{"informant_npc_lvl_3", 1, 6.37977, -0.894992, 6.05897, 225, 64, ""},
		{"informant_npc_lvl_3", 1, 5009, -192, 6652, 315, 0, ""},
		{"informant_npc_lvl_3", 1, 5277, -192, 6560, 0, 0, ""},
		{"junk_dealer", 0, -14.5, 1.1, 3.0, 116, 1741459, ""},
		{"junk_dealer", 0, 5143.9, -192, 6818, 47, 0, ""}
	}
}

registerScreenPlay("NabooKaadaraScreenPlay", true)

function NabooKaadaraScreenPlay:start()
	if (isZoneEnabled(self.planet)) then
		self:spawnMobiles()
		self:spawnSceneObjects()
		self:spawnGcwMobiles()
		self:spawnPatrolMobiles()
		self:spawnStationaryMobiles()
	end
end

function NabooKaadaraScreenPlay:spawnSceneObjects()

	--outside starport
	spawnSceneObject(self.planet, "object/tangible/crafting/station/public_space_station.iff", 5212.39, -192, 6686.37, 0, math.rad(-80) )
	spawnSceneObject(self.planet, "object/tangible/crafting/station/public_space_station.iff", 5210, -192, 6668.48, 0, math.rad(-80) )
end

function NabooKaadaraScreenPlay:spawnMobiles()
	local mobiles = self.mobiles

	for i = 1, #mobiles, 1 do
		local mob = mobiles[i]

		-- {template, respawn, x, z, y, direction, cell, mood}
		local pMobile = spawnMobile(self.planet, mob[1], mob[2], mob[3], mob[4], mob[5], mob[6], mob[7])

		if (pMobile ~= nil) then
			if mob[8] ~= "" then
				CreatureObject(pMobile):setMoodString(mob[8])
			end

			AiAgent(pMobile):addCreatureFlag(AI_STATIC)

			if CreatureObject(pMobile):getPvpStatusBitmask() == 0 then
				CreatureObject(pMobile):clearOptionBit(AIENABLED)
			end
		end
	end

	--thugs
	spawnMobile(self.planet, "skaak_tipper_mugger", 300, 5414.8, -192, 6713.9, 112, 0)
	spawnMobile(self.planet, "skaak_tipper_mugger", 300, 5414.8, -192, 6708.4, -132, 0)
	spawnMobile(self.planet, "skaak_tipper_mugger", 300, 5418.8, -192, 6711.3, -26, 0)
	spawnMobile(self.planet, "skaak_tipper_bandit", 300, 5242.9, -192, 6796.6, -151, 0)
	spawnMobile(self.planet, "skaak_tipper_bandit", 300, 5240.1, -192, 6796.9, 90, 0)
	spawnMobile(self.planet, "skaak_tipper_bandit", 300, 5241.1, -192, 6793.4, -15, 0)
	spawnMobile(self.planet, "skaak_tipper_bandit", 300, 5202.0, -192, 6560.6, 83, 0)
	spawnMobile(self.planet, "skaak_tipper_bandit", 300, 5205.0, -192, 6558.6, 3, 0)
	spawnMobile(self.planet, "skaak_tipper_bandit", 300, 5204.0, -192, 6559.3, -53, 0)
	spawnMobile(self.planet, "skaak_tipper_crook", 300, getRandomNumber(20) + 5347.0, -192, getRandomNumber(12) + 6805, getRandomNumber(180) + -90, 0)
	spawnMobile(self.planet, "skaak_tipper_crook", 300, getRandomNumber(20) + 5347.0, -192, getRandomNumber(12) + 6805, getRandomNumber(180) + -90, 0)
	spawnMobile(self.planet, "skaak_tipper_crook", 300, getRandomNumber(20) + 5347.0, -192, getRandomNumber(12) + 6805, getRandomNumber(180) + -90, 0)
	spawnMobile(self.planet, "skaak_tipper_crook", 300, getRandomNumber(20) + 5347.0, -192, getRandomNumber(12) + 6805, getRandomNumber(180) + -90, 0)
	spawnMobile(self.planet, "skaak_tipper_mugger", 300, 4917.3, -192, 6740.5, -90, 0)
	spawnMobile(self.planet, "skaak_tipper_mugger", 300, 4914.0, -192, 6743.7, -136, 0)
	spawnMobile(self.planet, "skaak_tipper_mugger", 300, 4912.2, -192, 6737.5, 76, 0)
	spawnMobile(self.planet, "skaak_tipper_crook", 300, getRandomNumber(20) + 5018, -191.4, getRandomNumber(20) + 6591, getRandomNumber(180) + 90, 0)
	spawnMobile(self.planet, "skaak_tipper_crook", 300, getRandomNumber(20) + 5018, -191.4, getRandomNumber(20) + 6591, getRandomNumber(180) + 90, 0)
	spawnMobile(self.planet, "skaak_tipper_crook", 300, getRandomNumber(20) + 5018, -191.4, getRandomNumber(20) + 6591, getRandomNumber(180) + 90, 0)
	spawnMobile(self.planet, "skaak_tipper_crook", 300, getRandomNumber(20) + 5018, -191.4, getRandomNumber(20) + 6591, getRandomNumber(180) + 90, 0)
	spawnMobile(self.planet, "skaak_tipper_crook", 300, getRandomNumber(20) + 5018, -191.4, getRandomNumber(20) + 6591, getRandomNumber(180) + 90, 0)

	--creatures
	--[[spawnMobile(self.planet, "nuna", 300, getRandomNumber(20) + 5346.5, -205.4, getRandomNumber(12) + 6888.5, getRandomNumber(360), 0)
	spawnMobile(self.planet, "nuna", 300, getRandomNumber(20) + 5346.5, -205.4, getRandomNumber(12) + 6888.5, getRandomNumber(360), 0)
	spawnMobile(self.planet, "nuna", 300, getRandomNumber(20) + 5346.5, -205.4, getRandomNumber(12) + 6888.5, getRandomNumber(360), 0)
	spawnMobile(self.planet, "nuna", 300, getRandomNumber(20) + 5346.5, -205.4, getRandomNumber(12) + 6888.5, getRandomNumber(360), 0)
	spawnMobile(self.planet, "nuna", 300, getRandomNumber(20) + 5346.5, -205.4, getRandomNumber(12) + 6888.5, getRandomNumber(360), 0)
	spawnMobile(self.planet, "nuna", 300, getRandomNumber(20) + 5346.5, -205.4, getRandomNumber(12) + 6888.5, getRandomNumber(360), 0)

	spawnMobile(self.planet, "nuna", 300, getRandomNumber(20) + 5159.7, -208.1, getRandomNumber(12) + 6906.3, getRandomNumber(360), 0)
	spawnMobile(self.planet, "nuna", 300, getRandomNumber(20) + 5159.7, -208.1, getRandomNumber(12) + 6906.3, getRandomNumber(360), 0)
	spawnMobile(self.planet, "nuna", 300, getRandomNumber(20) + 5159.7, -208.1, getRandomNumber(12) + 6906.3, getRandomNumber(360), 0)
	spawnMobile(self.planet, "nuna", 300, getRandomNumber(20) + 5159.7, -208.1, getRandomNumber(12) + 6906.3, getRandomNumber(360), 0)
	spawnMobile(self.planet, "nuna", 300, getRandomNumber(20) + 5159.7, -208.1, getRandomNumber(12) + 6906.3, getRandomNumber(360), 0)
	spawnMobile(self.planet, "nuna", 300, getRandomNumber(20) + 5159.7, -208.1, getRandomNumber(12) + 6906.3, getRandomNumber(360), 0)

	spawnMobile(self.planet, "nuna", 300, getRandomNumber(20) + 4925.0, -208.2, getRandomNumber(12) + 6934.8, getRandomNumber(360), 0)
	spawnMobile(self.planet, "nuna", 300, getRandomNumber(20) + 4925.0, -208.2, getRandomNumber(12) + 6934.8, getRandomNumber(360), 0)
	spawnMobile(self.planet, "nuna", 300, getRandomNumber(20) + 4925.0, -208.2, getRandomNumber(12) + 6934.8, getRandomNumber(360), 0)
	spawnMobile(self.planet, "nuna", 300, getRandomNumber(20) + 4925.0, -208.2, getRandomNumber(12) + 6934.8, getRandomNumber(360), 0)
	spawnMobile(self.planet, "nuna", 300, getRandomNumber(20) + 4925.0, -208.2, getRandomNumber(12) + 6934.8, getRandomNumber(360), 0)
	spawnMobile(self.planet, "nuna", 300, getRandomNumber(20) + 4925.0, -208.2, getRandomNumber(12) + 6934.8, getRandomNumber(360), 0)

	spawnMobile(self.planet, "nightspider", 300, getRandomNumber(20) + 4995.1, -193.5, getRandomNumber(20) + 6452, getRandomNumber(360), 0)
	spawnMobile(self.planet, "nightspider", 300, getRandomNumber(20) + 4995.1, -193.5, getRandomNumber(20) + 6452, getRandomNumber(360), 0)
	spawnMobile(self.planet, "nightspider", 300, getRandomNumber(20) + 4995.1, -193.5, getRandomNumber(20) + 6452, getRandomNumber(360), 0)
	spawnMobile(self.planet, "nightspider", 300, getRandomNumber(20) + 4995.1, -193.5, getRandomNumber(20) + 6452, getRandomNumber(360), 0)
	spawnMobile(self.planet, "nightspider", 300, getRandomNumber(20) + 4995.1, -193.5, getRandomNumber(20) + 6452, getRandomNumber(360), 0)
	spawnMobile(self.planet, "nightspider", 300, getRandomNumber(20) + 4995.1, -193.5, getRandomNumber(20) + 6452, getRandomNumber(360), 0)

	spawnMobile(self.planet, "mott_calf", 300, getRandomNumber(20) + 5150.0, -196.9, getRandomNumber(20) + 6404, getRandomNumber(360), 0)
	spawnMobile(self.planet, "mott_calf", 300, getRandomNumber(20) + 5150.0, -196.9, getRandomNumber(20) + 6404, getRandomNumber(360), 0)
	spawnMobile(self.planet, "mott_calf", 300, getRandomNumber(20) + 5150.0, -196.9, getRandomNumber(20) + 6404, getRandomNumber(360), 0)
	spawnMobile(self.planet, "mott_calf", 300, getRandomNumber(20) + 5150.0, -196.9, getRandomNumber(20) + 6404, getRandomNumber(360), 0)
	spawnMobile(self.planet, "mott_calf", 300, getRandomNumber(20) + 5150.0, -196.9, getRandomNumber(20) + 6404, getRandomNumber(360), 0)
	spawnMobile(self.planet, "mott_calf", 300, getRandomNumber(20) + 5150.0, -196.9, getRandomNumber(20) + 6404, getRandomNumber(360), 0)

	spawnMobile(self.planet, "hermit_spider", 300, getRandomNumber(20) + 5269, -195.4, getRandomNumber(20) + 6318, getRandomNumber(360), 0)
	spawnMobile(self.planet, "hermit_spider", 300, getRandomNumber(20) + 5269, -195.4, getRandomNumber(20) + 6318, getRandomNumber(360), 0)
	spawnMobile(self.planet, "hermit_spider", 300, getRandomNumber(20) + 5269, -195.4, getRandomNumber(20) + 6318, getRandomNumber(360), 0)
	spawnMobile(self.planet, "hermit_spider", 300, getRandomNumber(20) + 5269, -195.4, getRandomNumber(20) + 6318, getRandomNumber(360), 0)
	spawnMobile(self.planet, "hermit_spider", 300, getRandomNumber(20) + 5269, -195.4, getRandomNumber(20) + 6318, getRandomNumber(360), 0)
	spawnMobile(self.planet, "hermit_spider", 300, getRandomNumber(20) + 5269, -195.4, getRandomNumber(20) + 6318, getRandomNumber(360), 0)

	spawnMobile(self.planet, "flewt", 300, getRandomNumber(20) + 5471, -197.8, getRandomNumber(20) + 6480.6, getRandomNumber(360), 0)
	spawnMobile(self.planet, "flewt", 300, getRandomNumber(20) + 5471, -197.8, getRandomNumber(20) + 6480.6, getRandomNumber(360), 0)
	spawnMobile(self.planet, "flewt", 300, getRandomNumber(20) + 5471, -197.8, getRandomNumber(20) + 6480.6, getRandomNumber(360), 0)
	spawnMobile(self.planet, "flewt", 300, getRandomNumber(20) + 5471, -197.8, getRandomNumber(20) + 6480.6, getRandomNumber(360), 0)
	spawnMobile(self.planet, "flewt", 300, getRandomNumber(20) + 5471, -197.8, getRandomNumber(20) + 6480.6, getRandomNumber(360), 0)
	spawnMobile(self.planet, "flewt", 300, getRandomNumber(20) + 5471, -197.8, getRandomNumber(20) + 6480.6, getRandomNumber(360), 0)

	spawnMobile(self.planet, "mott_calf", 300, getRandomNumber(20) + 5558, -192.5, getRandomNumber(20) + 6634, getRandomNumber(360), 0)
	spawnMobile(self.planet, "mott_calf", 300, getRandomNumber(20) + 5558, -192.5, getRandomNumber(20) + 6634, getRandomNumber(360), 0)
	spawnMobile(self.planet, "mott_calf", 300, getRandomNumber(20) + 5558, -192.5, getRandomNumber(20) + 6634, getRandomNumber(360), 0)
	spawnMobile(self.planet, "mott_calf", 300, getRandomNumber(20) + 5558, -192.5, getRandomNumber(20) + 6634, getRandomNumber(360), 0)
	spawnMobile(self.planet, "mott_calf", 300, getRandomNumber(20) + 5558, -192.5, getRandomNumber(20) + 6634, getRandomNumber(360), 0)
	spawnMobile(self.planet, "mott_calf", 300, getRandomNumber(20) + 5558, -192.5, getRandomNumber(20) + 6634, getRandomNumber(360), 0)

	spawnMobile(self.planet, "nightspider", 300, getRandomNumber(20) + 5535.6, -193.9, getRandomNumber(20) + 6745, getRandomNumber(360), 0)
	spawnMobile(self.planet, "nightspider", 300, getRandomNumber(20) + 5535.6, -193.9, getRandomNumber(20) + 6745, getRandomNumber(360), 0)
	spawnMobile(self.planet, "nightspider", 300, getRandomNumber(20) + 5535.6, -193.9, getRandomNumber(20) + 6745, getRandomNumber(360), 0)
	spawnMobile(self.planet, "nightspider", 300, getRandomNumber(20) + 5535.6, -193.9, getRandomNumber(20) + 6745, getRandomNumber(360), 0)
	spawnMobile(self.planet, "nightspider", 300, getRandomNumber(20) + 5535.6, -193.9, getRandomNumber(20) + 6745, getRandomNumber(360), 0)
	spawnMobile(self.planet, "nightspider", 300, getRandomNumber(20) + 5535.6, -193.9, getRandomNumber(20) + 6745, getRandomNumber(360), 0)]]

	local pNpc = spawnMobile(self.planet, "junk_dealer", 0, 5149.79, -192, 6745.73, -72, 0)
	if pNpc ~= nil then
		AiAgent(pNpc):setConvoTemplate("junkDealerArmsConvoTemplate")
	end
	pNpc = spawnMobile(self.planet, "junk_dealer", 0, 5136.8, -192, 6746.75, 48, 0)
	if pNpc ~= nil then
		AiAgent(pNpc):setConvoTemplate("junkDealerFineryConvoTemplate")
	end
end
