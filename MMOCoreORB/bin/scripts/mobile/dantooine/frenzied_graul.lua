frenzied_graul = Creature:new {
	objectName = "@mob/creature_names:frenzied_graul",
	socialGroup = "graul",
	faction = "",
	level = 56,
	chanceHit = 0.5,
	damageMin = 445,
	damageMax = 600,
	baseXp = 5464,
	baseHAM = 9500,
	baseHAMmax = 11700,
	armor = 1,
	resists = {165,135,190,190,190,-1,5,5,-1},
	meatType = "meat_carnivore",
	meatAmount = 1000,
	hideType = "hide_leathery",
	hideAmount = 950,
	boneType = "bone_mammal",
	boneAmount = 855,
	milk = 0,
	tamingChance = 0.01,
	ferocity = 15,
	pvpBitmask = AGGRESSIVE + ATTACKABLE + ENEMY,
	creatureBitmask = PACK + KILLER,
	optionsBitmask = AIENABLED,
	diet = CARNIVORE,

	templates = {"object/mobile/frenzied_graul.iff"},
	controlDeviceTemplate = "object/intangible/pet/graul_hue.iff",
	scale = 1.2,
	lootGroups = {},
	weapons = {},
	conversationTemplate = "",
	attacks = {
		{"creatureareableeding",""},
		{"stunattack",""}
	}
}

CreatureTemplates:addCreatureTemplate(frenzied_graul, "frenzied_graul")
