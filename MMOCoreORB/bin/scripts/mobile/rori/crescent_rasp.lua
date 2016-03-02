crescent_rasp = Creature:new {
	objectName = "@mob/creature_names:crescent_rasp",
	socialGroup = "rasp",
	faction = "",
	level = 7,
	chanceHit = 0.26,
	damageMin = 80,
	damageMax = 90,
	baseXp = 147,
	baseHAM = 135,
	baseHAMmax = 165,
	armor = 0,
	resists = {0,0,0,0,0,115,0,-1,-1},
	meatType = "meat_avian",
	meatAmount = 10,
	hideType = "",
	hideAmount = 0,
	boneType = "bone_avian",
	boneAmount = 2,
	milk = 0,
	tamingChance = 0,
	ferocity = 0,
	pvpBitmask = ATTACKABLE,
	creatureBitmask = HERD,
	optionsBitmask = AIENABLED,
	diet = HERBIVORE,

	templates = {"object/mobile/crowned_rasp_hue.iff"},
	lootGroups = {},
	weapons = {},
	conversationTemplate = "",
	attacks = {
	}
}

CreatureTemplates:addCreatureTemplate(crescent_rasp, "crescent_rasp")
