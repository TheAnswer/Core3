dwarf_bantha = Creature:new {
	objectName = "@mob/creature_names:dwarf_bantha",
	socialGroup = "bantha",
	faction = "",
	level = 11,
	chanceHit = 0.29,
	damageMin = 90,
	damageMax = 110,
	baseXp = 356,
	baseHAM = 1200,
	baseHAMmax = 1400,
	armor = 0,
	resists = {0,0,0,115,0,0,0,-1,-1},
	meatType = "meat_herbivore",
	meatAmount = 300,
	hideType = "hide_wooly",
	hideAmount = 200,
	boneType = "bone_mammal",
	boneAmount = 100,
	milkType = "milk_wild",
	milk = 150,
	tamingChance = 0,
	ferocity = 0,
	pvpBitmask = ATTACKABLE,
	creatureBitmask = HERD,
	optionsBitmask = AIENABLED,
	diet = HERBIVORE,

	templates = {"object/mobile/dwarf_bantha.iff"},
	scale = 0.75,
	lootGroups = {},
	weapons = {},
	conversationTemplate = "",
	attacks = {
		{"",""},
		{"stunattack",""}
	}
}

CreatureTemplates:addCreatureTemplate(dwarf_bantha, "dwarf_bantha")
