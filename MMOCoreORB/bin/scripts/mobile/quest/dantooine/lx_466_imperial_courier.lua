lx_466_imperial_courier = Creature:new {
	objectName = "",
	customName = "(an Imperial Courier)",		
	socialGroup = "imperial",
	faction = "imperial",
	level = 30,
	chanceHit = 0.390000,
	damageMin = 290,
	damageMax = 300,
	baseXp = 2914,
	baseHAM = 8400,
	baseHAMmax = 10200,
	armor = 0,
	resists = {0,45,0,-1,40,-1,40,-1,-1},
	meatType = "",
	meatAmount = 0,
	hideType = "",
	hideAmount = 0,
	boneType = "",
	boneAmount = 0,
	milk = 0,
	tamingChance = 0.,
	ferocity = 0,
	pvpBitmask = NONE,
	creatureBitmask = NONE,
	optionsBitmask = AIENABLED + CONVERSABLE,	
	diet = HERBIVORE,

	templates = {
		"object/mobile/dressed_imperial_officer_f.iff",
		"object/mobile/dressed_imperial_officer_m.iff",
		"object/mobile/dressed_imperial_officer_m_2.iff",
		"object/mobile/dressed_imperial_officer_m_3.iff",
		"object/mobile/dressed_imperial_officer_m_4.iff",
		"object/mobile/dressed_imperial_officer_m_5.iff",
		"object/mobile/dressed_imperial_officer_m_6.iff"},
	lootGroups = {},
	weapons = {"imperial_weapons_medium"},
	conversationTemplate = "lx_466_mission_target_convotemplate",	
	attacks = {}

}

CreatureTemplates:addCreatureTemplate(lx_466_imperial_courier, "lx_466_imperial_courier")
