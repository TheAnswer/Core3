quenker = Creature:new {
	objectName = "@mob/creature_names:quenker",
	socialGroup = "quenker",
	faction = "",
	level = 23,
	chanceHit = 0.35,
	damageMin = 210,
	damageMax = 220,
	baseXp = 2219,
	baseHAM = 4100,
	baseHAMmax = 5000,
	armor = 0,
	resists = {125,125,10,10,10,10,10,-1,-1},
	meatType = "meat_wild",
	meatAmount = 35,
	hideType = "hide_scaley",
	hideAmount = 35,
	boneType = "",
	boneAmount = 0,
	milk = 0,
	tamingChance = 0.25,
	ferocity = 0,
	pvpBitmask = AGGRESSIVE + ATTACKABLE + ENEMY,
	creatureBitmask = PACK,
	optionsBitmask = AIENABLED,
	diet = CARNIVORE,

	templates = {"object/mobile/quenker.iff"},
	controlDeviceTemplate = "object/intangible/pet/quenker_hue.iff",
	lootGroups = {},
	weapons = {},
	conversationTemplate = "",
	attacks = {
		{"intimidationattack",""},
		{"knockdownattack",""}
	}
}

CreatureTemplates:addCreatureTemplate(quenker, "quenker")
