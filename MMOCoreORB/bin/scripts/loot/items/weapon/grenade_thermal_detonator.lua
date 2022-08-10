grenade_thermal_detonator = {
	minimumLevel = 0,
	maximumLevel = -1,
	customObjectName = "",
	directObjectTemplate = "object/weapon/ranged/grenade/grenade_thermal_detonator.iff",
	craftingValues = {
		{"mindamage",570,870,0},
		{"maxdamage",1334,1690,0},
		{"attackspeed",5.4,2.4,1},
		{"woundchance",7,13,1},
		{"hitpoints",1000,1000,0},
		{"zerorange",0,0,0},
		{"zerorangemod",-10,-10,0},
		{"midrange",20,20,0},
		{"midrangemod",10,10,0},
		{"maxrange",32,32,0},
		{"maxrangemod",-30,-30,0},
		{"attackhealthcost",98,53,0},
		{"attackactioncost",98,53,0},
		{"attackmindcost",46,25,0},
	},

	-- randomDotChance: The chance of this weapon object dropping with a random dot on it. Higher number means less chance. Set to 0 to always have a random dot.
	randomDotChance = 750,
	junkDealerTypeNeeded = JUNKARMS,
	junkMinValue = 30,
	junkMaxValue = 55
}

addLootItemTemplate("grenade_thermal_detonator", grenade_thermal_detonator)
