klin_nif = Creature:new {
	objectName = "",
	customName = "Klin Nif",
	socialGroup = "death_watch",
	faction = "",
	level = 178,
	chanceHit = 12.25,
	damageMin = 1020,
	damageMax = 1750,
	baseXp = 16794,
	baseHAM = 200000,
	baseHAMmax = 200000,
	armor = 2,
	resists = {75,75,90,80,45,45,100,70,-1},
	meatType = "",
	meatAmount = 0,
	hideType = "",
	hideAmount = 0,
	boneType = "",
	boneAmount = 0,
	milk = 0,
	tamingChance = 0,
	ferocity = 0,
	pvpBitmask = AGGRESSIVE + ATTACKABLE + ENEMY,
	creatureBitmask = KILLER,
	optionsBitmask = AIENABLED,
	diet = HERBIVORE,
	scale = 1.15,

	templates = {"object/mobile/dressed_death_watch_silver.iff"},
	lootGroups = {
		{
			groups = {
				{group = "klin_nif_drop", chance = 10000000}
			},
			lootChance = 10000000
		}
	},
	weapons = {"death_watch_commander_weapons"},
	conversationTemplate = "",
	attacks = merge(carbineermaster,marksmanmaster,brawlermaster,fencermaster,pistoleermaster)
}

CreatureTemplates:addCreatureTemplate(klin_nif, "klin_nif")
