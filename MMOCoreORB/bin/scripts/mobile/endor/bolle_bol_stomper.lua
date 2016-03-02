bolle_bol_stomper = Creature:new {
	objectName = "@mob/creature_names:bolle_bol_stomper",
	socialGroup = "bol",
	faction = "",
	level = 23,
	chanceHit = 0.34,
	damageMin = 220,
	damageMax = 230,
	baseXp = 2219,
	baseHAM = 5000,
	baseHAMmax = 6100,
	armor = 0,
	resists = {125,10,-1,-1,150,-1,150,-1,-1},
	meatType = "meat_herbivore",
	meatAmount = 450,
	hideType = "hide_leathery",
	hideAmount = 300,
	boneType = "bone_mammal",
	boneAmount = 180,
	milk = 0,
	tamingChance = 0,
	ferocity = 0,
	pvpBitmask = ATTACKABLE,
	creatureBitmask = PACK + HERD,
	optionsBitmask = AIENABLED,
	diet = HERBIVORE,

	templates = {"object/mobile/bolle_bol_hue.iff"},
	scale = 0.95,
	lootGroups = {},
	weapons = {},
	conversationTemplate = "",
	attacks = {
		{"dizzyattack",""},
		{"posturedownattack",""}
	}
}

CreatureTemplates:addCreatureTemplate(bolle_bol_stomper, "bolle_bol_stomper")
