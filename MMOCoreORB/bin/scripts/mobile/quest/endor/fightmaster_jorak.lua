fightmaster_jorak = Creature:new {
	objectName = "@mob/creature_names:fightmaster_jorak",
	socialGroup = "endor_marauder",
	faction = "endor_marauder",
	level = 22,
	chanceHit = 0.33,
	damageMin = 190,
	damageMax = 200,
	baseXp = 1803,
	baseHAM = 5900,
	baseHAMmax = 7200,
	armor = 0,
	resists = {30,30,0,-1,-1,-1,-1,-1,-1},
	meatType = "",
	meatAmount = 0,
	hideType = "",
	hideAmount = 0,
	boneType = "",
	boneAmount = 0,
	milk = 0,
	tamingChance = 0,
	ferocity = 0,
	pvpBitmask = NONE,
	creatureBitmask = NONE,
	optionsBitmask = INVULNERABLE + CONVERSABLE,
	diet = HERBIVORE,
	scale = 1.2,

	templates = {"object/mobile/dressed_fightmaster_jorak.iff"},
	lootGroups = {},
	weapons = {},
	conversationTemplate = "theme_park_marauder_raglith_jorak_mission_giver_convotemplate",
	attacks = {
	}
}

CreatureTemplates:addCreatureTemplate(fightmaster_jorak, "fightmaster_jorak")
