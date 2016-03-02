janta_herbalist = Creature:new {
	objectName = "@mob/creature_names:janta_herbalist",
	randomNameType = NAME_GENERIC,
	randomNameTag = true,
	socialGroup = "janta_tribe",
	faction = "janta_tribe",
	level = 27,
	chanceHit = 0.39,
	damageMin = 305,
	damageMax = 320,
	baseXp = 2730,
	baseHAM = 8200,
	baseHAMmax = 10000,
	armor = 0,
	resists = {-1,25,-1,25,25,25,25,-1,-1},
	meatType = "",
	meatAmount = 0,
	hideType = "",
	hideAmount = 0,
	boneType = "",
	boneAmount = 0,
	milk = 0,
	tamingChance = 0,
	ferocity = 0,
	pvpBitmask = ATTACKABLE,
	creatureBitmask = PACK + HERD,
	optionsBitmask = AIENABLED,
	diet = HERBIVORE,

	templates = {
		"object/mobile/dantari_male.iff",
		"object/mobile/dantari_female.iff"},
	lootGroups = {
		{
			groups = {
				{group = "junk", chance = 5500000},
				{group = "janta_common", chance = 1500000},
				{group = "loot_kit_parts", chance = 3000000}
			}
		}
	},
	weapons = {"primitive_weapons"},
	conversationTemplate = "",
	attacks = merge(pikemanmaster,fencermaster,brawlermaster)
}

CreatureTemplates:addCreatureTemplate(janta_herbalist, "janta_herbalist")
