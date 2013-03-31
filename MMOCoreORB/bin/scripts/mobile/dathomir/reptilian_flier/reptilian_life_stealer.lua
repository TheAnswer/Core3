reptilian_life_stealer = Creature:new {
	objectName = "@mob/creature_names:reptilian_life_stealer",
	socialGroup = "reptilian_flier",
	pvpFaction = "",
	faction = "",
	level = 125,
	chanceHit = 32.5,
	damageMin = 945,
	damageMax = 1600,
	baseXp = 11859,
	baseHAM = 34000,
	baseHAMmax = 42000,
	armor = 0,
	resists = {0,25,5,100,100,-1,0,0,-1},
	meatType = "meat_avian",
	meatAmount = 135,
	hideType = "hide_leathery",
	hideAmount = 90,
	boneType = "bone_avian",
	boneAmount = 85,
	milk = 0,
	tamingChance = 0,
	ferocity = 0,
	pvpBitmask = AGGRESSIVE + ATTACKABLE + ENEMY,
	creatureBitmask = PACK + KILLER,
	optionsBitmask = 128,
	diet = CARNIVORE,

	templates = {"object/mobile/reptilian_flier_hue.iff"},
	lootGroups = {},
	weapons = {},
	conversationTemplate = "",
	attacks = {
		{"blindattack","blindChance=50"},
		{"strongpoison",""}
	}
}

CreatureTemplates:addCreatureTemplate(reptilian_life_stealer, "reptilian_life_stealer")