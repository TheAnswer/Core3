gorge_vesp = Creature:new {
	objectName = "@mob/creature_names:gorge_vesp",
	socialGroup = "vesp",
	faction = "",
	level = 27,
	chanceHit = 0.36,
	damageMin = 250,
	damageMax = 260,
	baseXp = 2730,
	baseHAM = 8500,
	baseHAMmax = 10300,
	armor = 0,
	resists = {125,125,15,-1,-1,-1,-1,-1,-1},
	meatType = "meat_reptilian",
	meatAmount = 11,
	hideType = "hide_leathery",
	hideAmount = 5,
	boneType = "",
	boneAmount = 0,
	milk = 0,
	tamingChance = 0.25,
	ferocity = 0,
	pvpBitmask = AGGRESSIVE + ATTACKABLE + ENEMY,
	creatureBitmask = PACK,
	optionsBitmask = AIENABLED,
	diet = CARNIVORE,

	templates = {"object/mobile/vesp_hue.iff"},
	controlDeviceTemplate = "object/intangible/pet/vesp_hue.iff",
	scale = 1.1,
	lootGroups = {},
	weapons = {"creature_spit_small_yellow"},
	conversationTemplate = "",
	attacks = {
		{"blindattack",""},
		{"stunattack",""}
	}
}

CreatureTemplates:addCreatureTemplate(gorge_vesp, "gorge_vesp")
