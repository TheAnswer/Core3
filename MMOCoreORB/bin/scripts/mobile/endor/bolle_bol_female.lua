bolle_bol_female = Creature:new {
	objectName = "@mob/creature_names:bolle_bol_female",
	socialGroup = "bol",
	faction = "",
	level = 32,
	chanceHit = 0.39,
	damageMin = 290,
	damageMax = 300,
	baseXp = 3188,
	baseHAM = 8800,
	baseHAMmax = 10800,
	armor = 0,
	resists = {20,20,20,20,150,20,150,-1,-1},
	meatType = "meat_herbivore",
	meatAmount = 450,
	hideType = "hide_leathery",
	hideAmount = 300,
	boneType = "bone_mammal",
	boneAmount = 180,
	milkType = "milk_wild",
	milk = 200,
	tamingChance = 0,
	ferocity = 0,
	pvpBitmask = ATTACKABLE,
	creatureBitmask = PACK + HERD,
	optionsBitmask = AIENABLED,
	diet = HERBIVORE,

	templates = {"object/mobile/bolle_bol_hue.iff"},
	scale = 1.15,
	lootGroups = {},
	weapons = {},
	conversationTemplate = "",
	attacks = {
		{"posturedownattack",""},
		{"dizzyattack",""}
	}
}

CreatureTemplates:addCreatureTemplate(bolle_bol_female, "bolle_bol_female")
