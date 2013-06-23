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


object_static_space_debris_shared_cargo_destroyed_large_a = SharedStaticObjectTemplate:new {
	clientTemplateFileName = "object/static/space/debris/shared_cargo_destroyed_large_a.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/cargo_debris_crate_s02.apt",
	arrangementDescriptorFilename = "",

	clearFloraRadius = 0,
	clientDataFile = "",
	clientGameObjectType = 536870915,
	collisionActionBlockFlags = 255,
	collisionActionFlags = 1,
	collisionActionPassFlags = 0,
	collisionMaterialBlockFlags = 1,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 0,
	containerVolumeLimit = 0,

	detailedDescription = "@string_table:pristine wall",

	gameObjectType = 536870915,

	locationReservationRadius = 0,
	lookAtText = "@string_table:pristine wall",

	noBuildRadius = 0,

	objectName = "@obj_n:unknown_object",
	onlyVisibleInTools = 0,

	portalLayoutFilename = "",

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 0,
	slotDescriptorFilename = "",
	snapToTerrain = 0,
	surfaceType = 1,

	totalCellNumber = 0,

	clientObjectCRC = 2590979892,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/static/base/shared_static_base.iff", "object/static/structure/base/shared_static_structure_base.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_static_space_debris_shared_cargo_destroyed_large_a, "object/static/space/debris/shared_cargo_destroyed_large_a.iff")

object_static_space_debris_shared_cargo_destroyed_small_a = SharedStaticObjectTemplate:new {
	clientTemplateFileName = "object/static/space/debris/shared_cargo_destroyed_small_a.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/cargo_debris_chunk_s01.apt",
	arrangementDescriptorFilename = "",

	clearFloraRadius = 0,
	clientDataFile = "",
	clientGameObjectType = 536870915,
	collisionActionBlockFlags = 255,
	collisionActionFlags = 1,
	collisionActionPassFlags = 0,
	collisionMaterialBlockFlags = 1,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 0,
	containerVolumeLimit = 0,

	detailedDescription = "@string_table:pristine wall",

	gameObjectType = 536870915,

	locationReservationRadius = 0,
	lookAtText = "@string_table:pristine wall",

	noBuildRadius = 0,

	objectName = "@obj_n:unknown_object",
	onlyVisibleInTools = 0,

	portalLayoutFilename = "",

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 0,
	slotDescriptorFilename = "",
	snapToTerrain = 0,
	surfaceType = 1,

	totalCellNumber = 0,

	clientObjectCRC = 483044281,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/static/base/shared_static_base.iff", "object/static/structure/base/shared_static_structure_base.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_static_space_debris_shared_cargo_destroyed_small_a, "object/static/space/debris/shared_cargo_destroyed_small_a.iff")

object_static_space_debris_shared_cargo_pristine_large_a = SharedStaticObjectTemplate:new {
	clientTemplateFileName = "object/static/space/debris/shared_cargo_pristine_large_a.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/cargo_debris_crate_s01.apt",
	arrangementDescriptorFilename = "",

	clearFloraRadius = 0,
	clientDataFile = "",
	clientGameObjectType = 536870915,
	collisionActionBlockFlags = 255,
	collisionActionFlags = 1,
	collisionActionPassFlags = 0,
	collisionMaterialBlockFlags = 1,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 0,
	containerVolumeLimit = 0,

	detailedDescription = "@string_table:pristine wall",

	gameObjectType = 536870915,

	locationReservationRadius = 0,
	lookAtText = "@string_table:pristine wall",

	noBuildRadius = 0,

	objectName = "@obj_n:unknown_object",
	onlyVisibleInTools = 0,

	portalLayoutFilename = "",

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "",
	snapToTerrain = 0,
	surfaceType = 1,

	totalCellNumber = 0,

	clientObjectCRC = 3524446459,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/static/base/shared_static_base.iff", "object/static/structure/base/shared_static_structure_base.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_static_space_debris_shared_cargo_pristine_large_a, "object/static/space/debris/shared_cargo_pristine_large_a.iff")

object_static_space_debris_shared_death_star_debris_a = SharedStaticObjectTemplate:new {
	clientTemplateFileName = "object/static/space/debris/shared_death_star_debris_a.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/dsdebris_chunksmalla.apt",
	arrangementDescriptorFilename = "",

	clearFloraRadius = 0,
	clientDataFile = "",
	clientGameObjectType = 536870915,
	collisionActionBlockFlags = 255,
	collisionActionFlags = 1,
	collisionActionPassFlags = 0,
	collisionMaterialBlockFlags = 1,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 0,
	containerVolumeLimit = 0,

	detailedDescription = "@string_table:pristine wall",

	gameObjectType = 536870915,

	locationReservationRadius = 0,
	lookAtText = "@string_table:pristine wall",

	noBuildRadius = 0,

	objectName = "@obj_n:unknown_object",
	onlyVisibleInTools = 0,

	portalLayoutFilename = "",

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 0,
	slotDescriptorFilename = "",
	snapToTerrain = 0,
	surfaceType = 1,

	totalCellNumber = 0,

	clientObjectCRC = 2077093717,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/static/base/shared_static_base.iff", "object/static/structure/base/shared_static_structure_base.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_static_space_debris_shared_death_star_debris_a, "object/static/space/debris/shared_death_star_debris_a.iff")

