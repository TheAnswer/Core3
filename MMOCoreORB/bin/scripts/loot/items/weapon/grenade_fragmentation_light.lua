grenade_fragmentation_light = {
	minimumLevel = 0,
	maximumLevel = -1,
	customObjectName = "",
	directObjectTemplate = "object/weapon/ranged/grenade/grenade_fragmentation_light.iff",
	craftingValues = {
		{"mindamage",130,170,0},
		{"maxdamage",220,760,0},
		{"attackspeed",5.5,2.5,1},
		{"woundchance",7,13,1},
		{"hitpoints",1000,1000,0},
		{"zerorange",0,0,0},
		{"zerorangemod",-10,-10,0},
		{"midrange",20,20,0},
		{"midrangemod",10,10,0},
		{"maxrange",32,32,0},
		{"maxrangemod",-30,-30,0},
		{"attackhealthcost",75,35,0},
		{"attackactioncost",65,25,0},
		{"attackmindcost",13,7,0},
	},

	-- randomDotChance: The chance of this weapon object dropping with a random dot on it. Higher number means less chance. Set to 0 to always have a random dot.
	randomDotChance = 750,
	junkDealerTypeNeeded = JUNKARMS,
	junkMinValue = 20,
	junkMaxValue = 40
}

addLootItemTemplate("grenade_fragmentation_light", grenade_fragmentation_light)
