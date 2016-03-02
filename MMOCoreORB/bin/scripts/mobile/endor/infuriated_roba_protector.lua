infuriated_roba_protector = Creature:new {
	objectName = "@mob/creature_names:roba_infuriated_protectorate",
	socialGroup = "roba",
	faction = "",
	level = 57,
	chanceHit = 0.55,
	damageMin = 420,
	damageMax = 550,
	baseXp = 5555,
	baseHAM = 11000,
	baseHAMmax = 14000,
	armor = 0,
	resists = {135,135,15,120,15,120,15,-1,-1},
	meatType = "meat_carnivore",
	meatAmount = 120,
	hideType = "hide_leathery",
	hideAmount = 95,
	boneType = "bone_mammal",
	boneAmount = 70,
	milk = 0,
	tamingChance = 0.05,
	ferocity = 0,
	pvpBitmask = AGGRESSIVE + ATTACKABLE + ENEMY,
	creatureBitmask = PACK + KILLER,
	optionsBitmask = AIENABLED,
	diet = CARNIVORE,

	templates = {"object/mobile/roba_hue.iff"},
	controlDeviceTemplate = "object/intangible/pet/roba_hue.iff",
	scale = 1.3,
	lootGroups = {},
	weapons = {},
	conversationTemplate = "",
	attacks = {
		{"posturedownattack",""},
		{"stunattack",""}
	}
}

CreatureTemplates:addCreatureTemplate(infuriated_roba_protector, "infuriated_roba_protector")
