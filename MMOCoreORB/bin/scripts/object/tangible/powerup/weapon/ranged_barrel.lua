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


object_tangible_powerup_weapon_ranged_barrel = object_tangible_powerup_weapon_shared_ranged_barrel:new {

	templateType = POWERUP,

	pupType = "Ranged",

	baseName = "Barrel",

	primary = {
		{"healthAttackCost", "Ported", "cat_pup.pup_wpn_attack_cost_health"}, 
		{"pointBlankAccuracy", "Smoothed", "cat_pup.pup_wpn_range_attack_mod_zero"}, 
  		{"woundsRatio", "Taper-Bored", "cat_pup.pup_wpn_wound_chance"}
	},

	secondary = {
		{"idealAccuracy", "Alignment", "cat_pup.pup_wpn_range_attack_mod_mid"}, 
		{"maxDamage", "Concentration", "cat_pup.pup_wpn_damage_max"}, 
		{"mindAttackCost", "Dampening", "cat_pup.pup_wpn_attack_cost_mind"}, 
		{"idealRange", "Condensing", "cat_pup.pup_wpn_range_mid"}
	},

	factoryCrateSize = 10,

	numberExperimentalProperties = {1, 1, 1, 1},
	experimentalProperties = {"XX", "XX", "XX", "OQ"},
	experimentalWeights = {1, 1, 1, 1},
	experimentalGroupTitles = {"null", "null", "null", "exp_effectiveness"},
	experimentalSubGroupTitles = {"null", "null", "hitpoints", "effect"},
	experimentalMin = {0, 0, 1000, 1},
	experimentalMax = {0, 0, 1000, 100},
	experimentalPrecision = {0, 0, 0, 0},
	experimentalCombineType = {0, 0, 4, 1},
}

ObjectTemplates:addTemplate(object_tangible_powerup_weapon_ranged_barrel, "object/tangible/powerup/weapon/ranged_barrel.iff")
