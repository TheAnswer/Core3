death_watch_herald_rebel = Creature:new {
	objectName = "",
	customName = "Lutin Nightstalker",
	socialGroup = "rebel",
	faction = "rebel",
	level = 99,
	chanceHit = 0.99,
	damageMin = 640,
	damageMax = 990,
	baseXp = 9336,
	baseHAM = 24000,
	baseHAMmax = 29000,
	armor = 2,
	resists = {45,45,45,90,90,90,45,45,45},
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
	optionsBitmask = INVULNERABLE + CONVERSABLE,
	diet = HERBIVORE,

	templates = {"object/mobile/dressed_rebel_general_moncal_male_01.iff"},
	lootGroups = {},
	weapons = {},
	conversationTemplate = "lutinNightstalkerConvoTemplate",
	attacks = {
	}
}

CreatureTemplates:addCreatureTemplate(death_watch_herald_rebel, "death_watch_herald_rebel")