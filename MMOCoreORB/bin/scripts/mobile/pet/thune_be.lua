thune_be = Creature:new {
	objectName = "@mob/creature_names:bio_engineered_thune",
	socialGroup = "thune",
	faction = "",
	level = 15,
	chanceHit = 0.2,
	damageMin = 30,
	damageMax = 40,
	baseXp = 40,
	baseHAM = 45,
	baseHAMmax = 55,
	armor = 0,
	resists = {0,0,0,0,0,0,0,0,-1},
	meatType = "",
	meatAmount = 0,
	hideType = "",
	hideAmount = 0,
	boneType = "",
	boneAmount = 0,
	milk = 0,
	tamingChance = 0,
	ferocity = 0,
	pvpBitmask = ATTACKABLE,
	creatureBitmask = PACK + HERD,
	optionsBitmask = AIENABLED,
	diet = CARNIVORE,

	templates = {"object/mobile/thune_hue.iff"},
	lootGroups = {},
	weapons = {},
	conversationTemplate = "",
	attacks = {
	}
}

CreatureTemplates:addCreatureTemplate(thune_be, "thune_be")