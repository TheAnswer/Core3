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


object_tangible_wearables_armor_stormtrooper_shared_armor_stormtrooper_bicep_l = SharedTangibleObjectTemplate:new {
	clientTemplateFileName = "object/tangible/wearables/armor/stormtrooper/shared_armor_stormtrooper_bicep_l.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/armor_stormtrooper_bicep_l_f.sat",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/bicep_l.iff",

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "",
	clientGameObjectType = 261,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 0,
	containerVolumeLimit = 1,
	customizationVariableMapping = {},

	detailedDescription = "@wearables_detail:armor_stormtrooper_bicep_l",

	gameObjectType = 261,

	locationReservationRadius = 0,
	lookAtText = "@wearables_lookat:armor_stormtrooper_bicep_l",

	noBuildRadius = 0,

	objectName = "@wearables_name:armor_stormtrooper_bicep_l",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/tangible.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	clientObjectCRC = 1002440220,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/tangible/wearables/base/shared_wearables_base.iff", "object/tangible/wearables/base/shared_armor_base.iff", "object/tangible/wearables/base/shared_base_bicep_l.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_tangible_wearables_armor_stormtrooper_shared_armor_stormtrooper_bicep_l, "object/tangible/wearables/armor/stormtrooper/shared_armor_stormtrooper_bicep_l.iff")

object_tangible_wearables_armor_stormtrooper_shared_armor_stormtrooper_bicep_r = SharedTangibleObjectTemplate:new {
	clientTemplateFileName = "object/tangible/wearables/armor/stormtrooper/shared_armor_stormtrooper_bicep_r.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/armor_stormtrooper_bicep_r_f.sat",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/bicep_r.iff",

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "",
	clientGameObjectType = 261,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 0,
	containerVolumeLimit = 1,
	customizationVariableMapping = {},

	detailedDescription = "@wearables_detail:armor_stormtrooper_bicep_r",

	gameObjectType = 261,

	locationReservationRadius = 0,
	lookAtText = "@wearables_lookat:armor_stormtrooper_bicep_r",

	noBuildRadius = 0,

	objectName = "@wearables_name:armor_stormtrooper_bicep_r",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/tangible.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	clientObjectCRC = 1259684751,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/tangible/wearables/base/shared_wearables_base.iff", "object/tangible/wearables/base/shared_armor_base.iff", "object/tangible/wearables/base/shared_base_bicep_r.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_tangible_wearables_armor_stormtrooper_shared_armor_stormtrooper_bicep_r, "object/tangible/wearables/armor/stormtrooper/shared_armor_stormtrooper_bicep_r.iff")

object_tangible_wearables_armor_stormtrooper_shared_armor_stormtrooper_boots = SharedTangibleObjectTemplate:new {
	clientTemplateFileName = "object/tangible/wearables/armor/stormtrooper/shared_armor_stormtrooper_boots.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/armor_stormtrooper_boots_f.sat",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/shoe.iff",

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "",
	clientGameObjectType = 263,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 0,
	containerVolumeLimit = 1,
	customizationVariableMapping = {},

	detailedDescription = "@wearables_detail:armor_stormtrooper_boots",

	gameObjectType = 263,

	locationReservationRadius = 0,
	lookAtText = "@wearables_lookat:armor_stormtrooper_boots",

	noBuildRadius = 0,

	objectName = "@wearables_name:armor_stormtrooper_boots",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/tangible.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	clientObjectCRC = 2016668118,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/tangible/wearables/base/shared_wearables_base.iff", "object/tangible/wearables/base/shared_base_shoes.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_tangible_wearables_armor_stormtrooper_shared_armor_stormtrooper_boots, "object/tangible/wearables/armor/stormtrooper/shared_armor_stormtrooper_boots.iff")

