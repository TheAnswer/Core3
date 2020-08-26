--Copyright (C) 2010 <SWGEmu>


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


object_draft_schematic_weapon_lightsaber_lightsaber_training = object_draft_schematic_weapon_lightsaber_shared_lightsaber_training:new {

   templateType = DRAFTSCHEMATIC,

   customObjectName = "A Training Lightsaber",

   craftingToolTab = 2048, -- (See DraftSchematicObjectTemplate.h)
   complexity = 16, 
   size = 1, 
   factoryCrateType = "object/factory/factory_crate_weapon.iff",
   
   xpType = "jedi_general", 
   xp = 0, 

   assemblySkill = "jedi_saber_assembly", 
   experimentingSkill = "jedi_saber_experimentation", 
   customizationSkill = "jedi_customization", 
   factoryCrateSize = 0,   

   customizationOptions = {},
   customizationStringNames = {},
   customizationDefaults = {},

   ingredientTemplateNames = {"craft_weapon_ingredients_n", "craft_weapon_ingredients_n", "craft_weapon_ingredients_n", "craft_weapon_ingredients_n", "craft_weapon_ingredients_n", "craft_weapon_ingredients_n", "craft_weapon_ingredients_n"},
   ingredientTitleNames = {"emitter_shroud", "primary_crystal", "activator", "handgrip", "focusing_crystals", "power_field_insulator", "energizers"},
   ingredientSlotType = {0, 1, 0, 0, 1, 0, 0},
   resourceTypes = {"mineral", "object/tangible/component/weapon/lightsaber/shared_lightsaber_refined_crystal_pack.iff", "metal", "chemical", "object/tangible/component/weapon/lightsaber/shared_lightsaber_refined_crystal_pack.iff", "gas", "metal"},
   resourceQuantities = {10, 1, 10, 12, 1, 10, 10},
   contribution = {100, 100, 100, 100, 100, 100, 100},


   targetTemplate = "object/weapon/melee/sword/crafted_saber/sword_lightsaber_training.iff",

   additionalTemplates = {
              "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_s1_training.iff",
              "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_s2_training.iff",
              "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_s3_training.iff",
              "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_s4_training.iff",
              "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_s5_training.iff",
              "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_s6_training.iff",
              "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_s7_training.iff",
              "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_s8_training.iff",
              "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_s9_training.iff",
              "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_s10_training.iff",
              "object/weapon/melee/sword/crafted_saber/shared_sword_lightsaber_s11_training.iff",
             }

}
ObjectTemplates:addTemplate(object_draft_schematic_weapon_lightsaber_lightsaber_training, "object/draft_schematic/weapon/lightsaber/lightsaber_training.iff")
