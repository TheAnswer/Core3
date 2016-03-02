enhanced_gaping_spider = Creature:new {
	objectName = "@mob/creature_names:geonosian_gaping_spider_fire",
	customName = "Fire Breathing Spider",
	socialGroup = "geonosian_creature",
	faction = "",
	level = 108,
	chanceHit = 2.5,
	damageMin = 715,
	damageMax = 1140,
	baseXp = 10267,
	baseHAM = 32000,
	baseHAMmax = 39000,
	armor = 2,
	resists = {150,10,-1,195,-1,10,10,10,-1},
	meatType = "meat_insect",
	meatAmount = 50,
	hideType = "",
	hideAmount = 0,
	boneType = "",
	boneAmount = 0,
	milk = 0,
	tamingChance = 0,
	ferocity = 0,
	pvpBitmask = AGGRESSIVE + ATTACKABLE + ENEMY,
	creatureBitmask = PACK + KILLER,
	optionsBitmask = AIENABLED,
	diet = CARNIVORE,

	templates = {"object/mobile/gaping_spider.iff"},
	scale = 2.5,
	lootGroups = {
		{
			groups = {
				{group = "fire_breathing_spider", chance = 10000000}
			}
		}
	},
	weapons = {"creature_spit_heavy_flame"},
	conversationTemplate = "",
	attacks = {
		{"strongpoison",""},
		{"stunattack",""}
	}
}

CreatureTemplates:addCreatureTemplate(enhanced_gaping_spider, "enhanced_gaping_spider")
