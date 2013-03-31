fierce_huurton = Creature:new {
	objectName = "@mob/creature_names:warren_huurton_fierce",
	socialGroup = "warren_huurton",
	pvpFaction = "warren_huurton",
	faction = "warren_huurton",
	level = 22,
	chanceHit = 0.34,
	damageMin = 220,
	damageMax = 230,
	baseXp = 2006,
	baseHAM = 5400,
	baseHAMmax = 6600,
	armor = 0,
	resists = {20,20,0,0,-1,0,-1,-1,-1},
	meatType = "",
	meatAmount = 0,
	hideType = "hide_wooly",
	hideAmount = 65,
	boneType = "bone_mammal",
	boneAmount = 25,
	milk = 1,
	tamingChance = 0,
	ferocity = 0,
	pvpBitmask = AGGRESSIVE + ATTACKABLE + ENEMY,
	creatureBitmask = KILLER,
	optionsBitmask = 128,
	diet = CARNIVORE,

	templates = {"object/mobile/huurton_hue.iff"},
	lootGroups = {},
	weapons = {},
	conversationTemplate = "",
	attacks = {
		{"stunattack","stunChance=50"}
	}
}

CreatureTemplates:addCreatureTemplate(fierce_huurton, "fierce_huurton")