--This is to be used for static spawns that are NOT part of caves, cities, dungeons, poi's, or other large screenplays.
NabooStaticSpawnsSouthScreenPlay = ScreenPlay:new {
	numberOfActs = 1,

	screenplayName = "NabooStaticSpawnsSouthScreenPlay",

}
registerScreenPlay("NabooStaticSpawnsSouthScreenPlay", true)

function NabooStaticSpawnsSouthScreenPlay:start()
	if (isZoneEnabled("naboo")) then
		self:spawnMobiles()
	end
end

function NabooStaticSpawnsSouthScreenPlay:spawnMobiles()

		--Fishing Pond (-6310,-5075)
	local pNpc = spawnMobile("naboo", "commoner", 180, -6327.0, 3.3, -5082.4, 39.0, 0)
	self:setMoodString(pNpc, "fishing")
	spawnMobile("naboo", "commoner", 180, -6318.0, 4.7, -5077.5, -34.0, 0)
	spawnMobile("naboo", "commoner", 180, -6306.9, 5.5, -5082.5, -158.0, 0)

		--Gungan Shield Generator (-5184,-6366)
	pNpc = spawnMobile("naboo", "gungan_boss", 300, -5170.7, 5.2, -6382.9, -44, 0)
	self:setMoodString(pNpc, "threaten")
	pNpc = spawnMobile("naboo", "gungan_reject", 300, -5177.3, 2.4, -6373.0, -50, 0)
	self:setMoodString(pNpc, "angry")

		--Gungan Statue Head (7160,-5764)
	spawnMobile("naboo", "gungan_hunter", 300, 7153.6, 27.8, -5771.8, -78.0, 0)
	spawnMobile("naboo", "gungan_kaadu_rider", 300, 7152.7, 27.3, -5777.3, -62.0, 0)
	spawnMobile("naboo", "gungan_kaadu", 300, 7161.2, 25.3, -5783.4, -62.0, 0)

		--Gazebo (1010,-3466)
	spawnMobile("naboo", "commoner_old", 180, 1012.2, 185.3, -3468.7, 175.0, 0)
	spawnMobile("naboo", "commoner_naboo", 180, 1014.5, 185.3, -3461.2, -145.0, 0)

		--Statues + Fire (-1637,-4032)
	spawnMobile("naboo", "lesser_prophet_of_the_dark_side", 300, -1638.6, 14.0, -4033.7, 90, 0)

		--Small Imperial Outpost (-4767,-3975)
	spawnMobile("naboo", "imperial_warrant_officer_i", 300, -4773.3, 333.3, -3985.2, -90.0, 0)
	spawnMobile("naboo", "storm_commando", 300, -4773.3, 333.3, -3979.4, -90.0, 0)
	spawnMobile("naboo", "storm_commando", 300, -4767.3, 333.3, -3976.5, 0.0, 0)
	spawnMobile("naboo", "shadowy_figure", 300, 4.3, 0.5, 3.0, 180, 9525590)

		--Arch & Debris (-6592,-2682)
	spawnMobile("naboo", "gungan_hermit", 300, -6597.2, 152.4, -2679.5, 78.0, 0)

		--Arch & statue (-4421,-2226)
	spawnMobile("naboo", "gungan_hermit", 300, -4422.8, 267.4, -2216.8, -165.0, 0)

		--Crashed Escape Pod (-2433,-2492)
	spawnMobile("naboo", "r3", 300, -2433.6, 269.7, -2492.3, 65.0, 0)
	spawnMobile("naboo", "r5", 300, -2433.9, 269.3, -2489.1, 5.0, 0)

		--Broken Wall (3485,-2882)
	pNpc = spawnMobile("naboo", "gungan_reject", 300, 3487.9, 112.5, -2880.0, -13, 0)
	self:setMoodString(pNpc, "sad")

		--Statue Head (4662,-2489)
	pNpc = spawnMobile("naboo", "gungan_leader", 300, 4663.6, 224.6, -2491.4, -40, 0)
	self:setMoodString(pNpc, "explain")

		--Pillars (7263,-2973)
	spawnMobile("naboo", "gungan_hermit", 300, 7257.0, 111.1, -2970.9, 35.0, 0)
	spawnMobile("naboo", "gungan_hermit", 300, 7260.2, 110.8, -2967.6, -37.5, 0)

		--Ancient Stone Mast (1216,-1248)
	spawnMobile("naboo", "gungan_kaadu", 300, 1216.9, 225.8, -1252.1, -91, 0)
	spawnMobile("naboo", "giant_fanned_rawl", 900, 1219.6, 221.3, -1239.0, 169, 0)

		--Gungan Ruins (0,-1866)
	spawnMobile("naboo", "vine_rawl", 300, 7.6, 441.2, -1858.6, -140, 0)

		--Gate & Gazebo (-258,-46)
	spawnMobile("naboo", "commoner_naboo", 180, -258.6, 2.9, -74.4, -25.0, 0)

		--Statue & Ruins (-190,-901)
	spawnMobile("naboo", "gungan_hermit", 300, -195.5, 111.7, -898.2, -80.0, 0)
	spawnMobile("naboo", "gungan_hunter", 300, -171.8, 110.7, -902.4, 90.0, 0)

		--Fire & Pillars (-1775,-487)
	spawnMobile("naboo", "naboo_pirate", 300, -1784.0, 33.3, -490.8, -6.0, 0)
	spawnMobile("naboo", "naboo_pirate", 300, -1781.9, 33.2, -487.9, 92.0, 0)
	spawnMobile("naboo", "naboo_pirate", 300, -1769.5, 33.2, -495.6, -45.0, 0)

		--Ruined Walls (-3586,-880)
	spawnMobile("naboo", "mutant_hermit_spider", 900, getRandomNumber(5) + -3586, 92.6, getRandomNumber(5) + -872, getRandomNumber(360), 0)
	spawnMobile("naboo", "mutant_hermit_spider", 900, getRandomNumber(5) + -3586, 92.6, getRandomNumber(5) + -872, getRandomNumber(360), 0)
	spawnMobile("naboo", "mutant_hermit_spider", 900, getRandomNumber(5) + -3586, 92.6, getRandomNumber(5) + -872, getRandomNumber(360), 0)
	spawnMobile("naboo", "mutant_hermit_spider", 900, getRandomNumber(5) + -3586, 92.6, getRandomNumber(5) + -872, getRandomNumber(360), 0)

		--Ruined Gate (-7324, -4149)
	spawnMobile("naboo", "mutant_hermit_spider", 900, getRandomNumber(5) + -7330, 10.8, getRandomNumber(5) + -4164, getRandomNumber(360), 0)
	spawnMobile("naboo", "mutant_hermit_spider", 900, getRandomNumber(5) + -7330, 10.8, getRandomNumber(5) + -4164, getRandomNumber(360), 0)
	spawnMobile("naboo", "mutant_hermit_spider", 900, getRandomNumber(5) + -7330, 10.8, getRandomNumber(5) + -4164, getRandomNumber(360), 0)

end
