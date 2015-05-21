kunga_soothsayer = Creature:new {
	objectName = "@mob/creature_names:kunga_soothsayer",
	randomNameType = NAME_GENERIC_TAG,
	socialGroup = "kunga_tribe",
	faction = "kunga_tribe",
	level = 27,
	chanceHit = 0.36,
	damageMin = 270,
	damageMax = 280,
	baseXp = 2730,
	baseHAM = 5900,
	baseHAMmax = 7200,
	armor = 0,
	resists = {15,40,15,-1,-1,60,40,-1,-1},
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
	optionsBitmask = 128,
	diet = HERBIVORE,

	templates = {
		"object/mobile/dantari_male.iff",
		"object/mobile/dantari_female.iff"},
	lootGroups = {
		{
			groups = {
				{group = "junk", chance = 4000000},
				{group = "loot_kit_parts", chance = 3000000},
				{group = "armor_attachments", chance = 500000},
				{group = "clothing_attachments", chance = 500000},
				{group = "wearables_common", chance = 1000000},
				{group = "wearables_uncommon", chance = 1000000}
			}
		}
	},
	weapons = {"primitive_weapons"},
	conversationTemplate = "",
	attacks = merge(pikemanmaster,fencermaster,brawlermaster)
}

CreatureTemplates:addCreatureTemplate(kunga_soothsayer, "kunga_soothsayer")
