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


object_tangible_container_general_shared_cargo_pocket = SharedTangibleObjectTemplate:new {
	clientTemplateFileName = "object/tangible/container/general/shared_cargo_pocket.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/eqp_comp_cargo_pocket.apt",
	arrangementDescriptorFilename = "",

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "",
	clientGameObjectType = 8197,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 2,
	containerVolumeLimit = 20,
	customizationVariableMapping = {},

	detailedDescription = "@container_name:cargo_pocket",

	gameObjectType = 8197,

	locationReservationRadius = 0,
	lookAtText = "@container_lookat:base_container",

	noBuildRadius = 0,

	objectName = "@container_name:cargo_pocket",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	clientObjectCRC = 3378300955,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/container/base/shared_base_container.iff", "object/tangible/container/base/shared_base_container_volume.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_tangible_container_general_shared_cargo_pocket, "object/tangible/container/general/shared_cargo_pocket.iff")

object_tangible_container_general_shared_satchel = SharedTangibleObjectTemplate:new {
	clientTemplateFileName = "object/tangible/container/general/shared_satchel.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/edb_con_bag_ingredients_sm_s1.apt",
	arrangementDescriptorFilename = "",

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "",
	clientGameObjectType = 8197,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 2,
	containerVolumeLimit = 20,
	customizationVariableMapping = {},

	detailedDescription = "@container_name:satchel",

	gameObjectType = 8197,

	locationReservationRadius = 0,
	lookAtText = "@container_lookat:base_container",

	noBuildRadius = 0,

	objectName = "@container_name:satchel",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	clientObjectCRC = 2917538,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/container/base/shared_base_container.iff", "object/tangible/container/base/shared_base_container_volume.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_tangible_container_general_shared_satchel, "object/tangible/container/general/shared_satchel.iff")

object_tangible_container_general_shared_tech_armoire = SharedTangibleObjectTemplate:new {
	clientTemplateFileName = "object/tangible/container/general/shared_tech_armoire.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/frn_all_tech_armoire_s01.apt",
	arrangementDescriptorFilename = "",

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "",
	clientGameObjectType = 8197,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 2,
	containerVolumeLimit = 20,
	customizationVariableMapping = {},

	detailedDescription = "@container_name:tech_armoire",

	gameObjectType = 8197,

	locationReservationRadius = 0,
	lookAtText = "@container_lookat:base_container",

	noBuildRadius = 0,

	objectName = "@container_name:tech_armoire",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	clientObjectCRC = 997788135,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/container/base/shared_base_container.iff", "object/tangible/container/base/shared_base_container_volume.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_tangible_container_general_shared_tech_armoire, "object/tangible/container/general/shared_tech_armoire.iff")

object_tangible_container_general_shared_tech_bookcase = SharedTangibleObjectTemplate:new {
	clientTemplateFileName = "object/tangible/container/general/shared_tech_bookcase.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/frn_all_tech_bookcase_s01.apt",
	arrangementDescriptorFilename = "",

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "",
	clientGameObjectType = 8197,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 2,
	containerVolumeLimit = 20,
	customizationVariableMapping = {},

	detailedDescription = "@container_name:tech_bookcase",

	gameObjectType = 8197,

	locationReservationRadius = 0,
	lookAtText = "@container_lookat:base_container",

	noBuildRadius = 0,

	objectName = "@container_name:tech_bookcase",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	clientObjectCRC = 3363389476,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/container/base/shared_base_container.iff", "object/tangible/container/base/shared_base_container_volume.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_tangible_container_general_shared_tech_bookcase, "object/tangible/container/general/shared_tech_bookcase.iff")

object_tangible_container_general_shared_tech_chest = SharedTangibleObjectTemplate:new {
	clientTemplateFileName = "object/tangible/container/general/shared_tech_chest.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/frn_all_tech_chest_s01.apt",
	arrangementDescriptorFilename = "",

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "",
	clientGameObjectType = 8197,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 2,
	containerVolumeLimit = 20,
	customizationVariableMapping = {},

	detailedDescription = "@container_name:tech_chest",

	gameObjectType = 8197,

	locationReservationRadius = 0,
	lookAtText = "@container_lookat:base_container",

	noBuildRadius = 0,

	objectName = "@container_name:tech_chest",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	clientObjectCRC = 542811322,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/container/base/shared_base_container.iff", "object/tangible/container/base/shared_base_container_volume.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_tangible_container_general_shared_tech_chest, "object/tangible/container/general/shared_tech_chest.iff")
