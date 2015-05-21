foul_donkuwah_laborer = Creature:new {
	objectName = "@mob/creature_names:foul_donkuwah_laborer",
	randomNameType = NAME_GENERIC_TAG,
	socialGroup = "donkuwah_tribe",
	faction = "donkuwah_tribe",
	level = 24,
	chanceHit = 0.35,
	damageMin = 220,
	damageMax = 230,
	baseXp = 2443,
	baseHAM = 6300,
	baseHAMmax = 7700,
	armor = 0,
	resists = {25,15,0,0,0,0,0,-1,-1},
	meatType = "",
	meatAmount = 0,
	hideType = "",
	hideAmount = 0,
	boneType = "",
	boneAmount = 0,
	milk = 0,
	tamingChance = 0,
	ferocity = 0,
	pvpBitmask = AGGRESSIVE + ATTACKABLE + ENEMY,
	creatureBitmask = PACK,
	optionsBitmask = 128,
	diet = HERBIVORE,

	templates = {
		"object/mobile/dulok_male.iff",
		"object/mobile/dulok_female.iff"},
	lootGroups = {
		{
			groups = {
				{group = "donkuwah_common", chance = 10000000}
			},
			lootChance = 1480000
		}
	},
	weapons = {"donkuwah_weapons"},
	conversationTemplate = "",
	attacks = brawlermaster
}

CreatureTemplates:addCreatureTemplate(foul_donkuwah_laborer, "foul_donkuwah_laborer")
