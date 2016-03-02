gramm_rile = Creature:new {
	objectName = "@mob/creature_names:mos_taike_mayor",
	customName = "Gramm Rile",
	socialGroup = "townsperson",
	faction = "townsperson",
	level = 20,
	chanceHit = 0.33,
	damageMin = 190,
	damageMax = 200,
	baseXp = 1803,
	baseHAM = 5000,
	baseHAMmax = 6100,
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
	creatureBitmask = NONE,
	optionsBitmask = AIENABLED + CONVERSABLE,
	diet = HERBIVORE,

	templates = {"object/mobile/mos_taike_mayor.iff"},
	lootGroups = {},
	weapons = {},
	conversationTemplate = "gramm_rile_mission_giver_convotemplate",
	outfit = "mos_taike_mayor_outfit",
	attacks = {
	}
}

CreatureTemplates:addCreatureTemplate(gramm_rile, "gramm_rile")
