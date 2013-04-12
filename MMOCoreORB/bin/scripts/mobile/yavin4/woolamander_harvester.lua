woolamander_harvester = Creature:new {
	objectName = "@mob/creature_names:woolamander_harvester",
	socialGroup = "woolamander",
	pvpFaction = "",
	faction = "",
	level = 38,
	chanceHit = 0.43,
	damageMin = 335,
	damageMax = 380,
	baseXp = 3733,
	baseHAM = 9100,
	baseHAMmax = 11100,
	armor = 1,
	resists = {30,30,-1,80,80,80,-1,-1,-1},
	meatType = "meat_carnivore",
	meatAmount = 20,
	hideType = "hide_wooly",
	hideAmount = 20,
	boneType = "bone_mammal",
	boneAmount = 40,
	milk = 0,
	tamingChance = 0,
	ferocity = 0,
	pvpBitmask = AGGRESSIVE + ATTACKABLE + ENEMY,
	creatureBitmask = PACK + HERD,
	optionsBitmask = 0,
	diet = HERBIVORE,

	templates = {"object/mobile/woolamander.iff"},
	lootGroups = {},
	weapons = {},
	conversationTemplate = "",
	attacks = {
		{"creatureareaattack",""},
		{"dizzyattack","dizzyChance=50"}
	}
}

CreatureTemplates:addCreatureTemplate(woolamander_harvester, "woolamander_harvester")
