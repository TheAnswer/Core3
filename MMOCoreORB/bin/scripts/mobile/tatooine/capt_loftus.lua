capt_loftus = Creature:new {
	objectName = "",
	customName = "Captain Hantor Loftus",
	socialGroup = "rebel",
	faction = "rebel",
	level = 10,
	chanceHit = 0.28,
	damageMin = 120,
	damageMax = 130,
	baseXp = 356,
	baseHAM = 675,
	baseHAMmax = 825,
	armor = 0,
	resists = {0,0,0,0,10,10,0,-1,-1},
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

	templates = {"object/mobile/dressed_tatooine_lt_hantor_loftus.iff"},
	lootGroups = {},
	weapons = {},
	conversationTemplate = "",
	attacks = {
	}
}

CreatureTemplates:addCreatureTemplate(capt_loftus, "capt_loftus")
