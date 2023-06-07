--Automatically generated by SWGEmu Spawn Tool v0.12 loot editor.

baton_stun = {
	minimumLevel = 0,
	maximumLevel = -1,
	customObjectName = "",
	directObjectTemplate = "object/weapon/melee/baton/baton_stun.iff",
	craftingValues = {
		{"mindamage",28,52,0},
		{"maxdamage",53,98,0},
		{"attackspeed",4.2,2.9,1},
		{"woundchance",1,3,1},
		{"hitpoints",750,1500,0},
		{"zerorange",0,0,0},
		{"zerorangemod",0,0,0},
		{"midrange",2,2,0},
		{"midrangemod",0,0,0},
		{"maxrange",4,4,0},
		{"maxrangemod",0,0,0},
		{"attackhealthcost",10,4,0},
		{"attackactioncost",45,24,0},
		{"attackmindcost",39,19,0},
	},
	customizationStringNames = {},
	customizationValues = {},

	-- randomDotChance: The chance of this weapon object dropping with a random dot on it. Higher number means less chance. Set to 0 to always have a random dot.
	randomDotChance = 625,
	junkDealerTypeNeeded = JUNKARMS,
	junkMinValue = 30,
	junkMaxValue = 55
}

addLootItemTemplate("baton_stun", baton_stun)