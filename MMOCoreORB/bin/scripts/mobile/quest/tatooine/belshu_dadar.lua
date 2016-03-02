belshu_dadar = Creature:new {
	objectName = "@npc_spawner_n:smuggler_pilot",
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
	resists = {0,0,0,0,10,0,0,-1,-1},
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

	templates = {"object/mobile/dressed_tatooine_jabba_henchman.iff"},
	lootGroups = {},
	weapons = {},
	conversationTemplate = "theme_park_jabba_mission_target_convotemplate",
	attacks = {
	}
}

CreatureTemplates:addCreatureTemplate(belshu_dadar, "belshu_dadar")