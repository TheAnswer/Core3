NabooPirateBunkerScreenPlay = ScreenPlay:new {
	numberOfActs = 1,
	--location -1482 -1729
	screenplayName = "NabooPirateBunkerScreenPlay",

 	lootContainers = {
		5535582,
		5535589,
		5535590,
		5535591,
		5535606,
		5535607,
		5535608,
		5535609,
		5535610,
		5535611
	},

	lootLevel = 16,

	lootGroups = {
		{
			groups = {
				{group = "junk", chance = 8240000},
				{group = "rifles", chance = 500000},
				{group = "pistols", chance = 500000},
				{group = "clothing_attachments", chance = 300000},
				{group = "armor_attachments", chance = 300000}
			},
			lootChance = 8000000
		}
	},

	lootContainerRespawn = 1800
}

registerScreenPlay("NabooPirateBunkerScreenPlay", true)

function NabooPirateBunkerScreenPlay:start()
	if (isZoneEnabled("naboo")) then
		self:spawnMobiles()
		self:initializeLootContainers()
	end
end

function NabooPirateBunkerScreenPlay:spawnMobiles()
	--add mobiles here

	spawnMobile("naboo", "naboo_pirate", math.random(120) + 180, math.random(25) + -1540.0, 278, math.random(30) + -1759.7, math.random(360) + 0, 0)
	spawnMobile("naboo", "naboo_pirate", math.random(120) + 180, math.random(25) + -1540.0, 278, math.random(30) + -1759.7, math.random(360) + 0, 0)
	spawnMobile("naboo", "naboo_pirate", math.random(120) + 180, math.random(25) + -1540.0, 278, math.random(30) + -1759.7, math.random(360) + 0, 0)
	spawnMobile("naboo", "naboo_pirate", math.random(120) + 180, math.random(25) + -1540.0, 278, math.random(30) + -1759.7, math.random(360) + 0, 0)
	spawnMobile("naboo", "naboo_pirate", math.random(120) + 180, math.random(25) + -1540.0, 278, math.random(30) + -1759.7, math.random(360) + 0, 0)
	spawnMobile("naboo", "naboo_pirate", math.random(120) + 180, math.random(25) + -1540.0, 278, math.random(30) + -1759.7, math.random(360) + 0, 0)
	spawnMobile("naboo", "naboo_pirate", math.random(120) + 180, math.random(25) + -1540.0, 278, math.random(30) + -1759.7, math.random(360) + 0, 0)
	spawnMobile("naboo", "naboo_pirate", math.random(120) + 180, math.random(25) + -1540.0, 278, math.random(30) + -1759.7, math.random(360) + 0, 0)

	spawnMobile("naboo", "naboo_pirate_cuttroat", math.random(120) + 180, -1540.0, 278, -1759.7, 90, 0)
	spawnMobile("naboo", "naboo_pirate_cuttroat", math.random(120) + 180, -1512.0, 278, -1726.8, -90, 0)
	spawnMobile("naboo", "naboo_pirate_crewman", math.random(120) + 180, -1501.0, 278, -1713.2, 180, 0)
	spawnMobile("naboo", "naboo_pirate_crewman", math.random(120) + 180, -1484.1, 278, -1712.3, 140, 0)
	spawnMobile("naboo", "naboo_pirate_crewman", math.random(120) + 180, -1485.0, 278, -1740.4, 0, 0)
	spawnMobile("naboo", "naboo_pirate_crewman", math.random(120) + 180, -1501.8, 278, -1743.8, 10, 0)

	spawnMobile("naboo", "naboo_pirate_cuttroat", 300, 0.0, 0.3, 3.5, 0, 5535565)
	spawnMobile("naboo", "naboo_pirate_cuttroat", 300, -3.7, 0.3, -0.8, 0, 5535565)
	spawnMobile("naboo", "naboo_pirate_cuttroat", 300, 3.5, 0.0, 1.0, 180, 5535573)

	spawnMobile("naboo", "naboo_pirate", 300, math.random(8) + -1.3, -12.0, math.random(8) + 26.1, math.random(360) + 0, 5535567)
	spawnMobile("naboo", "naboo_pirate_crewman", 300, math.random(8) + -1.3, -12.0, math.random(8) + 26.1, math.random(360) + 0, 5535567)
	spawnMobile("naboo", "naboo_pirate_crewman", 300, math.random(8) + -1.3, -12.0, math.random(8) + 26.1, math.random(360) + 0, 5535567)
	spawnMobile("naboo", "naboo_pirate_cuttroat", 300, math.random(8) + -1.3, -12.0, math.random(8) + 26.1, math.random(360) + 0, 5535567)

	spawnMobile("naboo", "naboo_pirate_lieutenant", 300, 32.1, -12.0, 30.3, -90, 5535568)

	spawnMobile("naboo", "naboo_pirate_lieutenant", 300, math.random(44) + 39.5, -16.0, math.random(24) + 58.4, math.random(360) + 0, 5535571)
	spawnMobile("naboo", "naboo_pirate_armsman", 300, math.random(44) + 39.5, -16.0, math.random(24) + 58.4, math.random(360) + 0, 5535571)
	spawnMobile("naboo", "naboo_pirate_armsman", 300, math.random(44) + 39.5, -16.0, math.random(24) + 58.4, math.random(360) + 0, 5535571)
	spawnMobile("naboo", "naboo_pirate_armsman", 300, math.random(44) + 39.5, -16.0, math.random(24) + 58.4, math.random(360) + 0, 5535571)
	spawnMobile("naboo", "naboo_pirate_crewman", 300, math.random(44) + 39.5, -16.0, math.random(24) + 58.4, math.random(360) + 0, 5535571)
	spawnMobile("naboo", "naboo_pirate_crewman", 300, math.random(44) + 39.5, -16.0, math.random(24) + 58.4, math.random(360) + 0, 5535571)
	spawnMobile("naboo", "naboo_pirate_crewman", 300, math.random(44) + 39.5, -16.0, math.random(24) + 58.4, math.random(360) + 0, 5535571)
	spawnMobile("naboo", "naboo_pirate_cuttroat", 300, math.random(44) + 39.5, -16.0, math.random(24) + 58.4, math.random(360) + 0, 5535571)
	spawnMobile("naboo", "naboo_pirate_cuttroat", 300, math.random(44) + 39.5, -16.0, math.random(24) + 58.4, math.random(360) + 0, 5535571)
	spawnMobile("naboo", "naboo_pirate_cuttroat", 300, math.random(44) + 39.5, -16.0, math.random(24) + 58.4, math.random(360) + 0, 5535571)
	spawnMobile("naboo", "naboo_pirate", 300, math.random(44) + 39.5, -16.0, math.random(24) + 58.4, math.random(360) + 0, 5535571)
	spawnMobile("naboo", "naboo_pirate", 300, math.random(44) + 39.5, -16.0, math.random(24) + 58.4, math.random(360) + 0, 5535571)
	spawnMobile("naboo", "naboo_pirate", 300, math.random(44) + 39.5, -16.0, math.random(24) + 58.4, math.random(360) + 0, 5535571)
	spawnMobile("naboo", "naboo_pirate", 300, math.random(44) + 39.5, -16.0, math.random(24) + 58.4, math.random(360) + 0, 5535571)
	spawnMobile("naboo", "naboo_pirate", 300, math.random(44) + 39.5, -16.0, math.random(24) + 58.4, math.random(360) + 0, 5535571)

	spawnMobile("naboo", "naboo_pirate", 300, math.random(37) + -15.5, -16.0, math.random(10) + 72.5, math.random(360) + 0, 5535570)
	spawnMobile("naboo", "naboo_pirate", 300, math.random(37) + -15.5, -16.0, math.random(10) + 72.5, math.random(360) + 0, 5535570)
	spawnMobile("naboo", "naboo_pirate", 300, math.random(37) + -15.5, -16.0, math.random(10) + 72.5, math.random(360) + 0, 5535570)
	spawnMobile("naboo", "naboo_pirate", 300, math.random(37) + -15.5, -16.0, math.random(10) + 72.5, math.random(360) + 0, 5535570)
	spawnMobile("naboo", "naboo_pirate", 300, math.random(37) + -15.5, -16.0, math.random(10) + 72.5, math.random(360) + 0, 5535570)

end
