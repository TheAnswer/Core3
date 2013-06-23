--Copyright (C) 2009 <SWGEmu>

--This File is part of Core3.

--This program is free software; you can redistribute
--it and/or modify it under the terms of the GNU Lesser
--General Public License as published by the Free Software
--Foundation; either version 2 of the License,
--or (at your option) any later version.

--This program is distributed in the hope that it will be useful,
--but WITHOUT ANY WARRANTY; without even the implied warranty of
--MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
--See the GNU Lesser General Public License for
--more details.

--You should have received a copy of the GNU Lesser General
--Public License along with this program; if not, write to
--the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

--Linking Engine3 statically or dynamically with other modules
--is making a combined work based on Engine3.
--Thus, the terms and conditions of the GNU Lesser General Public License
--cover the whole combination.

--In addition, as a special exception, the copyright holders of Engine3
--give you permission to combine Engine3 program with free software
--programs or libraries that are released under the GNU LGPL and with
--code included in the standard release of Core3 under the GNU LGPL
--license (or modified versions of such code, with unchanged license).
--You may copy and distribute such a system following the terms of the
--GNU LGPL for Engine3 and the licenses of the other code concerned,
--provided that you include the source code of that other code when
--and as the GNU LGPL requires distribution of source code.

--Note that people who make modified versions of Engine3 are not obligated
--to grant this special exception for their modified versions;
--it is their choice whether to do so. The GNU Lesser General Public License
--gives permission to release a modified version without this exception;
--this exception also makes it possible to release a modified version
--which carries forward this exception.


object_waypoint_shared_path_waypoint = SharedWaypointObjectTemplate:new {
	clientTemplateFileName = "object/waypoint/shared_path_waypoint.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/thm_prp_gungan_plasma_sphere.apt",
	arrangementDescriptorFilename = "",

	clearFloraRadius = 0,
	clientDataFile = "",
	clientGameObjectType = 2053,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 0,
	collisionActionPassFlags = 0,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 0,
	collisionMaterialPassFlags = 0,
	containerType = 0,
	containerVolumeLimit = 0,

	detailedDescription = "string_id_table",

	gameObjectType = 2053,

	locationReservationRadius = 0,
	lookAtText = "string_id_table",

	noBuildRadius = 0,

	objectName = "@obj_n:waypoint",
	onlyVisibleInTools = 0,

	portalLayoutFilename = "",

	scale = 0,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "",
	snapToTerrain = 0,
	surfaceType = 0,

	totalCellNumber = 0,

	clientObjectCRC = 4195736526,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/intangible/base/shared_base_intangible.iff", "object/waypoint/base/shared_base_waypoint.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_waypoint_shared_path_waypoint, "object/waypoint/shared_path_waypoint.iff")

object_waypoint_shared_waypoint = SharedWaypointObjectTemplate:new {
	clientTemplateFileName = "object/waypoint/shared_waypoint.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/ui_planet_sel_default.apt",
	arrangementDescriptorFilename = "abstract/slot/arrangement/arrangement_waypoint.iff",

	clearFloraRadius = 0,
	clientDataFile = "",
	clientGameObjectType = 2053,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 0,
	collisionActionPassFlags = 0,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 0,
	collisionMaterialPassFlags = 0,
	containerType = 0,
	containerVolumeLimit = 0,

	detailedDescription = "string_id_table",

	gameObjectType = 2053,

	locationReservationRadius = 0,
	lookAtText = "string_id_table",

	noBuildRadius = 0,

	objectName = "@obj_n:waypoint",
	onlyVisibleInTools = 0,

	portalLayoutFilename = "",

	scale = 0,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "",
	snapToTerrain = 0,
	surfaceType = 0,

	totalCellNumber = 0,

	clientObjectCRC = 4251347533,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/intangible/base/shared_base_intangible.iff", "object/waypoint/base/shared_base_waypoint.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_waypoint_shared_waypoint, "object/waypoint/shared_waypoint.iff")

