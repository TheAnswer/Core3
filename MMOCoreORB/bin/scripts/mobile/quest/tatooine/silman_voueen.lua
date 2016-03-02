silman_voueen = Creature:new {
	objectName = "",
	socialGroup = "townsperson",
	faction = "townsperson",
	level = 4,
	chanceHit = 0.24,
	damageMin = 40,
	damageMax = 45,
	baseXp = 62,
	baseHAM = 113,
	baseHAMmax = 138,
	armor = 0,
	resists = {15,15,15,15,15,15,15,-1,-1},
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
	creatureBitmask = HERD,
	optionsBitmask = INVULNERABLE + CONVERSABLE,
	diet = HERBIVORE,

	templates = 	{"object/mobile/dressed_criminal_thug_bothan_female_01.iff"},	
	lootGroups = {},
	weapons = {},
	conversationTemplate = "rakir_banai_mission_target_convotemplate",
	attacks = {
	}
}

CreatureTemplates:addCreatureTemplate(silman_voueen, "silman_voueen")
