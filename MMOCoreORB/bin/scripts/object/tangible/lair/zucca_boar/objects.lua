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


object_tangible_lair_zucca_boar_shared_lair_zucca_boar = SharedTangibleObjectTemplate:new {
	clientTemplateFileName = "object/tangible/lair/zucca_boar/shared_lair_zucca_boar.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/poi_all_skeleton_human_headandbody.apt",
	arrangementDescriptorFilename = "",

	certificationsRequired = {},
	clearFloraRadius = 25,
	clientDataFile = "clientdata/lair/shared_poi_all_lair_bones.cdf",
	clientGameObjectType = 4,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 0,
	containerVolumeLimit = 1,
	customizationVariableMapping = {},

	detailedDescription = "@lair_d:zucca_boar",

	gameObjectType = 4,

	locationReservationRadius = 0,
	lookAtText = "string_id_table",

	noBuildRadius = 64,

	objectName = "@lair_n:zucca_boar",
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

	clientObjectCRC = 3052352906,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/lair/base/shared_lair_base.iff", "object/tangible/lair/base/shared_lair_base_cave_lion_light.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_tangible_lair_zucca_boar_shared_lair_zucca_boar, "object/tangible/lair/zucca_boar/shared_lair_zucca_boar.iff")

object_tangible_lair_zucca_boar_shared_lair_zucca_boar_desert = SharedTangibleObjectTemplate:new {
	clientTemplateFileName = "object/tangible/lair/zucca_boar/shared_lair_zucca_boar_desert.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/poi_all_skeleton_human_headandbody.apt",
	arrangementDescriptorFilename = "",

	certificationsRequired = {},
	clearFloraRadius = 25,
	clientDataFile = "clientdata/lair/shared_poi_all_lair_bones.cdf",
	clientGameObjectType = 4,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 0,
	containerVolumeLimit = 1,
	customizationVariableMapping = {},

	detailedDescription = "@lair_d:zucca_boar_desert",

	gameObjectType = 4,

	locationReservationRadius = 0,
	lookAtText = "string_id_table",

	noBuildRadius = 64,

	objectName = "@lair_n:zucca_boar_desert",
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

	clientObjectCRC = 415973727,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/lair/base/shared_lair_base.iff", "object/tangible/lair/base/shared_lair_base_cave_lion_light.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_tangible_lair_zucca_boar_shared_lair_zucca_boar_desert, "object/tangible/lair/zucca_boar/shared_lair_zucca_boar_desert.iff")

object_tangible_lair_zucca_boar_shared_lair_zucca_boar_dune = SharedTangibleObjectTemplate:new {
	clientTemplateFileName = "object/tangible/lair/zucca_boar/shared_lair_zucca_boar_dune.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/poi_all_skeleton_human_headandbody.apt",
	arrangementDescriptorFilename = "",

	certificationsRequired = {},
	clearFloraRadius = 25,
	clientDataFile = "clientdata/lair/shared_poi_all_lair_bones.cdf",
	clientGameObjectType = 4,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 0,
	containerVolumeLimit = 1,
	customizationVariableMapping = {},

	detailedDescription = "@lair_d:zucca_boar_dune",

	gameObjectType = 4,

	locationReservationRadius = 0,
	lookAtText = "string_id_table",

	noBuildRadius = 64,

	objectName = "@lair_n:zucca_boar_dune",
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

	clientObjectCRC = 3550514923,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/lair/base/shared_lair_base.iff", "object/tangible/lair/base/shared_lair_base_cave_lion_light.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_tangible_lair_zucca_boar_shared_lair_zucca_boar_dune, "object/tangible/lair/zucca_boar/shared_lair_zucca_boar_dune.iff")

object_tangible_lair_zucca_boar_shared_lair_zucca_boar_wasteland = SharedTangibleObjectTemplate:new {
	clientTemplateFileName = "object/tangible/lair/zucca_boar/shared_lair_zucca_boar_wasteland.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/poi_all_skeleton_human_headandbody.apt",
	arrangementDescriptorFilename = "",

	certificationsRequired = {},
	clearFloraRadius = 25,
	clientDataFile = "clientdata/lair/shared_poi_all_lair_bones.cdf",
	clientGameObjectType = 4,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 0,
	containerVolumeLimit = 1,
	customizationVariableMapping = {},

	detailedDescription = "@lair_d:zucca_boar_wasteland",

	gameObjectType = 4,

	locationReservationRadius = 0,
	lookAtText = "string_id_table",

	noBuildRadius = 64,

	objectName = "@lair_n:zucca_boar_wasteland",
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

	clientObjectCRC = 102331287,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/lair/base/shared_lair_base.iff", "object/tangible/lair/base/shared_lair_base_cave_lion_light.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_tangible_lair_zucca_boar_shared_lair_zucca_boar_wasteland, "object/tangible/lair/zucca_boar/shared_lair_zucca_boar_wasteland.iff")