object_waypoint_shared_world_waypoint = SharedWaypointObjectTemplate:new {
	clientTemplateFileName = "object/waypoint/shared_world_waypoint.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/pt_waypoint_blue.prt",
	arrangementDescriptorFilename = "",

	clearFloraRadius = 0,
	clientDataFile = "",
	clientGameObjectType = 2053,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 0,
	collisionActionPassFlags = 0,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 0,
	collisionMaterialPassFlags = 0,
	containerType = 0,
	containerVolumeLimit = 0,

	detailedDescription = "string_id_table",

	gameObjectType = 2053,

	locationReservationRadius = 0,
	lookAtText = "string_id_table",

	noBuildRadius = 0,

	objectName = "@obj_n:waypoint",
	onlyVisibleInTools = 0,

	portalLayoutFilename = "",

	scale = 0,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "",
	snapToTerrain = 0,
	surfaceType = 0,

	totalCellNumber = 0,

	clientObjectCRC = 629636525,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/intangible/base/shared_base_intangible.iff", "object/waypoint/base/shared_base_waypoint.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_waypoint_shared_world_waypoint, "object/waypoint/shared_world_waypoint.iff")

object_waypoint_shared_world_waypoint_blue = SharedWaypointObjectTemplate:new {
	clientTemplateFileName = "object/waypoint/shared_world_waypoint_blue.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/pt_waypoint_blue.prt",
	arrangementDescriptorFilename = "",

	clearFloraRadius = 0,
	clientDataFile = "",
	clientGameObjectType = 2053,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 0,
	collisionActionPassFlags = 0,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 0,
	collisionMaterialPassFlags = 0,
	containerType = 0,
	containerVolumeLimit = 0,

	detailedDescription = "string_id_table",

	gameObjectType = 2053,

	locationReservationRadius = 0,
	lookAtText = "string_id_table",

	noBuildRadius = 0,

	objectName = "@obj_n:waypoint",
	onlyVisibleInTools = 0,

	portalLayoutFilename = "",

	scale = 0,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "",
	snapToTerrain = 0,
	surfaceType = 0,

	totalCellNumber = 0,

	clientObjectCRC = 3038003230,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/intangible/base/shared_base_intangible.iff", "object/waypoint/base/shared_base_waypoint.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_waypoint_shared_world_waypoint_blue, "object/waypoint/shared_world_waypoint_blue.iff")

object_waypoint_shared_world_waypoint_green = SharedWaypointObjectTemplate:new {
	clientTemplateFileName = "object/waypoint/shared_world_waypoint_green.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/pt_waypoint_green.prt",
	arrangementDescriptorFilename = "",

	clearFloraRadius = 0,
	clientDataFile = "",
	clientGameObjectType = 2053,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 0,
	collisionActionPassFlags = 0,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 0,
	collisionMaterialPassFlags = 0,
	containerType = 0,
	containerVolumeLimit = 0,

	detailedDescription = "string_id_table",

	gameObjectType = 2053,

	locationReservationRadius = 0,
	lookAtText = "string_id_table",

	noBuildRadius = 0,

	objectName = "@obj_n:waypoint",
	onlyVisibleInTools = 0,

	portalLayoutFilename = "",

	scale = 0,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "",
	snapToTerrain = 0,
	surfaceType = 0,

	totalCellNumber = 0,

	clientObjectCRC = 211288739,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/intangible/base/shared_base_intangible.iff", "object/waypoint/base/shared_base_waypoint.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_waypoint_shared_world_waypoint_green, "object/waypoint/shared_world_waypoint_green.iff")

object_waypoint_shared_world_waypoint_orange = SharedWaypointObjectTemplate:new {
	clientTemplateFileName = "object/waypoint/shared_world_waypoint_orange.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/pt_waypoint_orange.prt",
	arrangementDescriptorFilename = "",

	clearFloraRadius = 0,
	clientDataFile = "",
	clientGameObjectType = 2053,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 0,
	collisionActionPassFlags = 0,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 0,
	collisionMaterialPassFlags = 0,
	containerType = 0,
	containerVolumeLimit = 0,

	detailedDescription = "string_id_table",

	gameObjectType = 2053,

	locationReservationRadius = 0,
	lookAtText = "string_id_table",

	noBuildRadius = 0,

	objectName = "@obj_n:waypoint",
	onlyVisibleInTools = 0,

	portalLayoutFilename = "",

	scale = 0,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "",
	snapToTerrain = 0,
	surfaceType = 0,

	totalCellNumber = 0,

	clientObjectCRC = 3990793280,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/intangible/base/shared_base_intangible.iff", "object/waypoint/base/shared_base_waypoint.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_waypoint_shared_world_waypoint_orange, "object/waypoint/shared_world_waypoint_orange.iff")