object_static_space_debris_shared_death_star_debris_b = SharedStaticObjectTemplate:new {
	clientTemplateFileName = "object/static/space/debris/shared_death_star_debris_b.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/poi_yavn_death_star_wreckage_s02.apt",
	arrangementDescriptorFilename = "",

	clearFloraRadius = 0,
	clientDataFile = "",
	clientGameObjectType = 536870915,
	collisionActionBlockFlags = 255,
	collisionActionFlags = 1,
	collisionActionPassFlags = 0,
	collisionMaterialBlockFlags = 1,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 0,
	containerVolumeLimit = 0,

	detailedDescription = "@string_table:pristine wall",

	gameObjectType = 536870915,

	locationReservationRadius = 0,
	lookAtText = "@string_table:pristine wall",

	noBuildRadius = 0,

	objectName = "@obj_n:unknown_object",
	onlyVisibleInTools = 0,

	portalLayoutFilename = "",

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 0,
	slotDescriptorFilename = "",
	snapToTerrain = 0,
	surfaceType = 1,

	totalCellNumber = 0,

	clientObjectCRC = 2698659778,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/static/base/shared_static_base.iff", "object/static/structure/base/shared_static_structure_base.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_static_space_debris_shared_death_star_debris_b, "object/static/space/debris/shared_death_star_debris_b.iff")

object_static_space_debris_shared_death_star_debris_c = SharedStaticObjectTemplate:new {
	clientTemplateFileName = "object/static/space/debris/shared_death_star_debris_c.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/poi_yavn_death_star_wreckage_s01.apt",
	arrangementDescriptorFilename = "",

	clearFloraRadius = 0,
	clientDataFile = "",
	clientGameObjectType = 536870915,
	collisionActionBlockFlags = 255,
	collisionActionFlags = 1,
	collisionActionPassFlags = 0,
	collisionMaterialBlockFlags = 1,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 0,
	containerVolumeLimit = 0,

	detailedDescription = "@string_table:pristine wall",

	gameObjectType = 536870915,

	locationReservationRadius = 0,
	lookAtText = "@string_table:pristine wall",

	noBuildRadius = 0,

	objectName = "@obj_n:unknown_object",
	onlyVisibleInTools = 0,

	portalLayoutFilename = "",

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 0,
	slotDescriptorFilename = "",
	snapToTerrain = 0,
	surfaceType = 1,

	totalCellNumber = 0,

	clientObjectCRC = 3923189839,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/static/base/shared_static_base.iff", "object/static/structure/base/shared_static_structure_base.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_static_space_debris_shared_death_star_debris_c, "object/static/space/debris/shared_death_star_debris_c.iff")

object_static_space_debris_shared_death_star_debris_d = SharedStaticObjectTemplate:new {
	clientTemplateFileName = "object/static/space/debris/shared_death_star_debris_d.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/poi_yavn_death_star_wreckage_s03.apt",
	arrangementDescriptorFilename = "",

	clearFloraRadius = 0,
	clientDataFile = "",
	clientGameObjectType = 536870915,
	collisionActionBlockFlags = 255,
	collisionActionFlags = 1,
	collisionActionPassFlags = 0,
	collisionMaterialBlockFlags = 1,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 0,
	containerVolumeLimit = 0,

	detailedDescription = "@string_table:pristine wall",

	gameObjectType = 536870915,

	locationReservationRadius = 0,
	lookAtText = "@string_table:pristine wall",

	noBuildRadius = 0,

	objectName = "@obj_n:unknown_object",
	onlyVisibleInTools = 0,

	portalLayoutFilename = "",

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 0,
	slotDescriptorFilename = "",
	snapToTerrain = 0,
	surfaceType = 1,

	totalCellNumber = 0,

	clientObjectCRC = 305400667,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/static/base/shared_static_base.iff", "object/static/structure/base/shared_static_structure_base.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_static_space_debris_shared_death_star_debris_d, "object/static/space/debris/shared_death_star_debris_d.iff")

object_static_space_debris_shared_death_star_debris_e = SharedStaticObjectTemplate:new {
	clientTemplateFileName = "object/static/space/debris/shared_death_star_debris_e.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/dsdebris_tubesmall.apt",
	arrangementDescriptorFilename = "",

	clearFloraRadius = 0,
	clientDataFile = "",
	clientGameObjectType = 536870915,
	collisionActionBlockFlags = 255,
	collisionActionFlags = 1,
	collisionActionPassFlags = 0,
	collisionMaterialBlockFlags = 1,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 0,
	containerVolumeLimit = 0,

	detailedDescription = "@string_table:pristine wall",

	gameObjectType = 536870915,

	locationReservationRadius = 0,
	lookAtText = "@string_table:pristine wall",

	noBuildRadius = 0,

	objectName = "@obj_n:unknown_object",
	onlyVisibleInTools = 0,

	portalLayoutFilename = "",

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 0,
	slotDescriptorFilename = "",
	snapToTerrain = 0,
	surfaceType = 1,

	totalCellNumber = 0,

	clientObjectCRC = 1530490070,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/static/base/shared_static_base.iff", "object/static/structure/base/shared_static_structure_base.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_static_space_debris_shared_death_star_debris_e, "object/static/space/debris/shared_death_star_debris_e.iff")

object_static_space_debris_shared_death_star_debris_f = SharedStaticObjectTemplate:new {
	clientTemplateFileName = "object/static/space/debris/shared_death_star_debris_f.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/dsdebris_tube.apt",
	arrangementDescriptorFilename = "",

	clearFloraRadius = 0,
	clientDataFile = "",
	clientGameObjectType = 536870915,
	collisionActionBlockFlags = 255,
	collisionActionFlags = 1,
	collisionActionPassFlags = 0,
	collisionMaterialBlockFlags = 1,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 0,
	containerVolumeLimit = 0,

	detailedDescription = "@string_table:pristine wall",

	gameObjectType = 536870915,

	locationReservationRadius = 0,
	lookAtText = "@string_table:pristine wall",

	noBuildRadius = 0,

	objectName = "@obj_n:unknown_object",
	onlyVisibleInTools = 0,

	portalLayoutFilename = "",

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 0,
	slotDescriptorFilename = "",
	snapToTerrain = 0,
	surfaceType = 1,

	totalCellNumber = 0,

	clientObjectCRC = 2150548545,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/static/base/shared_static_base.iff", "object/static/structure/base/shared_static_structure_base.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_static_space_debris_shared_death_star_debris_f, "object/static/space/debris/shared_death_star_debris_f.iff")

