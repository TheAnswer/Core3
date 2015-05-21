rorgungan_warchief = Creature:new {
	objectName = "@mob/creature_names:rorgungan_warchief",
	randomNameType = NAME_GENERIC_TAG,
	socialGroup = "rorgungan",
	faction = "rorgungan",
	level = 19,
	chanceHit = 0.32,
	damageMin = 170,
	damageMax = 180,
	baseXp = 1609,
	baseHAM = 4100,
	baseHAMmax = 5000,
	armor = 0,
	resists = {20,15,0,-1,30,0,0,-1,-1},
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
	creatureBitmask = PACK + KILLER + HERD,
	diet = HERBIVORE,

	templates = {"object/mobile/gungan_s02_male.iff"},
	lootGroups = {
		{
			groups = {
				{group = "junk", chance = 5500000},
				{group = "gungan_common", chance = 2000000},
				{group = "tailor_components", chance = 500000},
				{group = "loot_kit_parts", chance = 1500000},
				{group = "color_crystals", chance = 250000},
				{group = "crystals_poor", chance = 250000}
			}
		}
	},
	weapons = {"rebel_weapons_heavy"},
	attacks = merge(brawlermaster,marksmanmaster)
}

CreatureTemplates:addCreatureTemplate(rorgungan_warchief, "rorgungan_warchief")
