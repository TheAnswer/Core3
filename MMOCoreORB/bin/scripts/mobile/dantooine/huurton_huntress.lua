huurton_huntress = Creature:new {
	objectName = "@mob/creature_names:huurton_huntress",
	socialGroup = "huurton",
	pvpFaction = "",
	faction = "",
	level = 28,
	chanceHit = 0.37,
	damageMin = 270,
	damageMax = 280,
	baseXp = 2914,
	baseHAM = 8200,
	baseHAMmax = 10000,
	armor = 0,
	resists = {15,15,15,40,100,-1,-1,-1,-1},
	meatType = "meat_wild",
	meatAmount = 15,
	hideType = "hide_wooly",
	hideAmount = 15,
	boneType = "bone_mammal",
	boneAmount = 15,
	milk = 0,
	tamingChance = 0.25,
	ferocity = 0,
	pvpBitmask = AGGRESSIVE + ATTACKABLE + ENEMY,
	creatureBitmask = PACK + KILLER + STALKER,
	optionsBitmask = 128,
	diet = CARNIVORE,

	templates = {"object/mobile/huurton_hue.iff"},
	controlDeviceTemplate = "object/intangible/pet/huurton_hue.iff",
	lootGroups = {},
	weapons = {},
	conversationTemplate = "",
	attacks = {
		{"knockdownattack","knockdownChance=50"},
		{"stunattack","stunChance=50"}
	}
}

CreatureTemplates:addCreatureTemplate(huurton_huntress, "huurton_huntress")
