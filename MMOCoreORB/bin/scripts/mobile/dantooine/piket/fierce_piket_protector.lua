fierce_piket_protector = Creature:new {
	objectName = "@mob/creature_names:fierce_piket_protector",
	socialGroup = "piket",
	pvpFaction = "",
	faction = "",
	level = 45,
	chanceHit = 0.45,
	damageMin = 355,
	damageMax = 420,
	baseXp = 4461,
	baseHAM = 9800,
	baseHAMmax = 12000,
	armor = 0,
	resists = {50,55,-1,30,-1,30,30,30,-1},
	meatType = "meat_herbivore",
	meatAmount = 200,
	hideType = "hide_scaley",
	hideAmount = 120,
	boneType = "bone_mammal",
	boneAmount = 110,
	milk = 0,
	tamingChance = 0,
	ferocity = 0,
	pvpBitmask = AGGRESSIVE + ATTACKABLE + ENEMY,
	creatureBitmask = PACK + KILLER,
	optionsBitmask = 128,
	diet = CARNIVORE,

	templates = {"object/mobile/piket_hue.iff"},
	lootGroups = {},
	weapons = {},
	conversationTemplate = "",
	attacks = {
		{"blindattack","blindChance=50"},
		{"dizzyattack","dizzyChance=50"}
	}
}

CreatureTemplates:addCreatureTemplate(fierce_piket_protector, "fierce_piket_protector")