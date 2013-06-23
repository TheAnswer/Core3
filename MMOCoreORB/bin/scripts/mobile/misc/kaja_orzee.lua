kaja_orzee = Creature:new {
	objectName = "@mob/creature_names:kaja_orzee",
	socialGroup = "imperial",
	pvpFaction = "imperial",
	faction = "imperial",
	level = 100,
	chanceHit = 1,
	damageMin = 645,
	damageMax = 1000,
	baseXp = 9429,
	baseHAM = 24000,
	baseHAMmax = 30000,
	armor = 0,
	resists = {0,0,0,0,0,0,0,0,-1},
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
	creatureBitmask = PACK,
	optionsBitmask = 264,
	diet = HERBIVORE,

	templates = {"object/mobile/kaja_orzee.iff"},
	lootGroups = {},
	weapons = {},
	conversationTemplate = "imperial_retreat_kaja_orzee_convotemplate",
	outfit = "kaja_orzee_outfit",
	attacks = {
	}
}

CreatureTemplates:addCreatureTemplate(kaja_orzee, "kaja_orzee")