trainer_doctor = Creature:new {
	objectName = "@mob/creature_names:trainer_doctor",
	randomNameType = NAME_GENERIC,
	randomNameTag = true,
	faction = "",
	level = 100,
	chanceHit = 0.390000,
	damageMin = 290,
	damageMax = 300,
	baseXp = 2914,
	baseHAM = 8400,
	baseHAMmax = 10200,
	armor = 0,
	resists = {-1,-1,-1,-1,-1,-1,-1,-1,-1},
	meatType = "",
	meatAmount = 0,
	hideType = "",
	hideAmount = 0,
	boneType = "",
	boneAmount = 0,
	milk = 0,
	tamingChance = 0.000000,
	ferocity = 0,
	pvpBitmask = NONE,
	creatureBitmask = NONE,
	optionsBitmask = INVULNERABLE + CONVERSABLE,
	diet = HERBIVORE,

	templates = {
		"object/mobile/dressed_doctor_trainer_human_female_01.iff",
		"object/mobile/dressed_doctor_trainer_moncal_female_01.iff",
		"object/mobile/dressed_doctor_trainer_moncal_male_01.iff"
	},
	lootGroups = {},
	weapons = {},
	conversationTemplate = "doctorTrainerConvoTemplate",
	attacks = {}
}
CreatureTemplates:addCreatureTemplate(trainer_doctor,"trainer_doctor")
