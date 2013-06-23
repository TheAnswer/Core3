draya_korbinari_thief = Creature:new {
	objectName = "@mob/creature_names:thief",
	customName = "",
	socialGroup = "",
	pvpFaction = "",
	faction = "",
	level = 9,
	chanceHit = 0.27,
	damageMin = 90,
	damageMax = 110,
	baseXp = 292,
	baseHAM = 405,
	baseHAMmax = 495,
	armor = 0,
	resists = {0,0,0,0,0,0,0,-1,-1},
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
	creatureBitmask = PACK + STALKER,
	optionsBitmask = 264,
	diet = HERBIVORE,

	templates = {"object/mobile/dressed_kobola_thief_rodian_male_01.iff"},
	weapons = {"pirate_weapons_medium"},
	conversationTemplate = "draya_korbinari_mission_target_convotemplate",
	lootGroups = {
		{
			groups = {			},
 			lootChance = 0
		},
	},
	outfit = "",
	attacks = merge(marksmannovice)
}

CreatureTemplates:addCreatureTemplate(draya_korbinari_thief, "draya_korbinari_thief")