object_static_space_debris_shared_death_star_debris_g = SharedStaticObjectTemplate:new {
	clientTemplateFileName = "object/static/space/debris/shared_death_star_debris_g.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/dsdebris_conduit.apt",
	arrangementDescriptorFilename = "",

	clearFloraRadius = 0,
	clientDataFile = "",
	clientGameObjectType = 536870915,
	collisionActionBlockFlags = 255,
	collisionActionFlags = 1,
	collisionActionPassFlags = 0,
	collisionMaterialBlockFlags = 1,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 0,
	containerVolumeLimit = 0,

	detailedDescription = "@string_table:pristine wall",

	gameObjectType = 536870915,

	locationReservationRadius = 0,
	lookAtText = "@string_table:pristine wall",

	noBuildRadius = 0,

	objectName = "@obj_n:unknown_object",
	onlyVisibleInTools = 0,

	portalLayoutFilename = "",

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 0,
	slotDescriptorFilename = "",
	snapToTerrain = 0,
	surfaceType = 1,

	totalCellNumber = 0,

	clientObjectCRC = 3374556108,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/static/base/shared_static_base.iff", "object/static/structure/base/shared_static_structure_base.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_static_space_debris_shared_death_star_debris_g, "object/static/space/debris/shared_death_star_debris_g.iff")

object_static_space_debris_shared_death_star_debris_h = SharedStaticObjectTemplate:new {
	clientTemplateFileName = "object/static/space/debris/shared_death_star_debris_h.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/thm_prp_junk_s5.apt",
	arrangementDescriptorFilename = "",

	clearFloraRadius = 0,
	clientDataFile = "",
	clientGameObjectType = 536870915,
	collisionActionBlockFlags = 255,
	collisionActionFlags = 1,
	collisionActionPassFlags = 0,
	collisionMaterialBlockFlags = 1,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 0,
	containerVolumeLimit = 0,

	detailedDescription = "@string_table:pristine wall",

	gameObjectType = 536870915,

	locationReservationRadius = 0,
	lookAtText = "@string_table:pristine wall",

	noBuildRadius = 0,

	objectName = "@obj_n:unknown_object",
	onlyVisibleInTools = 0,

	portalLayoutFilename = "",

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 0,
	slotDescriptorFilename = "",
	snapToTerrain = 0,
	surfaceType = 1,

	totalCellNumber = 0,

	clientObjectCRC = 1932103646,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/static/base/shared_static_base.iff", "object/static/structure/base/shared_static_structure_base.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_static_space_debris_shared_death_star_debris_h, "object/static/space/debris/shared_death_star_debris_h.iff")

object_static_space_debris_shared_death_star_debris_i = SharedStaticObjectTemplate:new {
	clientTemplateFileName = "object/static/space/debris/shared_death_star_debris_i.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/thm_prp_junk_s6.apt",
	arrangementDescriptorFilename = "",

	clearFloraRadius = 0,
	clientDataFile = "",
	clientGameObjectType = 536870915,
	collisionActionBlockFlags = 255,
	collisionActionFlags = 1,
	collisionActionPassFlags = 0,
	collisionMaterialBlockFlags = 1,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 0,
	containerVolumeLimit = 0,

	detailedDescription = "@string_table:pristine wall",

	gameObjectType = 536870915,

	locationReservationRadius = 0,
	lookAtText = "@string_table:pristine wall",

	noBuildRadius = 0,

	objectName = "@obj_n:unknown_object",
	onlyVisibleInTools = 0,

	portalLayoutFilename = "",

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 0,
	slotDescriptorFilename = "",
	snapToTerrain = 0,
	surfaceType = 1,

	totalCellNumber = 0,

	clientObjectCRC = 975497299,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/static/base/shared_static_base.iff", "object/static/structure/base/shared_static_structure_base.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_static_space_debris_shared_death_star_debris_i, "object/static/space/debris/shared_death_star_debris_i.iff")

object_static_space_debris_shared_droid_fighter_debris_s01 = SharedStaticObjectTemplate:new {
	clientTemplateFileName = "object/static/space/debris/shared_droid_fighter_debris_s01.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/droid_fighter_debris_s01.apt",
	arrangementDescriptorFilename = "",

	clearFloraRadius = 0,
	clientDataFile = "",
	clientGameObjectType = 536870915,
	collisionActionBlockFlags = 255,
	collisionActionFlags = 1,
	collisionActionPassFlags = 0,
	collisionMaterialBlockFlags = 1,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 0,
	containerVolumeLimit = 0,

	detailedDescription = "@string_table:pristine wall",

	gameObjectType = 536870915,

	locationReservationRadius = 0,
	lookAtText = "@string_table:pristine wall",

	noBuildRadius = 0,

	objectName = "@obj_n:unknown_object",
	onlyVisibleInTools = 0,

	portalLayoutFilename = "",

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 0,
	slotDescriptorFilename = "",
	snapToTerrain = 0,
	surfaceType = 1,

	totalCellNumber = 0,

	clientObjectCRC = 3253175573,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/static/base/shared_static_base.iff", "object/static/structure/base/shared_static_structure_base.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_static_space_debris_shared_droid_fighter_debris_s01, "object/static/space/debris/shared_droid_fighter_debris_s01.iff")

