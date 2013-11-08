mountain_vesp_medium = Creature:new {
	objectName = "@monster_name:vesp_mountain_medium",
	socialGroup = "vesp",
	pvpFaction = "",
	faction = "",
	level = 24,
	chanceHit = 0.350000,
	damageMin = 230,
	damageMax = 240,
	baseXp = 2443,
	baseHAM = 7550,
	baseHAMmax = 7550,
	armor = 0,
	resists = {35,0,0,-1,-1,-1,-1,-1,-1},
	meatType = "meat_reptilian",
	meatAmount = 11,
	hideType = "hide_leathery",
	hideAmount = 5,
	boneType = "",
	boneAmount = 0,
	milk = 0,
	tamingChance = 0.250000,
	ferocity = 0,
	pvpBitmask = AGGRESSIVE + ATTACKABLE + ENEMY,
	creatureBitmask = PACK,
	diet = CARNIVORE,

	templates = {"object/mobile/vesp_hue.iff"},
	controlDeviceTemplate = "object/intangible/pet/vesp_hue.iff",
	lootGroups = {},
	weapons = {"creature_spit_small_yellow"},
	attacks = {
		{"blindattack","blindChance=50"},
		{"dizzyattack","dizzyChance=50"}
	}
}

CreatureTemplates:addCreatureTemplate(mountain_vesp_medium, "mountain_vesp_medium")