object_waypoint_shared_world_waypoint_purple = SharedWaypointObjectTemplate:new {
	clientTemplateFileName = "object/waypoint/shared_world_waypoint_purple.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/pt_waypoint_red.prt",
	arrangementDescriptorFilename = "",

	clearFloraRadius = 0,
	clientDataFile = "",
	clientGameObjectType = 2053,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 0,
	collisionActionPassFlags = 0,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 0,
	collisionMaterialPassFlags = 0,
	containerType = 0,
	containerVolumeLimit = 0,

	detailedDescription = "string_id_table",

	gameObjectType = 2053,

	locationReservationRadius = 0,
	lookAtText = "string_id_table",

	noBuildRadius = 0,

	objectName = "@obj_n:waypoint",
	onlyVisibleInTools = 0,

	portalLayoutFilename = "",

	scale = 0,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "",
	snapToTerrain = 0,
	surfaceType = 0,

	totalCellNumber = 0,

	clientObjectCRC = 186074874,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/intangible/base/shared_base_intangible.iff", "object/waypoint/base/shared_base_waypoint.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_waypoint_shared_world_waypoint_purple, "object/waypoint/shared_world_waypoint_purple.iff")

object_waypoint_shared_world_waypoint_space = SharedWaypointObjectTemplate:new {
	clientTemplateFileName = "object/waypoint/shared_world_waypoint_space.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/pt_space_waypoint.prt",
	arrangementDescriptorFilename = "",

	clearFloraRadius = 0,
	clientDataFile = "",
	clientGameObjectType = 2053,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 0,
	collisionActionPassFlags = 0,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 0,
	collisionMaterialPassFlags = 0,
	containerType = 0,
	containerVolumeLimit = 0,

	detailedDescription = "string_id_table",

	gameObjectType = 2053,

	locationReservationRadius = 0,
	lookAtText = "string_id_table",

	noBuildRadius = 0,

	objectName = "@obj_n:waypoint",
	onlyVisibleInTools = 0,

	portalLayoutFilename = "",

	scale = 0,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "",
	snapToTerrain = 0,
	surfaceType = 0,

	totalCellNumber = 0,

	clientObjectCRC = 2672628441,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/intangible/base/shared_base_intangible.iff", "object/waypoint/base/shared_base_waypoint.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_waypoint_shared_world_waypoint_space, "object/waypoint/shared_world_waypoint_space.iff")

object_waypoint_shared_world_waypoint_white = SharedWaypointObjectTemplate:new {
	clientTemplateFileName = "object/waypoint/shared_world_waypoint_white.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/pt_waypoint_white.prt",
	arrangementDescriptorFilename = "",

	clearFloraRadius = 0,
	clientDataFile = "",
	clientGameObjectType = 2053,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 0,
	collisionActionPassFlags = 0,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 0,
	collisionMaterialPassFlags = 0,
	containerType = 0,
	containerVolumeLimit = 0,

	detailedDescription = "string_id_table",

	gameObjectType = 2053,

	locationReservationRadius = 0,
	lookAtText = "string_id_table",

	noBuildRadius = 0,

	objectName = "@obj_n:waypoint",
	onlyVisibleInTools = 0,

	portalLayoutFilename = "",

	scale = 0,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "",
	snapToTerrain = 0,
	surfaceType = 0,

	totalCellNumber = 0,

	clientObjectCRC = 2818077908,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/intangible/base/shared_base_intangible.iff", "object/waypoint/base/shared_base_waypoint.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_waypoint_shared_world_waypoint_white, "object/waypoint/shared_world_waypoint_white.iff")

object_waypoint_shared_world_waypoint_yellow = SharedWaypointObjectTemplate:new {
	clientTemplateFileName = "object/waypoint/shared_world_waypoint_yellow.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/pt_waypoint_yellow.prt",
	arrangementDescriptorFilename = "",

	clearFloraRadius = 0,
	clientDataFile = "",
	clientGameObjectType = 2053,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 0,
	collisionActionPassFlags = 0,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 0,
	collisionMaterialPassFlags = 0,
	containerType = 0,
	containerVolumeLimit = 0,

	detailedDescription = "string_id_table",

	gameObjectType = 2053,

	locationReservationRadius = 0,
	lookAtText = "string_id_table",

	noBuildRadius = 0,

	objectName = "@obj_n:waypoint",
	onlyVisibleInTools = 0,

	portalLayoutFilename = "",

	scale = 0,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "",
	snapToTerrain = 0,
	surfaceType = 0,

	totalCellNumber = 0,

	clientObjectCRC = 2960507421,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/intangible/base/shared_base_intangible.iff", "object/waypoint/base/shared_base_waypoint.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_waypoint_shared_world_waypoint_yellow, "object/waypoint/shared_world_waypoint_yellow.iff")
