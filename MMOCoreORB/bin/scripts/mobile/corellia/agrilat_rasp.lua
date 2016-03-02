agrilat_rasp = Creature:new {
	objectName = "@mob/creature_names:agrilat_plumed_rasp",
	socialGroup = "rasp",
	faction = "",
	level = 8,
	chanceHit = 0.27,
	damageMin = 90,
	damageMax = 110,
	baseXp = 187,
	baseHAM = 180,
	baseHAMmax = 220,
	armor = 0,
	resists = {0,10,0,0,0,110,0,-1,-1},
	meatType = "meat_avian",
	meatAmount = 3,
	hideType = "",
	hideAmount = 0,
	boneType = "bone_avian",
	boneAmount = 2,
	milk = 0,
	tamingChance = 0.25,
	ferocity = 0,
	pvpBitmask = ATTACKABLE,
	creatureBitmask = NONE,
	optionsBitmask = AIENABLED,
	diet = HERBIVORE,

	templates = {"object/mobile/flite_rasp_hue.iff"},
	controlDeviceTemplate = "object/intangible/pet/horned_rasp_hue.iff",
	scale = 1.05,
	lootGroups = {},
	weapons = {},
	conversationTemplate = "",
	attacks = {
	}
}

CreatureTemplates:addCreatureTemplate(agrilat_rasp, "agrilat_rasp")