object_static_space_debris_shared_droid_fighter_debris_s02 = SharedStaticObjectTemplate:new {
	clientTemplateFileName = "object/static/space/debris/shared_droid_fighter_debris_s02.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/droid_fighter_debris_s02.apt",
	arrangementDescriptorFilename = "",

	clearFloraRadius = 0,
	clientDataFile = "",
	clientGameObjectType = 536870915,
	collisionActionBlockFlags = 255,
	collisionActionFlags = 1,
	collisionActionPassFlags = 0,
	collisionMaterialBlockFlags = 1,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 0,
	containerVolumeLimit = 0,

	detailedDescription = "@string_table:pristine wall",

	gameObjectType = 536870915,

	locationReservationRadius = 0,
	lookAtText = "@string_table:pristine wall",

	noBuildRadius = 0,

	objectName = "@obj_n:unknown_object",
	onlyVisibleInTools = 0,

	portalLayoutFilename = "",

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 0,
	slotDescriptorFilename = "",
	snapToTerrain = 0,
	surfaceType = 1,

	totalCellNumber = 0,

	clientObjectCRC = 451947906,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/static/base/shared_static_base.iff", "object/static/structure/base/shared_static_structure_base.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_static_space_debris_shared_droid_fighter_debris_s02, "object/static/space/debris/shared_droid_fighter_debris_s02.iff")

object_static_space_debris_shared_droid_fighter_debris_s03 = SharedStaticObjectTemplate:new {
	clientTemplateFileName = "object/static/space/debris/shared_droid_fighter_debris_s03.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/droid_fighter_debris_s03.apt",
	arrangementDescriptorFilename = "",

	clearFloraRadius = 0,
	clientDataFile = "",
	clientGameObjectType = 536870915,
	collisionActionBlockFlags = 255,
	collisionActionFlags = 1,
	collisionActionPassFlags = 0,
	collisionMaterialBlockFlags = 1,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 0,
	containerVolumeLimit = 0,

	detailedDescription = "@string_table:pristine wall",

	gameObjectType = 536870915,

	locationReservationRadius = 0,
	lookAtText = "@string_table:pristine wall",

	noBuildRadius = 0,

	objectName = "@obj_n:unknown_object",
	onlyVisibleInTools = 0,

	portalLayoutFilename = "",

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 0,
	slotDescriptorFilename = "",
	snapToTerrain = 0,
	surfaceType = 1,

	totalCellNumber = 0,

	clientObjectCRC = 1409108495,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/static/base/shared_static_base.iff", "object/static/structure/base/shared_static_structure_base.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_static_space_debris_shared_droid_fighter_debris_s03, "object/static/space/debris/shared_droid_fighter_debris_s03.iff")

object_static_space_debris_shared_droid_fighter_debris_s04 = SharedStaticObjectTemplate:new {
	clientTemplateFileName = "object/static/space/debris/shared_droid_fighter_debris_s04.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/droid_fighter_debris_s04.apt",
	arrangementDescriptorFilename = "",

	clearFloraRadius = 0,
	clientDataFile = "",
	clientGameObjectType = 536870915,
	collisionActionBlockFlags = 255,
	collisionActionFlags = 1,
	collisionActionPassFlags = 0,
	collisionMaterialBlockFlags = 1,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 0,
	containerVolumeLimit = 0,

	detailedDescription = "@string_table:pristine wall",

	gameObjectType = 536870915,

	locationReservationRadius = 0,
	lookAtText = "@string_table:pristine wall",

	noBuildRadius = 0,

	objectName = "@obj_n:unknown_object",
	onlyVisibleInTools = 0,

	portalLayoutFilename = "",

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 0,
	slotDescriptorFilename = "",
	snapToTerrain = 0,
	surfaceType = 1,

	totalCellNumber = 0,

	clientObjectCRC = 2820563227,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/static/base/shared_static_base.iff", "object/static/structure/base/shared_static_structure_base.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_static_space_debris_shared_droid_fighter_debris_s04, "object/static/space/debris/shared_droid_fighter_debris_s04.iff")

object_static_space_debris_shared_tie_fighter_debris_a = SharedStaticObjectTemplate:new {
	clientTemplateFileName = "object/static/space/debris/shared_tie_fighter_debris_a.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/tie_fighter_debris_a.apt",
	arrangementDescriptorFilename = "",

	clearFloraRadius = 0,
	clientDataFile = "",
	clientGameObjectType = 536870915,
	collisionActionBlockFlags = 255,
	collisionActionFlags = 1,
	collisionActionPassFlags = 0,
	collisionMaterialBlockFlags = 1,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 0,
	containerVolumeLimit = 0,

	detailedDescription = "@string_table:pristine wall",

	gameObjectType = 536870915,

	locationReservationRadius = 0,
	lookAtText = "@string_table:pristine wall",

	noBuildRadius = 0,

	objectName = "@obj_n:unknown_object",
	onlyVisibleInTools = 0,

	portalLayoutFilename = "",

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 0,
	slotDescriptorFilename = "",
	snapToTerrain = 0,
	surfaceType = 1,

	totalCellNumber = 0,

	clientObjectCRC = 3891582202,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/static/base/shared_static_base.iff", "object/static/structure/base/shared_static_structure_base.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_static_space_debris_shared_tie_fighter_debris_a, "object/static/space/debris/shared_tie_fighter_debris_a.iff")

