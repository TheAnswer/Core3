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


object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_gen1 = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_gen1.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_anakin.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber_gen1",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber_gen1",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_gen1",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 225373638,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_gen1, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_gen1.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_gen2 = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_gen2.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_anakin.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber_gen2",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber_gen2",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_gen2",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 3598271825,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_gen2, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_gen2.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_gen3 = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_gen3.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_anakin.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber_gen3",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber_gen3",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_gen3",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 2675188444,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_gen3, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_gen3.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_gen4 = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_gen4.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_anakin.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber_gen4",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber_gen4",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_gen4",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 1687620040,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_gen4, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_gen4.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_gen5 = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_gen5.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_anakin.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber_gen5",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber_gen5",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_gen5",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 765095493,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_gen5, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_gen5.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s1 = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s1.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_anakin.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type1",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 1955018838,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s1, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s1.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s10 = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s10.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_quigon.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type10",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 3319201365,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s10, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s10.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s10_gen1 = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s10_gen1.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_quigon.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber_gen1",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type10",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 974526116,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff", "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_gen1.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s10_gen1, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s10_gen1.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s10_gen2 = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s10_gen2.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_quigon.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber_gen2",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type10",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 3774987827,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff", "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_gen2.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s10_gen2, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s10_gen2.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s10_gen3 = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s10_gen3.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_quigon.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber_gen3",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type10",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 2819414462,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff", "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_gen3.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s10_gen3, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s10_gen3.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s10_gen4 = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s10_gen4.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_quigon.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber_gen4",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type10",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 1408234154,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff", "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_gen4.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s10_gen4, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s10_gen4.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s11 = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s11.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_sleekblack.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type11",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 2363202008,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s11, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s11.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s11_gen1 = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s11_gen1.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_sleekblack.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber_gen1",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type11",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 3129390787,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff", "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_gen1.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s11_gen1, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s11_gen1.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s11_gen2 = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s11_gen2.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_sleekblack.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber_gen2",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type11",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 1636900436,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff", "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_gen2.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s11_gen2, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s11_gen2.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s11_gen3 = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s11_gen3.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_sleekblack.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber_gen3",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type11",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 681343449,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff", "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_gen3.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s11_gen3, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s11_gen3.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s11_gen4 = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s11_gen4.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_sleekblack.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber_gen4",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type11",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 3548336845,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff", "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_gen4.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s11_gen4, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s11_gen4.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s12 = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s12.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_sleeksilver.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type12",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 1472999759,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s12, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s12.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s12_gen1 = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s12_gen1.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_sleeksilver.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber_gen1",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type12",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 1073105885,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff", "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_gen1.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s12_gen1, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s12_gen1.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s12_gen2 = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s12_gen2.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_sleeksilver.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber_gen2",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type12",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 3840009034,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff", "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_gen2.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s12_gen2, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s12_gen2.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s12_gen3 = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s12_gen3.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_sleeksilver.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber_gen3",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type12",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 2917958855,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff", "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_gen3.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s12_gen3, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s12_gen3.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s12_gen4 = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s12_gen4.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_sleeksilver.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber_gen4",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type12",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 1443868627,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff", "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_gen4.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s12_gen4, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s12_gen4.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s13 = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s13.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_vader.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type13",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 515987138,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s13, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s13.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s13_gen1 = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s13_gen1.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_vader.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber_gen1",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type13",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 3211193274,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff", "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_gen1.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s13_gen1, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s13_gen1.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s13_gen2 = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s13_gen2.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_vader.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber_gen2",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type13",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 1685144365,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff", "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_gen2.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s13_gen2, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s13_gen2.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s13_gen3 = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s13_gen3.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_vader.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber_gen3",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type13",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 763110560,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff", "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_gen3.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s13_gen3, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s13_gen3.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s13_gen4 = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s13_gen4.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_vader.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber_gen4",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type13",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 3600748468,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff", "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_gen4.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s13_gen4, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s13_gen4.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s1_gen1 = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s1_gen1.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_anakin.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber_gen1",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type1",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 3893856758,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff", "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_gen1.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s1_gen1, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s1_gen1.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s1_gen2 = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s1_gen2.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_anakin.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber_gen2",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type1",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 855646561,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff", "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_gen2.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s1_gen2, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s1_gen2.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s1_gen3 = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s1_gen3.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_anakin.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber_gen3",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type1",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 2047690476,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff", "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_gen3.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s1_gen3, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s1_gen3.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s1_gen4 = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s1_gen4.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_anakin.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber_gen4",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type1",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 2179886584,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff", "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_gen4.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s1_gen4, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s1_gen4.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s2 = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s2.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_andael.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type2",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 2945487041,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s2, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s2.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s2_gen1 = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s2_gen1.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_andael.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber_gen1",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type2",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 1835496680,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff", "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_gen1.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s2_gen1, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s2_gen1.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s2_gen2 = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s2_gen2.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_andael.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber_gen2",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type2",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 3060849791,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff", "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_gen2.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s2_gen2, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s2_gen2.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s2_gen3 = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s2_gen3.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_andael.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber_gen3",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type2",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 4286430194,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff", "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_gen3.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s2_gen3, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s2_gen3.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s2_gen4 = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s2_gen4.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_andael.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber_gen4",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type2",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 77500646,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff", "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_gen4.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s2_gen4, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s2_gen4.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s3 = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s3.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_ben.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type3",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 3869112140,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s3, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s3.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s3_gen1 = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s3_gen1.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_ben.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber_gen1",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type3",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 3992441999,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff", "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_gen1.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s3_gen1, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s3_gen1.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s3_gen2 = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s3_gen2.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_ben.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber_gen2",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type3",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 920681496,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff", "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_gen2.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s3_gen2, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s3_gen2.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s3_gen3 = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s3_gen3.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_ben.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber_gen3",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type3",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 2146245525,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff", "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_gen3.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s3_gen3, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s3_gen3.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s3_gen4 = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s3_gen4.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_ben.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber_gen4",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type3",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 2215522433,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff", "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_gen4.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s3_gen4, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s3_gen4.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s4 = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s4.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_dugald.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type4",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 494853208,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s4, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s4.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s4_gen1 = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s4_gen1.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_dugald.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber_gen1",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type4",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 1665635171,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff", "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_gen1.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s4_gen1, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s4_gen1.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s4_gen2 = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s4_gen2.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_dugald.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber_gen2",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type4",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 3092265972,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff", "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_gen2.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s4_gen2, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s4_gen2.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s4_gen3 = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s4_gen3.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_dugald.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber_gen3",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type4",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 4049427577,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff", "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_gen3.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s4_gen3, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s4_gen3.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s4_gen4 = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s4_gen4.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_dugald.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber_gen4",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type4",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 180254573,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff", "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_gen4.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s4_gen4, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s4_gen4.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s5 = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s5.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_leather.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type5",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 1416870869,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s5, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s5.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s5_gen1 = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s5_gen1.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_leather.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber_gen1",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type5",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 3822531332,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff", "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_gen1.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s5_gen1, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s5_gen1.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s5_gen2 = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s5_gen2.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_leather.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber_gen2",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type5",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 952146835,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff", "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_gen2.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s5_gen2, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s5_gen2.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s5_gen3 = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s5_gen3.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_leather.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber_gen3",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type5",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 1909324830,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff", "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_gen3.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s5_gen3, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s5_gen3.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s5_gen4 = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s5_gen4.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_leather.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber_gen4",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type5",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 2318325514,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff", "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_gen4.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s5_gen4, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s5_gen4.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s6 = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s6.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_luke.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type6",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 2405700418,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s6, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s6.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s6_gen1 = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s6_gen1.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_luke.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber_gen1",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type6",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 1722273306,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff", "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_gen1.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s6_gen1, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s6_gen1.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s6_gen2 = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s6_gen2.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_luke.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber_gen2",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type6",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 3182454413,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff", "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_gen2.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s6_gen2, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s6_gen2.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s6_gen3 = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s6_gen3.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_luke.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber_gen3",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type6",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 4106027264,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff", "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_gen3.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s6_gen3, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s6_gen3.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s6_gen4 = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s6_gen4.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_luke.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber_gen4",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type6",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 257829396,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff", "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_gen4.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s6_gen4, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s6_gen4.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s7 = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s7.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_mace.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type7",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 3328799951,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s7, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s7.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s7_gen1 = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s7_gen1.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_mace.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber_gen1",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type7",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 3862392445,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff", "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_gen1.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s7_gen1, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s7_gen1.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s7_gen2 = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s7_gen2.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_mace.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber_gen2",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type7",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 1025558250,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff", "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_gen2.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s7_gen2, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s7_gen2.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s7_gen3 = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s7_gen3.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_mace.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber_gen3",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type7",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 1949147495,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff", "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_gen3.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s7_gen3, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s7_gen3.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s7_gen4 = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s7_gen4.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_mace.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber_gen4",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type7",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 2412677747,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff", "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_gen4.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s7_gen4, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s7_gen4.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s8 = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s8.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_nebu.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type8",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 2086885597,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s8, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s8.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s8_gen1 = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s8_gen1.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_nebu.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber_gen1",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type8",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 2131123317,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff", "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_gen1.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s8_gen1, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s8_gen1.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s8_gen2 = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s8_gen2.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_nebu.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber_gen2",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type8",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 2752640226,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff", "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_gen2.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s8_gen2, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s8_gen2.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s8_gen3 = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s8_gen3.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_nebu.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber_gen3",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type8",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 3978072943,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff", "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_gen3.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s8_gen3, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s8_gen3.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s8_gen4 = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s8_gen4.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_nebu.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber_gen4",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type8",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 385857659,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff", "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_gen4.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s8_gen4, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s8_gen4.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s9 = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s9.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_obi.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type9",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 896417616,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s9, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s9.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s9_gen1 = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s9_gen1.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_obi.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber_gen1",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type9",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 4288085010,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff", "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_gen1.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s9_gen1, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s9_gen1.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s9_gen2 = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s9_gen2.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_obi.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber_gen2",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type9",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 612455557,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff", "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_gen2.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s9_gen2, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s9_gen2.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s9_gen3 = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s9_gen3.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_obi.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber_gen3",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type9",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 1837904648,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff", "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_gen3.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s9_gen3, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s9_gen3.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s9_gen4 = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s9_gen4.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_obi.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber_gen4",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type9",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 2523863068,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff", "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_gen4.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_one_handed_s9_gen4, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_s9_gen4.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_s10_training = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_s10_training.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_quigon.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber_training",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber_training",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type10",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 4009691393,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff", "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_training.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_s10_training, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_s10_training.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_s11_training = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_s11_training.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_sleekblack.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber_training",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber_training",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type11",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 2835924928,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff", "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_one_handed_gen1.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_s11_training, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_s11_training.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_s1_training = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_s1_training.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_anakin.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber_training",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber_training",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type1",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 901085030,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff", "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_training.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_s1_training, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_s1_training.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_s2_training = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_s2_training.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_andael.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber_training",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber_training",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type2",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 4255982629,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff", "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_training.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_s2_training, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_s2_training.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_s3_training = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_s3_training.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_ben.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber_training",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber_training",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type3",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 3126520548,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff", "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_training.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_s3_training, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_s3_training.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_s4_training = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_s4_training.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_dugald.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber_training",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber_training",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type4",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 1767677716,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff", "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_training.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_s4_training, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_s4_training.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_s5_training = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_s5_training.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_leather.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber_training",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber_training",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type5",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 782979541,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff", "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_training.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_s5_training, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_s5_training.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_s6_training = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_s6_training.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_luke.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber_training",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber_training",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type6",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 3870491286,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff", "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_training.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_s6_training, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_s6_training.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_s7_training = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_s7_training.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_mace.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber_training",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber_training",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type7",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 2705640535,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff", "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_training.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_s7_training, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_s7_training.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_s8_training = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_s8_training.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_nebu.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber_training",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber_training",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type8",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 1149170881,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff", "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_training.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_s8_training, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_s8_training.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_s9_training = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_s9_training.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_obi.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber_training",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber_training",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_type9",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 59324928,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff", "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_training.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_s9_training, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_s9_training.iff")

