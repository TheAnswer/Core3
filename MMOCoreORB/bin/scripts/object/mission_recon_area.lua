mission_recon_area = SharedObjectTemplate:new {
	planetMapCategory = "",
	planetMapSubCategory = "",
	autoRegisterWithPlanetMap = 1,

	zoneComponent = "ZoneComponent",
	objectMenuComponent = "ObjectMenuComponent",
	containerComponent = "ContainerComponent",
	gameObjectType = 33554435
}

ObjectTemplates:addTemplate(mission_recon_area, "object/mission_recon_area.iff")
