senior_specforce_lieutenant = Creature:new {
	objectName = "@mob/creature_names:senior_specforce_lieutenant",
	randomNameType = NAME_GENERIC,
	randomNameTag = true,
	socialGroup = "rebel",
	faction = "rebel",
	level = 24,
	chanceHit = 0.35,
	damageMin = 230,
	damageMax = 240,
	baseXp = 2543,
	baseHAM = 6800,
	baseHAMmax = 8300,
	armor = 0,
	resists = {15,15,0,0,0,-1,0,-1,-1},
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
	creatureBitmask = PACK + KILLER,
	diet = HERBIVORE,

	templates = {
		"object/mobile/dressed_rebel_specforce_guerilla_human_male_01.iff",
		"object/mobile/dressed_rebel_specforce_guerilla_moncal_male_01.iff",
		"object/mobile/dressed_rebel_specforce_guerrilla_human_female_01.iff",
		"object/mobile/dressed_rebel_specforce_guerrilla_rodian_female_01.iff",
		"object/mobile/dressed_rebel_specforce_guerrilla_rodian_male_01.iff",
		"object/mobile/dressed_rebel_specforce_guerrilla_zabrak_female_01.iff",},
	lootGroups = {
		{
			groups = {
				{group = "color_crystals", chance = 100000},
				{group = "junk", chance = 4250000},
				{group = "rifles", chance = 1000000},
				{group = "pistols", chance = 1000000},
				{group = "melee_weapons", chance = 1000000},
				{group = "carbines", chance = 1000000},
				{group = "clothing_attachments", chance = 100000},
				{group = "armor_attachments", chance = 100000},
				{group = "rebel_officer_common", chance = 450000},
				{group = "wearables_common", chance = 1000000}
			}
		}
	},
	weapons = {"imperial_weapons_medium"},
	reactionStf = "@npc_reaction/military",
	personalityStf = "@hireling/hireling_military",
	attacks = merge(brawlermaster,marksmanmaster)
}

CreatureTemplates:addCreatureTemplate(senior_specforce_lieutenant, "senior_specforce_lieutenant")
