enhanced_force_kliknik = Creature:new {
	objectName = "",
	customName = "Force Kliknik",
	socialGroup = "geonosian_creature",
	faction = "",
	level = 94,
	chanceHit = 0.95,
	damageMin = 610,
	damageMax = 930,
	baseXp = 8964,
	baseHAM = 22000,
	baseHAMmax = 27000,
	armor = 2,
	resists = {35,35,0,95,-1,0,0,0,-1},
	meatType = "meat_carnivore",
	meatAmount = 45,
	hideType = "hide_scaley",
	hideAmount = 40,
	boneType = "",
	boneAmount = 0,
	milk = 0,
	tamingChance = 0,
	ferocity = 15,
	pvpBitmask = AGGRESSIVE + ATTACKABLE + ENEMY,
	creatureBitmask = PACK + KILLER,
	optionsBitmask = 128,
	diet = CARNIVORE,

	templates = {"object/mobile/kliknik_hue.iff"},
	scale = 1.45,
	lootGroups = {
	 {
	        groups = {
				{group = "geo_kliknik", chance = 10000000}
			},
			lootChance = 2880000
		}
	},
	weapons = {"creature_spit_heavy_flame"},
	conversationTemplate = "",
	attacks = {
		{"stunattack",""},
		{"creatureareaattack",""}
	}
}

CreatureTemplates:addCreatureTemplate(enhanced_force_kliknik, "enhanced_force_kliknik")
