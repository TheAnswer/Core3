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


object_draft_schematic_armor_armor_segment_ris_acklay = object_draft_schematic_armor_shared_armor_segment_ris_acklay:new {

   templateType = DRAFTSCHEMATIC,

   customObjectName = "Interwoven R. I. S. Segment",

   craftingToolTab = 2, -- (See DraftSchemticImplementation.h)
   complexity = 33, 
   size = 1, 

   xpType = "crafting_clothing_armor", 
   xp = 600, 

   assemblySkill = "armor_assembly", 
   experimentingSkill = "armor_experimentation", 
   customizationSkill = "armor_customization", 

   customizationOptions = {},
   customizationStringNames = {},
   customizationDefaults = {},

   ingredientTemplateNames = {"craft_armor_ingredients_n", "craft_armor_ingredients_n", "craft_armor_ingredients_n", "craft_armor_ingredients_n", "craft_armor_ingredients_n", "craft_armor_ingredients_n", "craft_armor_ingredients_n", "craft_armor_ingredients_n", "craft_armor_ingredients_n", "craft_weapon_ingredients_n"},
   ingredientTitleNames = {"segment_layer_assembly_plate", "armor_layer_weld_tabs", "segment_mounting_tabs", "reinforcement_matrix", "flexor_matrix", "defensive_layer", "segment_enhancement", "defensive_layer_2", "defensive_layer_3", "power_supply"},
   ingredientSlotType = {0, 0, 0, 1, 1, 3, 3, 3, 3, 3},
   resourceTypes = {"aluminum_titanium", "ore_intrusive_chanlon", "bone_mammal_yavin4", "object/tangible/component/armor/shared_bone_fragment_woolamander_harrower.iff", "object/tangible/component/armor/shared_hide_gurk_king.iff", "object/tangible/component/armor/shared_armor_layer.iff", "object/tangible/component/armor/shared_base_armor_segment_enhancement.iff", "object/tangible/component/armor/shared_armor_layer.iff", "object/tangible/component/armor/shared_armor_layer.iff", "object/tangible/component/weapon/shared_geonosian_power_cube_base.iff"},
   resourceQuantities = {160, 160, 75, 1, 1, 1, 1, 1, 1, 1},
   contribution = {100, 100, 100, 100, 100, 100, 100, 100, 100, 100},


   targetTemplate = "object/tangible/component/armor/armor_segment_ris_acklay.iff",

   additionalTemplates = {
             }

}
ObjectTemplates:addTemplate(object_draft_schematic_armor_armor_segment_ris_acklay, "object/draft_schematic/armor/armor_segment_ris_acklay.iff")
