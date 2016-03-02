tech_mor = Creature:new {
	objectName = "@npc_name:bith_base_male",
	customName = "Tech Mo'r",
	socialGroup = "townsperson",
	faction = "townsperson",
	level = 10,
	chanceHit = 0.28,
	damageMin = 90,
	damageMax = 110,
	baseXp = 356,
	baseHAM = 810,
	baseHAMmax = 990,
	armor = 0,
	resists = {0,0,0,0,0,0,0,-1,-1},
	meatType = "",
	meatAmount = 0,
	hideType = "",
	hideAmount = 0,
	boneType = "",
	boneAmount = 0,
	milk = 0,
	tamingChance = 0,
	ferocity = 0,
	pvpBitmask = NONE,
	creatureBitmask = PACK,
	optionsBitmask = AIENABLED,
	diet = HERBIVORE,

	templates = {"object/mobile/tatooine_npc/figrin_dan.iff"},
	lootGroups = {},
	weapons = {},
	conversationTemplate = "",
	outfit = "figrin_dan_band_outfit",
	attacks = {
	}
}

CreatureTemplates:addCreatureTemplate(tech_mor, "tech_mor")
