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


object_draft_schematic_weapon_lightsaber_lightsaber_two_hand_gen1 = object_draft_schematic_weapon_lightsaber_shared_lightsaber_two_hand_gen1:new {

   templateType = DRAFTSCHEMATIC,

   customObjectName = "Two-Handed First Generation Lightsaber",

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
   resourceQuantities = {20, 1, 19, 28, 1, 30, 20},
   contribution = {100, 100, 100, 100, 100, 100, 100},


   targetTemplate = "object/weapon/melee/2h_sword/crafted_saber/sword_lightsaber_two_handed_gen1.iff",

   additionalTemplates = {
              "object/weapon/melee/2h_sword/crafted_saber/shared_sword_lightsaber_two_handed_s1_gen1.iff",
              "object/weapon/melee/2h_sword/crafted_saber/shared_sword_lightsaber_two_handed_s2_gen1.iff",
              "object/weapon/melee/2h_sword/crafted_saber/shared_sword_lightsaber_two_handed_s3_gen1.iff",
              "object/weapon/melee/2h_sword/crafted_saber/shared_sword_lightsaber_two_handed_s4_gen1.iff",
              "object/weapon/melee/2h_sword/crafted_saber/shared_sword_lightsaber_two_handed_s5_gen1.iff",
              "object/weapon/melee/2h_sword/crafted_saber/shared_sword_lightsaber_two_handed_s6_gen1.iff",
              "object/weapon/melee/2h_sword/crafted_saber/shared_sword_lightsaber_two_handed_s7_gen1.iff",
              "object/weapon/melee/2h_sword/crafted_saber/shared_sword_lightsaber_two_handed_s8_gen1.iff",
              "object/weapon/melee/2h_sword/crafted_saber/shared_sword_lightsaber_two_handed_s9_gen1.iff",
              "object/weapon/melee/2h_sword/crafted_saber/shared_sword_lightsaber_two_handed_s10_gen1.iff",
              "object/weapon/melee/2h_sword/crafted_saber/shared_sword_lightsaber_two_handed_s11_gen1.iff",
              "object/weapon/melee/2h_sword/crafted_saber/shared_sword_lightsaber_two_handed_s12_gen1.iff",
              "object/weapon/melee/2h_sword/crafted_saber/shared_sword_lightsaber_two_handed_s13_gen1.iff",
             }

}
ObjectTemplates:addTemplate(object_draft_schematic_weapon_lightsaber_lightsaber_two_hand_gen1, "object/draft_schematic/weapon/lightsaber/lightsaber_two_hand_gen1.iff")
