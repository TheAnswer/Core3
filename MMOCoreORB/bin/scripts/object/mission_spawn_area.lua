mission_spawn_area = SharedObjectTemplate:new {
	planetMapCategory = "",
	planetMapSubCategory = "",
	autoRegisterWithPlanetMap = 1,

	zoneComponent = "ZoneComponent",
	objectMenuComponent = "ObjectMenuComponent",
	containerComponent = "ContainerComponent",
	gameObjectType = 33554434
}

ObjectTemplates:addTemplate(mission_spawn_area, "object/mission_spawn_area.iff")