object_static_space_debris_shared_tie_fighter_debris_b = SharedStaticObjectTemplate:new {
	clientTemplateFileName = "object/static/space/debris/shared_tie_fighter_debris_b.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/tie_fighter_debris_b.apt",
	arrangementDescriptorFilename = "",

	clearFloraRadius = 0,
	clientDataFile = "",
	clientGameObjectType = 536870915,
	collisionActionBlockFlags = 255,
	collisionActionFlags = 1,
	collisionActionPassFlags = 0,
	collisionMaterialBlockFlags = 1,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 0,
	containerVolumeLimit = 0,

	detailedDescription = "@string_table:pristine wall",

	gameObjectType = 536870915,

	locationReservationRadius = 0,
	lookAtText = "@string_table:pristine wall",

	noBuildRadius = 0,

	objectName = "@obj_n:unknown_object",
	onlyVisibleInTools = 0,

	portalLayoutFilename = "",

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 0,
	slotDescriptorFilename = "",
	snapToTerrain = 0,
	surfaceType = 1,

	totalCellNumber = 0,

	clientObjectCRC = 1021541485,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/static/base/shared_static_base.iff", "object/static/structure/base/shared_static_structure_base.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_static_space_debris_shared_tie_fighter_debris_b, "object/static/space/debris/shared_tie_fighter_debris_b.iff")

object_static_space_debris_shared_tie_fighter_debris_c = SharedStaticObjectTemplate:new {
	clientTemplateFileName = "object/static/space/debris/shared_tie_fighter_debris_c.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/tie_fighter_debris_c.apt",
	arrangementDescriptorFilename = "",

	clearFloraRadius = 0,
	clientDataFile = "",
	clientGameObjectType = 536870915,
	collisionActionBlockFlags = 255,
	collisionActionFlags = 1,
	collisionActionPassFlags = 0,
	collisionMaterialBlockFlags = 1,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 0,
	containerVolumeLimit = 0,

	detailedDescription = "@string_table:pristine wall",

	gameObjectType = 536870915,

	locationReservationRadius = 0,
	lookAtText = "@string_table:pristine wall",

	noBuildRadius = 0,

	objectName = "@obj_n:unknown_object",
	onlyVisibleInTools = 0,

	portalLayoutFilename = "",

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 0,
	slotDescriptorFilename = "",
	snapToTerrain = 0,
	surfaceType = 1,

	totalCellNumber = 0,

	clientObjectCRC = 1978538976,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/static/base/shared_static_base.iff", "object/static/structure/base/shared_static_structure_base.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_static_space_debris_shared_tie_fighter_debris_c, "object/static/space/debris/shared_tie_fighter_debris_c.iff")

object_static_space_debris_shared_tie_fighter_debris_d = SharedStaticObjectTemplate:new {
	clientTemplateFileName = "object/static/space/debris/shared_tie_fighter_debris_d.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/tie_fighter_debris_d.apt",
	arrangementDescriptorFilename = "",

	clearFloraRadius = 0,
	clientDataFile = "",
	clientGameObjectType = 536870915,
	collisionActionBlockFlags = 255,
	collisionActionFlags = 1,
	collisionActionPassFlags = 0,
	collisionMaterialBlockFlags = 1,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 0,
	containerVolumeLimit = 0,

	detailedDescription = "@string_table:pristine wall",

	gameObjectType = 536870915,

	locationReservationRadius = 0,
	lookAtText = "@string_table:pristine wall",

	noBuildRadius = 0,

	objectName = "@obj_n:unknown_object",
	onlyVisibleInTools = 0,

	portalLayoutFilename = "",

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 0,
	slotDescriptorFilename = "",
	snapToTerrain = 0,
	surfaceType = 1,

	totalCellNumber = 0,

	clientObjectCRC = 2383229172,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/static/base/shared_static_base.iff", "object/static/structure/base/shared_static_structure_base.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_static_space_debris_shared_tie_fighter_debris_d, "object/static/space/debris/shared_tie_fighter_debris_d.iff")

object_static_space_debris_shared_tie_fighter_debris_e = SharedStaticObjectTemplate:new {
	clientTemplateFileName = "object/static/space/debris/shared_tie_fighter_debris_e.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/tie_fighter_debris_e.apt",
	arrangementDescriptorFilename = "",

	clearFloraRadius = 0,
	clientDataFile = "",
	clientGameObjectType = 536870915,
	collisionActionBlockFlags = 255,
	collisionActionFlags = 1,
	collisionActionPassFlags = 0,
	collisionMaterialBlockFlags = 1,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 0,
	containerVolumeLimit = 0,

	detailedDescription = "@string_table:pristine wall",

	gameObjectType = 536870915,

	locationReservationRadius = 0,
	lookAtText = "@string_table:pristine wall",

	noBuildRadius = 0,

	objectName = "@obj_n:unknown_object",
	onlyVisibleInTools = 0,

	portalLayoutFilename = "",

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 0,
	slotDescriptorFilename = "",
	snapToTerrain = 0,
	surfaceType = 1,

	totalCellNumber = 0,

	clientObjectCRC = 3338688377,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/static/base/shared_static_base.iff", "object/static/structure/base/shared_static_structure_base.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_static_space_debris_shared_tie_fighter_debris_e, "object/static/space/debris/shared_tie_fighter_debris_e.iff")