object_tangible_wearables_armor_stormtrooper_shared_armor_stormtrooper_bracer_l = SharedTangibleObjectTemplate:new {
	clientTemplateFileName = "object/tangible/wearables/armor/stormtrooper/shared_armor_stormtrooper_bracer_l.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/armor_stormtrooper_bracer_l_f.sat",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/bracer_l.iff",

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "",
	clientGameObjectType = 261,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 0,
	containerVolumeLimit = 1,
	customizationVariableMapping = {},

	detailedDescription = "@wearables_detail:armor_stormtrooper_bracer_l",

	gameObjectType = 261,

	locationReservationRadius = 0,
	lookAtText = "@wearables_lookat:armor_stormtrooper_bracer_l",

	noBuildRadius = 0,

	objectName = "@wearables_name:armor_stormtrooper_bracer_l",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/tangible.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	clientObjectCRC = 2538033013,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/tangible/wearables/base/shared_wearables_base.iff", "object/tangible/wearables/base/shared_armor_base.iff", "object/tangible/wearables/base/shared_base_bracer_l.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_tangible_wearables_armor_stormtrooper_shared_armor_stormtrooper_bracer_l, "object/tangible/wearables/armor/stormtrooper/shared_armor_stormtrooper_bracer_l.iff")

object_tangible_wearables_armor_stormtrooper_shared_armor_stormtrooper_bracer_r = SharedTangibleObjectTemplate:new {
	clientTemplateFileName = "object/tangible/wearables/armor/stormtrooper/shared_armor_stormtrooper_bracer_r.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/armor_stormtrooper_bracer_r_f.sat",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/bracer_r.iff",

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "",
	clientGameObjectType = 261,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 0,
	containerVolumeLimit = 1,
	customizationVariableMapping = {},

	detailedDescription = "@wearables_detail:armor_stormtrooper_bracer_r",

	gameObjectType = 261,

	locationReservationRadius = 0,
	lookAtText = "@wearables_lookat:armor_stormtrooper_bracer_r",

	noBuildRadius = 0,

	objectName = "@wearables_name:armor_stormtrooper_bracer_r",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/tangible.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	clientObjectCRC = 3885107942,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/tangible/wearables/base/shared_wearables_base.iff", "object/tangible/wearables/base/shared_armor_base.iff", "object/tangible/wearables/base/shared_base_bracer_r.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_tangible_wearables_armor_stormtrooper_shared_armor_stormtrooper_bracer_r, "object/tangible/wearables/armor/stormtrooper/shared_armor_stormtrooper_bracer_r.iff")

object_tangible_wearables_armor_stormtrooper_shared_armor_stormtrooper_chest_plate = SharedTangibleObjectTemplate:new {
	clientTemplateFileName = "object/tangible/wearables/armor/stormtrooper/shared_armor_stormtrooper_chest_plate.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/armor_stormtrooper_chest_plate_f.sat",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/vest.iff",

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "",
	clientGameObjectType = 257,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 0,
	containerVolumeLimit = 1,
	customizationVariableMapping = {},

	detailedDescription = "@wearables_detail:armor_stormtrooper_chest_plate",

	gameObjectType = 257,

	locationReservationRadius = 0,
	lookAtText = "@wearables_lookat:armor_stormtrooper_chest_plate",

	noBuildRadius = 0,

	objectName = "@wearables_name:armor_stormtrooper_chest_plate",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/tangible.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	clientObjectCRC = 253917454,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/tangible/wearables/base/shared_wearables_base.iff", "object/tangible/wearables/base/shared_base_vest.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_tangible_wearables_armor_stormtrooper_shared_armor_stormtrooper_chest_plate, "object/tangible/wearables/armor/stormtrooper/shared_armor_stormtrooper_chest_plate.iff")

object_tangible_wearables_armor_stormtrooper_shared_armor_stormtrooper_chest_plate_quest = SharedTangibleObjectTemplate:new {
	clientTemplateFileName = "object/tangible/wearables/armor/stormtrooper/shared_armor_stormtrooper_chest_plate_quest.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/armor_stormtrooper_chest_plate_f.sat",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/vest.iff",

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "",
	clientGameObjectType = 257,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 0,
	containerVolumeLimit = 1,
	customizationVariableMapping = {},

	detailedDescription = "@wearables_detail:armor_stormtrooper_chest_plate_quest",

	gameObjectType = 257,

	locationReservationRadius = 0,
	lookAtText = "@wearables_lookat:armor_stormtrooper_chest_plate",

	noBuildRadius = 0,

	objectName = "@wearables_name:armor_stormtrooper_chest_plate_quest",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/tangible.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	clientObjectCRC = 1122862762,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/tangible/wearables/base/shared_wearables_base.iff", "object/tangible/wearables/base/shared_base_vest.iff", "object/tangible/wearables/armor/stormtrooper/shared_armor_stormtrooper_chest_plate.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_tangible_wearables_armor_stormtrooper_shared_armor_stormtrooper_chest_plate_quest, "object/tangible/wearables/armor/stormtrooper/shared_armor_stormtrooper_chest_plate_quest.iff")

