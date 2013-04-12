vrelt = Creature:new {
	objectName = "@mob/creature_names:vrelt",
	customName = "",
	socialGroup = "vrelt",
	pvpFaction = "",
	faction = "",
	level = 6,
	chanceHit = 0.25,
	damageMin = 50,
	damageMax = 55,
	baseXp = 113,
	baseHAM = 180,
	baseHAMmax = 220,
	armor = 0,
	resists = {0,0,0,0,0,0,0,0,0},
	meatType = "meat_wild",
	meatAmount = 4,
	hideType = "hide_leathery",
	hideAmount = 3,
	boneType = "bone_mammal",
	boneAmount = 2,
	milk = 0,
	tamingChance = 0,
	ferocity = 0,
	pvpBitmask = ATTACKABLE,
	creatureBitmask = NONE,
	optionsBitmask = 0,
	diet = HERBIVORE,

	templates = {"object/mobile/vrelt.iff"},
	weapons = {},
	conversationTemplate = "",
	lootGroups = {
	},
	outfit = "",
	attacks = {
	}
}

CreatureTemplates:addCreatureTemplate(vrelt, "vrelt")