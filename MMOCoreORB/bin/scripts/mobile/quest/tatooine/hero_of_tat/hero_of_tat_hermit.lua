hero_of_tat_hermit = Creature:new {
	objectName = "@mob/creature_names:quest_hero_of_tatooine_hermit",
	socialGroup = "townsperson",
	faction = "townsperson",
	level = 100,
	chanceHit = 1,
	damageMin = 645,
	damageMax = 1000,
	baseXp = 9336,
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
	creatureBitmask = NONE,
	optionsBitmask = AIENABLED + CONVERSABLE + INTERESTING,
	diet = HERBIVORE,

	templates = {"object/mobile/dressed_hermit_of_tatooine.iff"},
	lootGroups = {},
	weapons = {},
	conversationTemplate = "heroOfTatHermitConvoTemplate",
	attacks = {
	}
}

CreatureTemplates:addCreatureTemplate(hero_of_tat_hermit, "hero_of_tat_hermit")