object_tangible_wearables_armor_stormtrooper_shared_armor_stormtrooper_gloves = SharedTangibleObjectTemplate:new {
	clientTemplateFileName = "object/tangible/wearables/armor/stormtrooper/shared_armor_stormtrooper_gloves.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/armor_stormtrooper_gloves_m.sat",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/gauntlets.iff",

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "",
	clientGameObjectType = 262,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 0,
	containerVolumeLimit = 1,
	customizationVariableMapping = {},

	detailedDescription = "@wearables_detail:armor_stormtrooper_gloves",

	gameObjectType = 262,

	locationReservationRadius = 0,
	lookAtText = "@wearables_lookat:armor_stormtrooper_gloves",

	noBuildRadius = 0,

	objectName = "@wearables_name:armor_stormtrooper_gloves",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/tangible.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	clientObjectCRC = 1918468725,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/tangible/wearables/base/shared_wearables_base.iff", "object/tangible/wearables/base/shared_armor_base.iff", "object/tangible/wearables/base/shared_base_gauntlets.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_tangible_wearables_armor_stormtrooper_shared_armor_stormtrooper_gloves, "object/tangible/wearables/armor/stormtrooper/shared_armor_stormtrooper_gloves.iff")

object_tangible_wearables_armor_stormtrooper_shared_armor_stormtrooper_helmet = SharedTangibleObjectTemplate:new {
	clientTemplateFileName = "object/tangible/wearables/armor/stormtrooper/shared_armor_stormtrooper_helmet.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/armor_stormtrooper_helmet_f.sat",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/helmet_closed_full.iff",

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "",
	clientGameObjectType = 258,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 0,
	containerVolumeLimit = 1,
	customizationVariableMapping = {},

	detailedDescription = "@wearables_detail:armor_stormtrooper_helmet",

	gameObjectType = 258,

	locationReservationRadius = 0,
	lookAtText = "@wearables_lookat:armor_stormtrooper_helmet",

	noBuildRadius = 0,

	objectName = "@wearables_name:armor_stormtrooper_helmet",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/tangible.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	clientObjectCRC = 3298386813,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/tangible/wearables/base/shared_wearables_base.iff", "object/tangible/wearables/base/shared_armor_base.iff", "object/tangible/wearables/base/shared_base_helmet_closed_full.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_tangible_wearables_armor_stormtrooper_shared_armor_stormtrooper_helmet, "object/tangible/wearables/armor/stormtrooper/shared_armor_stormtrooper_helmet.iff")

object_tangible_wearables_armor_stormtrooper_shared_armor_stormtrooper_helmet_quest = SharedTangibleObjectTemplate:new {
	clientTemplateFileName = "object/tangible/wearables/armor/stormtrooper/shared_armor_stormtrooper_helmet_quest.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/armor_stormtrooper_helmet_f.sat",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/helmet_closed_full.iff",

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "",
	clientGameObjectType = 258,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 0,
	containerVolumeLimit = 1,
	customizationVariableMapping = {},

	detailedDescription = "@wearables_detail:armor_stormtrooper_helmet_quest",

	gameObjectType = 258,

	locationReservationRadius = 0,
	lookAtText = "@wearables_lookat:armor_stormtrooper_helmet",

	noBuildRadius = 0,

	objectName = "@wearables_name:armor_stormtrooper_helmet_quest",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/tangible.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	clientObjectCRC = 880764068,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/tangible/wearables/base/shared_wearables_base.iff", "object/tangible/wearables/base/shared_armor_base.iff", "object/tangible/wearables/base/shared_base_helmet_closed_full.iff", "object/tangible/wearables/armor/stormtrooper/shared_armor_stormtrooper_helmet.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_tangible_wearables_armor_stormtrooper_shared_armor_stormtrooper_helmet_quest, "object/tangible/wearables/armor/stormtrooper/shared_armor_stormtrooper_helmet_quest.iff")

