veermok_be = Creature:new {
	objectName = "@mob/creature_names:bio_engineered_veermok",
	socialGroup = "veermok",
	faction = "",
	level = 10,
	chanceHit = 0.2,
	damageMin = 30,
	damageMax = 40,
	baseXp = 40,
	baseHAM = 45,
	baseHAMmax = 55,
	armor = 0,
	resists = {0,0,0,0,0,0,0,0,0},
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
	creatureBitmask = PACK,
	optionsBitmask = AIENABLED,
	diet = CARNIVORE,

	templates = {"object/mobile/veermok_hue.iff"},
	lootGroups = {},
	weapons = {},
	conversationTemplate = "",
	attacks = {
	}
}

CreatureTemplates:addCreatureTemplate(veermok_be, "veermok_be")