object_static_space_debris_shared_tie_fighter_debris_f = SharedStaticObjectTemplate:new {
	clientTemplateFileName = "object/static/space/debris/shared_tie_fighter_debris_f.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/tie_fighter_debris_f.apt",
	arrangementDescriptorFilename = "",

	clearFloraRadius = 0,
	clientDataFile = "",
	clientGameObjectType = 536870915,
	collisionActionBlockFlags = 255,
	collisionActionFlags = 1,
	collisionActionPassFlags = 0,
	collisionMaterialBlockFlags = 1,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 0,
	containerVolumeLimit = 0,

	detailedDescription = "@string_table:pristine wall",

	gameObjectType = 536870915,

	locationReservationRadius = 0,
	lookAtText = "@string_table:pristine wall",

	noBuildRadius = 0,

	objectName = "@obj_n:unknown_object",
	onlyVisibleInTools = 0,

	portalLayoutFilename = "",

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 0,
	slotDescriptorFilename = "",
	snapToTerrain = 0,
	surfaceType = 1,

	totalCellNumber = 0,

	clientObjectCRC = 471334894,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/static/base/shared_static_base.iff", "object/static/structure/base/shared_static_structure_base.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_static_space_debris_shared_tie_fighter_debris_f, "object/static/space/debris/shared_tie_fighter_debris_f.iff")

object_static_space_debris_shared_tie_fighter_debris_g = SharedStaticObjectTemplate:new {
	clientTemplateFileName = "object/static/space/debris/shared_tie_fighter_debris_g.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/poi_all_tie_bomber_debris_s01.apt",
	arrangementDescriptorFilename = "",

	clearFloraRadius = 0,
	clientDataFile = "",
	clientGameObjectType = 536870915,
	collisionActionBlockFlags = 255,
	collisionActionFlags = 1,
	collisionActionPassFlags = 0,
	collisionMaterialBlockFlags = 1,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 0,
	containerVolumeLimit = 0,

	detailedDescription = "@string_table:pristine wall",

	gameObjectType = 536870915,

	locationReservationRadius = 0,
	lookAtText = "@string_table:pristine wall",

	noBuildRadius = 0,

	objectName = "@obj_n:unknown_object",
	onlyVisibleInTools = 0,

	portalLayoutFilename = "",

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 0,
	slotDescriptorFilename = "",
	snapToTerrain = 0,
	surfaceType = 1,

	totalCellNumber = 0,

	clientObjectCRC = 1427806307,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/static/base/shared_static_base.iff", "object/static/structure/base/shared_static_structure_base.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_static_space_debris_shared_tie_fighter_debris_g, "object/static/space/debris/shared_tie_fighter_debris_g.iff")

object_static_space_debris_shared_tradefed_hulk_debris_s01 = SharedStaticObjectTemplate:new {
	clientTemplateFileName = "object/static/space/debris/shared_tradefed_hulk_debris_s01.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/debris_tradefed_hulk_radial_s01.apt",
	arrangementDescriptorFilename = "",

	clearFloraRadius = 0,
	clientDataFile = "",
	clientGameObjectType = 536870915,
	collisionActionBlockFlags = 255,
	collisionActionFlags = 1,
	collisionActionPassFlags = 0,
	collisionMaterialBlockFlags = 1,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 0,
	containerVolumeLimit = 0,

	detailedDescription = "@string_table:pristine wall",

	gameObjectType = 536870915,

	locationReservationRadius = 0,
	lookAtText = "@string_table:pristine wall",

	noBuildRadius = 0,

	objectName = "@obj_n:unknown_object",
	onlyVisibleInTools = 0,

	portalLayoutFilename = "",

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 0,
	slotDescriptorFilename = "",
	snapToTerrain = 0,
	surfaceType = 1,

	totalCellNumber = 0,

	clientObjectCRC = 3363541547,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/static/base/shared_static_base.iff", "object/static/structure/base/shared_static_structure_base.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_static_space_debris_shared_tradefed_hulk_debris_s01, "object/static/space/debris/shared_tradefed_hulk_debris_s01.iff")

object_static_space_debris_shared_tradefed_hulk_debris_s02 = SharedStaticObjectTemplate:new {
	clientTemplateFileName = "object/static/space/debris/shared_tradefed_hulk_debris_s02.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/debris_tradefed_hulk_radial_s02.apt",
	arrangementDescriptorFilename = "",

	clearFloraRadius = 0,
	clientDataFile = "",
	clientGameObjectType = 536870915,
	collisionActionBlockFlags = 255,
	collisionActionFlags = 1,
	collisionActionPassFlags = 0,
	collisionMaterialBlockFlags = 1,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 0,
	containerVolumeLimit = 0,

	detailedDescription = "@string_table:pristine wall",

	gameObjectType = 536870915,

	locationReservationRadius = 0,
	lookAtText = "@string_table:pristine wall",

	noBuildRadius = 0,

	objectName = "@obj_n:unknown_object",
	onlyVisibleInTools = 0,

	portalLayoutFilename = "",

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 0,
	slotDescriptorFilename = "",
	snapToTerrain = 0,
	surfaceType = 1,

	totalCellNumber = 0,

	clientObjectCRC = 325860028,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/static/base/shared_static_base.iff", "object/static/structure/base/shared_static_structure_base.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_static_space_debris_shared_tradefed_hulk_debris_s02, "object/static/space/debris/shared_tradefed_hulk_debris_s02.iff")

