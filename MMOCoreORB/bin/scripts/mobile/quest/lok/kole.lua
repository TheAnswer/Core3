kole = Creature:new {
	objectName = "@mob/creature_names:kole",
	socialGroup = "nym",
	faction = "nym",
	level = 22,
	chanceHit = 0.33,
	damageMin = 190,
	damageMax = 200,
	baseXp = 1803,
	baseHAM = 5000,
	baseHAMmax = 6100,
	armor = 1,
	resists = {20,20,40,20,0,20,0,-1,-1},
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
	optionsBitmask = AIENABLED + CONVERSABLE,
	diet = HERBIVORE,

	templates = {"object/mobile/kole.iff"},
	lootGroups = {},
	weapons = {},
	conversationTemplate = "koleConvoTemplate",
	attacks = {
	}
}

CreatureTemplates:addCreatureTemplate(kole, "kole")