corellia_meatlump_outpost_neutral_medium_theater = Lair:new {
	mobiles = {{"meatlump_clod",1},{"meatlump_oaf",1},{"meatlump_loon",1},{"meatlump_cretin",1}},
	spawnLimit = 15,
	buildingsVeryEasy = {"object/building/poi/corellia_meatlump_large1.iff","object/building/poi/corellia_meatlump_large2.iff","object/building/poi/corellia_meatlump_large3.iff"},
	buildingsEasy = {"object/building/poi/corellia_meatlump_large1.iff","object/building/poi/corellia_meatlump_large2.iff","object/building/poi/corellia_meatlump_large3.iff"},
	buildingsMedium = {"object/building/poi/corellia_meatlump_large1.iff","object/building/poi/corellia_meatlump_large2.iff","object/building/poi/corellia_meatlump_large3.iff"},
	buildingsHard = {"object/building/poi/corellia_meatlump_large1.iff","object/building/poi/corellia_meatlump_large2.iff","object/building/poi/corellia_meatlump_large3.iff"},
	buildingsVeryHard = {"object/building/poi/corellia_meatlump_large1.iff","object/building/poi/corellia_meatlump_large2.iff","object/building/poi/corellia_meatlump_large3.iff"},
	missionBuilding = "object/tangible/lair/base/objective_power_transformer.iff",
	mobType = "npc",
	buildingType = "theater"
}

addLairTemplate("corellia_meatlump_outpost_neutral_medium_theater", corellia_meatlump_outpost_neutral_medium_theater)
