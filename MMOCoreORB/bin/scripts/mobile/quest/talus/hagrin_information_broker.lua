hagrin_information_broker = Creature:new {
	objectName = "@mob/creature_names:bothan_information_broker",
	socialGroup = "spynet",
	faction = "",
	level = 3,
	chanceHit = 0.23,
	damageMin = 35,
	damageMax = 45,
	baseXp = 45,
	baseHAM = 90,
	baseHAMmax = 110,
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
	pvpBitmask = NONE,
	creatureBitmask = HERD,
	optionsBitmask = AIENABLED + CONVERSABLE,
	diet = HERBIVORE,

	templates = {
		"object/mobile/dressed_hutt_informant_quest.iff",
		"object/mobile/dressed_eisley_officer_bothan_female_01.iff",
		"object/mobile/dressed_eisley_officer_bothan_male_01.iff"
					},
	lootGroups = {},
	weapons = {},
	conversationTemplate = "haleen_snowline_hagrin_zeed_mission_target_convotemplate",
	attacks = {
	}
}

CreatureTemplates:addCreatureTemplate(hagrin_information_broker, "hagrin_information_broker")