object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_training = SharedWeaponObjectTemplate:new {
	clientTemplateFileName = "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_training.iff"
	--Data below here is deprecated and loaded from the tres, keeping for easy lookups
--[[
	appearanceFilename = "appearance/wp_mle_sword_lightsaber_blade_anakin.lsb",
	arrangementDescriptorFilename = "abstract/slot/arrangement/wearables/hold_both.iff",
	attackType = 1,

	certificationsRequired = {},
	clearFloraRadius = 0,
	clientDataFile = "clientdata/weapon/client_melee_lightsaber_basic.cdf",
	clientGameObjectType = 131080,
	collisionActionBlockFlags = 0,
	collisionActionFlags = 51,
	collisionActionPassFlags = 1,
	collisionMaterialBlockFlags = 0,
	collisionMaterialFlags = 1,
	collisionMaterialPassFlags = 0,
	containerType = 1,
	containerVolumeLimit = 0,
	customizationVariableMapping = {},

	detailedDescription = "@weapon_detail:sword_lightsaber_training",

	gameObjectType = 131080,

	locationReservationRadius = 0,
	lookAtText = "@weapon_lookat:sword_lightsaber_training",

	noBuildRadius = 0,

	objectName = "@weapon_name:sword_lightsaber_training",
	onlyVisibleInTools = 0,

	paletteColorCustomizationVariables = {},
	portalLayoutFilename = "",

	rangedIntCustomizationVariables = {},

	scale = 1,
	scaleThresholdBeforeExtentTest = 0.5,
	sendToClient = 1,
	slotDescriptorFilename = "abstract/slot/descriptor/default_lightsaber.iff",
	snapToTerrain = 1,
	socketDestinations = {},
	structureFootprintFileName = "",
	surfaceType = 0,

	targetable = 1,
	totalCellNumber = 0,

	useStructureFootprintOutline = 0,

	weaponEffect = "bolt",
	weaponEffectIndex = 0,

	clientObjectCRC = 3675065336,
	derivedFromTemplates = {"object/object/base/shared_base_object.iff", "object/tangible/base/shared_tangible_base.iff", "object/tangible/base/shared_tangible_craftable.iff", "object/weapon/base/shared_base_weapon.iff", "object/weapon/melee/base/shared_base_melee.iff", "object/weapon/melee/sword/base/shared_crafted_lightsaber_base.iff"}
]]
}

ObjectTemplates:addClientTemplate(object_weapon_melee_sword_crafted_saber_shared_sword_lightsaber_training, "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_training.iff")
