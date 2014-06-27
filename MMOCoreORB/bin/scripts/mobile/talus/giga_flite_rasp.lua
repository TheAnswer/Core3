giga_flite_rasp = Creature:new {
	objectName = "@mob/creature_names:giga_flite_rasp",
	socialGroup = "rasp",
	pvpFaction = "",
	faction = "",
	level = 11,
	chanceHit = 0.29,
	damageMin = 130,
	damageMax = 140,
	baseXp = 430,
	baseHAM = 810,
	baseHAMmax = 990,
	armor = 0,
	resists = {0,0,0,0,0,15,0,-1,-1},
	meatType = "meat_avian",
	meatAmount = 20,
	hideType = "",
	hideAmount = 0,
	boneType = "bone_avian",
	boneAmount = 6,
	milk = 0,
	tamingChance = 0,
	ferocity = 0,
	pvpBitmask = ATTACKABLE,
	creatureBitmask = PACK + HERD,
	optionsBitmask = 128,
	diet = CARNIVORE,

	templates = {"object/mobile/flite_rasp_hue.iff"},
	scale = 1.2,
	lootGroups = {},
	weapons = {},
	conversationTemplate = "",
	attacks = {
		{"posturedownattack","postureDownChance=50"}
	}
}

CreatureTemplates:addCreatureTemplate(giga_flite_rasp, "giga_flite_rasp")
