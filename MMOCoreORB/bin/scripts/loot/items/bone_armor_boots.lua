bone_armor_boots = {
	minimumLevel = 0,
	maximumLevel = -1,
	customObjectName = "Bone Armor Boots",
	directObjectTemplate = "object/tangible/wearables/armor/bone/armor_bone_s01_boots.iff",
	craftingValues = {
		{"armor_rating",1,1,0},
		{"energyeffectiveness",10,35,10},
		{"armor_effectiveness",4,19,10},
		{"armor_integrity",7500, 12500,0},
		{"armor_health_encumbrance",6,4,0},
		{"armor_action_encumbrance",16,8,0},
		{"armor_mind_encumbrance",4,2,0},
	},
	skillMods = {

	},
	customizationStringNames = {},
	customizationValues = {}
}

addLootItemTemplate("bone_armor_boots", bone_armor_boots)
