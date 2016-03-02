lilas_dinhint = Creature:new {
	objectName = "",
	customName = "Lilas Dinhint (museum curator)",
	socialGroup = "townsperson",
	faction = "townsperson",
	level = 10,
	chanceHit = 0.28,
	damageMin = 90,
	damageMax = 110,
	baseXp = 356,
	baseHAM = 810,
	baseHAMmax = 990,
	armor = 0,
	resists = {0,0,0,0,0,0,0,-1,-1},
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
	optionsBitmask = AIENABLED + CONVERSABLE,
	diet = HERBIVORE,

	templates = {"object/mobile/dressed_tatooine_lilas_dinhint.iff"},
	lootGroups = {},
	weapons = {},
	conversationTemplate = "museumCuratorConvoTemplate",
	attacks = {
	}
}

CreatureTemplates:addCreatureTemplate(lilas_dinhint, "lilas_dinhint")