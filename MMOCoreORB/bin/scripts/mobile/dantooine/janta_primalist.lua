janta_primalist = Creature:new {
	objectName = "@mob/creature_names:janta_primalist",
	randomNameType = NAME_GENERIC,
	randomNameTag = true,
	socialGroup = "janta_tribe",
	faction = "janta_tribe",
	level = 42,
	chanceHit = 0.47,
	damageMin = 405,
	damageMax = 520,
	baseXp = 4097,
	baseHAM = 9700,
	baseHAMmax = 11900,
	armor = 1,
	resists = {-1,0,-1,0,0,60,0,-1,-1},
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

CreatureTemplates:addCreatureTemplate(janta_primalist, "janta_primalist")
