bio_engineered_ronto = Creature:new {
	objectName = "@mob/creature_names:bio_engineered_ronto",
	customName = "",
	socialGroup = "ronto",
	pvpFaction = "",
	faction = "",
	level = 1,
	chanceHit = 0.2,
	damageMin = 30,
	damageMax = 40,
	baseXp = 30,
	baseHAM = 45,
	baseHAMmax = 55,
	armor = 0,
	resists = {0,0,0,0,0,0,0,0,-1},
	meatType = "",
	meatAmount = 0,
	hideType = "",
	hideAmount = 0,
	boneType = "",
	boneAmount = 0,
	milk = 1,
	tamingChance = 0,
	ferocity = 0,
	pvpBitmask = ATTACKABLE,
	creatureBitmask = HERD,
	optionsBitmask = 128,
	diet = CARNIVORE,

	templates = {"object/mobile/ronto_hue.iff"},
	weapons = {},
	conversationTemplate = "",
	lootGroups = {
	},
	outfit = "",
	attacks = {
	}
}

CreatureTemplates:addCreatureTemplate(bio_engineered_ronto, "bio_engineered_ronto")