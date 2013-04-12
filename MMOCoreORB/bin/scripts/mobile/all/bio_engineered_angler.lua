bio_engineered_angler = Creature:new {
	objectName = "@mob/creature_names:bio_engineered_angler",
	customName = "",
	socialGroup = "angler",
	pvpFaction = "",
	faction = "",
	level = 1,
	chanceHit = 0.2,
	damageMin = 30,
	damageMax = 40,
	baseXp = 40,
	baseHAM = 45,
	baseHAMmax = 55,
	armor = 0,
	resists = {0,0,0,0,0,0,0,0,0},
	meatType = "",
	meatAmount = 0,
	hideType = "",
	hideAmount = 0,
	boneType = "",
	boneAmount = 0,
	milk = 0,
	tamingChance = 0,
	ferocity = 0,
	pvpBitmask = AGGRESSIVE + ATTACKABLE,
	creatureBitmask = PACK + STALKER,
	optionsBitmask = 128,
	diet = CARNIVORE,

	templates = {"object/mobile/angler_hue.iff"},
	weapons = {},
	conversationTemplate = "",
	lootGroups = {
	},
	outfit = "",
	attacks = {
	}
}

CreatureTemplates:addCreatureTemplate(bio_engineered_angler, "bio_engineered_angler")