object_static_space_debris_shared_tradefed_hulk_debris_s03 = SharedStaticObjectTemplate:new {
	clientTemplateFileName = "object/static/space/debris/shared_tradefed_hulk_debris_s03.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/debris_tradefed_hulk_radial_s03.apt",
	arrangementDescriptorFilename = "",

	clearFloraRadius = 0,
	clientDataFile = "",
	clientGameObjectType = 536870915,
	collisionActionBlockFlags = 255,
	collisionActionFlags = 1,
	collisionActionPassFlags = 0,
	collisionMaterialBlockFlags = 1,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 0,
	containerVolumeLimit = 0,

	detailedDescription = "@string_table:pristine wall",

	gameObjectType = 536870915,

	locationReservationRadius = 0,
	lookAtText = "@string_table:pristine wall",

	noBuildRadius = 0,

	objectName = "@obj_n:unknown_object",
	onlyVisibleInTools = 0,

	portalLayoutFilename = "",

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 0,
	slotDescriptorFilename = "",
	snapToTerrain = 0,
	surfaceType = 1,

	totalCellNumber = 0,

	clientObjectCRC = 1516330289,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/static/base/shared_static_base.iff", "object/static/structure/base/shared_static_structure_base.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_static_space_debris_shared_tradefed_hulk_debris_s03, "object/static/space/debris/shared_tradefed_hulk_debris_s03.iff")

object_static_space_debris_shared_tradefed_hulk_debris_s04 = SharedStaticObjectTemplate:new {
	clientTemplateFileName = "object/static/space/debris/shared_tradefed_hulk_debris_s04.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/debris_tradefed_hulk_radial_s04.apt",
	arrangementDescriptorFilename = "",

	clearFloraRadius = 0,
	clientDataFile = "",
	clientGameObjectType = 536870915,
	collisionActionBlockFlags = 255,
	collisionActionFlags = 1,
	collisionActionPassFlags = 0,
	collisionMaterialBlockFlags = 1,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 0,
	containerVolumeLimit = 0,

	detailedDescription = "@string_table:pristine wall",

	gameObjectType = 536870915,

	locationReservationRadius = 0,
	lookAtText = "@string_table:pristine wall",

	noBuildRadius = 0,

	objectName = "@obj_n:unknown_object",
	onlyVisibleInTools = 0,

	portalLayoutFilename = "",

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 0,
	slotDescriptorFilename = "",
	snapToTerrain = 0,
	surfaceType = 1,

	totalCellNumber = 0,

	clientObjectCRC = 2709681701,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/static/base/shared_static_base.iff", "object/static/structure/base/shared_static_structure_base.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_static_space_debris_shared_tradefed_hulk_debris_s04, "object/static/space/debris/shared_tradefed_hulk_debris_s04.iff")

object_static_space_debris_shared_xwing_debris_a = SharedStaticObjectTemplate:new {
	clientTemplateFileName = "object/static/space/debris/shared_xwing_debris_a.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/xwing_debris_a.apt",
	arrangementDescriptorFilename = "",

	clearFloraRadius = 0,
	clientDataFile = "",
	clientGameObjectType = 536870915,
	collisionActionBlockFlags = 255,
	collisionActionFlags = 1,
	collisionActionPassFlags = 0,
	collisionMaterialBlockFlags = 1,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 0,
	containerVolumeLimit = 0,

	detailedDescription = "@string_table:pristine wall",

	gameObjectType = 536870915,

	locationReservationRadius = 0,
	lookAtText = "@string_table:pristine wall",

	noBuildRadius = 0,

	objectName = "@obj_n:unknown_object",
	onlyVisibleInTools = 0,

	portalLayoutFilename = "",

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 0,
	slotDescriptorFilename = "",
	snapToTerrain = 0,
	surfaceType = 1,

	totalCellNumber = 0,

	clientObjectCRC = 3548707546,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/static/base/shared_static_base.iff", "object/static/structure/base/shared_static_structure_base.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_static_space_debris_shared_xwing_debris_a, "object/static/space/debris/shared_xwing_debris_a.iff")

object_static_space_debris_shared_xwing_debris_b = SharedStaticObjectTemplate:new {
	clientTemplateFileName = "object/static/space/debris/shared_xwing_debris_b.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/xwing_debris_b.apt",
	arrangementDescriptorFilename = "",

	clearFloraRadius = 0,
	clientDataFile = "",
	clientGameObjectType = 536870915,
	collisionActionBlockFlags = 255,
	collisionActionFlags = 1,
	collisionActionPassFlags = 0,
	collisionMaterialBlockFlags = 1,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 0,
	containerVolumeLimit = 0,

	detailedDescription = "@string_table:pristine wall",

	gameObjectType = 536870915,

	locationReservationRadius = 0,
	lookAtText = "@string_table:pristine wall",

	noBuildRadius = 0,

	objectName = "@obj_n:unknown_object",
	onlyVisibleInTools = 0,

	portalLayoutFilename = "",

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 0,
	slotDescriptorFilename = "",
	snapToTerrain = 0,
	surfaceType = 1,

	totalCellNumber = 0,

	clientObjectCRC = 143872589,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/static/base/shared_static_base.iff", "object/static/structure/base/shared_static_structure_base.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_static_space_debris_shared_xwing_debris_b, "object/static/space/debris/shared_xwing_debris_b.iff")

object_static_space_debris_shared_xwing_debris_c = SharedStaticObjectTemplate:new {
	clientTemplateFileName = "object/static/space/debris/shared_xwing_debris_c.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/xwing_debris_c.apt",
	arrangementDescriptorFilename = "",

	clearFloraRadius = 0,
	clientDataFile = "",
	clientGameObjectType = 536870915,
	collisionActionBlockFlags = 255,
	collisionActionFlags = 1,
	collisionActionPassFlags = 0,
	collisionMaterialBlockFlags = 1,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 0,
	containerVolumeLimit = 0,

	detailedDescription = "@string_table:pristine wall",

	gameObjectType = 536870915,

	locationReservationRadius = 0,
	lookAtText = "@string_table:pristine wall",

	noBuildRadius = 0,

	objectName = "@obj_n:unknown_object",
	onlyVisibleInTools = 0,

	portalLayoutFilename = "",

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 0,
	slotDescriptorFilename = "",
	snapToTerrain = 0,
	surfaceType = 1,

	totalCellNumber = 0,

	clientObjectCRC = 1100887488,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/static/base/shared_static_base.iff", "object/static/structure/base/shared_static_structure_base.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_static_space_debris_shared_xwing_debris_c, "object/static/space/debris/shared_xwing_debris_c.iff")