object_tangible_wearables_armor_stormtrooper_shared_armor_stormtrooper_leggings = SharedTangibleObjectTemplate:new {
	clientTemplateFileName = "object/tangible/wearables/armor/stormtrooper/shared_armor_stormtrooper_leggings.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/armor_stormtrooper_leggings_f.sat",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/skirt.iff",

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "",
	clientGameObjectType = 260,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 0,
	containerVolumeLimit = 1,
	customizationVariableMapping = {},

	detailedDescription = "@wearables_detail:armor_stormtrooper_leggings",

	gameObjectType = 260,

	locationReservationRadius = 0,
	lookAtText = "@wearables_lookat:armor_stormtrooper_leggings",

	noBuildRadius = 0,

	objectName = "@wearables_name:armor_stormtrooper_leggings",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/tangible.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	clientObjectCRC = 409178730,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/tangible/wearables/base/shared_wearables_base.iff", "object/tangible/wearables/base/shared_base_skirt.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_tangible_wearables_armor_stormtrooper_shared_armor_stormtrooper_leggings, "object/tangible/wearables/armor/stormtrooper/shared_armor_stormtrooper_leggings.iff")

object_tangible_wearables_armor_stormtrooper_shared_armor_stormtrooper_leggings_quest = SharedTangibleObjectTemplate:new {
	clientTemplateFileName = "object/tangible/wearables/armor/stormtrooper/shared_armor_stormtrooper_leggings_quest.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/armor_stormtrooper_leggings_f.sat",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/skirt.iff",

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "",
	clientGameObjectType = 260,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 0,
	containerVolumeLimit = 1,
	customizationVariableMapping = {},

	detailedDescription = "@wearables_detail:armor_stormtrooper_leggings_quest",

	gameObjectType = 260,

	locationReservationRadius = 0,
	lookAtText = "@wearables_lookat:armor_stormtrooper_leggings",

	noBuildRadius = 0,

	objectName = "@wearables_name:armor_stormtrooper_leggings_quest",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/tangible.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	clientObjectCRC = 3027159239,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/tangible/wearables/base/shared_wearables_base.iff", "object/tangible/wearables/base/shared_base_skirt.iff", "object/tangible/wearables/armor/stormtrooper/shared_armor_stormtrooper_leggings.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_tangible_wearables_armor_stormtrooper_shared_armor_stormtrooper_leggings_quest, "object/tangible/wearables/armor/stormtrooper/shared_armor_stormtrooper_leggings_quest.iff")

object_tangible_wearables_armor_stormtrooper_shared_armor_stormtrooper_utility_belt = SharedTangibleObjectTemplate:new {
	clientTemplateFileName = "object/tangible/wearables/armor/stormtrooper/shared_armor_stormtrooper_utility_belt.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/armor_stormtrooper_utility_belt_m.sat",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/belt.iff",

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "",
	clientGameObjectType = 259,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 0,
	containerVolumeLimit = 1,
	customizationVariableMapping = {},

	detailedDescription = "@wearables_detail:armor_stormtrooper_utility_belt",

	gameObjectType = 259,

	locationReservationRadius = 0,
	lookAtText = "@wearables_lookat:armor_stormtrooper_utility_belt",

	noBuildRadius = 0,

	objectName = "@wearables_name:armor_stormtrooper_utility_belt",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/tangible.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	clientObjectCRC = 1212660794,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/tangible/wearables/base/shared_wearables_base.iff", "object/tangible/wearables/base/shared_base_belt.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_tangible_wearables_armor_stormtrooper_shared_armor_stormtrooper_utility_belt, "object/tangible/wearables/armor/stormtrooper/shared_armor_stormtrooper_utility_belt.iff")
