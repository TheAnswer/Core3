noble = Creature:new {
	objectName = "@mob/creature_names:noble",
	randomNameType = NAME_GENERIC,
	randomNameTag = true,
	socialGroup = "townsperson",
	faction = "townsperson",
	level = 6,
	chanceHit = 0.250000,
	damageMin = 50,
	damageMax = 55,
	baseXp = 113,
	baseHAM = 180,
	baseHAMmax = 220,
	armor = 0,
	resists = {0,0,0,0,0,0,0,-1,-1},
	meatType = "",
	meatAmount = 0,
	hideType = "",
	hideAmount = 0,
	boneType = "",
	boneAmount = 0,
	milk = 0,
	tamingChance = 0.000000,
	ferocity = 0,
	pvpBitmask = NONE,
	creatureBitmask = HERD,
	optionsBitmask = AIENABLED + CONVERSABLE + INTERESTING,
	diet = HERBIVORE,


	templates = {"noble"},
	lootGroups = {},
	weapons = {},
	conversationTemplate = "generic_noble_mission_giver_convotemplate",
	attacks = {}
}

CreatureTemplates:addCreatureTemplate(noble, "noble")
