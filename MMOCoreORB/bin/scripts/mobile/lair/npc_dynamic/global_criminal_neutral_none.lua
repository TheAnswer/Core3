global_criminal_neutral_none = Lair:new {
	mobiles = {{"criminal", 0}},
	spawnLimit = 8,
	buildingsVeryEasy = {"object/tangible/lair/base/objective_power_generator.iff"},
	buildingsEasy = {"object/tangible/lair/base/objective_power_generator.iff"},
	buildingsMedium = {"object/tangible/lair/base/objective_power_generator.iff"},
	buildingsHard = {"object/tangible/lair/base/objective_power_generator.iff"},
	buildingsVeryHard = {"object/tangible/lair/base/objective_power_generator.iff"},
	lairType = "npc"
}

addLairTemplate("global_criminal_neutral_none", global_criminal_neutral_none)
