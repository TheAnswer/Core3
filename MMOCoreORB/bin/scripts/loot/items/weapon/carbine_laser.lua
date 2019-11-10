carbine_laser = {
	minimumLevel = 0,
	maximumLevel = -1,
	customObjectName = "",
	directObjectTemplate = "object/weapon/ranged/carbine/carbine_laser.iff",
	craftingValues = {
		{"hitpoints",750,750,0},
		{"mindamage",7,13,0},
		{"maxdamage",130,240,0},
		{"attackspeed",6,4.1,1},
		{"woundchance",8,16,1},
		{"zerorangemod",-10,-10,0},
		{"midrangemod",3,3,0},
		{"maxrangemod",-40,-40,0},
		{"midrange",50,50,0},
		{"maxrange",64,64,0},
		{"attackhealthcost",33,18,0},
		{"attackactioncost",52,28,0},
		{"attackmindcost",26,14,0},
	},
	customizationStringNames = {},
	customizationValues = {},

	-- randomDotChance: The chance of this weapon object dropping with a random dot on it. Higher number means less chance. Set to 0 to always have a random dot.
	randomDotChance = 625,
	junkDealerTypeNeeded = JUNKARMS,
	junkMinValue = 25,
	junkMaxValue = 45

}

addLootItemTemplate("carbine_laser", carbine_laser)