object_static_space_debris_shared_xwing_debris_d = SharedStaticObjectTemplate:new {
	clientTemplateFileName = "object/static/space/debris/shared_xwing_debris_d.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/xwing_debris_d.apt",
	arrangementDescriptorFilename = "",

	clearFloraRadius = 0,
	clientDataFile = "",
	clientGameObjectType = 536870915,
	collisionActionBlockFlags = 255,
	collisionActionFlags = 1,
	collisionActionPassFlags = 0,
	collisionMaterialBlockFlags = 1,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 0,
	containerVolumeLimit = 0,

	detailedDescription = "@string_table:pristine wall",

	gameObjectType = 536870915,

	locationReservationRadius = 0,
	lookAtText = "@string_table:pristine wall",

	noBuildRadius = 0,

	objectName = "@obj_n:unknown_object",
	onlyVisibleInTools = 0,

	portalLayoutFilename = "",

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 0,
	slotDescriptorFilename = "",
	snapToTerrain = 0,
	surfaceType = 1,

	totalCellNumber = 0,

	clientObjectCRC = 3128762068,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/static/base/shared_static_base.iff", "object/static/structure/base/shared_static_structure_base.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_static_space_debris_shared_xwing_debris_d, "object/static/space/debris/shared_xwing_debris_d.iff")

object_static_space_debris_shared_xwing_debris_e = SharedStaticObjectTemplate:new {
	clientTemplateFileName = "object/static/space/debris/shared_xwing_debris_e.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/xwing_debris_e.apt",
	arrangementDescriptorFilename = "",

	clearFloraRadius = 0,
	clientDataFile = "",
	clientGameObjectType = 536870915,
	collisionActionBlockFlags = 255,
	collisionActionFlags = 1,
	collisionActionPassFlags = 0,
	collisionMaterialBlockFlags = 1,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 0,
	containerVolumeLimit = 0,

	detailedDescription = "@string_table:pristine wall",

	gameObjectType = 536870915,

	locationReservationRadius = 0,
	lookAtText = "@string_table:pristine wall",

	noBuildRadius = 0,

	objectName = "@obj_n:unknown_object",
	onlyVisibleInTools = 0,

	portalLayoutFilename = "",

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 0,
	slotDescriptorFilename = "",
	snapToTerrain = 0,
	surfaceType = 1,

	totalCellNumber = 0,

	clientObjectCRC = 4084234585,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/static/base/shared_static_base.iff", "object/static/structure/base/shared_static_structure_base.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_static_space_debris_shared_xwing_debris_e, "object/static/space/debris/shared_xwing_debris_e.iff")

object_static_space_debris_shared_xwing_debris_f = SharedStaticObjectTemplate:new {
	clientTemplateFileName = "object/static/space/debris/shared_xwing_debris_f.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/xwing_debris_f.apt",
	arrangementDescriptorFilename = "",

	clearFloraRadius = 0,
	clientDataFile = "",
	clientGameObjectType = 536870915,
	collisionActionBlockFlags = 255,
	collisionActionFlags = 1,
	collisionActionPassFlags = 0,
	collisionMaterialBlockFlags = 1,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 0,
	containerVolumeLimit = 0,

	detailedDescription = "@string_table:pristine wall",

	gameObjectType = 536870915,

	locationReservationRadius = 0,
	lookAtText = "@string_table:pristine wall",

	noBuildRadius = 0,

	objectName = "@obj_n:unknown_object",
	onlyVisibleInTools = 0,

	portalLayoutFilename = "",

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 0,
	slotDescriptorFilename = "",
	snapToTerrain = 0,
	surfaceType = 1,

	totalCellNumber = 0,

	clientObjectCRC = 677892558,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/static/base/shared_static_base.iff", "object/static/structure/base/shared_static_structure_base.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_static_space_debris_shared_xwing_debris_f, "object/static/space/debris/shared_xwing_debris_f.iff")

object_static_space_debris_shared_xwing_debris_g = SharedStaticObjectTemplate:new {
	clientTemplateFileName = "object/static/space/debris/shared_xwing_debris_g.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/xwing_debris_g.apt",
	arrangementDescriptorFilename = "",

	clearFloraRadius = 0,
	clientDataFile = "",
	clientGameObjectType = 536870915,
	collisionActionBlockFlags = 255,
	collisionActionFlags = 1,
	collisionActionPassFlags = 0,
	collisionMaterialBlockFlags = 1,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 0,
	containerVolumeLimit = 0,

	detailedDescription = "@string_table:pristine wall",

	gameObjectType = 536870915,

	locationReservationRadius = 0,
	lookAtText = "@string_table:pristine wall",

	noBuildRadius = 0,

	objectName = "@obj_n:unknown_object",
	onlyVisibleInTools = 0,

	portalLayoutFilename = "",

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 0,
	slotDescriptorFilename = "",
	snapToTerrain = 0,
	surfaceType = 1,

	totalCellNumber = 0,

	clientObjectCRC = 1634383427,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/static/base/shared_static_base.iff", "object/static/structure/base/shared_static_structure_base.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_static_space_debris_shared_xwing_debris_g, "object/static/space/debris/shared_xwing_debris_g.iff")
