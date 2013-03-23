composit_armor_helmet = {
	minimumLevel = 0,
	maximumLevel = -1,
	customObjectName = "a Composit Armor Helmet",
	directObjectTemplate = "object/tangible/wearables/armor/composite/armor_composite_helmet.iff",
	craftingValues = {
		{"armor_rating",1,1,0},
		{"electricityeffectiveness",10,55,10},
		{"armor_effectiveness",8,39,10},
		{"armor_integrity",30000, 65000,0},
		{"armor_health_encumbrance",25,15,0},
		{"armor_action_encumbrance",22,13,0},
		{"armor_mind_encumbrance",300,140,0},
	},
	skillMods = {

	},
	customizationStringNames = {},
	customizationValues = {}
}

addLootItemTemplate("composit_armor_helmet", composit_armor_helmet)
