blood_fanged_gackle_bat = Creature:new {
	objectName = "@mob/creature_names:bloodfanged_gackle_bat",
	socialGroup = "gacklebat",
	faction = "",
	level = 14,
	chanceHit = 0.3,
	damageMin = 160,
	damageMax = 170,
	baseXp = 831,
	baseHAM = 1500,
	baseHAMmax = 1900,
	armor = 0,
	resists = {0,0,115,0,0,0,0,-1,-1},
	meatType = "meat_avian",
	meatAmount = 9,
	hideType = "hide_leathery",
	hideAmount = 9,
	boneType = "bone_avian",
	boneAmount = 9,
	milk = 0,
	tamingChance = 0.25,
	ferocity = 0,
	pvpBitmask = AGGRESSIVE + ATTACKABLE + ENEMY,
	creatureBitmask = PACK + HERD + KILLER,
	optionsBitmask = AIENABLED,
	diet = CARNIVORE,

	templates = {"object/mobile/bloodfanged_gackle_bat.iff"},
	controlDeviceTemplate = "object/intangible/pet/gackle_bat_hue.iff",
	scale = 1.15,
	lootGroups = {},
	weapons = {},
	conversationTemplate = "",
	attacks = {
		{"blindattack",""},
		{"intimidationattack",""}
	}
}

CreatureTemplates:addCreatureTemplate(blood_fanged_gackle_bat, "blood_fanged_gackle_bat")
