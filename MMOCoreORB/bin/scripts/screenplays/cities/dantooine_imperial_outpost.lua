DantooineImperialOutpostScreenPlay = CityScreenPlay:new {
	numberOfActs = 1,

	screenplayName = "DantooineImperialOutpostScreenPlay",

	planet = "dantooine",

	patrolMobiles = {
		--{patrolPoints, template, level, x, z, y, direction, cell, mood},
		{"cll8_1", "cll8_binary_load_lifter", 60, -4244.24, 3, -2381.33, 238.12, 0, ""},
		{"r4_1", "r4", 60, -4227.33, 3, -2366.11, 269.389, 0, ""},
		{"r5_1", "r5", 60, -4228.33, 3, -2367.11, 48.8478, 0, ""},
	},

	patrolPoints = {
		--table_name = {{x, z, y, cell, delayAtNextPoint}} 1 = no delay 0 = delay
		cll8_1 = {{-4244, 3, -2383, 0, 1}, {-4251, 3, -2383, 0, 1}, {-4229, 3, -2385, 0, 1}, {-4217, 3, -2380, 0, 1}, {-4206, 3, -2387, 0, 1}, {-4217, 3, -2380, 0, 1}, {-4229, 3, -2384, 0, 0}},
		r4_1 = {{-4227, 3, -2366, 0, 1}, {-4212, 3, -2356, 0, 1}, {-4212, 3, -2349, 0, 1}, {-4204, 3, -2366, 0, 1}},
		r5_1 = {{-4228, 3, -2367, 0, 1}, {-4212, 3, -2372, 0, 1}, {-4242, 3, -2352, 0, 1}},
	},
}

registerScreenPlay("DantooineImperialOutpostScreenPlay", true)

function DantooineImperialOutpostScreenPlay:start()
	if (isZoneEnabled("dantooine")) then
		self:spawnMobiles()
		self:spawnPatrolMobiles()
	end
end

function DantooineImperialOutpostScreenPlay:spawnMobiles()

	--tavern
	local pNpc = spawnMobile("dantooine", "scientist",60,3.3,0.1,4.5,127,1365879)
	self:setMoodString(pNpc, "npc_sitting_table_eating")

	--outside
	spawnMobile("dantooine", "at_st", 900, -4230, 3, -2330, 80, 0)
	spawnMobile("dantooine", "at_st", 900, -4230, 3, -2475, 0, 0)
	spawnMobile("dantooine", "stormtrooper", 450, -4212, 3, -2380, 300, 0)
	spawnMobile("dantooine", "stormtrooper", 450, -4217, 3, -2389, 330, 0)
	spawnMobile("dantooine", "stormtrooper", 450, -4232, 3, -2390, 40, 0)
	spawnMobile("dantooine", "stormtrooper", 450, -4234, 3, -2382, 330, 0)
	spawnMobile("dantooine", "stormtrooper", 450, -4222, 3, -2398, 340, 0)
	spawnMobile("dantooine", "stormtrooper", 450, -4228, 3, -2398, 10, 0)
	spawnMobile("dantooine", "stormtrooper", 450, -4249, 3, -2395, 160, 0)
	spawnMobile("dantooine", "stormtrooper", 450, -4267, 3, -2388, 120, 0)
	spawnMobile("dantooine", "stormtrooper", 450, -4275, 3, -2404, 80, 0)
	spawnMobile("dantooine", "stormtrooper", 450, -4275, 3, -2412, 60, 0)
	spawnMobile("dantooine", "stormtrooper", 450, -4266, 3, -2431, 20, 0)
	spawnMobile("dantooine", "stormtrooper", 450, -4287, 3, -2414, 230, 0)
	spawnMobile("dantooine", "stormtrooper", 450, -4293, 3, -2397, 320, 0)
	spawnMobile("dantooine", "stormtrooper", 450, -4199, 3, -2342, 40, 0)
	spawnMobile("dantooine", "stormtrooper", 450, -4164, 3, -2400, 10, 0)
	spawnMobile("dantooine", "stormtrooper", 450, -4167, 3, -2410, 0, 0)
	spawnMobile("dantooine", "stormtrooper", 450, -4180, 3, -2408, 90, 0)
	spawnMobile("dantooine", "stormtrooper", 450, -4228, 3, -2410, 190, 0)
	spawnMobile("dantooine", "stormtrooper", 450, -4240, 3, -2421, 350, 0)
	spawnMobile("dantooine", "imperial_corporal", 450, -4229, 3, -2431, 20, 0)
	spawnMobile("dantooine", "imperial_corporal", 450, -4204, 3, -2421, 0, 0)
	spawnMobile("dantooine", "imperial_private", 450, -4241, 3, -2403, 300, 0)
	spawnMobile("dantooine", "imperial_private", 450, -4198, 3, -2404, 240, 0)
	spawnMobile("dantooine", "bek_rabor", 60, -4223.5, 3, -2386, 115, 0)
	spawnMobile("dantooine", "businessman", 60, -4203, 3, -2357, 230, 0)
	spawnMobile("dantooine", "businessman", 60, -4214.17, 3, -2394.5, 357.839, 0)
	spawnMobile("dantooine", "businessman", 60, -4196.58, 3, -2425.71, 32.429, 0)
	spawnMobile("dantooine", "commoner", 60, -4234.61, 3, -2380.77, 150.893, 0)
	spawnMobile("dantooine", "commoner", 60, -4244.41, 3, -2373.1, 149.71, 0)
	spawnMobile("dantooine", "commoner_tatooine", 60, -4272, 3, -2391, 117, 0)
	spawnMobile("dantooine", "commoner", 60, -4258.64, 3, -2425.92, 281.664, 0)
	spawnMobile("dantooine", "commoner", 60, -4229.36, 3, -2423.44, 122.777, 0)
	spawnMobile("dantooine", "commoner", 60, -4210.71, 3, -2411.83, 81.6274, 0)
	spawnMobile("dantooine", "commoner", 60, -4221.08, 3, -2370.49, 124.39, 0)
	spawnMobile("dantooine", "commoner_naboo", 60, -4182.56, 3, -2422.72, 157.31, 0)
	spawnMobile("dantooine", "commoner", 60, -4186.81, 3, -2388.66, 141.892, 0)
	spawnMobile("dantooine", "juntah_herm", 60, -4222, 3, -2386.5, 300, 0)
	spawnMobile("dantooine", "noble", 60, -4261, 3, -2384, 200, 0)
	spawnMobile("dantooine", "imperial_recruiter", 0, -4197, 3, -2411, 270, 0)

	pNpc = spawnMobile("dantooine", "noble", 60,-4223.6,3,-2409.3,345,0)
	self:setMoodString(pNpc, "npc_use_terminal_high")
	pNpc = spawnMobile("dantooine", "insurgent",60,0,0,-5.7,0,1365878)
	self:setMoodString(pNpc, "npc_imperial")
end
