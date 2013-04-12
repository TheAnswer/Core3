bio_engineered_verne = Creature:new {
	objectName = "@mob/creature_names:bio_engineered_verne",
	customName = "",
	socialGroup = "verne",
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
	milk = 1,
	tamingChance = 0,
	ferocity = 0,
	pvpBitmask = ATTACKABLE,
	creatureBitmask = PACK + HERD,
	optionsBitmask = 0,
	diet = HERBIVORE,

	templates = {"object/mobile/verne_hue.iff"},
	weapons = {},
	conversationTemplate = "",
	lootGroups = {
	},
	outfit = "",
	attacks = {
	}
}

CreatureTemplates:addCreatureTemplate(bio_engineered_verne, "bio_engineered_verne")