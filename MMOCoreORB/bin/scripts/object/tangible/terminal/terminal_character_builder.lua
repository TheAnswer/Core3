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
--
--NOTES ABOUT CHARACTER BUILDER TERMINAL ITEM LIST FORMAT
---------------------------------------------------------
--The item list is an infinitely deep node tree system.
--To create a sub menu, use the following format:
--"Name of the sub menu", {Menu items contained in brackets}
--To create an item that is selectable, use the following format (within brackets of a submenu):
--"Name of the item", "Path to the server or client template."
--Be sure to pay attention to commas.

object_tangible_terminal_terminal_character_builder = object_tangible_terminal_shared_terminal_character_builder:new {
	gameObjectType = 16400,

	maxCondition = 0,

	templateType = CHARACTERBUILDERTERMINAL,

	glowyBadgeIds = {12, 14, 15, 16, 17, 19, 20, 21, 23, 30, 38, 39, 71, 105, 106, 107},

	villageBranchUnlocks = {
		"force_sensitive_combat_prowess_ranged_accuracy",
		"force_sensitive_combat_prowess_ranged_speed",
		"force_sensitive_combat_prowess_melee_accuracy",
		"force_sensitive_combat_prowess_melee_speed",
		"force_sensitive_enhanced_reflexes_ranged_defense",
		"force_sensitive_enhanced_reflexes_melee_defense"
	},

	itemList = {
		"Character Options",
		{
			"Learn Skill",
			{
				"Architect",
				{
					"Novice", "crafting_architect_novice",
					"Furniture",
					{
						"Furniture I", "crafting_architect_production_01",
						"Furniture II", "crafting_architect_production_02",
						"Furniture III", "crafting_architect_production_03",
						"Furniture IV", "crafting_architect_production_04"
					},
					"Construction",
					{
						"Construction I", "crafting_architect_techniques_01",
						"Construction II", "crafting_architect_techniques_02",
						"Construction III", "crafting_architect_techniques_03",
						"Construction IV", "crafting_architect_techniques_04"
					},
					"Installations",
					{
						"Installations I", "crafting_architect_harvesting_01",
						"Installations II", "crafting_architect_harvesting_02",
						"Installations III", "crafting_architect_harvesting_03",
						"Installations IV", "crafting_architect_harvesting_04"
					},
					"Buildings",
					{
						"Buildings I", "crafting_architect_blueprints_01",
						"Buildings II", "crafting_architect_blueprints_02",
						"Buildings III", "crafting_architect_blueprints_03",
						"Buildings IV", "crafting_architect_blueprints_04"
					},
					"Master", "crafting_architect_master"
				},
				"Armorsmith",
				{
					"Novice", "crafting_armorsmith_novice",
					"Personal Armor",
					{
						"Personal Armor I", "crafting_armorsmith_personal_01",
						"Personal Armor II", "crafting_armorsmith_personal_02",
						"Personal Armor III", "crafting_armorsmith_personal_03",
						"Personal Armor IV", "crafting_armorsmith_personal_04"
					},
					"Layers",
					{
						"Layers I", "crafting_armorsmith_heavy_01",
						"Layers II", "crafting_armorsmith_heavy_02",
						"Layers III", "crafting_armorsmith_heavy_03",
						"Layers IV", "crafting_armorsmith_heavy_04"
					},
					"Deflectors",
					{
						"Deflectors I", "crafting_armorsmith_deflectors_01",
						"Deflectors II", "crafting_armorsmith_deflectors_02",
						"Deflectors III", "crafting_armorsmith_deflectors_03",
						"Deflectors IV", "crafting_armorsmith_deflectors_04"
					},
					"Technique",
					{
						"Technique I", "crafting_armorsmith_complexity_01",
						"Technique II", "crafting_armorsmith_complexity_02",
						"Technique III", "crafting_armorsmith_complexity_03",
						"Technique IV", "crafting_armorsmith_complexity_04"
					},
					"Master", "crafting_armorsmith_master"
				},
				"Artisan",
				{
					"Novice", "crafting_artisan_novice",
					"Engineering",
					{
						"Engineering I", "crafting_artisan_engineering_01",
						"Engineering II", "crafting_artisan_engineering_02",
						"Engineering III", "crafting_artisan_engineering_03",
						"Engineering IV", "crafting_artisan_engineering_04"
					},
					"Domestic Arts",
					{
						"Domestic Arts I", "crafting_artisan_domestic_01",
						"Domestic Arts II", "crafting_artisan_domestic_02",
						"Domestic Arts III", "crafting_artisan_domestic_03",
						"Domestic Arts IV", "crafting_artisan_domestic_04"
					},
					"Business",
					{
						"Business I", "crafting_artisan_business_01",
						"Business II", "crafting_artisan_business_02",
						"Business III", "crafting_artisan_business_03",
						"Business IV", "crafting_artisan_business_04"
					},
					"Surveying",
					{
						"Surveying I", "crafting_artisan_survey_01",
						"Surveying II", "crafting_artisan_survey_02",
						"Surveying III", "crafting_artisan_survey_03",
						"Surveying IV", "crafting_artisan_survey_04"
					},
					"Master", "crafting_artisan_master"
				},
				"Bio-Engineer",
				{
					"Novice", "outdoors_bio_engineer_novice",
					"Clone Engineering",
					{
						"Clone Engineering I", "outdoors_bio_engineer_creature_01",
						"Clone Engineering II", "outdoors_bio_engineer_creature_02",
						"Clone Engineering III", "outdoors_bio_engineer_creature_03",
						"Clone Engineering IV", "outdoors_bio_engineer_creature_04"
					},
					"Tissue Engineering",
					{
						"Tissue Engineering I", "outdoors_bio_engineer_tissue_01",
						"Tissue Engineering II", "outdoors_bio_engineer_tissue_02",
						"Tissue Engineering III", "outdoors_bio_engineer_tissue_03",
						"Tissue Engineering IV", "outdoors_bio_engineer_tissue_04"
					},
					"DNA Sampling",
					{
						"DNA Sampling I", "outdoors_bio_engineer_dna_harvesting_01",
						"DNA Sampling II", "outdoors_bio_engineer_dna_harvesting_02",
						"DNA Sampling III", "outdoors_bio_engineer_dna_harvesting_03",
						"DNA Sampling IV", "outdoors_bio_engineer_dna_harvesting_04"
					},
					"Engineering Techniques",
					{
						"Engineering Techniques I", "outdoors_bio_engineer_production_01",
						"Engineering Techniques II", "outdoors_bio_engineer_production_02",
						"Engineering Techniques III", "outdoors_bio_engineer_production_03",
						"Engineering Techniques IV", "outdoors_bio_engineer_production_04"
					},
					"Master", "outdoors_bio_engineer_master"
				},
				"Bounty Hunter",
				{
					"Novice", "combat_bountyhunter_novice",
					"Investigation",
					{
						"Investigation I", "combat_bountyhunter_investigation_01",
						"Investigation II", "combat_bountyhunter_investigation_02",
						"Investigation III", "combat_bountyhunter_investigation_03",
						"Investigation IV", "combat_bountyhunter_investigation_04"
					},
					"Bounty Carbine",
					{
						"Bounty Carbine Specialization I", "combat_bountyhunter_droidcontrol_01",
						"Bounty Carbine Specialization II", "combat_bountyhunter_droidcontrol_02",
						"Bounty Carbine Specialization III", "combat_bountyhunter_droidcontrol_03",
						"Bounty Carbine Specialization IV", "combat_bountyhunter_droidcontrol_04"
					},
					"Bounty Pistol",
					{
						"Bounty Pistol Specialization I", "combat_bountyhunter_droidresponse_01",
						"Bounty Pistol Specialization II", "combat_bountyhunter_droidresponse_02",
						"Bounty Pistol Specialization III", "combat_bountyhunter_droidresponse_03",
						"Bounty Pistol Specialization IV", "combat_bountyhunter_droidresponse_04"
					},
					"Light Lightning Cannon",
					{
						"Light Lightning Cannon Specialization I", "combat_bountyhunter_support_01",
						"Light Lightning Cannon Specialization II", "combat_bountyhunter_support_02",
						"Light Lightning Cannon Specialization III", "combat_bountyhunter_support_03",
						"Light Lightning Cannon Specialization IV", "combat_bountyhunter_support_04"
					},
					"Master", "combat_bountyhunter_master"
				},
				"Brawler",
				{
					"Novice", "combat_brawler_novice",
					"Unarmed",
					{
						"Unarmed I", "combat_brawler_unarmed_01",
						"Unarmed II", "combat_brawler_unarmed_02",
						"Unarmed III", "combat_brawler_unarmed_03",
						"Unarmed IV", "combat_brawler_unarmed_04"
					},
					"Fencer",
					{
						"One Handed I", "combat_brawler_1handmelee_01",
						"One Handed II", "combat_brawler_1handmelee_02",
						"One Handed III", "combat_brawler_1handmelee_03",
						"One Handed IV", "combat_brawler_1handmelee_04"
					},
					"Swordsman",
					{
						"Two Handed I", "combat_brawler_2handmelee_01",
						"Two Handed II", "combat_brawler_2handmelee_02",
						"Two Handed III", "combat_brawler_2handmelee_03",
						"Two Handed IV", "combat_brawler_2handmelee_04"
					},
					"Pikeman",
					{
						"Polearms I", "combat_brawler_polearm_01",
						"Polearms II", "combat_brawler_polearm_02",
						"Polearms III", "combat_brawler_polearm_03",
						"Polearms IV", "combat_brawler_polearm_04"
					},
					"Master", "combat_brawler_master"
				},
				"Carbineer",
				{
					"Novice", "combat_carbine_novice",
					"Assault Tactics",
					{
						"Intermediate Carbine Assault Tactics", "combat_carbine_accuracy_01",
						"Advanced Carbine Assault Tactics", "combat_carbine_accuracy_02",
						"Expert Carbine Assault Tactics", "combat_carbine_accuracy_03",
						"Master Carbine Assault Tactics", "combat_carbine_accuracy_04"
					},
					"Marksmanship",
					{
						"Intermediate Carbine Marksmanship", "combat_carbine_speed_01",
						"Advanced Carbine Marksmanship", "combat_carbine_speed_02",
						"Expert Carbine Marksmanship", "combat_carbine_speed_03",
						"Master Carbine Marksmanship", "combat_carbine_speed_04"
					},
					"Counterinsurgency Tactics",
					{
						"Intermediate Carbine Counterinsurgency Tactics", "combat_carbine_ability_01",
						"Advanced Carbine Counterinsurgency Tactics", "combat_carbine_ability_02",
						"Expert Carbine Counterinsurgency Tactics", "combat_carbine_ability_03",
						"Master Carbine Counterinsurgency Tactics", "combat_carbine_ability_04"
					},
					"Special Abilities",
					{
						"Intermediate Carbine Special Abilities", "combat_carbine_support_01",
						"Advanced Carbine Special Abilities", "combat_carbine_support_02",
						"Expert Carbine Special Abilities", "combat_carbine_support_03",
						"Master Carbine Special Abilities", "combat_carbine_support_04"
					},
					"Master", "combat_carbine_master"
				},
				"Chef",
				{
					"Novice", "crafting_chef_novice",
					"Entrees",
					{
						"Entrees I", "crafting_chef_dish_01",
						"Entrees II", "crafting_chef_dish_02",
						"Entrees III", "crafting_chef_dish_03",
						"Entrees IV", "crafting_chef_dish_04"
					},
					"Desserts",
					{
						"Desserts I", "crafting_chef_dessert_01",
						"Desserts II", "crafting_chef_dessert_02",
						"Desserts III", "crafting_chef_dessert_03",
						"Desserts IV", "crafting_chef_dessert_04"
					},
					"Mixology",
					{
						"Mixology I", "crafting_chef_drink_01",
						"Mixology II", "crafting_chef_drink_02",
						"Mixology III", "crafting_chef_drink_03",
						"Mixology IV", "crafting_chef_drink_04"
					},
					"Cooking",
					{
						"Cooking I", "crafting_chef_techniques_01",
						"Cooking II", "crafting_chef_techniques_02",
						"Cooking III", "crafting_chef_techniques_03",
						"Cooking IV", "crafting_chef_techniques_04"
					},
					"Master", "crafting_chef_master"
				},
				"Combat Medic",
				{
					"Novice", "science_combatmedic_novice",
					"Distance",
					{
						"Intermediate Ranged Healing Distance", "science_combatmedic_healing_range_01",
						"Advanced Ranged Healing Distance", "science_combatmedic_healing_range_02",
						"Expert Ranged Healing Distance", "science_combatmedic_healing_range_03",
						"Master Ranged Healing Distance", "science_combatmedic_healing_range_04"
					},
					"Speed",
					{
						"Intermediate Ranged Healing Speed", "science_combatmedic_healing_range_speed_01",
						"Advanced Ranged Healing Speed", "science_combatmedic_healing_range_speed_02",
						"Expert Ranged Healing Speed", "science_combatmedic_healing_range_speed_03",
						"Master Ranged Healing Speed", "science_combatmedic_healing_range_speed_04"
					},
					"Crafting",
					{
						"Intermediate Combat Medicine Crafting", "science_combatmedic_medicine_01",
						"Advanced Combat Medicine Crafting", "science_combatmedic_medicine_02",
						"Expert Combat Medicine Crafting", "science_combatmedic_medicine_03",
						"Master Combat Medicine Crafting", "science_combatmedic_medicine_04"
					},
					"Support",
					{
						"Intermediate Combat Medic Support", "science_combatmedic_support_01",
						"Advanced Combat Medic Support", "science_combatmedic_support_02",
						"Expert Combat Medic Support", "science_combatmedic_support_03",
						"Master Combat Medic Support", "science_combatmedic_support_04"
					},
					"Master", "science_combatmedic_master"
				},
				"Commando",
				{
					"Novice", "combat_commando_novice",
					"Field Tactics",
					{
						"Field Tactics I", "combat_commando_heavyweapon_accuracy_01",
						"Field Tactics II", "combat_commando_heavyweapon_accuracy_02",
						"Field Tactics III", "combat_commando_heavyweapon_accuracy_03",
						"Field Tactics IV", "combat_commando_heavyweapon_accuracy_04"
					},
					"Heavy Support Weapons",
					{
						"Heavy Support Weapons I", "combat_commando_heavyweapon_speed_01",
						"Heavy Support Weapons II", "combat_commando_heavyweapon_speed_02",
						"Heavy Support Weapons III", "combat_commando_heavyweapon_speed_03",
						"Heavy Support Weapons IV", "combat_commando_heavyweapon_speed_04"
					},
					"Flamethrower",
					{
						"Flamethrower Specialization I", "combat_commando_thrownweapon_01",
						"Flamethrower Specialization II", "combat_commando_thrownweapon_02",
						"Flamethrower Specialization III", "combat_commando_thrownweapon_03",
						"Flamethrower Specialization IV", "combat_commando_thrownweapon_04"
					},
					"Acid Rifle",
					{
						"Acid Rifle Specialization I", "combat_commando_support_01",
						"Acid Rifle Specialization II", "combat_commando_support_02",
						"Acid Rifle Specialization III", "combat_commando_support_03",
						"Acid Rifle Specialization IV", "combat_commando_support_04"
					},
					"Master", "combat_commando_master"
				},
				"Creature Handler",
				{
					"Novice", "outdoors_creaturehandler_novice",
					"Taming",
					{
						"Creature Taming I", "outdoors_creaturehandler_taming_01",
						"Creature Taming II", "outdoors_creaturehandler_taming_02",
						"Creature Taming III", "outdoors_creaturehandler_taming_03",
						"Creature Taming IV", "outdoors_creaturehandler_taming_04"
					},
					"Training",
					{
						"Creature Training I", "outdoors_creaturehandler_training_01",
						"Creature Training II", "outdoors_creaturehandler_training_02",
						"Creature Training III", "outdoors_creaturehandler_training_03",
						"Creature Training IV", "outdoors_creaturehandler_training_04"
					},
					"Empathy",
					{
						"Creature Empathy I", "outdoors_creaturehandler_healing_01",
						"Creature Empathy II", "outdoors_creaturehandler_healing_02",
						"Creature Empathy III", "outdoors_creaturehandler_healing_03",
						"Creature Empathy IV", "outdoors_creaturehandler_healing_04"
					},
					"Management",
					{
						"Creature Management I", "outdoors_creaturehandler_support_01",
						"Creature Management II", "outdoors_creaturehandler_support_02",
						"Creature Management III", "outdoors_creaturehandler_support_03",
						"Creature Management IV", "outdoors_creaturehandler_support_04"
					},
					"Master", "outdoors_creaturehandler_master"
				},
				"Dancer",
				{
					"Novice", "social_dancer_novice",
					"Techniques",
					{
						"Dancing Techniques I", "social_dancer_ability_01",
						"Dancing Techniques II", "social_dancer_ability_02",
						"Dancing Techniques III", "social_dancer_ability_03",
						"Technique Specialist IV", "social_dancer_ability_04"
					},
					"Wound Healing",
					{
						"Dancer's Wound Healing I", "social_dancer_wound_01",
						"Dancer's Wound Healing II", "social_dancer_wound_02",
						"Dancer's Wound Healing III", "social_dancer_wound_03",
						"Dancer's Wound Healing IV", "social_dancer_wound_04"
					},
					"Knowledge",
					{
						"Dancing Knowledge I", "social_dancer_knowledge_01",
						"Dancing Knowledge II", "social_dancer_knowledge_02",
						"Dancing Knowledge III", "social_dancer_knowledge_03",
						"Dancing Knowledge IV", "social_dancer_knowledge_04"
					},
					"Fatigue Healing",
					{
						"Dancer's Fatigue Healing I", "social_dancer_shock_01",
						"Dancer's Fatigue Healing II", "social_dancer_shock_02",
						"Dancer's Fatigue Healing III", "social_dancer_shock_03",
						"Dancer's Fatigue Healing IV", "social_dancer_shock_04"
					},
					"Master", "social_dancer_master"
				},
				"Doctor",
				{
					"Novice", "science_doctor_novice",
					"Wound Treatment Speed",
					{
						"Intermediate Wound Treatment Speed", "science_doctor_wound_speed_01",
						"Advanced Wound Treatment Speed", "science_doctor_wound_speed_02",
						"Expert Wound Treatment Speed", "science_doctor_wound_speed_03",
						"Master Wound Treatment Speed", "science_doctor_wound_speed_04"
					},
					"Wound Treatment",
					{
						"Intermediate Wound Treatment", "science_doctor_wound_01",
						"Advanced Wound Treatment", "science_doctor_wound_02",
						"Expert Wound Treatment", "science_doctor_wound_03",
						"Master Wound Treatment", "science_doctor_wound_04"
					},
					"Medicine Knowledge",
					{
						"Intermediate Doctor's Medicine Knowledge", "science_doctor_ability_01",
						"Advanced Doctor's Medicine Knowledge", "science_doctor_ability_02",
						"Expert Doctor's Medicine Knowledge", "science_doctor_ability_03",
						"Master Doctor's Medicine Knowledge", "science_doctor_ability_04"
					},
					"Medicine Crafting",
					{
						"Intermediate Doctor's Medicine Crafting", "science_doctor_support_01",
						"Advanced Doctor's Medicine Crafting", "science_doctor_support_02",
						"Expert Doctor's Medicine Crafting", "science_doctor_support_03",
						"Master Doctor's Medicine Crafting", "science_doctor_support_04"
					},
					"Master", "science_doctor_master"
				},
				"Droid Engineer",
				{
					"Novice", "crafting_droidengineer_novice",
					"Production",
					{
						"Intermediate Droid Production", "crafting_droidengineer_production_01",
						"Advanced Droid Production", "crafting_droidengineer_production_02",
						"Expert Droid Production", "crafting_droidengineer_production_03",
						"Master Droid Production", "crafting_droidengineer_production_04"
					},
					"Construction Techniques",
					{
						"Intermediate Droid Construction Techniques", "crafting_droidengineer_techniques_01",
						"Advanced Droid Construction Techniques", "crafting_droidengineer_techniques_02",
						"Expert Droid Construction Techniques", "crafting_droidengineer_techniques_03",
						"Master Droid Construction Techniques", "crafting_droidengineer_techniques_04"
					},
					"Refinement",
					{
						"Intermediate Droid Refinement", "crafting_droidengineer_refinement_01",
						"Advanced Droid Refinement", "crafting_droidengineer_refinement_02",
						"Expert Droid Refinement", "crafting_droidengineer_refinement_03",
						"Master Droid Refinement", "crafting_droidengineer_refinement_04"
					},
					"Blueprints",
					{
						"Intermediate Droid Blueprints", "crafting_droidengineer_blueprints_01",
						"Advanced Droid Blueprints", "crafting_droidengineer_blueprints_02",
						"Expert Droid Blueprints", "crafting_droidengineer_blueprints_03",
						"Master Droid Blueprints", "crafting_droidengineer_blueprints_04"
					},
					"Master", "crafting_droidengineer_master"
				},
				"Entertainer",
				{
					"Novice", "social_entertainer_novice",
					"Image Design",
					{
						"Image Design I", "social_entertainer_hairstyle_01",
						"Image Design II", "social_entertainer_hairstyle_02",
						"Image Design III", "social_entertainer_hairstyle_03",
						"Image Design IV", "social_entertainer_hairstyle_04"
					},
					"Musicianship",
					{
						"Musicianship I", "social_entertainer_music_01",
						"Musicianship II", "social_entertainer_music_02",
						"Musicianship III", "social_entertainer_music_03",
						"Musicianship IV", "social_entertainer_music_04"
					},
					"Dancing",
					{
						"Dancing I", "social_entertainer_dance_01",
						"Dancing II", "social_entertainer_dance_02",
						"Dancing III", "social_entertainer_dance_03",
						"Dancing IV", "social_entertainer_dance_04"
					},
					"Entertainment Healing",
					{
						"Entertainment Healing I", "social_entertainer_healing_01",
						"Entertainment Healing II", "social_entertainer_healing_02",
						"Entertainment Healing III", "social_entertainer_healing_03",
						"Entertainment Healing IV", "social_entertainer_healing_04"
					},
					"Master", "social_entertainer_master"
				},
				"Fencer",
				{
					"Novice", "combat_1hsword_novice",
					"Stances and Grips",
					{
						"Intermediate Fencing Stances and Grips", "combat_1hsword_accuracy_01",
						"Advanced Fencing Stances and Grips", "combat_1hsword_accuracy_02",
						"Expert Fencing Stances and Grips", "combat_1hsword_accuracy_03",
						"Master Fencing Stances and Grips", "combat_1hsword_accuracy_04"
					},
					"Footwork",
					{
						"Intermediate Footwork", "combat_1hsword_speed_01",
						"Advanced Footwork", "combat_1hsword_speed_02",
						"Expert Footwork", "combat_1hsword_speed_03",
						"Master Footwork", "combat_1hsword_speed_04"
					},
					"Technique",
					{
						"Intermediate Fencing Technique", "combat_1hsword_ability_01",
						"Advanced Fencing Technique", "combat_1hsword_ability_02",
						"Expert Fencing Technique", "combat_1hsword_ability_03",
						"Master Fencing Technique", "combat_1hsword_ability_04"
					},
					"Finesse",
					{
						"Intermediate Fencing Finesse", "combat_1hsword_support_01",
						"Advanced Fencing Finesse", "combat_1hsword_support_02",
						"Expert Fencing Finesse", "combat_1hsword_support_03",
						"Master Fencing Finesse", "combat_1hsword_support_04"
					},
					"Master", "combat_1hsword_master"
				},
				--"Force Sensitive",
				--{
				--	"Crafting Mastery",
				--{
				--	"Novice", "force_sensitive_crafting_mastery_novice",
				--	"Experimentation",
				--	{
				--		"Experimentation I", "force_sensitive_crafting_mastery_experimentation_01",
				--		"Experimentation II", "force_sensitive_crafting_mastery_experimentation_02",
				--		"Experimentation III", "force_sensitive_crafting_mastery_experimentation_03",
				--		"Experimentation IV", "force_sensitive_crafting_mastery_experimentation_04"
				--	},
				--	"Assembly",
				--	{
				--		"Assembly I", "force_sensitive_crafting_mastery_assembly_01",
				--		"Assembly II", "force_sensitive_crafting_mastery_assembly_02",
				--		"Assembly III", "force_sensitive_crafting_mastery_assembly_03",
				--		"Assembly IV", "force_sensitive_crafting_mastery_assembly_04"
				--	},
				--	"Repair",
				--	{
				--		"Repair I", "force_sensitive_crafting_mastery_repair_01",
				--		"Repair II", "force_sensitive_crafting_mastery_repair_02",
				--		"Repair III", "force_sensitive_crafting_mastery_repair_03",
				--		"Repair IV", "force_sensitive_crafting_mastery_repair_04"
				--	},
				--	"Technique",
				--	{
				--		"Technique I", "force_sensitive_crafting_mastery_technique_01",
				--		"Technique II", "force_sensitive_crafting_mastery_technique_02",
				--		"Technique III", "force_sensitive_crafting_mastery_technique_03",
				--		"Technique IV", "force_sensitive_crafting_mastery_technique_04"
				--	},
				--	"Master", "force_sensitive_crafting_mastery_master"
				--},
				--"Enhanced Reflexes",
				--{
				--	"Novice", "force_sensitive_enhanced_reflexes_novice",
				--	"Ranged Defense",
				--	{
				--		"Ranged Defense I", "force_sensitive_enhanced_reflexes_ranged_defense_01",
				--		"Ranged Defense II", "force_sensitive_enhanced_reflexes_ranged_defense_02",
				--		"Ranged Defense III", "force_sensitive_enhanced_reflexes_ranged_defense_03",
				--		"Ranged Defense IV", "force_sensitive_enhanced_reflexes_ranged_defense_04"
				--	},
				--	"Melee Defense",
				--	{
				--		"Melee Defense I", "force_sensitive_enhanced_reflexes_melee_defense_01",
				--		"Melee Defense II", "force_sensitive_enhanced_reflexes_melee_defense_02",
				--		"Melee Defense III", "force_sensitive_enhanced_reflexes_melee_defense_03",
				--		"Melee Defense IV", "force_sensitive_enhanced_reflexes_melee_defense_04"
				--	},
				--	"Vehicle Control",
				--	{
				--		"Vehicle Control I", "force_sensitive_enhanced_reflexes_vehicle_control_01",
				--		"Vehicle Control II", "force_sensitive_enhanced_reflexes_vehicle_control_02",
				--		"Vehicle Control III", "force_sensitive_enhanced_reflexes_vehicle_control_03",
				--		"Vehicle Control Iv", "force_sensitive_enhanced_reflexes_vehicle_control_04"
				--	},
				--	"Survival",
				--	{
				--		"Survival I", "force_sensitive_enhanced_reflexes_survival_01",
				--		"Survival II", "force_sensitive_enhanced_reflexes_survival_02",
				--		"Survival III", "force_sensitive_enhanced_reflexes_survival_03",
				--		"Survival IV", "force_sensitive_enhanced_reflexes_survival_04"
				--	},
				--	"Master", "force_sensitive_enhanced_reflexes_master"
				--},
				--"Heightened Senses",
				--{
				--	"Novice", "force_sensitive_heightened_senses_novice",
				--	"Healing",
				--	{
				--		"Healing I", "force_sensitive_heightened_senses_healing_01",
				--		"Healing II", "force_sensitive_heightened_senses_healing_02",
				--		"Healing III", "force_sensitive_heightened_senses_healing_03",
				--		"Healing IV", "force_sensitive_heightened_senses_healing_04"
				--	},
				--	"Surveying",
				--	{
				--		"Surveying I", "force_sensitive_heightened_senses_surveying_01",
				--		"Surveying II", "force_sensitive_heightened_senses_surveying_02",
				--		"Surveying III", "force_sensitive_heightened_senses_surveying_03",
				--		"Surveying IV", "force_sensitive_heightened_senses_surveying_04"
				--	},
				--	"Persuasion",
				--	{
				--		"Persuasion I", "force_sensitive_heightened_senses_persuasion_01",
				--		"Persuasion II", "force_sensitive_heightened_senses_persuasion_02",
				--		"Persuasion III", "force_sensitive_heightened_senses_persuasion_03",
				--		"Persuasion Iv", "force_sensitive_heightened_senses_persuasion_04"
				--	},
				--	"Luck",
				--	{
				--		"Luck I", "force_sensitive_heightened_senses_luck_01",
				--		"Luck II", "force_sensitive_heightened_senses_luck_02",
				--		"Luck III", "force_sensitive_heightened_senses_luck_03",
				--		"Luck IV", "force_sensitive_heightened_senses_luck_04"
				--	},
				--	"Master", "force_sensitive_heightened_senses_master"
				--},
				"Image Designer",
				{
					"Novice", "social_imagedesigner_novice",
					"Hairstyling",
					{
						"Hairstyling I", "social_imagedesigner_hairstyle_01",
						"Hairstyling II", "social_imagedesigner_hairstyle_02",
						"Hairstyling III", "social_imagedesigner_hairstyle_03",
						"Hairstyling IV", "social_imagedesigner_hairstyle_04"
					},
					"Face",
					{
						"Face I", "social_imagedesigner_exotic_01",
						"Face II", "social_imagedesigner_exotic_02",
						"Face III", "social_imagedesigner_exotic_03",
						"Face IV", "social_imagedesigner_exotic_04"
					},
					"Bodyform",
					{
						"Bodyform I", "social_imagedesigner_bodyform_01",
						"Bodyform II", "social_imagedesigner_bodyform_02",
						"Bodyform III", "social_imagedesigner_bodyform_03",
						"Bodyform Iv", "social_imagedesigner_bodyform_04"
					},
					"Markings",
					{
						"Markings I", "social_imagedesigner_markings_01",
						"Markings II", "social_imagedesigner_markings_02",
						"Markings III", "social_imagedesigner_markings_03",
						"Markings IV", "social_imagedesigner_markings_04"
					},
					"Master", "social_imagedesigner_master"
				},
				--"Jedi",
				--{
				--	"Rank",
				--	{
				--	"Jedi Dark Rank",
				--	{
				--		"Rank Member", "force_rank_dark_novice",
				--		"Enforcer",
				--		{
				--			"Enforcer I", "force_rank_dark_rank_01",
				--			"Enforcer II", "force_rank_dark_rank_02",
				--			"Enforcer III", "force_rank_dark_rank_03",
				--			"Enforcer IV", "force_rank_dark_rank_04"
				--		},
				--		"Templar",
				--		{
				--			"Templar I", "force_rank_dark_rank_05",
				--			"Templar II", "force_rank_dark_rank_06",
				--			"Templar III", "force_rank_dark_rank_07"
				--		},
				--		"Oppressor",
				--		{
				--			"Oppressor I", "force_rank_dark_rank_08",
				--			"Oppressor II", "force_rank_dark_rank_09"
				--		},
				--	},
				--
				--	"Jedi Light Rank",
				--	{
				--		"Rank Member", "force_rank_dark_novice",
				--		"Sentinel",
				--		{
				--			"Sentinel I", "force_rank_light_rank_01",
				--			"Sentinel II", "force_rank_light_rank_02",
				--			"Sentinel III", "force_rank_light_rank_03",
				--			"Sentinel IV", "force_rank_light_rank_04"
				--		},
				--		"Consular",
				--		{
				--			"Consular I", "force_rank_light_rank_05",
				--			"Consular II", "force_rank_light_rank_06",
				--			"Consular III", "force_rank_light_rank_07"
				--		},
				--		"Arbiter",
				--		{
				--			"Arbiter I", "force_rank_light_rank_08",
				--			"Arbiter II", "force_rank_light_rank_09"
				--		},
				--	},
				--	},
				--	"Light Saber",
				--{
				--	"Novice", "force_discipline_light_saber_novice",
				--	"One Handed Light Saber",
				--	{
				--		"One Handed Light Saber I", "force_discipline_light_saber_one_hand_01",
				--		"One Handed Light Saber II", "force_discipline_light_saber_one_hand_02",
				--		"One Handed Light Saber III", "force_discipline_light_saber_one_hand_03",
				--		"One Handed Light Saber IV", "force_discipline_light_saber_one_hand_04"
				--	},
				--	"Two Handed Light Saber",
				--	{
				--		"Two Handed Light Saber I", "force_discipline_light_saber_two_hand_01",
				--		"Two Handed Light Saber II", "force_discipline_light_saber_two_hand_02",
				--		"Two Handed Light Saber III", "force_discipline_light_saber_two_hand_03",
				--		"Two Handed Light Saber IV", "force_discipline_light_saber_two_hand_04"
				--	},
				--	"Double Bladed Light Saber",
				--	{
				--		"Double Bladed Light Saber I", "force_discipline_light_saber_polearm_01",
				--		"Double Bladed Light Saber II", "force_discipline_light_saber_polearm_02",
				--		"Double Bladed Light Saber III", "force_discipline_light_saber_polearm_03",
				--		"Double Bladed Light Saber IV", "force_discipline_light_saber_polearm_04"
				--	},
				--	"Light Saber Technique",
				--	{
				--		"Light Saber Technique I", "force_discipline_light_saber_technique_01",
				--		"Light Saber Technique II", "force_discipline_light_saber_technique_02",
				--		"Light Saber Technique III", "force_discipline_light_saber_technique_03",
				--		"Light Saber Technique IV", "force_discipline_light_saber_technique_04"
				--	},
				--	"Master", "force_discipline_light_saber_master"
				--},
				--	"Force Powers",
				--{
				--	"Novice", "force_discipline_powers_novice",
				--	"Force Lightning",
				--	{
				--		"Force Lightning I", "force_discipline_powers_lightning_01",
				--		"Force Lightning II", "force_discipline_powers_lightning_02",
				--		"Force Lightning III", "force_discipline_powers_lightning_03",
				--		"Force Lightning IV", "force_discipline_powers_lightning_04"
				--	},
				--	"Force Subjugate",
				--	{
				--		"Force Subjugate I", "force_discipline_powers_mental_01",
				--		"Force Subjugate II", "force_discipline_powers_mental_02",
				--		"Force Subjugate III", "force_discipline_powers_mental_03",
				--		"Force Subjugate IV", "force_discipline_powers_mental_04"
				--	},
				--	"Force Diminish",
				--	{
				--		"Force Diminish I", "force_discipline_powers_debuff_01",
				--		"Force Diminish II", "force_discipline_powers_debuff_02",
				--		"Force Diminish III", "force_discipline_powers_debuff_03",
				--		"Force Diminish IV", "force_discipline_powers_debuff_04"
				--	},
				--	"Psychokineses",
				--	{
				--		"Psychokineses I", "force_discipline_powers_push_01",
				--		"Psychokineses II", "force_discipline_powers_push_02",
				--		"Psychokineses III", "force_discipline_powers_push_03",
				--		"Psychokineses IV", "force_discipline_powers_push_04"
				--	},
				--	"Master", "force_discipline_powers_master"
				--},
				--	"Force Healing",
				--{
				--	"Novice", "force_discipline_healing_novice",
				--	"Force Rejuvenation",
				--	{
				--		"Force Rejuvenation I", "force_discipline_healing_damage_01",
				--		"Force Rejuvenation II", "force_discipline_healing_damage_02",
				--		"Force Rejuvenation III", "force_discipline_healing_damage_03",
				--		"Force Rejuvenation IV", "force_discipline_healing_damage_04"
				--	},
				--	"Force Restoration",
				--	{
				--		"Force Restoration I", "force_discipline_healing_wound_01",
				--		"Force Restoration II", "force_discipline_healing_wound_02",
				--		"Force Restoration III", "force_discipline_healing_wound_03",
				--		"Force Restoration IV", "force_discipline_healing_wound_04"
				--	},
				--	"Force Assist",
				--	{
				--		"Force Assist I", "force_discipline_healing_other_01",
				--		"Force Assist II", "force_discipline_healing_other_02",
				--		"Force Assist III", "force_discipline_healing_other_03",
				--		"Force Assist IV", "force_discipline_healing_other_04"
				--	},
				--	"Force Purification",
				--	{
				--		"Force Purification I", "force_discipline_healing_states_01",
				--		"Force Purification II", "force_discipline_healing_states_02",
				--		"Force Purification III", "force_discipline_healing_states_03",
				--		"Force Purification IV", "force_discipline_healing_states_04"
				--	},
				--	"Master", "force_discipline_healing_master"
				--},
				--	"Force Enhancement",
				--{
				--	"Novice", "force_discipline_enhancements_novice",
				--	"Force Celerity",
				--	{
				--		"Force Celerity I", "force_discipline_enhancements_movement_01",
				--		"Force Celerity II", "force_discipline_enhancements_movement_02",
				--		"Force Celerity III", "force_discipline_enhancements_movement_03",
				--		"Force Celerity IV", "force_discipline_enhancements_movement_04"
				--	},
				--	"Force Protection",
				--	{
				--		"Force Protection I", "force_discipline_enhancements_protection_01",
				--		"Force Protection II", "force_discipline_enhancements_protection_02",
				--		"Force Protection III", "force_discipline_enhancements_protection_03",
				--		"Force Protection IV", "force_discipline_enhancements_protection_04"
				--	},
				--	"Force Resistance",
				--	{
				--		"Force Resistance I", "force_discipline_enhancements_resistance_01",
				--		"Force Resistance II", "force_discipline_enhancements_resistance_02",
				--		"Force Resistance III", "force_discipline_enhancements_resistance_03",
				--		"Force Resistance IV", "force_discipline_enhancements_resistance_04"
				--	},
				--	"Force Synergy",
				--	{
				--		"Force Synergy I", "force_discipline_enhancements_synergy_01",
				--		"Force Synergy II", "force_discipline_enhancements_synergy_02",
				--		"Force Synergy III", "force_discipline_enhancements_synergy_03",
				--		"Force Synergy IV", "force_discipline_enhancements_synergy_04"
				--	},
				--	"Master", "force_discipline_enhancements_master"
				--},
				--"Force Defense",
				--{
				--	"Novice", "force_discipline_defender_novice",
				--	"Force Melee Defense",
				--	{
				--		"Force Melee Defense I", "force_discipline_defender_melee_defense_01",
				--		"Force Melee Defense II", "force_discipline_defender_melee_defense_02",
				--		"Force Melee Defense III", "force_discipline_defender_melee_defense_03",
				--		"Force Melee Defense IV", "force_discipline_defender_melee_defense_04"
				--	},
				--	"Force Ranged Defense",
				--	{
				--		"Force Ranged Defense I", "force_discipline_defender_ranged_defense_01",
				--		"Force Ranged Defense II", "force_discipline_defender_ranged_defense_02",
				--		"Force Ranged Defense III", "force_discipline_defender_ranged_defense_03",
				--		"Force Ranged Defense IV", "force_discipline_defender_ranged_defense_04"
				--	},
				--	"Force Defense",
				--	{
				--		"Force Defense I", "force_discipline_defender_force_defense_01",
				--		"Force Defense II", "force_discipline_defender_force_defense_02",
				--		"Force Defense III", "force_discipline_defender_force_defense_03",
				--		"Force Defense IV", "force_discipline_defender_force_defense_04"
				--	},
				--	"Preternatural Defense",
				--	{
				--		"Preternatural Defense I", "force_discipline_defender_preternatural_defense_01",
				--		"Preternatural Defense II", "force_discipline_defender_preternatural_defense_02",
				--		"Preternatural Defense III", "force_discipline_defender_preternatural_defense_03",
				--		"Preternatural Defense IV", "force_discipline_defender_preternatural_defense_04"
				--	},
				--	"Master", "force_discipline_defender_master"
				--},
				"Marksman",
				{
					"Novice", "combat_marksman_novice",
					"Rifles",
					{
						"Rifles I", "combat_marksman_rifle_01",
						"Rifles II", "combat_marksman_rifle_02",
						"Rifles III", "combat_marksman_rifle_03",
						"Rifles IV", "combat_marksman_rifle_04"
					},
					"Pistols",
					{
						"Pistols I", "combat_marksman_pistol_01",
						"Pistols II", "combat_marksman_pistol_02",
						"Pistols III", "combat_marksman_pistol_03",
						"Pistols IV", "combat_marksman_pistol_04"
					},
					"Carbines",
					{
						"Carbines I", "combat_marksman_carbine_01",
						"Carbines II", "combat_marksman_carbine_02",
						"Carbines III", "combat_marksman_carbine_03",
						"Carbines IV", "combat_marksman_carbine_04"
					},
					"Ranged Support",
					{
						"Ranged Support I", "combat_marksman_support_01",
						"Ranged Support II", "combat_marksman_support_02",
						"Ranged Support III", "combat_marksman_support_03",
						"Ranged Support IV", "combat_marksman_support_04"
					},
					"Master", "combat_marksman_master"
				},
				"Medic",
				{
					"Novice", "science_medic_novice",
					"First Aid",
					{
						"First Aid I", "science_medic_injury_01",
						"First Aid II", "science_medic_injury_02",
						"First Aid III", "science_medic_injury_03",
						"First Aid IV", "science_medic_injury_04"
					},
					"Diagnostics",
					{
						"Diagnostics I", "science_medic_injury_speed_01",
						"Diagnostics II", "science_medic_injury_speed_02",
						"Diagnostics III", "science_medic_injury_speed_03",
						"Diagnostics IV", "science_medic_injury_speed_04"
					},
					"Pharmacology",
					{
						"Pharmacology I", "science_medic_ability_01",
						"Pharmacology II", "science_medic_ability_02",
						"Pharmacology III", "science_medic_ability_03",
						"Pharmacology IV", "science_medic_ability_04"
					},
					"Organic Chemistry",
					{
						"Organic Chemistry I", "science_medic_crafting_01",
						"Organic Chemistry II", "science_medic_crafting_02",
						"Organic Chemistry III", "science_medic_crafting_03",
						"Organic Chemistry IV", "science_medic_crafting_04"
					},
					"Master", "science_medic_master"
				},
				"Merchant",
				{
					"Novice", "crafting_merchant_novice",
					"Advertising",
					{
						"Advertising I", "crafting_merchant_advertising_01",
						"Advertising II", "crafting_merchant_advertising_02",
						"Advertising III", "crafting_merchant_advertising_03",
						"Advertising IV", "crafting_merchant_advertising_04"
					},
					"Efficiency",
					{
						"Efficiency I", "crafting_merchant_sales_01",
						"Efficiency II", "crafting_merchant_sales_02",
						"Efficiency III", "crafting_merchant_sales_03",
						"Efficiency IV", "crafting_merchant_sales_04"
					},
					"Hiring",
					{
						"Hiring I", "crafting_merchant_hiring_01",
						"Hiring II", "crafting_merchant_hiring_02",
						"Hiring III", "crafting_merchant_hiring_03",
						"Hiring IV", "crafting_merchant_hiring_04"
					},
					"Management",
					{
						"Management I", "crafting_merchant_management_01",
						"Management II", "crafting_merchant_management_02",
						"Management III", "crafting_merchant_management_03",
						"Management IV", "crafting_merchant_management_04"
					},
					"Master", "crafting_merchant_master"
				},
				"Musician",
				{
					"Novice", "social_musician_novice",
					"Techniques",
					{
						"Musical Techniques I", "social_musician_ability_01",
						"Musical Techniques II", "social_musician_ability_02",
						"Musical Techniques III", "social_musician_ability_03",
						"Technique Specialist IV", "social_musician_ability_04"
					},
					"Wound Healing",
					{
						"Musician's Wound Healing I", "social_musician_wound_01",
						"Musician's Wound Healing II", "social_musician_wound_02",
						"Musician's Wound Healing III", "social_musician_wound_03",
						"Musician's Wound Healing IV", "social_musician_wound_04"
					},
					"Knowledge",
					{
						"Musical Knowledge I", "social_musician_knowledge_01",
						"Musical Knowledge II", "social_musician_knowledge_02",
						"Musical Knowledge III", "social_musician_knowledge_03",
						"Musical Knowledge IV", "social_musician_knowledge_04"
					},
					"Fatigue Healing",
					{
						"Musician's Fatigue Healing I", "social_musician_shock_01",
						"Musician's Fatigue Healing II", "social_musician_shock_02",
						"Musician's Fatigue Healing III", "social_musician_shock_03",
						"Musician's Fatigue Healing IV", "social_musician_shock_04"
					},
					"Master", "social_musician_master"
				},
				"Pikeman",
				{
					"Novice", "combat_polearm_novice",
					"Stances",
					{
						"Intermediate Polearm Stances", "combat_polearm_accuracy_01",
						"Advanced Polearm Stances", "combat_polearm_accuracy_02",
						"Expert Polearm Stances", "combat_polearm_accuracy_03",
						"Master Polearm Stances", "combat_polearm_accuracy_04"
					},
					"Offensive Techniques",
					{
						"Intermediate Polearm Offensive Techniques", "combat_polearm_speed_01",
						"Advanced Polearm Offensive Techniques", "combat_polearm_speed_02",
						"Expert Polearm Offensive Techniques", "combat_polearm_speed_03",
						"Master Polearm Offensive Techniques", "combat_polearm_speed_04"
					},
					"Defensive Techniques",
					{
						"Intermediate Polearm Defensive Techniques", "combat_polearm_ability_01",
						"Advanced Polearm Defensive Techniques", "combat_polearm_ability_02",
						"Expert Polearm Defensive Techniques", "combat_polearm_ability_03",
						"Master Polearm Defensive Techniques", "combat_polearm_ability_04"
					},
					"Support Abilities",
					{
						"Intermediate Polearm Support Abilities", "combat_polearm_support_01",
						"Advanced Polearm Support Abilities", "combat_polearm_support_02",
						"Expert Polearm Support Abilities", "combat_polearm_support_03",
						"Master Polearm Support Abilities", "combat_polearm_support_04"
					},
					"Master", "combat_polearm_master"
				},
				--"Pilot",
				--{
				--},
				"Pistoleer",
				{
					"Novice", "combat_pistol_novice",
					"Grips",
					{
						"Pistol Grips I", "combat_pistol_accuracy_01",
						"Pistol Grips II", "combat_pistol_accuracy_02",
						"Pistol Grips III", "combat_pistol_accuracy_03",
						"Pistol Grips IV", "combat_pistol_accuracy_04"
					},
					"Marksmanship",
					{
						"Pistol Marksmanship I", "combat_pistol_speed_01",
						"Pistol Marksmanship II", "combat_pistol_speed_02",
						"Pistol Marksmanship III", "combat_pistol_speed_03",
						"Pistol Marksmanship IV", "combat_pistol_speed_04"
					},
					"Techniques",
					{
						"Pistol Techniques I", "combat_pistol_ability_01",
						"Pistol Techniques II", "combat_pistol_ability_02",
						"Pistol Techniques III", "combat_pistol_ability_03",
						"Pistol Techniques IV", "combat_pistol_ability_04"
					},
					"Stances",
					{
						"Pistol Stances I", "combat_pistol_support_01",
						"Pistol Stances II", "combat_pistol_support_02",
						"Pistol Stances III", "combat_pistol_support_03",
						"Pistol Stances IV", "combat_pistol_support_04"
					},
					"Master", "combat_pistol_master"
				},
				"Politician",
				{
					"Novice", "social_politician_novice",
					"Fiscal Policy",
					{
						"Fiscal Policy I", "social_politician_fiscal_01",
						"Fiscal Policy II", "social_politician_fiscal_02",
						"Fiscal Policy III", "social_politician_fiscal_03",
						"Fiscal Policy IV", "social_politician_fiscal_04"
					},
					"Martial Policy",
					{
						"Martial Policy I", "social_politician_martial_01",
						"Martial Policy II", "social_politician_martial_02",
						"Martial Policy III", "social_politician_martial_03",
						"Martial Policy IV", "social_politician_martial_04"
					},
					"Civic Policy",
					{
						"Civic Policy I", "social_politician_civic_01",
						"Civic Policy II", "social_politician_civic_02",
						"Civic Policy III", "social_politician_civic_03",
						"Civic Policy IV", "social_politician_civic_04"
					},
					"City Customization",
					{
						"City Customization I", "social_politician_urban_01",
						"City Customization II", "social_politician_urban_02",
						"City Customization III", "social_politician_urban_03",
						"City Customization IV", "social_politician_urban_04"
					},
					"Master", "social_politician_master"
				},
				"Ranger",
				{
					"Novice", "outdoors_ranger_novice",
					"Wayfaring",
					{
						"Wayfaring I", "outdoors_ranger_movement_01",
						"Wayfaring II", "outdoors_ranger_movement_02",
						"Wayfaring III", "outdoors_ranger_movement_03",
						"Wayfaring IV", "outdoors_ranger_movement_04"
					},
					"Frontiering",
					{
						"Frontiering I", "outdoors_ranger_tracking_01",
						"Frontiering II", "outdoors_ranger_tracking_02",
						"Frontiering III", "outdoors_ranger_tracking_03",
						"Frontiering IV", "outdoors_ranger_tracking_04"
					},
					"Tracking",
					{
						"Tracking I", "outdoors_ranger_harvest_01",
						"Tracking II", "outdoors_ranger_harvest_02",
						"Tracking III", "outdoors_ranger_harvest_03",
						"Tracking IV", "outdoors_ranger_harvest_04"
					},
					"Advanced Trapping",
					{
						"Advanced Trapping I", "outdoors_ranger_support_01",
						"Advanced Trapping II", "outdoors_ranger_support_02",
						"Advanced Trapping III", "outdoors_ranger_support_03",
						"Advanced Trapping IV", "outdoors_ranger_support_04"
					},
					"Master", "outdoors_ranger_master"
				},
				"Rifleman",
				{
					"Novice", "combat_rifleman_novice",
					"Sniping",
					{
						"Sniping I", "combat_rifleman_accuracy_01",
						"Sniping II", "combat_rifleman_accuracy_02",
						"Sniping III", "combat_rifleman_accuracy_03",
						"Sniping IV", "combat_rifleman_accuracy_04"
					},
					"Concealment",
					{
						"Concealment I", "combat_rifleman_speed_01",
						"Concealment II", "combat_rifleman_speed_02",
						"Concealment III", "combat_rifleman_speed_03",
						"Concealment IV", "combat_rifleman_speed_04"
					},
					"Counter-Sniping",
					{
						"Counter-Sniping I", "combat_rifleman_ability_01",
						"Counter-Sniping II", "combat_rifleman_ability_02",
						"Counter-Sniping III", "combat_rifleman_ability_03",
						"Counter-Sniping IV", "combat_rifleman_ability_04"
					},
					"Rifle Abilities",
					{
						"Rifle Abilities I", "combat_rifleman_support_01",
						"Rifle Abilities II", "combat_rifleman_support_02",
						"Rifle Abilities III", "combat_rifleman_support_03",
						"Rifle Abilities IV", "combat_rifleman_support_04"
					},
					"Master", "combat_rifleman_master"
				},
				"Scout",
				{
					"Novice", "outdoors_scout_novice",
					"Exploration",
					{
						"Exploration I", "outdoors_scout_movement_01",
						"Exploration II", "outdoors_scout_movement_02",
						"Exploration III", "outdoors_scout_movement_03",
						"Exploration IV", "outdoors_scout_movement_04"
					},
					"Trapping",
					{
						"Trapping I", "outdoors_scout_tools_01",
						"Trapping II", "outdoors_scout_tools_02",
						"Trapping III", "outdoors_scout_tools_03",
						"Trapping IV", "outdoors_scout_tools_04"
					},
					"Hunting",
					{
						"Hunting I", "outdoors_scout_harvest_01",
						"Hunting II", "outdoors_scout_harvest_02",
						"Hunting III", "outdoors_scout_harvest_03",
						"Hunting IV", "outdoors_scout_harvest_04"
					},
					"Survival",
					{
						"Survival I", "outdoors_scout_camp_01",
						"Survival II", "outdoors_scout_camp_02",
						"Survival III", "outdoors_scout_camp_03",
						"Survival IV", "outdoors_scout_camp_04"
					},
					"Master", "outdoors_scout_master"
				},
				"Shipwright",
				{
					"Novice", "crafting_shipwright_novice",
					"Spaceframe Engineering",
					{
						"Spaceframe Engineering I", "crafting_shipwright_engineering_01",
						"Spaceframe Engineering II", "crafting_shipwright_engineering_02",
						"Spaceframe Engineering III", "crafting_shipwright_engineering_03",
						"Spaceframe Engineering IV", "crafting_shipwright_engineering_04"
					},
					"Propulsion Technology",
					{
						"Propulsion Technology I", "crafting_shipwright_propulsion_01",
						"Propulsion Technology II", "crafting_shipwright_propulsion_02",
						"Propulsion Technology III", "crafting_shipwright_propulsion_03",
						"Propulsion Technology IV", "crafting_shipwright_propulsion_04"
					},
					"Core Systems",
					{
						"Core Systems I", "crafting_shipwright_systems_01",
						"Core Systems II", "crafting_shipwright_systems_02",
						"Core Systems III", "crafting_shipwright_systems_03",
						"Core Systems IV", "crafting_shipwright_systems_04"
					},
					"Defense Systems",
					{
						"Defense Systems I", "crafting_shipwright_defense_01",
						"Defense Systems II", "crafting_shipwright_defense_02",
						"Defense Systems III", "crafting_shipwright_defense_03",
						"Defense Systems IV", "crafting_shipwright_defense_04"
					},
					"Master", "crafting_shipwright_master"
				},
				"Smuggler",
				{
					"Novice", "combat_smuggler_novice",
					"Underworld",
					{
						"Underworld I", "combat_smuggler_underworld_01",
						"Underworld II", "combat_smuggler_underworld_02",
						"Underworld III", "combat_smuggler_underworld_03",
						"Underworld IV", "combat_smuggler_underworld_04"
					},
					"Slicing",
					{
						"Slicing I", "combat_smuggler_slicing_01",
						"Slicing II", "combat_smuggler_slicing_02",
						"Slicing III", "combat_smuggler_slicing_03",
						"Slicing IV", "combat_smuggler_slicing_04"
					},
					"Dirty Fighting",
					{
						"Dirty Fighting I", "combat_smuggler_combat_01",
						"Dirty Fighting II", "combat_smuggler_combat_02",
						"Dirty Fighting III", "combat_smuggler_combat_03",
						"Dirty Fighting IV", "combat_smuggler_combat_04"
					},
					"Spices",
					{
						"Spices I", "combat_smuggler_spice_01",
						"Spices II", "combat_smuggler_spice_02",
						"Spices III", "combat_smuggler_spice_03",
						"Spices IV", "combat_smuggler_spice_04"
					},
					"Master", "combat_smuggler_master"
				},
				"Squad Leader",
				{
					"Novice", "outdoors_squadleader_novice",
					"Mobility",
					{
						"Mobility I", "outdoors_squadleader_movement_01",
						"Mobility II", "outdoors_squadleader_movement_02",
						"Mobility III", "outdoors_squadleader_movement_03",
						"Mobility IV", "outdoors_squadleader_movement_04"
					},
					"Strategy",
					{
						"Strategy I", "outdoors_squadleader_offense_01",
						"Strategy II", "outdoors_squadleader_offense_02",
						"Strategy III", "outdoors_squadleader_offense_03",
						"Strategy IV", "outdoors_squadleader_offense_04"
					},
					"Leadership",
					{
						"Leadership I", "outdoors_squadleader_defense_01",
						"Leadership II", "outdoors_squadleader_defense_02",
						"Leadership III", "outdoors_squadleader_defense_03",
						"Leadership IV", "outdoors_squadleader_defense_04"
					},
					"Tactics",
					{
						"Tactics I", "outdoors_squadleader_support_01",
						"Tactics II", "outdoors_squadleader_support_02",
						"Tactics III", "outdoors_squadleader_support_03",
						"Tactics IV", "outdoors_squadleader_support_04"
					},
					"Master", "outdoors_squadleader_master"
				},
				"Swordsman",
				{
					"Novice", "combat_2hsword_novice",
					"Offense",
					{
						"Intermediate Sword Offense", "combat_2hsword_accuracy_01",
						"Advanced Sword Offense", "combat_2hsword_accuracy_02",
						"Expert Sword Offense", "combat_2hsword_accuracy_03",
						"Master Sword Offense", "combat_2hsword_accuracy_04"
					},
					"Techniques",
					{
						"Intermediate Sword Techniques", "combat_2hsword_speed_01",
						"Advanced Sword Techniques", "combat_2hsword_speed_02",
						"Expert Sword Techniques", "combat_2hsword_speed_03",
						"Master Sword Techniques", "combat_2hsword_speed_04"
					},
					"Defense",
					{
						"Intermediate Sword Defense", "combat_2hsword_ability_01",
						"Advanced Sword Defense", "combat_2hsword_ability_02",
						"Expert Sword Defense", "combat_2hsword_ability_03",
						"Master Sword Defense", "combat_2hsword_ability_04"
					},
					"Finesse",
					{
						"Intermediate Sword Finesse", "combat_2hsword_support_01",
						"Advanced Sword Finesse", "combat_2hsword_support_02",
						"Expert Sword Finesse", "combat_2hsword_support_03",
						"Master Sword Finesse", "combat_2hsword_support_04"
					},
					"Master", "combat_2hsword_master"
				},
				"Tailor",
				{
					"Novice", "crafting_tailor_novice",
					"Casual Wear",
					{
						"Casual Wear I", "crafting_tailor_casual_01",
						"Casual Wear II", "crafting_tailor_casual_02",
						"Casual Wear III", "crafting_tailor_casual_03",
						"Casual Wear IV", "crafting_tailor_casual_04"
					},
					"Field Wear",
					{
						"Field Wear I", "crafting_tailor_field_01",
						"Field Wear II", "crafting_tailor_field_02",
						"Field Wear III", "crafting_tailor_field_03",
						"Field Wear IV", "crafting_tailor_field_04"
					},
					"Formal Wear",
					{
						"Formal Wear I", "crafting_tailor_formal_01",
						"Formal Wear II", "crafting_tailor_formal_02",
						"Formal Wear III", "crafting_tailor_formal_03",
						"Formal Wear IV", "crafting_tailor_formal_04"
					},
					"Tailoring",
					{
						"Tailoring I", "crafting_tailor_production_01",
						"Tailoring II", "crafting_tailor_production_02",
						"Tailoring III", "crafting_tailor_production_03",
						"Tailoring IV", "crafting_tailor_production_04"
					},
					"Master", "crafting_tailor_master"
				},
				"Teras Kasi Artist",
				{
					"Novice", "combat_unarmed_novice",
					"Meditative Techniques",
					{
						"Intermediate Meditative Techniques", "combat_unarmed_accuracy_01",
						"Advanced Meditative Techniques", "combat_unarmed_accuracy_02",
						"Expert Meditative Techniques", "combat_unarmed_accuracy_03",
						"Master Meditative Techniques", "combat_unarmed_accuracy_04"
					},
					"Balance Conditioning",
					{
						"Intermediate Balance Conditioning", "combat_unarmed_speed_01",
						"Advanced Balance Conditioning", "combat_unarmed_speed_02",
						"Expert Balance Conditioning", "combat_unarmed_speed_03",
						"Master Balance Conditioning", "combat_unarmed_speed_04"
					},
					"Power Techniques",
					{
						"Intermediate Power Techniques", "combat_unarmed_ability_01",
						"Advanced Power Techniques", "combat_unarmed_ability_02",
						"Expert Power Techniques", "combat_unarmed_ability_03",
						"Master Power Techniques", "combat_unarmed_ability_04"
					},
					"Precision Striking",
					{
						"Intermediate Precision Striking", "combat_unarmed_support_01",
						"Advanced Precision Striking", "combat_unarmed_support_02",
						"Expert Precision Striking", "combat_unarmed_support_03",
						"Master Precision Striking", "combat_unarmed_support_04"
					},
					"Master", "combat_unarmed_master"
				},
				"Weaponsmith",
				{
					"Novice", "crafting_weaponsmith_novice",
					"Melee",
					{
						"Intermediate Melee Weapons Crafting", "crafting_weaponsmith_melee_01",
						"Advanced Melee Weapons Crafting", "crafting_weaponsmith_melee_02",
						"Expert Melee Weapons Crafting", "crafting_weaponsmith_melee_03",
						"Master Melee Weapons Crafting", "crafting_weaponsmith_melee_04"
					},
					"Firearms",
					{
						"Intermediate Firearms Crafting", "crafting_weaponsmith_firearms_01",
						"Advanced Firearms Crafting", "crafting_weaponsmith_firearms_02",
						"Expert Firearms Crafting", "crafting_weaponsmith_firearms_03",
						"Master Firearms Crafting", "crafting_weaponsmith_firearms_04"
					},
					"Munitions",
					{
						"Intermediate Munitions Crafting", "crafting_weaponsmith_munitions_01",
						"Advanced Munitions Crafting", "crafting_weaponsmith_munitions_02",
						"Expert Munitions Crafting", "crafting_weaponsmith_munitions_03",
						"Master Munitions Crafting", "crafting_weaponsmith_munitions_04"
					},
					"Techniques",
					{
						"Intermediate Weapon Crafting Techniques", "crafting_weaponsmith_techniques_01",
						"Advanced Weapon Crafting Techniques", "crafting_weaponsmith_techniques_02",
						"Expert Weapon Crafting Techniques", "crafting_weaponsmith_techniques_03",
						"Master Weapon Crafting Techniques", "crafting_weaponsmith_techniques_04"
					},
					"Master", "crafting_weaponsmith_master"
				}
			},
			"Unlearn All Skills", "unlearn_all_skills",
			"Cleanse Character", "cleanse_character",
			"Enhance Character", "enhance_character",
			"Unlock Jedi Initiate", "unlock_jedi_initiate",
			"Fill Force Bar", "fill_force_bar",
			"Reset Buffs", "reset_buffs",
			"Learn Languages", "language",
			"Credits", "credits",
			"Apply ALL DoTs", "apply_all_dots",
			"Apply Poison DOT", "apply_poison_dot",
			"Apply Bleed DOT", "apply_bleed_dot",
			"Apply Disease DOT", "apply_disease_dot",
			"Apply Fire DOT", "apply_fire_dot",
			"Clear DOTs", "clear_dots",
			"Max XP", "max_xp",
			"Become Glowy", "become_glowy",
			"Faction Points",
			{
				"Imperial", "faction_imperial",
				"Rebel", "faction_rebel"
			}
		},
		"Items",
		{
			--"Attachments",
			--{
			--	"Armor Attachment", "object/tangible/gem/armor.iff",
			--	"Clothing Attachment", "object/tangible/gem/clothing.iff",
			--},
			"Armor",
			{
				--	"Bone Armor",
				--	{
				--		"Bone Left Bicep", "object/tangible/wearables/armor/bone/armor_bone_s01_bicep_l.iff",
				--		"Bone Right Bicep", "object/tangible/wearables/armor/bone/armor_bone_s01_bicep_r.iff",
				--		"Bone Boots", "object/tangible/wearables/armor/bone/armor_bone_s01_boots.iff",
				--		"Bone Left Bracer", "object/tangible/wearables/armor/bone/armor_bone_s01_bracer_l.iff",
				--		"Bone Right Bracer", "object/tangible/wearables/armor/bone/armor_bone_s01_bracer_r.iff",
				--		"Bone Chest Plate", "object/tangible/wearables/armor/bone/armor_bone_s01_chest_plate.iff",
				--		"Bone Gloves", "object/tangible/wearables/armor/bone/armor_bone_s01_gloves.iff",
				--		"Bone Helmet", "object/tangible/wearables/armor/bone/armor_bone_s01_helmet.iff",
				--		"Bone Leggings", "object/tangible/wearables/armor/bone/armor_bone_s01_leggings.iff"
				--	},
				"Ithorian Armor",
				{
					"Ithorian Sentinel",
					{
						"Ithorian Sentinel Left Bicep", "object/tangible/wearables/armor/ithorian_sentinel/ith_armor_s03_bicep_l.iff",
						"Ithorian Sentinel Right Bicep", "object/tangible/wearables/armor/ithorian_sentinel/ith_armor_s03_bicep_r.iff",
						"Ithorian Sentinel Boots", "object/tangible/wearables/armor/ithorian_sentinel/ith_armor_s03_boots.iff",
						"Ithorian Sentinel Left Bracer", "object/tangible/wearables/armor/ithorian_sentinel/ith_armor_s03_bracer_l.iff",
						"Ithorian Sentinel Right Bracer", "object/tangible/wearables/armor/ithorian_sentinel/ith_armor_s03_bracer_r.iff",
						"Ithorian Sentinel Chest Plate", "object/tangible/wearables/armor/ithorian_sentinel/ith_armor_s03_chest_plate.iff",
						"Ithorian Sentinel Gloves", "object/tangible/wearables/armor/ithorian_sentinel/ith_armor_s03_gloves.iff",
						"Ithorian Sentinel Helmet", "object/tangible/wearables/armor/ithorian_sentinel/ith_armor_s03_helmet.iff",
						"Ithorian Sentinel Leggings", "object/tangible/wearables/armor/ithorian_sentinel/ith_armor_s03_leggings.iff"
					}
				},
				"Kashyyykian Armor",
				{
					--		"Kashyyykian Black Mountain",
					--		{
					--			"Kashyyykian Black Mountain Left Bracer", "object/tangible/wearables/armor/kashyyykian_black_mtn/armor_kashyyykian_black_mtn_bracer_l.iff",
					--			"Kashyyykian Black Mountain Right Bracer", "object/tangible/wearables/armor/kashyyykian_black_mtn/armor_kashyyykian_black_mtn_bracer_r.iff",
					--			"Kashyyykian Black Mountain Chest Plate", "object/tangible/wearables/armor/kashyyykian_black_mtn/armor_kashyyykian_black_mtn_chest_plate.iff",
					--			"Kashyyykian Black Mountain Leggings", "object/tangible/wearables/armor/kashyyykian_black_mtn/armor_kashyyykian_black_mtn_leggings.iff"
					--		},
					--
					--		"Kashyyykian Ceremonial",
					--		{
					--			"Kashyyykian Ceremonial Left Bracer", "object/tangible/wearables/armor/kashyyykian_ceremonial/armor_kashyyykian_ceremonial_bracer_l.iff",
					--			"Kashyyykian Ceremonial Right Bracer", "object/tangible/wearables/armor/kashyyykian_ceremonial/armor_kashyyykian_ceremonial_bracer_r.iff",
					--			"Kashyyykian Ceremonial Chest Plate", "object/tangible/wearables/armor/kashyyykian_ceremonial/armor_kashyyykian_ceremonial_chest_plate.iff",
					--			"Kashyyykian Ceremonial Leggings", "object/tangible/wearables/armor/kashyyykian_ceremonial/armor_kashyyykian_ceremonial_leggings.iff"
					--		},
					"Kashyyykian Hunting",
					{
						"Kashyyykian Hunting Left Bracer", "object/tangible/wearables/armor/kashyyykian_hunting/armor_kashyyykian_hunting_bracer_l.iff",
						"Kashyyykian Hunting Right Bracer", "object/tangible/wearables/armor/kashyyykian_hunting/armor_kashyyykian_hunting_bracer_r.iff",
						"Kashyyykian Hunting Chest Plate", "object/tangible/wearables/armor/kashyyykian_hunting/armor_kashyyykian_hunting_chest_plate.iff",
						"Kashyyykian Hunting Leggings", "object/tangible/wearables/armor/kashyyykian_hunting/armor_kashyyykian_hunting_leggings.iff"
					}
				},
				--	"Chitin Armor",
				--	{
				--		"Chitin Left Bicep", "object/tangible/wearables/armor/chitin/armor_chitin_s01_bicep_l.iff",
				--		"Chitin Right Bicep", "object/tangible/wearables/armor/chitin/armor_chitin_s01_bicep_r.iff",
				--		"Chitin Boots", "object/tangible/wearables/armor/chitin/armor_chitin_s01_boots.iff",
				--		"Chitin Left Bracer", "object/tangible/wearables/armor/chitin/armor_chitin_s01_bracer_l.iff",
				--		"Chitin Right Bracer", "object/tangible/wearables/armor/chitin/armor_chitin_s01_bracer_r.iff",
				--		"Chitin Chest Plate", "object/tangible/wearables/armor/chitin/armor_chitin_s01_chest_plate.iff",
				--		"Chitin Gloves", "object/tangible/wearables/armor/chitin/armor_chitin_s01_gloves.iff",
				--		"Chitin Helmet", "object/tangible/wearables/armor/chitin/armor_chitin_s01_helmet.iff",
				--		"Chitin Leggings", "object/tangible/wearables/armor/chitin/armor_chitin_s01_leggings.iff"
				--	},
				"Composite Armor",
				{
					"Composite Left Bicep", "object/tangible/wearables/armor/composite/armor_composite_bicep_l.iff",
					"Composite Right Bicep", "object/tangible/wearables/armor/composite/armor_composite_bicep_r.iff",
					"Composite Boots", "object/tangible/wearables/armor/composite/armor_composite_boots.iff",
					"Composite Left Bracer", "object/tangible/wearables/armor/composite/armor_composite_bracer_l.iff",
					"Composite Right Bracer", "object/tangible/wearables/armor/composite/armor_composite_bracer_r.iff",
					"Composite Chest Plate", "object/tangible/wearables/armor/composite/armor_composite_chest_plate.iff",
					"Composite Gloves", "object/tangible/wearables/armor/composite/armor_composite_gloves.iff",
					"Composite Helmet", "object/tangible/wearables/armor/composite/armor_composite_helmet.iff",
					"Composite Leggings", "object/tangible/wearables/armor/composite/armor_composite_leggings.iff"
				},
			--	"Singing Mountain Clan Armor",
			--	{
			--		"Singing Mountain Clan Bicep", "object/tangible/wearables/armor/bone/armor_smc_s01_bicep_l.iff"
			--	}
			},
			"Clothes",
			{
				"Crafting Aprons",
				{
					"Architect", "crafting_apron_architect",
					"Armorsmith", "crafting_apron_armorsmith",
					"Chef", "crafting_apron_chef",
					"Combat Medic", "crafting_apron_combat_medic",
					"Doctor", "crafting_apron_doctor",
					"Droid Engineer", "crafting_apron_droid_engineer",
					"Tailor", "crafting_apron_tailor",
					"Weaponsmith", "crafting_apron_weaponsmith",
				},
				"Backpacks",
				{
					"Backpack 01", "object/tangible/wearables/backpack/backpack_s01.iff"
				}
			},
			--"Consumables",
			--{
			--	"Desserts",
			--	{
			--"Air Cake", "object/tangible/food/crafted/dessert_air_cake.iff",
			--"Almond Kwevvu Crisp Munchies", "object/tangible/food/crafted/dessert_almond_kwevvu_crisp_munchies.iff",
			--"Bantha Butter", "object/tangible/food/crafted/dessert_bantha_butter.iff",
			--"Blap Biscuit", "object/tangible/food/crafted/dessert_blap_biscuit.iff",
			--"Blob Candy", "object/tangible/food/crafted/dessert_blob_candy.iff",
			--"Bofa Treat", "object/tangible/food/crafted/dessert_bofa_treat.iff",
			--"Cavaellin Creams", "object/tangible/food/crafted/dessert_cavaellin_creams.iff",
			--"Chandad", "object/tangible/food/crafted/dessert_chandad.iff",
			--"Citros Snow Cake", "object/tangible/food/crafted/dessert_citros_snow_cake.iff",
			--"Corellian Fried Icecream", "object/tangible/food/crafted/dessert_corellian_fried_icecream.iff",
			--"Deneelian Fizz Pudding", "object/tangible/food/crafted/dessert_deneelian_fizz_pudding.iff",
			--"Dweezel", "object/tangible/food/crafted/dessert_dweezel.iff",
			--"Felbar", "object/tangible/food/crafted/dessert_felbar.iff",
			--"Glazed Glucose Pate", "object/tangible/food/crafted/dessert_glazed_glucose_pate.iff",
			--"Gorrnar", "object/tangible/food/crafted/dessert_gorrnar.iff",
			--"Kiwik Clusjo Swirl", "object/tangible/food/crafted/dessert_kiwik_clusjo_swirl.iff",
			--"Nanana Twist", "object/tangible/food/crafted/dessert_nanana_twist.iff",
			--"Para Roll", "object/tangible/food/crafted/dessert_para_roll.iff",
			--"Parwan Nutricake", "object/tangible/food/crafted/dessert_parwan_nutricake.iff",
			--"Pastebread", "object/tangible/food/crafted/dessert_pastebread.iff",
			--"Pikatta Pie", "object/tangible/food/crafted/dessert_pikatta_pie.iff",
			--"Pkneb", "object/tangible/food/crafted/dessert_pkneb.iff",
			--"Puffcake", "object/tangible/food/crafted/dessert_puffcake.iff",
			--"Pyollian Cake", "object/tangible/food/crafted/dessert_pyollian_cake.iff",
			--"Ryshcate", "object/tangible/food/crafted/dessert_ryshcate.iff",
			--"Smugglers Delight", "object/tangible/food/crafted/dessert_smugglers_delight.iff",
			--"Sweesonberry Rolls", "object/tangible/food/crafted/dessert_sweesonberry_rolls.iff",
			--"Sweet Cake Mix", "object/tangible/food/crafted/dessert_sweet_cake_mix.iff",
			--"Tranna Nougat Cream", "object/tangible/food/crafted/dessert_tranna_nougat_cream.iff",
			--"Vagnerian Canape", "object/tangible/food/crafted/dessert_vagnerian_canape.iff",
			--"Wedding Cake", "object/tangible/food/crafted/dessert_wedding_cake.iff",
			--"Won Won", "object/tangible/food/crafted/dessert_won_won.iff",
			--},
			--"Dishes",
			--{
			--"Ahrisa", "object/tangible/food/crafted/dish_ahrisa.iff",
			--"Bivoli Tempari", "object/tangible/food/crafted/dish_bivoli_tempari.iff",
			--"Blood Chowder", "object/tangible/food/crafted/dish_blood_chowder.iff",
			--"Braised Canron", "object/tangible/food/crafted/dish_braised_canron.iff",
			--"Cho Nor Hoola", "object/tangible/food/crafted/dish_cho_nor_hoola.iff",
			--"Crispic", "object/tangible/food/crafted/dish_crispic.iff",
			--"Dustcrepe", "object/tangible/food/crafted/dish_dustcrepe.iff",
			--"Exo Protein Wafers", "object/tangible/food/crafted/dish_exo_protein_wafers.iff",
			--"Fire Stew", "object/tangible/food/crafted/dish_fire_stew.iff",
			--"Fried Endwa", "object/tangible/food/crafted/dish_fried_endwa.iff",
			--"Gruuvan Shaal", "object/tangible/food/crafted/dish_gruuvan_shaal.iff",
			--"Havla", "object/tangible/food/crafted/dish_havla.iff",
			--"Kanali Wafers", "object/tangible/food/crafted/dish_kanali_wafers.iff",
			--"Karkan Ribenes", "object/tangible/food/crafted/dish_karkan_ribenes.iff",
			--"Meatlump", "object/tangible/food/crafted/dish_meatlump.iff",
			--"ormachek", "object/tangible/food/crafted/dish_ormachek.iff",
			--"Patot Panak", "object/tangible/food/crafted/dish_patot_panak.iff",
			--"Protato", "object/tangible/food/crafted/dish_protato.iff",
			--"Puk", "object/tangible/food/crafted/dish_puk.iff",
			--"Rakririan Burnout Sauce", "object/tangible/food/crafted/dish_rakririan_burnout_sauce.iff",
			--"Ramorrean Capanata", "object/tangible/food/crafted/dish_ramorrean_capanata.iff",
			--"Rations", "object/tangible/food/crafted/dish_rations.iff",
			--"Scrimpi", "object/tangible/food/crafted/dish_scrimpi.iff",
			--"Soypro", "object/tangible/food/crafted/dish_soypro.iff",
			--"Stewed Gwouch", "object/tangible/food/crafted/dish_stewed_gwouch.iff",
			--"Synthsteak", "object/tangible/food/crafted/dish_synthsteak.iff",
			--"Teltier Noodles", "object/tangible/food/crafted/dish_teltier_noodles.iff",
			--"Terratta", "object/tangible/food/crafted/dish_terratta.iff",
			--"Thakitillo", "object/tangible/food/crafted/dish_thakitillo.iff",
			--"Travel Biscuits", "object/tangible/food/crafted/dish_travel_biscuits.iff",
			--"Trimpian", "object/tangible/food/crafted/dish_trimpian.iff",
			--"Vegeparsine", "object/tangible/food/crafted/dish_vegeparsine.iff",
			--"Veghash", "object/tangible/food/crafted/dish_veghash.iff",
			--"Vercupti of Agazza Boleruuee", "object/tangible/food/crafted/dish_vercupti_of_agazza_boleruuee.iff",
			--"Wastril Bread", "object/tangible/food/crafted/dish_wastril_bread.iff",
			--"Xermaauc", "object/tangible/food/crafted/dish_xermaauc.iff"
			--},
			--"Drinks",
			--{
			--"Accarragm", "object/tangible/food/crafted/drink_accarragm.iff",
			--"Aitha", "object/tangible/food/crafted/drink_aitha.iff",
			--"Alcohol", "object/tangible/food/crafted/drink_alcohol.iff",
			--"Aludium Pu36", "object/tangible/food/crafted/drink_aludium_pu36.iff",
			--"Angerian Fishak Surprise", "object/tangible/food/crafted/drink_angerian_fishak_surprise.iff",
			--"Antakarian Fire Dancer", "object/tangible/food/crafted/drink_antakarian_fire_dancer.iff",
			--"Bantha Blaster", "object/tangible/food/crafted/drink_bantha_blaster.iff",
			--"Bespin Port", "object/tangible/food/crafted/drink_bespin_port.iff",
			--"Blue Milk", "object/tangible/food/crafted/drink_blue_milk.iff",
			--"Breath of Heaven", "object/tangible/food/crafted/drink_breath_of_heaven.iff",
			--"Caf", "object/tangible/food/crafted/drink_caf.iff",
			--"Charde", "object/tangible/food/crafted/drink_charde.iff",
			--"Corellian Ale", "object/tangible/food/crafted/drink_corellian_ale.iff",
			--"Corellian Brandy", "object/tangible/food/crafted/drink_corellian_brandy.iff",
			--"Cortyg", "object/tangible/food/crafted/drink_cortyg.iff",
			--"Deuterium Pyro", "object/tangible/food/crafted/drink_deuterium_pyro.iff",
			--"Double Dip outer Rim Rumdrop", "object/tangible/food/crafted/drink_double_dip_outer_rim_rumdrop.iff",
			--"Durindfire", "object/tangible/food/crafted/drink_durindfire.iff",
			--"Elshandruu Pica Thundercloud", "object/tangible/food/crafted/drink_elshandruu_pica_thundercloud.iff",
			--"Flameout", "object/tangible/food/crafted/drink_flameout.iff",
			--"Garrmorl", "object/tangible/food/crafted/drink_garrmorl.iff",
			--"Gralinyn Juice", "object/tangible/food/crafted/drink_gralinyn_juice.iff",
			--"Ice Blaster", "object/tangible/food/crafted/drink_ice_blaster.iff",
			--"Ithorian Mist", "object/tangible/food/crafted/drink_ithorian_mist.iff",
			--"Jaar", "object/tangible/food/crafted/drink_jaar.iff",
			--"Jawa Beer", "object/tangible/food/crafted/drink_jawa_beer.iff",
			--"Kylessian Fruit Distillate", "object/tangible/food/crafted/drink_kylessian_fruit_distillate.iff",
			--"Mandalorian Wine", "object/tangible/food/crafted/drink_mandalorian_wine.iff",
			--"Ruby Bliel", "object/tangible/food/crafted/drink_ruby_bliel.iff",
			--"Skannbult Likker", "object/tangible/food/crafted/drink_skannbult_likker.iff",
			--"Spiced Tea", "object/tangible/food/crafted/drink_spiced_tea.iff",
			--"Starshine Surprise", "object/tangible/food/crafted/drink_starshine_surprise.iff",
			--"Sullustan Gin", "object/tangible/food/crafted/drink_sullustan_gin.iff",
			--"Tatooine Sunburn", "object/tangible/food/crafted/drink_tatooine_sunburn.iff",
			--"Tilla Tiil", "object/tangible/food/crafted/drink_tilla_tiil.iff",
			--"Tssolok", "object/tangible/food/crafted/drink_tssolok.iff",
			--"Vasarian Brandy", "object/tangible/food/crafted/drink_vasarian_brandy.iff",
			--"Vayerbok", "object/tangible/food/crafted/drink_vayerbok.iff",
			--"Veronian Berry Wine", "object/tangible/food/crafted/drink_veronian_berry_wine.iff"
			--},
			--"Foraged Food",
			--{
			--"Jar of Berries", "object/tangible/food/foraged/edible_jar_berries.iff",
			--"Jar of Bugs", "object/tangible/food/foraged/edible_jar_bugs.iff",
			--"Jar of Fungus", "object/tangible/food/foraged/edible_jar_fungus.iff",
			--"Jar of Funk", "object/tangible/food/foraged/edible_jar_funk.iff",
			--"Jar of Livers", "object/tangible/food/foraged/edible_jar_livers.iff",
			--"Fruit Style 1", "object/tangible/food/foraged/foraged_fruit_s1.iff",
			--"Fruit Style 2", "object/tangible/food/foraged/foraged_fruit_s2.iff",
			--"Fruit Style 3", "object/tangible/food/foraged/foraged_fruit_s3.iff",
			--"Fruit Style 4", "object/tangible/food/foraged/foraged_fruit_s4.iff",
			--"Fruit Style 5", "object/tangible/food/foraged/foraged_fruit_s5.iff",
			--"Vegetable Style 2", "object/tangible/food/foraged/foraged_vegetable_s2.iff",
			--"Vegetable Style 3", "object/tangible/food/foraged/foraged_vegetable_s3.iff",
			--"Vegetable Style 4", "object/tangible/food/foraged/foraged_vegetable_s4.iff",
			--"Vegetable Style 5", "object/tangible/food/foraged/foraged_vegetable_s5.iff"
			--},
			--"Generic Food",
			--{
			--"Bread loaf", "object/tangible/food/bread_loaf_full_s1.iff",
			--"Fruit Melon", "object/tangible/food/fruit_melon.iff",
			--"Meat Kabob", "object/tangible/food/meat_kabob.iff",
			--"Meat Object", "object/tangible/food/meat_object.iff",
			--"Nectar", "object/tangible/food/nectar.iff"
			--},
			--"Spice",
			--{
			--"Booster Blue", "object/tangible/food/spice/spice_booster_blue.iff",
			--"Crash N Burn", "object/tangible/food/spice/spice_brash_n_burn.iff",
			--"Droid Lube", "object/tangible/food/spice/spice_droid_lube.iff",
			--"Giggledust", "object/tangible/food/spice/spice_giggledust.iff",
			--"Grey Gabaki", "object/tangible/food/spice/spice_grey_gabaki.iff",
			--"Gunjack", "object/tangible/food/spice/spice_gunjack.iff",
			--"Kliknik Boost", "object/tangible/food/spice/spice_kliknik_boost.iff",
			--"Kwi Boost", "object/tangible/food/spice/spice_kwi_boost.iff",
			--"Muon Gold", "object/tangible/food/spice/spice_muon_gold.iff",
			--"Neutron Pixie", "object/tangible/food/spice/spice_neutron_pixey.iff",
			--"Pyrepenol", "object/tangible/food/spice/spice_pyrepenol.iff",
			--"Scramjet", "object/tangible/food/spice/spice_Scramjet.iff",
			--"Sedative H4B", "object/tangible/food/spice/spice_sedative_h4b.iff",
			--"Shadowpaw", "object/tangible/food/spice/spice_shadowpaw.iff",
			--"Sweetblossom", "object/tangible/food/spice/spice_sweetblossom.iff",
			--"Thruster Head", "object/tangible/food/spice/spice_thruster_head.iff",
			--"Yarrock", "object/tangible/food/spice/spice_yarrock.iff"
			--}
			--},
			"Deeds",
			{
				"Resource Deed", "object/tangible/veteran_reward/resource.iff",
				"Vehicle Deeds",
				{
					"Swoop", "object/tangible/deed/vehicle_deed/speederbike_swoop_deed.iff",
					--"Jetpack", "object/tangible/deed/vehicle_deed/jetpack_deed.iff",
					"Speederbike", "object/tangible/deed/vehicle_deed/speederbike_deed.iff",
				--"Speederbike Flash", "object/tangible/deed/vehicle_deed/speederbike_flash_deed.iff",
				--"Landspeeder av21", "object/tangible/deed/vehicle_deed/landspeeder_av21_deed.iff",
				--"Landspeeder x31", "object/tangible/deed/vehicle_deed/landspeeder_x31_deed.iff",
				--"Landspeeder x34", "object/tangible/deed/vehicle_deed/landspeeder_x34_deed.iff"
				},
				"Structure Deeds",
				{
					--"Factional Bases",
					--{
					--"Generic Base 1", "object/tangible/deed/faction_perk/hq/hq_s01.iff",
					--"Generic PVP Base 1", "object/tangible/deed/faction_perk/hq/hq_s01_pvp.iff",
					--"Generic Base 2", "object/tangible/deed/faction_perk/hq/hq_s02.iff",
					--"Generic PVP Base 2", "object/tangible/deed/faction_perk/hq/hq_s02_pvp.iff",
					--"Generic Base 3", "object/tangible/deed/faction_perk/hq/hq_s03.iff",
					--"Generic PVP Base 3", "object/tangible/deed/faction_perk/hq/hq_s03_pvp.iff",
					--"Generic Base 4", "object/tangible/deed/faction_perk/hq/hq_s04.iff",
					--"Generic PVP Base 4", "object/tangible/deed/faction_perk/hq/hq_s04_pvp.iff",
					--"Generic Base 5", "object/tangible/deed/faction_perk/hq/hq_s05.iff",
					--"Generic PVP Base 5", "object/tangible/deed/faction_perk/hq/hq_s05_pvp.iff",
					--"Imperial Base 1", "object/tangible/deed/faction_perk/hq/hq_s01_imp.iff",
					--"Imperial PVP Base 1", "object/tangible/deed/faction_perk/hq/hq_s01_imp_pvp.iff",
					--"Imperial Base 2", "object/tangible/deed/faction_perk/hq/hq_s02_imp.iff",
					--"Imperial PVP Base 2", "object/tangible/deed/faction_perk/hq/hq_s02_imp_pvp.iff",
					--"Imperial Base 3", "object/tangible/deed/faction_perk/hq/hq_s03_imp.iff",
					--"Imperial PVP Base 3", "object/tangible/deed/faction_perk/hq/hq_s03_imp_pvp.iff",
					--"Imperial Base 4", "object/tangible/deed/faction_perk/hq/hq_s04_imp.iff",
					--"Imperial PVP Base 4", "object/tangible/deed/faction_perk/hq/hq_s04_imp_pvp.iff",
					--"Imperial Base 5", "object/tangible/deed/faction_perk/hq/hq_s05_imp.iff",
					--"Imperial PVP Base 5", "object/tangible/deed/faction_perk/hq/hq_s05_imp_pvp.iff",
					--"Rebel Base 1", "object/tangible/deed/faction_perk/hq/hq_s01_reb.iff",
					--"Rebel PVP Base 1", "object/tangible/deed/faction_perk/hq/hq_s01_reb_pvp.iff",
					--"Rebel Base 2", "object/tangible/deed/faction_perk/hq/hq_s02_reb.iff",
					--"Rebel PVP Base 2", "object/tangible/deed/faction_perk/hq/hq_s02_reb_pvp.iff",
					--"Rebel Base 3", "object/tangible/deed/faction_perk/hq/hq_s03_reb.iff",
					--"Rebel PVP Base 3", "object/tangible/deed/faction_perk/hq/hq_s03_reb_pvp.iff",
					--"Rebel Base 4", "object/tangible/deed/faction_perk/hq/hq_s04_reb.iff",
					--"Rebel PVP Base 4", "object/tangible/deed/faction_perk/hq/hq_s04_reb_pvp.iff",
					--"Rebel Base 5", "object/tangible/deed/faction_perk/hq/hq_s05_reb.iff",
					--"Rebel PVP Base 5", "object/tangible/deed/faction_perk/hq/hq_s05_reb_pvp.iff",
					--},
					"Merchant Tent Deeds",
					{
						"Merchant Tent Deed (Style 1)", "object/tangible/deed/player_house_deed/merchant_tent_style_01_deed.iff",
						"Merchant Tent Deed (Style 2)", "object/tangible/deed/player_house_deed/merchant_tent_style_02_deed.iff",
						"Merchant Tent Deed (Style 3)", "object/tangible/deed/player_house_deed/merchant_tent_style_03_deed.iff"
					},
					"Installation Deeds",
					{
						--"Harvester Deeds",
						--{
						--"Creature Harvester Deed", "object/tangible/deed/harvester_deed/harvester_creature_deed.iff",
						--"Personal Flora Harvester Deed", "object/tangible/deed/harvester_deed/harvester_flora_deed.iff",
						--"Heavy Flora Harvester Deed", "object/tangible/deed/harvester_deed/harvester_flora_deed_heavy.iff",
						--"Medium Flora Harvester Deed", "object/tangible/deed/harvester_deed/harvester_flora_deed_medium.iff",
						--"Personal Gas Harvester Deed", "object/tangible/deed/harvester_deed/harvester_gas_deed.iff",
						--"Heavy Gas Harvester Deed", "object/tangible/deed/harvester_deed/harvester_gas_deed_heavy.iff",
						--"Medium Gas Harvester Deed", "object/tangible/deed/harvester_deed/harvester_gas_deed_medium.iff",
						--"Personal Liquid Harvester Deed", "object/tangible/deed/harvester_deed/harvester_liquid_deed.iff",
						--"Heavy Liquid Harvester Deed", "object/tangible/deed/harvester_deed/harvester_liquid_deed_heavy.iff",
						--"Medium Liquid Harvester Deed", "object/tangible/deed/harvester_deed/harvester_liquid_deed_medium.iff",
						--"Personal Moisture Harvester Deed", "object/tangible/deed/harvester_deed/harvester_moisture_deed.iff",
						--"Heavy Moisture Harvester Deed", "object/tangible/deed/harvester_deed/harvester_moisture_deed_heavy.iff",
						--"Medium Moisture Harvester Deed", "object/tangible/deed/harvester_deed/harvester_moisture_deed_medium.iff",
						--"Personal Mineral Harvester Deed", "object/tangible/deed/harvester_deed/harvester_ore_s1_deed.iff",
						--"Heavy Mineral Harvester Deed", "object/tangible/deed/harvester_deed/harvester_ore_heavy_deed.iff",
						--"Medium Mineral Harvester Deed", "object/tangible/deed/harvester_deed/harvester_ore_s2_deed.iff"
						--},
						--"Generator Deeds",
						--{
						--"Fusion Generator Deed", "object/tangible/deed/generator_deed/generator_fusion_deed.iff",
						--"Photo Bio Generator Deed", "object/tangible/deed/generator_deed/generator_photo_bio_deed.iff",
						--"Solar Generator Deed", "object/tangible/deed/generator_deed/generator_solar_deed.iff",
						--"Wind Generator Deed", "object/tangible/deed/generator_deed/generator_wind_deed.iff",
						--},
						"Factory Deeds",
						{
							"Clothing Factory Deed", "object/tangible/deed/factory_deed/factory_clothing_deed.iff",
							"Food Factory Deed", "object/tangible/deed/factory_deed/factory_food_deed.iff",
							"Item Factory Deed", "object/tangible/deed/factory_deed/factory_item_deed.iff",
							"Structure Factory Deed", "object/tangible/deed/factory_deed/factory_structure_deed.iff"
						}
					},
					"Generic Deeds",
					{
						"Player House Deeds",
						{
							"Generic Guild Hall Deed", "object/tangible/deed/guild_deed/generic_guild_deed.iff",
							"Large Generic House Deed", "object/tangible/deed/player_house_deed/generic_house_large_deed.iff",
							"Large Generic House Deed (Style 2)", "object/tangible/deed/player_house_deed/generic_house_large_style_02_deed.iff",
							"Medium Generic House Deed", "object/tangible/deed/player_house_deed/generic_house_medium_deed.iff",
							"Medium Generic House Deed (Style 2)", "object/tangible/deed/player_house_deed/generic_house_medium_style_02_deed.iff",
							"Small Generic House Deed", "object/tangible/deed/player_house_deed/generic_house_small_deed.iff",
							"Small Generic House Deed Floorplan 2", "object/tangible/deed/player_house_deed/generic_house_small_floor_02_deed.iff",
							"Small Generic House Deed (Style 2)", "object/tangible/deed/player_house_deed/generic_house_small_style_02_deed.iff",
							"Small Generic House Deed Floorplan 2 (Style 2)", "object/tangible/deed/player_house_deed/generic_house_small_style_02_floor_02_deed.iff"
						},
						"City Deeds",
						{
							"Large Dantooine Garden", "object/tangible/deed/city_deed/garden_dantooine_lrg_01_deed.iff",
							"Medium Dantooine Garden", "object/tangible/deed/city_deed/garden_dantooine_med_01_deed.iff",
							"Small Dantooine Garden", "object/tangible/deed/city_deed/garden_dantooine_sml_01_deed.iff",
							"Large Dathomir Garden", "object/tangible/deed/city_deed/garden_dathomir_lrg_01_deed.iff",
							"Medium Dathomir Garden", "object/tangible/deed/city_deed/garden_dathomir_med_01_deed.iff",
							"Small Dathomir Garden", "object/tangible/deed/city_deed/garden_dathomir_sml_01_deed.iff",
							"Large Endor Garden", "object/tangible/deed/city_deed/garden_endor_lrg_01_deed.iff",
							"Medium Endor Garden", "object/tangible/deed/city_deed/garden_endor_med_01_deed.iff",
							"Small Endor Garden", "object/tangible/deed/city_deed/garden_endor_sml_01_deed.iff"
						}
					},
					"Corellia Deeds",
					{
						"Player House Deeds",
						{
							"Corellian Guild Hall Deed", "object/tangible/deed/guild_deed/corellia_guild_deed.iff",
							"Large Corellian House Deed", "object/tangible/deed/player_house_deed/corellia_house_large_deed.iff",
							"Large Corellian House Deed (Style 2)", "object/tangible/deed/player_house_deed/corellia_house_large_style_02_deed.iff",
							"Medium Corellian House Deed", "object/tangible/deed/player_house_deed/corellia_house_medium_deed.iff",
							"Medium Corellian House Deed (Style 2)", "object/tangible/deed/player_house_deed/corellia_house_medium_style_02_deed.iff",
							"Small Corellian House Deed", "object/tangible/deed/player_house_deed/corellia_house_small_deed.iff",
							"Small Corellian House Deed Floorplan 2", "object/tangible/deed/player_house_deed/corellia_house_small_floor_02_deed.iff",
							"Small Corellian House Deed (Style 2)", "object/tangible/deed/player_house_deed/corellia_house_small_style_02_deed.iff",
							"Small Corellian House Deed Floorplan 2 (Style 2)", "object/tangible/deed/player_house_deed/corellia_house_small_style_02_floor_02_deed.iff"
						},
						"City Deeds",
						{
							"Corellia Bank Deed", "object/tangible/deed/city_deed/bank_corellia_deed.iff",
							"Corellia Cantina Deed", "object/tangible/deed/city_deed/cantina_corellia_deed.iff",
							"Corellia Garage Deed", "object/tangible/deed/city_deed/garage_corellia_deed.iff",
							"Corellia Shuttleport Deed", "object/tangible/deed/city_deed/shuttleport_corellia_deed.iff",
							"Corellia City Hall Deed", "object/tangible/deed/city_deed/cityhall_corellia_deed.iff",
							"Corellia Cloning Center Deed", "object/tangible/deed/city_deed/cloning_corellia_deed.iff",
							"Corellia Medical Center Deed", "object/tangible/deed/city_deed/hospital_corellia_deed.iff",
							"Corellia Theater Deed", "object/tangible/deed/city_deed/theater_corellia_deed.iff",
							"Large Corellia Garden (Style 1)", "object/tangible/deed/city_deed/garden_corellia_lrg_01_deed.iff",
							"Large Corellia Garden (Style 2)", "object/tangible/deed/city_deed/garden_corellia_lrg_02_deed.iff",
							"Large Corellia Garden (Style 3)", "object/tangible/deed/city_deed/garden_corellia_lrg_03_deed.iff",
							"Large Corellia Garden (Style 4)", "object/tangible/deed/city_deed/garden_corellia_lrg_04_deed.iff",
							"Large Corellia Garden (Style 5)", "object/tangible/deed/city_deed/garden_corellia_lrg_05_deed.iff",
							"Medium Corellia Garden (Style 1)", "object/tangible/deed/city_deed/garden_corellia_med_01_deed.iff",
							"Medium Corellia Garden (Style 2)", "object/tangible/deed/city_deed/garden_corellia_med_02_deed.iff",
							"Medium Corellia Garden (Style 3)", "object/tangible/deed/city_deed/garden_corellia_med_03_deed.iff",
							"Medium Corellia Garden (Style 4)", "object/tangible/deed/city_deed/garden_corellia_med_04_deed.iff",
							"Medium Corellia Garden (Style 5)", "object/tangible/deed/city_deed/garden_corellia_med_05_deed.iff",
							"Small Corellia Garden (Style 1)", "object/tangible/deed/city_deed/garden_corellia_sml_01_deed.iff",
							"Small Corellia Garden (Style 2)", "object/tangible/deed/city_deed/garden_corellia_sml_02_deed.iff",
							"Small Corellia Garden (Style 3)", "object/tangible/deed/city_deed/garden_corellia_sml_03_deed.iff",
							"Small Corellia Garden (Style 4)", "object/tangible/deed/city_deed/garden_corellia_sml_04_deed.iff",
							"Small Corellia Garden (Style 5)", "object/tangible/deed/city_deed/garden_corellia_sml_05_deed.iff"
						}
					},
					"Naboo Deeds",
					{
						"Player House Deeds",
						{
							"Naboo Guild Hall Deed", "object/tangible/deed/guild_deed/naboo_guild_deed.iff",
							"Large Naboo House Deed", "object/tangible/deed/player_house_deed/naboo_house_large_deed.iff",
							"Medium Naboo House Deed", "object/tangible/deed/player_house_deed/naboo_house_medium_deed.iff",
							--"Medium Naboo House Deed (Style 2)", "object/tangible/deed/player_house_deed/naboo_house_medium_style_02_deed.iff",
							"Small Naboo House Deed", "object/tangible/deed/player_house_deed/naboo_house_small_deed.iff",
							"Small Naboo House Deed (Style 2)", "object/tangible/deed/player_house_deed/naboo_house_small_style_02_deed.iff"
						},
						"City Deeds",
						{
							"Naboo Bank Deed", "object/tangible/deed/city_deed/bank_naboo_deed.iff",
							"Naboo Cantina Deed", "object/tangible/deed/city_deed/cantina_naboo_deed.iff",
							"Naboo Garage Deed", "object/tangible/deed/city_deed/garage_naboo_deed.iff",
							"Naboo Shuttleport Deed", "object/tangible/deed/city_deed/shuttleport_naboo_deed.iff",
							"Naboo City Hall Deed", "object/tangible/deed/city_deed/cityhall_naboo_deed.iff",
							"Naboo Cloning Center Deed", "object/tangible/deed/city_deed/cloning_naboo_deed.iff",
							"Naboo Medical Center Deed", "object/tangible/deed/city_deed/hospital_naboo_deed.iff",
							"Naboo Theater Deed", "object/tangible/deed/city_deed/theater_naboo_deed.iff",
							"Large Naboo Garden (Style 1)", "object/tangible/deed/city_deed/garden_naboo_lrg_01_deed.iff",
							"Large Naboo Garden (Style 2)", "object/tangible/deed/city_deed/garden_naboo_lrg_02_deed.iff",
							"Large Naboo Garden (Style 3)", "object/tangible/deed/city_deed/garden_naboo_lrg_03_deed.iff",
							"Large Naboo Garden (Style 4)", "object/tangible/deed/city_deed/garden_naboo_lrg_04_deed.iff",
							"Large Naboo Garden (Style 5)", "object/tangible/deed/city_deed/garden_naboo_lrg_05_deed.iff",
							"Medium Naboo Garden (Style 1)", "object/tangible/deed/city_deed/garden_naboo_med_01_deed.iff",
							"Medium Naboo Garden (Style 2)", "object/tangible/deed/city_deed/garden_naboo_med_02_deed.iff",
							"Medium Naboo Garden (Style 3)", "object/tangible/deed/city_deed/garden_naboo_med_03_deed.iff",
							"Medium Naboo Garden (Style 4)", "object/tangible/deed/city_deed/garden_naboo_med_04_deed.iff",
							"Medium Naboo Garden (Style 5)", "object/tangible/deed/city_deed/garden_naboo_med_05_deed.iff",
							"Small Naboo Garden (Style 1)", "object/tangible/deed/city_deed/garden_naboo_sml_01_deed.iff",
							"Small Naboo Garden (Style 2)", "object/tangible/deed/city_deed/garden_naboo_sml_02_deed.iff",
							"Small Naboo Garden (Style 3)", "object/tangible/deed/city_deed/garden_naboo_sml_03_deed.iff",
							"Small Naboo Garden (Style 4)", "object/tangible/deed/city_deed/garden_naboo_sml_04_deed.iff",
							"Small Naboo Garden (Style 5)", "object/tangible/deed/city_deed/garden_naboo_sml_05_deed.iff"
						}
					},
					"Tatooine Deeds",
					{
						"Player House Deeds",
						{
							"Tatooine Guild Hall Deed", "object/tangible/deed/guild_deed/tatooine_guild_deed.iff",
							"Tatooine Guild Hall (Style 2) Deed", "object/tangible/deed/guild_deed/tatooine_guild_style_02_deed.iff",
							"Large Tatooine House Deed", "object/tangible/deed/player_house_deed/tatooine_house_large_deed.iff",
							"Medium Tatooine House Deed", "object/tangible/deed/player_house_deed/tatooine_house_medium_deed.iff",
							"Small Tatooine House Deed", "object/tangible/deed/player_house_deed/tatooine_house_small_deed.iff",
							"Small Tatooine House Deed (Style 2)", "object/tangible/deed/player_house_deed/tatooine_house_small_style_02_deed.iff"
						},
						"City Deeds",
						{
							"Tatooine Bank Deed", "object/tangible/deed/city_deed/bank_tatooine_deed.iff",
							"Tatooine Cantina Deed", "object/tangible/deed/city_deed/cantina_tatooine_deed.iff",
							"Tatooine Garage Deed", "object/tangible/deed/city_deed/garage_tatooine_deed.iff",
							"Tatooine Shuttleport Deed", "object/tangible/deed/city_deed/shuttleport_tatooine_deed.iff",
							"Tatooine City Hall Deed", "object/tangible/deed/city_deed/cityhall_tatooine_deed.iff",
							"Tatooine Cloning Center Deed", "object/tangible/deed/city_deed/cloning_tatooine_deed.iff",
							"Tatooine Medical Center Deed", "object/tangible/deed/city_deed/hospital_tatooine_deed.iff",
							"Tatooine Theater Deed", "object/tangible/deed/city_deed/theater_tatooine_deed.iff",
							"Large Tatooine Garden (Style 1)", "object/tangible/deed/city_deed/garden_tatooine_lrg_01_deed.iff",
							"Large Tatooine Garden (Style 2)", "object/tangible/deed/city_deed/garden_tatooine_lrg_02_deed.iff",
							"Large Tatooine Garden (Style 3)", "object/tangible/deed/city_deed/garden_tatooine_lrg_03_deed.iff",
							"Large Tatooine Garden (Style 4)", "object/tangible/deed/city_deed/garden_tatooine_lrg_04_deed.iff",
							"Large Tatooine Garden (Style 5)", "object/tangible/deed/city_deed/garden_tatooine_lrg_05_deed.iff",
							"Medium Tatooine Garden (Style 1)", "object/tangible/deed/city_deed/garden_tatooine_med_01_deed.iff",
							"Medium Tatooine Garden (Style 2)", "object/tangible/deed/city_deed/garden_tatooine_med_02_deed.iff",
							"Medium Tatooine Garden (Style 3)", "object/tangible/deed/city_deed/garden_tatooine_med_03_deed.iff",
							"Medium Tatooine Garden (Style 4)", "object/tangible/deed/city_deed/garden_tatooine_med_04_deed.iff",
							"Medium Tatooine Garden (Style 5)", "object/tangible/deed/city_deed/garden_tatooine_med_05_deed.iff",
							"Small Tatooine Garden (Style 1)", "object/tangible/deed/city_deed/garden_tatooine_sml_01_deed.iff",
							"Small Tatooine Garden (Style 2)", "object/tangible/deed/city_deed/garden_tatooine_sml_02_deed.iff",
							"Small Tatooine Garden (Style 3)", "object/tangible/deed/city_deed/garden_tatooine_sml_03_deed.iff",
							"Small Tatooine Garden (Style 4)", "object/tangible/deed/city_deed/garden_tatooine_sml_04_deed.iff",
							"Small Tatooine Garden (Style 5)", "object/tangible/deed/city_deed/garden_tatooine_sml_05_deed.iff"
						}
					}
				},
				"Event Perk Deeds",
				{
					--"Rental: Audience Seating", "object/tangible/deed/event_perk/audience_seating_deed.iff",
					--"Rental: AT-AT", "object/tangible/deed/event_perk/atat_static_deed.iff",
					--"Rental: AT-ST", "object/tangible/deed/event_perk/atst_static_deed.iff",
					"Rental: Banner Style 1", "object/tangible/deed/event_perk/banner_style_1_static_deed.iff",
					"Rental: Banner Style 2", "object/tangible/deed/event_perk/banner_style_2_static_deed.iff",
					--"Rental: Bantha", "object/tangible/deed/event_perk/bantha_static_deed.iff",
					--"Rental: Barbed Wire Enclosure", "object/tangible/deed/event_perk/fenced_enclosure_deed.iff",
					"Rental: Corellian Banner", "object/tangible/deed/event_perk/corellian_banner_static_deed.iff",
					--"Rental: Corsec Honor Guard 5 X 1", "object/tangible/deed/event_perk/corsec_1x10_honorguard_deed.iff",
					--"Rental: Corsec Honor Guard 5 X 2", "object/tangible/deed/event_perk/corsec_2x10_honorguard_deed.iff",
					--"Rental: Corsec Honor Guard 5 X 3", "object/tangible/deed/event_perk/corsec_3x10_honorguard_deed.iff",
					--"Rental: Corsec Honor Guard 10 X 2", "object/tangible/deed/event_perk/corsec_4x10_honorguard_deed.iff",
					--"Rental: Corsec Stage", "object/tangible/deed/event_perk/corsec_stage_theater_deed.iff",
					--"Rental: Crashed Tie Bomber", "object/tangible/deed/event_perk/crashed_tie_bomber_theater_deed.iff",
					--"Rental: Crashed Tie Fighter", "object/tangible/deed/event_perk/crashed_tie_fighter_theater_deed.iff",
					--"Rental: Crashed X-Wing", "object/tangible/deed/event_perk/crashed_xwing_theater_deed.iff",
					--"Rental: Destroyed AT-AT", "object/tangible/deed/event_perk/destroyed_atat_theater_deed.iff",
					--"Rental: Fed-Dub Honor Guard 5 X 1", "object/tangible/deed/event_perk/fed_dub_1x10_honorguard_deed.iff",
					--"Rental: Fed-Dub Honor Guard 5 X 2", "object/tangible/deed/event_perk/fed_dub_2x10_honorguard_deed.iff",
					--"Rental: Fed-Dub Honor Guard 5 X 3", "object/tangible/deed/event_perk/fed_dub_3x10_honorguard_deed.iff",
					--"Rental: Fed-Dub Honor Guard 10 X 2", "object/tangible/deed/event_perk/fed_dub_4x10_honorguard_deed.iff",
					--"Rental: Fed-Dub Stage", "object/tangible/deed/event_perk/fed_dub_stage_theater_deed.iff",
					--"Rental: Fire Pit", "object/tangible/deed/event_perk/fire_pit_deed.iff",
					"Rental: Flag of Corellia", "object/tangible/deed/event_perk/corellia_flag_deed.iff",
					"Rental: Flag of Dantooine", "object/tangible/deed/event_perk/dantooine_flag_deed.iff",
					"Rental: Flag of Dathomir", "object/tangible/deed/event_perk/dathomir_flag_deed.iff",
					"Rental: Flag of Endor", "object/tangible/deed/event_perk/endor_flag_deed.iff",
					"Rental: Flag of Lok", "object/tangible/deed/event_perk/lok_flag_deed.iff",
					"Rental: Flag of Naboo", "object/tangible/deed/event_perk/naboo_flag_deed.iff",
					"Rental: Flag of Rori", "object/tangible/deed/event_perk/rori_flag_deed.iff",
					"Rental: Flag of Talus", "object/tangible/deed/event_perk/talus_flag_deed.iff",
					"Rental: Flag of Tatooine", "object/tangible/deed/event_perk/tatooine_flag_deed.iff",
					"Rental: Flag of Yavin", "object/tangible/deed/event_perk/yavin_flag_deed.iff",
					--"Rental: Flags of the Universe", "object/tangible/deed/event_perk/universe_flags_theater.iff",
					--"Rental: Fruit Stand", "object/tangible/deed/event_perk/fruit_stand.iff",
					--"", "object/tangible/deed/event_perk/game_flag_deed.iff",
					--"Rental: Grave Stone", "object/tangible/deed/event_perk/gravestone.iff",
					--"Rental: Graveyard", "object/tangible/deed/event_perk/graveyard_deed.iff",
					--"Rental: Imperial At-AT Landing Party", "object/tangible/deed/event_perk/imperial_landing_party_atat_theater_deed.iff",
					"Rental: Imperial Banner", "object/tangible/deed/event_perk/imperial_banner_static_deed.iff",
					--"Rental: Imperial Honor Guard 5 X 1", "object/tangible/deed/event_perk/imperial_1x10_honorguard_deed.iff",
					--"Rental: Imperial Honor Guard 5 X 2", "object/tangible/deed/event_perk/imperial_2x10_honorguard_deed.iff",
					--"Rental: Imperial Honor Guard 5 X 3", "object/tangible/deed/event_perk/imperial_3x10_honorguard_deed.iff",
					--"Rental: Imperial Honor Guard 10 X 2", "object/tangible/deed/event_perk/imperial_4x10_honorguard_deed.iff",
					--"Rental: Imperial Lambda Shuttle Landing Party", "object/tangible/deed/event_perk/imperial_landing_party_lambda_theater_deed.iff",
					--"Rental: Imperial Pavillion", "object/tangible/deed/event_perk/imperial_recruiting_station_theater_deed.iff",
					--"Rental: Imperial Recruiter", "object/tangible/deed/event_perk/imperial_recruiter_deed.iff",
					--"Rental: Imperial Stage", "object/tangible/deed/event_perk/imperial_stage_theater_deed.iff",
					--"Rental: Jawas and Droids", "object/tangible/deed/event_perk/jawas_and_droids_deed.iff",
					"Rental: Lambda Shuttle", "object/tangible/deed/event_perk/lambda_shuttle_static_deed.iff",
					--"Rental: Large Imperial Wedding Garden", "object/tangible/deed/event_perk/wedding_garden_imperial_large_theater_deed.iff",
					--"Rental: Large Rebel Wedding Garden", "object/tangible/deed/event_perk/wedding_garden_rebel_large_theater_deed.iff",
					--"Rental: Lottery Droid", "object/tangible/deed/event_perk/lottery_droid_deed.iff",
					"Rental: Marble Gazebo", "object/tangible/deed/event_perk/marble_gazebo_static_deed.iff",
					--"Rental: Meat Rack", "object/tangible/deed/event_perk/meat_rack.iff",
					"Rental: Military Outpost", "object/tangible/deed/event_perk/military_outpost_deed.iff",
					"Rental: Naboo Style Gazebo", "object/tangible/deed/event_perk/naboo_gazebo_static_deed.iff",
					--"Rental: NPC Actor", "object/tangible/deed/event_perk/npc_actor_deed.iff",
					--"Rental: Party Pavillion", "object/tangible/deed/event_perk/picnic_pavillion_deed.iff",
					--"Rental: Party Stage", "object/tangible/deed/event_perk/party_stage_theater_deed.iff",
					"Rental: Pavillion Style 1", "object/tangible/deed/event_perk/pavillion_1_static_deed.iff",
					"Rental: Pavillion Style 2", "object/tangible/deed/event_perk/pavillion_2_static_deed.iff",
					--"Rental: Race Coordinator Droid", "object/tangible/deed/event_perk/race_droid_deed.iff",
					--"Rental: Radiologically Shielded Chest", "object/tangible/deed/event_perk/radiologically_shielded_chest_deed.iff",
					--"Rental: Alliance Honor Guard 5 X 1", "object/tangible/deed/event_perk/rebel_1x10_honorguard_deed.iff",
					--"Rental: Alliance Honor Guard 5 X 2", "object/tangible/deed/event_perk/rebel_2x10_honorguard_deed.iff",
					--"Rental: Alliance Honor Guard 5 X 3", "object/tangible/deed/event_perk/rebel_3x10_honorguard_deed.iff",
					--"Rental: Alliance Honor Guard 2 X 10", "object/tangible/deed/event_perk/rebel_4x10_honorguard_deed.iff",
					"Rental: Rebel Banner", "object/tangible/deed/event_perk/rebel_banner_static_deed.iff",
					--"Rental: Rebel Pavillion", "object/tangible/deed/event_perk/rebel_recruiting_station_theater_deed.iff",
					--"Rental: Rebel Recruiter", "object/tangible/deed/event_perk/rebel_recruiter_deed.iff",
					--"Rental: Rebel Stage", "object/tangible/deed/event_perk/rebel_stage_theater_deed.iff",
					--"Rental: RSF Honor Guard 5 X 1", "object/tangible/deed/event_perk/rsf_1x10_honorguard_deed.iff",
					--"Rental: RSF Honor Guard 5 X 2", "object/tangible/deed/event_perk/rsf_2x10_honorguard_deed.iff",
					--"Rental: RSF Honor Guard 5 X 3", "object/tangible/deed/event_perk/rsf_3x10_honorguard_deed.iff",
					--"Rental: RSF Honor Guard 2 X 10", "object/tangible/deed/event_perk/rsf_4x10_honorguard_deed.iff",
					--"Rental: RSF Stage", "object/tangible/deed/event_perk/rsf_stage_theater_deed.iff",
					"Rental: Sandcrawler", "object/tangible/deed/event_perk/sandcrawler.iff",
					--"Rental: Scavenger Hunt Droid", "object/tangible/deed/event_perk/scavenger_droid_deed.iff",
					--"", "object/tangible/deed/event_perk/shuttle_beacon.iff",
					"Rental: Shuttle", "object/tangible/deed/event_perk/shuttle_static_deed.iff",
					--"Rental: Skeleton (Treasure Chest)", "object/tangible/deed/event_perk/skeleton_chest_deed.iff",
					--"Rental: Sparking Treasure Chest", "object/tangible/deed/event_perk/sparking_treasure_box_chest_deed.iff",
					"Rental: Stone Based Banner", "object/tangible/deed/event_perk/stone_base_banner_static_deed.iff",
					"Rental: Tatooine Banner", "object/tangible/deed/event_perk/tatooine_banner_static_deed.iff",
					--"Rental: Technical Chest", "object/tangible/deed/event_perk/technical_chest_deed.iff",
					--"", "object/tangible/deed/event_perk/test_gaming_table_deed.iff",
					--"", "object/tangible/deed/event_perk/test_npc_deed.iff",
					--"", "object/tangible/deed/event_perk/test_theater_deed.iff",
					"Rental: Tie Bomber", "object/tangible/deed/event_perk/tie_bomber_static_deed.iff",
					--"Rental: Tie Fighter", "object/tangible/deed/event_perk/tie_fighter_static_deed.iff",
					--"Rental: Treasure Box", "object/tangible/deed/event_perk/treasure_box_chest_deed.iff",
					--"Rental: Treasure Crate", "object/tangible/deed/event_perk/treasure_crate_chest_deed.iff",
					"Rental: Web Filled Tree", "object/tangible/deed/event_perk/web_tree.iff",
					--"Rental: Wrecked Sandcrawler", "object/tangible/deed/event_perk/wrecked_sandcrawler_deed.iff",
					"Rental: X-Wing", "object/tangible/deed/event_perk/xwing_static_deed.iff",
				},
			},
			"Droids",
			{
				"Arakyd Probe Droid", "object/tangible/mission/mission_bounty_droid_probot.iff",
				"Seeker Droid", "object/tangible/mission/mission_bounty_droid_seeker.iff"
			},
			--"Fireworks",
			--{
			--"Firework 1", "object/tangible/firework/firework_s01.iff",
			--"Firework 2", "object/tangible/firework/firework_s02.iff",
			--"Firework 3", "object/tangible/firework/firework_s03.iff",
			--"Firework 4", "object/tangible/firework/firework_s04.iff",
			--"Firework 5", "object/tangible/firework/firework_s05.iff",
			--"Firework 6", "object/tangible/firework/firework_s10.iff",
			--"Firework 7", "object/tangible/firework/firework_s11.iff",
			--"Firework 8", "object/tangible/firework/firework_s18.iff",
			--},
			"Veteran Rewards (COMPLETE)",
			{
				--90 Day Rewards
				"@veteran_new:poweredharvester","object/tangible/veteran_reward/harvester.iff",
				"@veteran_new:dataterminal1","object/tangible/veteran_reward/data_terminal_s1.iff",
				"@veteran_new:dataterminal2","object/tangible/veteran_reward/data_terminal_s2.iff",
				"@veteran_new:dataterminal3","object/tangible/veteran_reward/data_terminal_s3.iff",
				"@veteran_new:dataterminal4","object/tangible/veteran_reward/data_terminal_s4.iff",
				"@veteran_new:mini_protocoldroid","object/tangible/veteran_reward/frn_vet_protocol_droid_toy.iff",
				"@veteran_new:mini_r2","object/tangible/veteran_reward/frn_vet_r2_toy.iff",
				--180 Day Rewards
				"@veteran_new:resourcecrate","object/tangible/veteran_reward/resource.iff",
				"@veteran_new:falconcouch","object/tangible/veteran_reward/frn_couch_falcon_corner_s01.iff",
				"@veteran_new:falconchair","object/tangible/veteran_reward/frn_couch_falcon_section_s01.iff",
				"@veteran_new:mini_tiefighter","object/tangible/veteran_reward/frn_vet_tie_fighter_toy.iff",
				"@veteran_new:mini_xwing","object/tangible/veteran_reward/frn_vet_x_wing_toy.iff",
				--270 Day Rewards
				"Deed for Flash Speeder","object/tangible/deed/vehicle_deed/speederbike_flash_deed.iff",
				"Special Edition Goggles Style 1","object/tangible/wearables/goggles/goggles_s01.iff",
				"Special Edition Goggles Style 2","object/tangible/wearables/goggles/goggles_s02.iff",
				"Special Edition Goggles Style 3","object/tangible/wearables/goggles/goggles_s03.iff",
				"Special Edition Goggles Style 4","object/tangible/wearables/goggles/goggles_s04.iff",
				"Special Edition Goggles Style 5","object/tangible/wearables/goggles/goggles_s05.iff",
				"Special Edition Goggles Style 6","object/tangible/wearables/goggles/goggles_s06.iff",
				"@veteran_new:mini_darthvader","object/tangible/veteran_reward/frn_vet_darth_vader_toy.iff",
				--360 Day Rewards
				"@veteran_new:techconsole_a","object/tangible/veteran_reward/frn_tech_console_sectional_a.iff",
				"@veteran_new:techconsole_b","object/tangible/veteran_reward/frn_tech_console_sectional_b.iff",
				"@veteran_new:techconsole_c","object/tangible/veteran_reward/frn_tech_console_sectional_c.iff",
				"@veteran_new:techconsole_d","object/tangible/veteran_reward/frn_tech_console_sectional_d.iff",
				"@veteran_new:mini_jabba","object/tangible/veteran_reward/frn_vet_jabba_toy.iff",
				"@veteran_new:mini_stormtrooper","object/tangible/veteran_reward/frn_vet_stormtrooper_toy.iff",
				"@veteran_new:antidecay","object/tangible/veteran_reward/antidecay.iff",
				--450 Day Rewards
				"Camp Center (Small)","object/tangible/camp/camp_spit_s2.iff",
				"Camp Center (Large)","object/tangible/camp/camp_spit_s3.iff",
				"Gold Ornamental Vase (Style 1)","object/tangible/furniture/tatooine/frn_tato_vase_style_01.iff",
				"Gold Ornamental Vase (Style 2)","object/tangible/furniture/tatooine/frn_tato_vase_style_02.iff",
				"Foodcart","object/tangible/furniture/decorative/foodcart.iff",
				"Park Bench","object/tangible/furniture/all/frn_bench_generic.iff",
				--540 Day Rewards
				"Professor Desk","object/tangible/furniture/decorative/professor_desk.iff",
				"@frn_n:diagnostic_screen","object/tangible/furniture/decorative/diagnostic_screen.iff",
				"Large Potted Plant (Style 2)","object/tangible/furniture/all/frn_all_plant_potted_lg_s2.iff",
				"Large Potted Plant (Style 3)","object/tangible/furniture/all/frn_all_plant_potted_lg_s3.iff",
				"Large Potted Plant (Style 4)","object/tangible/furniture/all/frn_all_plant_potted_lg_s4.iff",
				--630 Day Rewards
				"Bar Countertop","object/tangible/furniture/modern/bar_counter_s1.iff",
				"Bar Countertop (Curved, Style 1)","object/tangible/furniture/modern/bar_piece_curve_s1.iff",
				"Bar Countertop (Curved, Style 2)","object/tangible/furniture/modern/bar_piece_curve_s2.iff",
				"Bar Countertop (Straight, Style 1)","object/tangible/furniture/modern/bar_piece_straight_s1.iff",
				"Bar Countertop (Straight, Style 2)","object/tangible/furniture/modern/bar_piece_straight_s2.iff",
				--720 Day Rewards
				"Round Cantina Table (Style 1)","object/tangible/furniture/all/frn_all_table_s01.iff",
				"Round Cantina Table (Style 2)","object/tangible/furniture/all/frn_all_table_s02.iff",
				"Round Cantina Table (Style 3)","object/tangible/furniture/all/frn_all_table_s03.iff",
				"Large Cantina Sofa","object/tangible/furniture/tatooine/frn_tatt_chair_cantina_seat_2.iff",
				"@frn_n:frn_tato_cafe_parasol","object/tangible/furniture/tatooine/frn_tato_cafe_parasol.iff",
				--810 Day Rewards
				"Medium Oval Rug","object/tangible/furniture/modern/rug_oval_m_s02.iff",
				"Small Oval Rug","object/tangible/furniture/modern/rug_oval_sml_s01.iff",
				"Medium Rectangular Rug","object/tangible/furniture/modern/rug_rect_m_s01.iff",
				"Small Rectangular Rug","object/tangible/furniture/modern/rug_rect_sml_s01.iff",
				"Medium Round Rug","object/tangible/furniture/modern/rug_rnd_m_s01.iff",
				"Small Round Rug","object/tangible/furniture/modern/rug_rnd_sml_s01.iff",
				--900 Day Rewards
				"A Bith Skull","object/tangible/loot/misc/loot_skull_bith.iff",
				"A Human Skull","object/tangible/loot/misc/loot_skull_human.iff",
				"A Ithorian Skull","object/tangible/loot/misc/loot_skull_ithorian.iff",
				"A Thune Skull","object/tangible/loot/misc/loot_skull_thune.iff",
				"A Voritor Lizard Skull","object/tangible/loot/misc/loot_skull_voritor.iff",
				--990 Day Rewards
				"Large Rectangular Rug (Style 1)","object/tangible/furniture/modern/rug_rect_lg_s01.iff",
				"Large Rectangular Rug (Style 2)","object/tangible/furniture/modern/rug_rect_lg_s02.iff",
				"Large Oval Rug","object/tangible/furniture/modern/rug_oval_lg_s01.iff",
				"Large Round Rug","object/tangible/furniture/modern/rug_rnd_lg_s01.iff",
				"Round Data Terminal","object/tangible/furniture/all/frn_all_desk_map_table.iff",
				--1080 Day Rewards
				"Painting: Cast Wing in Flight","object/tangible/veteran_reward/one_year_anniversary/painting_01.iff",
				"Painting: Decimator","object/tangible/veteran_reward/one_year_anniversary/painting_02.iff",
				"Painting: Tatooine Dune Speeder","object/tangible/veteran_reward/one_year_anniversary/painting_03.iff",
				"Painting: Weapon of War","object/tangible/veteran_reward/one_year_anniversary/painting_04.iff",
				"Painting: Fighter Study","object/tangible/veteran_reward/one_year_anniversary/painting_05.iff",
				"Painting: Hutt Greed","object/tangible/veteran_reward/one_year_anniversary/painting_06.iff",
				"Painting: Smuggler's Run","object/tangible/veteran_reward/one_year_anniversary/painting_07.iff",
				"Painting: Imperial Oppression (TIE Oppressor)","object/tangible/veteran_reward/one_year_anniversary/painting_08.iff",
				"Painting: Emperor's Eyes (TIE Sentinel)","object/tangible/veteran_reward/one_year_anniversary/painting_09.iff",
			},
			"Furniture",
			{
				"Trophies (NEW COMPLETE)",
				{
					"Picture Handheld (Style 1)", "object/tangible/trophy/picture_handheld_s01.iff",
					"Picture Handheld (Style 2)", "object/tangible/trophy/picture_handheld_s02.iff",
					"Shisha Decorative", "object/tangible/trophy/shisa_decorative_s01.iff"
				},
				"Paintings (UPDATED)",
				{
					"Bestine Painting 1", "object/tangible/painting/bestine_history_quest_painting.iff",
					"Bestine Painting 2", "object/tangible/painting/bestine_quest_painting.iff",
					"Agrilat Swamp", "object/tangible/painting/painting_agrilat_s01.iff",
					"Armor Blueprints", "object/tangible/painting/painting_armor_blueprint.iff",
					"Blue Leaf Temple", "object/tangible/painting/painting_bestine_blueleaf_temple.iff",
					"Blumbush", "object/tangible/painting/painting_bestine_blumbush.iff",
					"Boffa Plant", "object/tangible/painting/painting_bestine_boffa.iff",
					"Abstract Painting of Golden Petals No. 1", "object/tangible/painting/painting_bestine_golden_flower_01.iff",
					"Abstract Painting of Golden Petals No. 2", "object/tangible/painting/painting_bestine_golden_flower_02.iff",
					"Abstract Painting of Golden Petals No. 3", "object/tangible/painting/painting_bestine_golden_flower_03.iff",
					"Bestine House", "object/tangible/painting/painting_bestine_house.iff",
					"Krayt Dragon Skeleton", "object/tangible/painting/painting_bestine_krayt_skeleton.iff",
					"Lucky Despot", "object/tangible/painting/painting_bestine_lucky_despot.iff",
					"Mattberry Plant", "object/tangible/painting/painting_bestine_mattberry.iff",
					"Close-up of Mon Calamari Eye 1", "object/tangible/painting/painting_bestine_moncal_eye_01.iff",
					"Close-up of Mon Calamari Eye 2", "object/tangible/painting/painting_bestine_moncal_eye_02.iff",
					"Rainbow Berry Bush", "object/tangible/painting/painting_bestine_rainbow_berry_bush.iff",
					"Raventhorn", "object/tangible/painting/painting_bestine_raventhorn.iff",
					"Ronka", "object/tangible/painting/painting_bestine_ronka.iff",
					"Bio Engineer (orange)", "object/tangible/painting/painting_bioengineer_orange.iff",
					"Bothan (female)", "object/tangible/painting/painting_bothan_f.iff",
					"Bothan (male)", "object/tangible/painting/painting_bothan_m.iff",
					"Cargo Port", "object/tangible/painting/painting_cargoport.iff",
					"Dance Party", "object/tangible/painting/painting_dance_party.iff",
					"Double Helix", "object/tangible/painting/painting_double_helix.iff",
					"Droid Bright", "object/tangible/painting/painting_droid_bright.iff",
					"Endor (Style 1)", "object/tangible/painting/painting_endor_style_01.iff",
					"Fighter Pilot (human)", "object/tangible/painting/painting_fighter_pilot_human_01.iff",
					"Food Baking 1", "object/tangible/painting/painting_food_baking_s01.iff",
					"Food Baking 2", "object/tangible/painting/painting_food_baking_s01.iff",
					"Theed Waterfall", "object/tangible/painting/painting_waterfall.iff",
					"Freedom", "object/tangible/painting/painting_freedom.iff",
					"Human (female)", "object/tangible/painting/painting_human_f.iff",
					"Kite Plant", "object/tangible/painting/painting_kite_plant.iff",
					"Nebula Flower", "object/tangible/painting/painting_nebula_flower.iff",
					"Palowick Ad 1", "object/tangible/painting/painting_palowick_ad_s01.iff",
					"Palowick Ad 2", "object/tangible/painting/painting_palowick_ad_s02.iff",
					"Palowick Ad 3", "object/tangible/painting/painting_palowick_ad_s03.iff",
					"Palowick Ad 4", "object/tangible/painting/painting_palowick_ad_s04.iff",
					"Planet 1", "object/tangible/painting/painting_planet_s01.iff",
					"Rodian (female)", "object/tangible/painting/painting_rodian_f.iff",
					"Rodian Ad 1 (female)", "object/tangible/painting/painting_rodian_f_ad_01.iff",
					"Rodian (male)", "object/tangible/painting/painting_rodian_m.iff",
					"Stormtrooper", "object/tangible/painting/painting_bw_stormtrooper.iff",
					"Han Solo Wanted", "object/tangible/painting/painting_han_wanted.iff",
					"Luke Skywalker Wanted", "object/tangible/painting/painting_luke_wanted.iff",
					"Princess Leia Wanted", "object/tangible/painting/painting_leia_wanted.iff",
					"Schematic (Droid)", "object/tangible/painting/painting_schematic_droid.iff",
					"Schematic (Transport Ship)", "object/tangible/painting/painting_schematic_transport_ship.iff",
					"Schematic (Weapon)", "object/tangible/painting/painting_schematic_weapon.iff",
					"Schematic (Weapon) 3", "object/tangible/painting/painting_schematic_weapon_s03.iff",
					"Skyscraper", "object/tangible/painting/painting_skyscraper.iff",
					"Smoking Ad", "object/tangible/painting/painting_smoking_ad.iff",
					"Star Map", "object/tangible/painting/painting_starmap.iff",
					"Tatooine 3", "object/tangible/painting/painting_tato_s03.iff",
					"Tatooine 4", "object/tangible/painting/painting_tato_s04.iff",
					"Teras Kasi", "object/tangible/painting/painting_teras_kasi.iff",
					"Teras Kasi 2", "object/tangible/painting/painting_teras_kasi_2.iff",
					"Trandoshan (male)", "object/tangible/painting/painting_trandoshan_m.iff",
					"Trandoshan (male Style 1)", "object/tangible/painting/painting_trandoshan_m_01.iff",
					"Trandoshan Wanted", "object/tangible/painting/painting_trandoshan_wanted.iff",
					"Tree", "object/tangible/painting/painting_tree.iff",
					"Trees (Style 1)", "object/tangible/painting/painting_trees_s01.iff",
					"Twilek (female)", "object/tangible/painting/painting_twilek_f.iff",
					"Twilek Large Style 1 (female)", "object/tangible/painting/painting_twilek_f_lg_s01.iff",
					"Twilek Large Style 2 (female)", "object/tangible/painting/painting_twilek_f_lg_s02.iff",
					"Twilek Large Style 3 (female)", "object/tangible/painting/painting_twilek_f_lg_s03.iff",
					"Twilek Large Style 4 (female)", "object/tangible/painting/painting_twilek_f_lg_s04.iff",
					"Twilek Smalle Style 1 (female)", "object/tangible/painting/painting_twilek_f_sm_s01.iff",
					"Twilek Smalle Style 2 (female)", "object/tangible/painting/painting_twilek_f_sm_s02.iff",
					"Twilek Smalle Style 3 (female)", "object/tangible/painting/painting_twilek_f_sm_s03.iff",
					"Twilek Smalle Style 4 (female)", "object/tangible/painting/painting_twilek_f_sm_s04.iff",
					"Twilek (male)", "object/tangible/painting/painting_twilek_m.iff",
					"Vader Victory", "object/tangible/painting/painting_vader_victory.iff",
					"Valley View", "object/tangible/painting/painting_valley_view.iff",
					"Victorious Reign", "object/tangible/painting/painting_victorious_reign.iff",
					"Waterfall", "object/tangible/painting/painting_waterfall.iff",
					"Wookie (female)", "object/tangible/painting/painting_wookiee_f.iff",
					"Wookie (male)", "object/tangible/painting/painting_wookiee_m.iff",
					"Zabrak (male)", "object/tangible/painting/painting_zabrak_m.iff",
					"Zabrak (female)", "object/tangible/painting/painting_zabrak_f.iff",
					"Veteran Reward (Cast Wing in Flight)", "object/tangible/veteran_reward/one_year_anniversary/painting_01.iff",
					"Veteran Reward (Decimator)", "object/tangible/veteran_reward/one_year_anniversary/painting_02.iff",
					"Veteran Reward (Weapon of War)", "object/tangible/veteran_reward/one_year_anniversary/painting_04.iff",
					"Veteran Reward (Tatooine Dune Speeder)", "object/tangible/veteran_reward/one_year_anniversary/painting_03.iff",
					"Veteran Reward (Fighter Study)", "object/tangible/veteran_reward/one_year_anniversary/painting_05.iff",
					"Veteran Reward (Hutt Greed)", "object/tangible/veteran_reward/one_year_anniversary/painting_06.iff",
					"Veteran Reward (Smuggler's Run)", "object/tangible/veteran_reward/one_year_anniversary/painting_07.iff",
					"Veteran Reward (Imperial TIE Oppressor)", "object/tangible/veteran_reward/one_year_anniversary/painting_08.iff",
					"Veteran Reward (Emperor's Eyes TIE Sentinel)", "object/tangible/veteran_reward/one_year_anniversary/painting_09.iff"
				},
				"Lighting",
				{
					"Candle (Bestine)", "object/tangible/furniture/all/frn_all_lamp_candlestick_free_s01.iff",
					"Candle (Bestine) Lit", "object/tangible/furniture/all/frn_all_lamp_candlestick_free_s01_lit.iff",
					"Candle (Dearic)", "object/tangible/furniture/all/frn_all_lamp_candlestick_free_s02.iff",
					"Candle (Dearic) Lit", "object/tangible/furniture/all/frn_all_lamp_candlestick_free_s02_lit.iff",
					"Candle (Restus)", "object/tangible/furniture/all/frn_all_lamp_candlestick_tbl_s01.iff",
					"Candle (Theed)", "object/tangible/furniture/all/frn_all_lamp_candlestick_tbl_s02.iff",
					"Candle (Coronet)", "object/tangible/furniture/all/frn_all_lamp_candlestick_tbl_s03.iff",
					"Desk Lamp (Bantha)", "object/tangible/furniture/all/frn_all_lamp_desk_s01.iff",
					"Desk Lamp (Bantha) Lit", "object/tangible/furniture/all/frn_all_lamp_desk_s01_lit.iff",
					"Desk Lamp (Rancor)", "object/tangible/furniture/all/frn_all_lamp_desk_s02.iff",
					"Desk Lamp (Rancor) Lit", "object/tangible/furniture/all/frn_all_lamp_desk_s02_lit.iff",
					"Free-standing Lamp (Blue-leaf)", "object/tangible/furniture/all/frn_all_lamp_free_s01.iff",
					"Free-standing Lamp (Blue-leaf) Lit", "object/tangible/furniture/all/frn_all_lamp_free_s01_lit.iff",
					"Free-standing Lamp (Razorcoil)", "object/tangible/furniture/all/frn_all_lamp_free_s02.iff",
					"Free-standing Lamp (Razorcoil) Lit", "object/tangible/furniture/all/frn_all_lamp_free_s02_lit.iff",
					"Free-standing Lamp (Tallbirch)", "object/tangible/furniture/all/frn_all_lamp_free_s03.iff",
					"Free-standing Lamp (Tallbirch) Lit", "object/tangible/furniture/all/frn_all_lamp_free_s03_lit.iff",
					"Free-standing Lamp (Azeron)", "object/tangible/furniture/all/frn_all_lamp_free_s04.iff",
					"Free-standing Lamp (Azeron) Lit", "object/tangible/furniture/all/frn_all_lamp_free_s04_lit.iff",
					"Table-top Lamp (Tatooine)", "object/tangible/furniture/all/frn_all_lamp_table_s01.iff",
					"Table-top Lamp (Tatooine) Lit", "object/tangible/furniture/all/frn_all_lamp_tbl_s01_lit.iff",
					"Table-top Lamp (Corellia)", "object/tangible/furniture/all/frn_all_lamp_table_s02.iff",
					"Table-top Lamp (Corellia) Lit", "object/tangible/furniture/all/frn_all_lamp_tbl_s02_lit.iff",
					"Table-top Lamp (Naboo)", "object/tangible/furniture/all/frn_all_lamp_table_s03.iff",
					"Table-top Lamp (Naboo) Lit", "object/tangible/furniture/all/frn_all_lamp_tbl_s03_lit.iff",
					"Table-top Lamp (Coruscant)", "object/tangible/furniture/all/frn_all_lamp_tatt_s01.iff",
					"Table-top Lamp(Coruscant) Lit", "object/tangible/furniture/all/frn_all_lamp_tatt_s01_lit.iff",

				},
				"Rugs and Tapestry",
				{
					"Large Rectangular Rug (Style 1)", "object/tangible/furniture/modern/rug_rect_lg_s01.iff",
					"Large Rectangular Rug (Style 2)", "object/tangible/furniture/modern/rug_rect_lg_s02.iff",
					"Large Round Rug (Style 1)", "object/tangible/furniture/modern/rug_rnd_lg_s01.iff",
					"Large Round Rug (Style 2)", "object/tangible/furniture/modern/rug_rnd_sml_s01.iff",
					"Large Oval Rug (Style 1)", "object/tangible/furniture/modern/rug_oval_sml_s01.iff",
					"Large Oval Rug (Style 2)", "object/tangible/furniture/modern/rug_oval_m_s02.iff",
					"Large Square Rug", "object/tangible/furniture/modern/rug_sqr_m_s01.iff",
					"Ornate Blue Rug", "object/tangible/loot/collectible/collectible_rewards/blue_rug.iff",
					"Ornate Orange Rug", "object/tangible/loot/collectible/collectible_rewards/orange_rug.iff",
					"Fine Woven Rug (Style 1)", "object/tangible/furniture/all/frn_all_rug_rectangle_large_style_01.iff",
					"Fine Woven Rug (Style 2)", "object/tangible/furniture/all/frn_all_rug_rectangle_large_style_02.iff",
					"Fine Woven Rug (Style 3)", "object/tangible/furniture/all/frn_all_rug_rectangle_large_style_03.iff",
					"Fine Woven Rug (Style 4)", "object/tangible/furniture/all/frn_all_rug_rectangle_large_style_04.iff",
					"Fine Woven Rug (Style 5)", "object/tangible/furniture/all/frn_all_rug_rectangle_large_style_05.iff",
					"Tatooine Tapestry","object/tangible/furniture/decorative/tatooine_tapestry.iff",
				-- The following do not generate an item, but are in the TRE files.
				--"Bestine Rug","object/tangible/furniture/modern/bestine_quest_rug",
				--"Large Oval Rug (Style 3)","object/tangible/furniture/modern/rug_oval_lg_s01",
				--"Large Round Rug (Style 3)","object/tangible/furniture/modern/rug_rnd_m_s01",
				--"Small Rectangular Rug","object/tangible/furniture/modern/rug_rect_sml_s01",
				--"Medium Rectangular Rug","object/tangible/furniture/modern/rug_rect_m_s01",
				},
				"Plants",
				{
					"Large Potted Plant (Style 1)", "object/tangible/furniture/all/frn_all_plant_potted_lg_s1.iff",
					"Large Potted Plant (Style 2)", "object/tangible/furniture/all/frn_all_plant_potted_lg_s2.iff",
					"Large Potted Plant (Style 3)", "object/tangible/furniture/all/frn_all_plant_potted_lg_s3.iff",
					"Large Potted Plant (Style 4)", "object/tangible/furniture/all/frn_all_plant_potted_lg_s4.iff",
					"Small Potted Plant (Style 1)", "object/tangible/furniture/all/frn_all_potted_plants_sml_s01.iff",
					"Small Potted Plant (Style 2)", "object/tangible/furniture/all/frn_all_potted_plants_sml_s02.iff",
					"Small Potted Plant (Style 3)", "object/tangible/furniture/all/frn_all_potted_plants_sml_s03.iff",
					"Small Potted Plant (Style 4)", "object/tangible/furniture/all/frn_all_potted_plants_sml_s04.iff",
					"Small Potted Plant (Style 5)", "object/tangible/furniture/all/frn_all_potted_plants_sml_s05.iff",
					"Potted Tree (Style 1)", "object/tangible/furniture/all/frn_all_tree_potted_s1.iff",
					"Potted Tree (Style 2)", "object/tangible/furniture/all/frn_all_tree_potted_s2.iff",
					"Hanging Planter","object/tangible/furniture/decorative/hanging_planter.iff",
					"Small Aurilian Plant", "object/tangible/loot/plant_grow/plant_stage_1.iff"
				},
				"Miscellaneous Decorations",
				{
					"Bacta Tank", "object/tangible/item/quest/force_sensitive/bacta_tank.iff",
					"Bottle (Style 1)", "object/tangible/furniture/decorative/bottle_pear.iff",
					"Bottle (Style 2)", "object/tangible/furniture/decorative/bottle_fat.iff",
					"Bottle (Style 3)", "object/tangible/furniture/decorative/bottle_tall.iff",
					"Bowl", "object/tangible/furniture/decorative/carved_bowl.iff",
					"Diagnostic Screen", "object/tangible/furniture/decorative/diagnostic_screen.iff",
					"Microphone", "object/tangible/furniture/decorative/microphone_s01.iff",
					"Radio", "object/tangible/furniture/decorative/radio.iff",
					"Imperial Banner", "object/tangible/furniture/all/bestine_quest_imp_banner.iff",
					"Rebel Banner", "object/tangible/furniture/all/frn_all_banner_rebel.iff",
					"Tiki Torch", "object/tangible/furniture/all/frn_all_tiki_torch_s1.iff",
					"Stuffed Fish", "object/tangible/furniture/decorative/stuffed_fish.iff",
					"Blowfish", "object/tangible/fishing/fish/blowfish.iff",
					"Nabooian Sculpture", "object/tangible/loot/collectible/collectible_rewards/collectible_sculpture_s01.iff",
					"Glass Table", "object/tangible/loot/collectible/collectible_rewards/collectible_table.iff",
					"Large Bed (Style 1)", "object/tangible/furniture/all/frn_all_bed_lg_s1.iff",
					"Large Bed (Style 2)", "object/tangible/furniture/all/frn_all_bed_lg_s2.iff",
					"Small Bed (Style 1) Red", "object/tangible/furniture/all/frn_all_bed_sm_s1.iff",
					"Small Bed (Style 2) Blue", "object/tangible/furniture/all/frn_all_bed_sm_s2.iff",
					"Giant Bunker Table", "object/tangible/furniture/imperial/bunker_table_giant_style_01.iff",
					"Decorative Campfire", "object/tangible/furniture/decorative/campfire.iff",
					"Portable Stove", "object/tangible/furniture/decorative/portable_stove.iff",
					"Spear Rack", "object/tangible/furniture/decorative/spear_rack.iff",
					"Tanning Hide (Style 1)", "object/tangible/furniture/decorative/tanning_hide_s01.iff",
					"Tanning Hide (Style 2)", "object/tangible/furniture/decorative/tanning_hide_s02.iff",
					"Command Console", "object/tangible/furniture/all/frn_all_command_console.iff",
					"Wooden Chair (Style 1) Orange", "object/tangible/furniture/all/frn_all_chair_wooden_s1.iff",
					"Wooden Chair (Style 2) Orange", "object/tangible/furniture/all/frn_all_chair_wooden_s2.iff",
					"Love Seat (Style 2)", "object/tangible/furniture/all/frn_all_couch_love_seat_s2.iff",
					"A Piece of Art (Style 1)", "object/tangible/furniture/all/frn_all_decorative_sm_s1.iff",
					"A Piece of Art (Style 2)", "object/tangible/furniture/all/frn_all_decorative_sm_s2.iff",
					"A Piece of Art (Style 3)", "object/tangible/furniture/all/frn_all_decorative_sm_s3.iff",
					"A Piece of Art (Style 4)", "object/tangible/furniture/all/frn_all_decorative_sm_s4.iff",
					"Gaming Kiosk", "object/tangible/furniture/all/frn_all_gaming_kiosk_s01.iff",
					"Technical Console (Style 1)", "object/tangible/furniture/all/frn_all_technical_console_s01.iff",
					"Technical Console (Style 2)", "object/tangible/furniture/all/frn_all_technical_console_s02.iff",
					"Large Toolchest", "object/tangible/furniture/all/frn_all_toolchest_lg_s01.iff",
					"Small Toolchest", "object/tangible/furniture/all/frn_all_toolchest_med_s01.iff",
					"Throwpillow (Brown)", "object/tangible/furniture/all/frn_all_throwpillow_med_s01.iff",
					"Throwpillow (Red)", "object/tangible/furniture/all/frn_all_throwpillow_med_s02.iff",
					"Throwpillow (Blue)", "object/tangible/furniture/all/frn_all_throwpillow_med_s03.iff"
				},
				"Elegant Furniture (COMPLETE)",
				{
					"Elegant Armoire", "object/tangible/furniture/elegant/armoire_elegant_s01.iff",
					"Elegant Bookcase", "object/tangible/furniture/elegant/bookcase_s01.iff",
					"Elegant Cabinet", "object/tangible/furniture/elegant/cabinet_s01.iff",
					"Elegant Chair", "object/tangible/furniture/elegant/chair_s01.iff",
					"Elegant Chest", "object/tangible/furniture/elegant/chest_s01.iff",
					"Elegant Coffee Table", "object/tangible/furniture/elegant/coffee_table_s01.iff",
					"Elegant Couch", "object/tangible/furniture/elegant/couch_s01.iff",
					"Elegant End Table", "object/tangible/furniture/elegant/end_table_s01.iff",
					"Elegant Love Seat", "object/tangible/furniture/elegant/love_seat_s01.iff"
				},
				"Cheap Furniture (COMPLETE)",
				{
					"Cheap Armoire", "object/tangible/furniture/cheap/armoire_s01.iff",
					"Cheap Bookcase", "object/tangible/furniture/cheap/bookcase_s01.iff",
					"Cheap Cabinet", "object/tangible/furniture/cheap/cabinet_s01.iff",
					"Cheap Chair", "object/tangible/furniture/cheap/chair_s01.iff",
					"Cheap Chest", "object/tangible/furniture/cheap/chest_s01.iff",
					"Cheap Coffee Table", "object/tangible/furniture/cheap/coffee_table_s01.iff",
					"Cheap Couch", "object/tangible/furniture/cheap/couch_s01.iff",
					"Cheap End Table", "object/tangible/furniture/cheap/end_table_s01.iff",
					"Cheap Love Seat", "object/tangible/furniture/cheap/love_seat_s01.iff"
				},
				"Technical Furniture (COMPLETE)",
				{
					"Technical Armoire", "object/tangible/furniture/technical/armoire_s01.iff",
					"Technical Bookcase", "object/tangible/furniture/technical/bookcase_s01.iff",
					"Technical Cabinet", "object/tangible/furniture/technical/cabinet_s01.iff",
					"Technical Chair", "object/tangible/furniture/technical/chair_s01.iff",
					"Technical Chest", "object/tangible/furniture/technical/chest_s01.iff",
					"Technical Coffee Table", "object/tangible/furniture/technical/coffee_table_s01.iff",
					"Technical Couch", "object/tangible/furniture/technical/couch_s01.iff",
					"Technical End Table", "object/tangible/furniture/technical/end_table_s01.iff"
				},
				"Plain Furniture (COMPLETE)",
				{
					"Plain Armoire", "object/tangible/furniture/plain/plain_armoire_s01.iff",
					"Plain Bookcase", "object/tangible/furniture/plain/plain_bookcase_s01.iff",
					"Plain Cabinet", "object/tangible/furniture/plain/plain_cabinet_s01.iff",
					"Plain Chair", "object/tangible/furniture/plain/plain_chair_s01.iff",
					"Plain Chest", "object/tangible/furniture/plain/plain_chest_s01.iff",
					"Plain Coffee Table", "object/tangible/furniture/plain/plain_coffee_table_s01.iff",
					"Plain Couch", "object/tangible/furniture/plain/plain_couch_s01.iff",
					"Plain End Table", "object/tangible/furniture/plain/plain_end_table_s01.iff",
					"Plain Love Seat", "object/tangible/furniture/plain/plain_love_seat_s01.iff"
				},
				"Modern Furniture (COMPLETE)",
				{
					"Modern Armoire", "object/tangible/furniture/modern/armoire_modern_style_01.iff",
					"Modern Bookcase", "object/tangible/furniture/modern/bookcase_modern_style_01.iff",
					"Modern Cabinet", "object/tangible/furniture/modern/cabinet_modern_style_01.iff",
					"Modern Chair", "object/tangible/furniture/modern/chair_modern_style_01.iff",
					"Modern Chest", "object/tangible/furniture/modern/chest_modern_style_01.iff",
					"Modern Coffee Table", "object/tangible/furniture/modern/coffee_table_modern_style_01.iff",
					"Modern Couch (Style 1)", "object/tangible/furniture/modern/couch_modern_style_01.iff",
					"Modern Couch (Style 2)", "object/tangible/furniture/modern/couch_modern_style_02.iff",
					"Modern Dining Table", "object/tangible/furniture/modern/dining_table_modern_style_01.iff",
					"Modern End Table", "object/tangible/furniture/modern/end_table_large_modern_style_01.iff",
					"Modern Love Seat", "object/tangible/furniture/modern/love_seat_modern_style_01.iff",
					"Modern Coffee Table (Style 2)", "object/tangible/furniture/modern/coffee_table_modern_style_02.iff",
					"Modern End Table - Small", "object/tangible/furniture/modern/end_table_small_modern_style_01.iff",
					"Modern Sofa Chair", "object/tangible/furniture/modern/sofa_chair_modern_style_01.iff",
					"Modern Table", "object/tangible/furniture/modern/table_modern_style_01.iff"
				},
				"Camp Furniture",
				-- Commented items don't have a mouseover hit box, which makes them too difficult to interact with.
				-- Uncomment them when the issue has been resolved.
				{
					--"Ashes","object/tangible/camp/campfire_logs_ash.iff",
					--"Logs (Burnt)","object/tangible/camp/campfire_logs_burnt.iff",
					--"Logs (Fresh)","object/tangible/camp/campfire_logs_fresh.iff",
					--"Logs (Smoldering)","object/tangible/camp/campfire_logs_smoldering.iff",
					"Chair (Style 1)","object/tangible/camp/camp_chair_s1.iff",
					"Chair (Style 2)","object/tangible/camp/camp_chair_s2.iff",
					"Chair (Style 3)","object/tangible/camp/camp_chair_s3.iff",
					--"Cot (Style 1)","object/tangible/camp/camp_cot_s1.iff",
					--"Cot (Style 2)","object/tangible/camp/camp_cot_s2.iff",
					--"Cot (Style 3)","object/tangible/camp/camp_cot_s3.iff",
					--"Crate","object/tangible/camp/camp_crate_s1.iff",
					--"Lantern (Style 1)","object/tangible/camp/camp_lantern_s1.iff",
					--"Lantern (Style 2)","object/tangible/camp/camp_lantern_s2.iff",
					--"Lantern (Style 3)","object/tangible/camp/camp_lantern_s3.iff",
					--"Torch (Tall)","object/tangible/camp/camp_light_s1.iff",
					--"Light Post","object/tangible/camp/camp_light_s2.iff",
					--"Light Post (Beacon)","object/tangible/camp/camp_light_s3.iff",
					"Meat Skewer","object/tangible/camp/camp_spit_s1.iff",
					"Camp Center (Small)","object/tangible/camp/camp_spit_s2.iff",
					"Camp Center (Large)","object/tangible/camp/camp_spit_s3.iff",
				--"Stool (Short)","object/tangible/camp/camp_stool_short.iff",
				--"Stool (Tall)","object/tangible/camp/camp_stool_tall.iff",
				},
				"Jedi Furniture",
				{
					"Dark Banner", "object/tangible/furniture/jedi/frn_all_banner_dark.iff",
					"Light Banner", "object/tangible/furniture/jedi/frn_all_banner_light.iff",
					"Dark Chair (Style 1)", "object/tangible/furniture/jedi/frn_all_dark_chair_s01.iff",
					"Dark Chair (Style 2)", "object/tangible/furniture/jedi/frn_all_dark_chair_s02.iff",
					"Dark Throne", "object/tangible/furniture/jedi/frn_all_dark_throne.iff",
					"Light Chair (Style 1)", "object/tangible/furniture/jedi/frn_all_light_chair_s01.iff",
					"Light Chair (Style 2)", "object/tangible/furniture/jedi/frn_all_light_chair_s02.iff",
					"Light Throne","object/tangible/furniture/jedi/frn_all_light_throne.iff",
					"Dark Table (Style 1)", "object/tangible/furniture/jedi/frn_all_table_dark_01.iff",
					"Dark Table (Style 2)", "object/tangible/furniture/jedi/frn_all_table_dark_02.iff",
					"Light Table (Style 1)", "object/tangible/furniture/jedi/frn_all_table_light_01.iff",
					"Light Table (Style 2)", "object/tangible/furniture/jedi/frn_all_table_light_02.iff",
					"Jedi Council Seat", "object/tangible/furniture/all/frn_all_jedi_council_seat.iff",
				},
				"City Decorations",
				{
					"Brazier - Round", "object/tangible/furniture/city/fountain_brazier_round.iff",
					"Brazier - Square", "object/tangible/furniture/city/fountain_brazier_square.iff",
					"Circular Fountain", "object/tangible/furniture/city/fountain_circle.iff",
					"Fountain of Contemplation", "object/tangible/furniture/city/fountain_contemplate.iff",
					"Fountain (No Water)", "object/tangible/furniture/city/fountain_generic.iff",
					"Fountain of Heroism", "object/tangible/furniture/city/fountain_heroic.iff",
					"Retangular Fountain", "object/tangible/furniture/city/fountain_rectangle.iff",
					"Gungan Head Statue", "object/tangible/furniture/city/statue_gungan_head.iff",
					"Sentinel Statue", "object/tangible/furniture/city/statue_sentinel_01.iff",
					"Regal Battle Statue", "object/tangible/furniture/city/statue_sentinel_02.iff",
					"Tatooine Sun Symbol", "object/tangible/furniture/city/statue_tatooine_sunball.iff",
					"Strange Statue", "object/tangible/furniture/city/statue_weird_01.iff",
					"Weird Statue", "object/tangible/furniture/city/statue_weird_02.iff",
					"Street Lamp (Style 1) White", "object/tangible/furniture/city/streetlamp_large_01.iff",
					"Street Lamp (Style 2) White", "object/tangible/furniture/city/streetlamp_large_02.iff",
					"Street Lamp (Style 3) White", "object/tangible/furniture/city/streetlamp_med_01.iff",
					"Street Lamp (Style 4) White", "object/tangible/furniture/city/streetlamp_med_02.iff",
					"Street Lamp (Style 1) Blue", "object/tangible/furniture/city/streetlamp_large_blue_01.iff",
					"Street Lamp (Style 2) Blue", "object/tangible/furniture/city/streetlamp_large_blue_02.iff",
					"Street Lamp (Style 3) Blue", "object/tangible/furniture/city/streetlamp_med_blue_01.iff",
					"Street Lamp (Style 4) Blue", "object/tangible/furniture/city/streetlamp_med_blue_02.iff",
					"Street Lamp (Style 1) Green", "object/tangible/furniture/city/streetlamp_large_green_01.iff",
					"Street Lamp (Style 1) Green", "object/tangible/furniture/city/streetlamp_large_green_02.iff",
					"Street Lamp (Style 3) Green", "object/tangible/furniture/city/streetlamp_med_green_01.iff",
					"Street Lamp (Style 4) Green", "object/tangible/furniture/city/streetlamp_med_green_02.iff",
					"Street Lamp (Style 1) Red", "object/tangible/furniture/city/streetlamp_large_red_01.iff",
					"Street Lamp (Style 1) Red", "object/tangible/furniture/city/streetlamp_large_red_02.iff",
					"Street Lamp (Style 3) Red", "object/tangible/furniture/city/streetlamp_med_red_01.iff",
					"Street Lamp (Style 4) Red", "object/tangible/furniture/city/streetlamp_med_red_02.iff"
				},
			},
			--"Intruments",
			--{
			--"Bandfill", "object/tangible/instrument/bandfill.iff",
			--"Fanfar", "object/tangible/instrument/fanfar.iff",
			--"Fizz", "object/tangible/instrument/fizz.iff",
			--"Flute Droopy", "object/tangible/instrument/flute_droopy.iff",
			--"Kloo Horn", "object/tangible/instrument/kloo_horn.iff",
			--"Mandoviol", "object/tangible/instrument/mandoviol.iff",
			--"Nalargon", "object/tangible/instrument/nalargon.iff",
			--"Ommni Box", "object/tangible/instrument/ommni_box.iff",
			--"Slitherhorn", "object/tangible/instrument/slitherhorn.iff",
			--"Traz", "object/tangible/instrument/traz.iff"
			--},
			"Pharmaceutical",
			{
				"StimPack A", "object/tangible/medicine/crafted/crafted_stimpack_sm_s1_a.iff",
				"StimPack B", "object/tangible/medicine/crafted/crafted_stimpack_sm_s1_b.iff",
				"StimPack C", "object/tangible/medicine/crafted/crafted_stimpack_sm_s1_c.iff",
				"StimPack D", "object/tangible/medicine/crafted/crafted_stimpack_sm_s1_d.iff",
				"StimPack E", "object/tangible/medicine/crafted/crafted_stimpack_sm_s1_e.iff",
				"Medpack Grenade Damage A", "object/tangible/medicine/crafted/medpack_grenade_damage_a.iff",
				"Medpack Grenade Damage B", "object/tangible/medicine/crafted/medpack_grenade_damage_b.iff",
				"Medpack Grenade Damage C", "object/tangible/medicine/crafted/medpack_grenade_damage_c.iff",
				"Medpack Grenade Damage D", "object/tangible/medicine/crafted/medpack_grenade_damage_d.iff",
				"Medpack Grenade Damage E", "object/tangible/medicine/crafted/medpack_grenade_damage_e.iff",
				"Medpack Grenade Area A", "object/tangible/medicine/crafted/medpack_grenade_area_a.iff",
				"Medpack Grenade Area B", "object/tangible/medicine/crafted/medpack_grenade_area_b.iff",
				"Medpack Grenade Area C", "object/tangible/medicine/crafted/medpack_grenade_area_c.iff",
				"Medpack Enhance Disease A", "object/tangible/medicine/crafted/medpack_enhance_disease_a.iff",
				"Medpack Enhance Disease B", "object/tangible/medicine/crafted/medpack_enhance_disease_b.iff",
				"Medpack Enhance Disease C", "object/tangible/medicine/crafted/medpack_enhance_disease_c.iff",
				"Medpack Enhance Poison A", "object/tangible/medicine/crafted/medpack_enhance_poison_a.iff",
				"Medpack Enhance Poison B", "object/tangible/medicine/crafted/medpack_enhance_poison_b.iff",
				"Medpack Enhance Poison C", "object/tangible/medicine/crafted/medpack_enhance_poison_c.iff",
				"Fire blanket", "object/tangible/medicine/crafted/medic_fire_blanket.iff",
				"Medpack Cure Disease A", "object/tangible/medicine/crafted/medpack_cure_disease_a.iff",
				"Medpack Cure Disease Area A", "object/tangible/medicine/crafted/medpack_cure_disease_area_a.iff",
				"Medpack Cure Poison A", "object/tangible/medicine/crafted/medpack_cure_poison_a.iff",
				"Medpack Cure Poison Area A", "object/tangible/medicine/crafted/medpack_cure_poison_area_a.iff",
				"Disease Health Area A", "object/tangible/medicine/crafted/medpack_disease_area_health_a.iff",
				"Poison Health Area A", "object/tangible/medicine/crafted/medpack_poison_area_health_a.iff",
				"Medpack Blinded", "object/tangible/medicine/crafted/medpack_blinded_a.iff",
				"Medpack Dizzy", "object/tangible/medicine/crafted/medpack_dizzy_a.iff",
				"Medpack Intimidated", "object/tangible/medicine/crafted/medpack_intimidated_a.iff",
				"Medpack Stunned", "object/tangible/medicine/crafted/medpack_stunned_a.iff",
				"Revive Pack", "object/tangible/medicine/medpack_revive.iff"
			},
			--"Scout Equipment",
			--{
			--"Camokit",
			--{
			--"Corellia", "object/tangible/scout/camokit/camokit_corellia.iff",
			--"Dantooine", "object/tangible/scout/camokit/camokit_dantooine.iff",
			--"Dathomir", "object/tangible/scout/camokit/camokit_dathomir.iff",
			--"Endor", "object/tangible/scout/camokit/camokit_endor.iff",
			--"Lok", "object/tangible/scout/camokit/camokit_lok.iff",
			--"Naboo", "object/tangible/scout/camokit/camokit_naboo.iff",
			--"Rori", "object/tangible/scout/camokit/camokit_rori.iff",
			--"Talus", "object/tangible/scout/camokit/camokit_talus.iff",
			--"Tatooine", "object/tangible/scout/camokit/camokit_tatooine.iff",
			--"Yavin", "object/tangible/scout/camokit/camokit_yavin.iff"
			--},
			--"Camps",
			--{
			--"Basic", "object/tangible/scout/camp/camp_basic.iff",
			--"Elite", "object/tangible/scout/camp/camp_elite.iff",
			--"Improved", "object/tangible/scout/camp/camp_improved.iff",
			--"Luxury", "object/tangible/scout/camp/camp_luxury.iff",
			--"Multi", "object/tangible/scout/camp/camp_multi.iff",
			--"Quality", "object/tangible/scout/camp/camp_quality.iff"
			--},
			--"Traps",
			--{
			--"Drowsy Dart", "object/tangible/scout/trap/trap_drowsy_dart.iff",
			--"Enraging Spur", "object/tangible/scout/trap/trap_enraging_spur.iff",
			--"Flash Bomb", "object/tangible/scout/trap/trap_flash_bomb.iff",
			--"Melee Defence", "object/tangible/scout/trap/trap_melee_def_1.iff",
			--"Melee Ranged defence", "object/tangible/scout/trap/trap_melee_ranged_def_1.iff",
			--"Noise Maker", "object/tangible/scout/trap/trap_noise_maker.iff",
			--"Ranged Defence", "object/tangible/scout/trap/trap_ranged_def_1.iff",
			--"Ranged Speed", "object/tangible/scout/trap/trap_ranged_speed_1.iff",
			--"Sonic Pulse", "object/tangible/scout/trap/trap_sonic_pulse.iff",
			--"State Defence", "object/tangible/scout/trap/trap_state_def_1.iff",
			--"Tranq Dart", "object/tangible/scout/trap/trap_tranq_dart.iff",
			--"Webber", "object/tangible/scout/trap/trap_webber.iff"
			--}
			--}
			"Tools",
			{
				"Crafting Tools",
				{
					"Clothing and Armor Crafting Tool", "object/tangible/crafting/station/clothing_tool.iff",
					"Food and Chemical Crafting Tool", "object/tangible/crafting/station/food_tool.iff",
					"Generic Crafting Tool", "object/tangible/crafting/station/generic_tool.iff",
					"Lightsaber Crafting Toolkit", "object/tangible/crafting/station/jedi_tool.iff",
					"Starship Crafting Tool", "object/tangible/crafting/station/space_tool.iff",
					"Structure and Furniture Crafting Tool", "object/tangible/crafting/station/structure_tool.iff",
					"Weapon, Droid, and General Item Crafting Tool", "object/tangible/crafting/station/weapon_tool.iff"
				},
				"Crafting Stations",
				{
					"Clothing Crafting Station", "object/tangible/crafting/station/clothing_station.iff",
					"Food Crafting Station", "object/tangible/crafting/station/food_station.iff",
					"Starship Crafting Station", "object/tangible/crafting/station/space_station.iff",
					"Structure Crafting Station", "object/tangible/crafting/station/structure_station.iff",
					"Weapon Crafting Station", "object/tangible/crafting/station/weapon_station.iff"
				},
				"Survey Tools",
				{
					"Gas Survey Tool", "object/tangible/survey_tool/survey_tool_gas.iff",
					--"Inorganic Survey Tool", "oobject/tangible/survey_tool/survey_tool_inorganic.iff",
					"Chemical Survey Tool", "object/tangible/survey_tool/survey_tool_liquid.iff",
					"Flora Survey Tool", "object/tangible/survey_tool/survey_tool_lumber.iff",
					"Mineral Survey Tool", "object/tangible/survey_tool/survey_tool_mineral.iff",
					"Moisture Survey Tool", "object/tangible/survey_tool/survey_tool_moisture.iff",
					--"Organic Survey Tool", "object/tangible/survey_tool/survey_tool_organic.iff",
					"Solar Survey Tool", "object/tangible/survey_tool/survey_tool_solar.iff",
					"Wind Survey Tool", "object/tangible/survey_tool/survey_tool_wind.iff"
				}
			--"Repair Tools",
			--{
			--"Armor Repair", "object/tangible/crafting/station/armor_repair.iff",
			--"Clothing Repair", "object/tangible/crafting/station/clothing_repair.iff",
			--"Droid Repair", "object/tangible/crafting/station/droid_repair.iff",
			--"Structure Repair", "object/tangible/crafting/station/structure_repair.iff",
			--"Weapon Repair", "object/tangible/crafting/station/weapon_repair.iff"
			--}
			},
			"Weapons",
			{
				"Carbines",
				{
					--		"CDEF Carbine", "object/weapon/ranged/carbine/carbine_cdef.iff",
					--		"Corsec CDEF Carbine", "object/weapon/ranged/carbine/carbine_cdef_corsec.iff",
					--		"DH17 Carbine", "object/weapon/ranged/carbine/carbine_dh17.iff",
					--		"DH17 Carbine (black)", "object/weapon/ranged/carbine/carbine_dh17_black.iff",
					--		"DH17 Short Carbine", "object/weapon/ranged/carbine/carbine_dh17_snubnose.iff",
					"DXR6 Carbine", "object/weapon/ranged/carbine/carbine_dxr6.iff",
					--		"E11 Carbine", "object/weapon/ranged/carbine/carbine_e11.iff",
					--		"Enhanced E11 Carbine", "object/weapon/ranged/carbine/carbine_e11_quest.iff",
					--		"Lithitanium Carbine", "object/weapon/ranged/carbine/carbine_e11_victor_quest.iff",
					--		"EE3 Carbine", "object/weapon/ranged/carbine/carbine_ee3.iff",
					--		"Elite Carbine", "object/weapon/ranged/carbine/carbine_elite.iff",
					"Laser Carbine", "object/weapon/ranged/carbine/carbine_laser.iff",
				--		"Nym's Slug-thrower Carbine", "object/weapon/ranged/carbine/carbine_nym_slugthrower.iff"
				},
				"Heavy",
				{
					--		"Acid Stream Launcher", "object/weapon/ranged/heavy/heavy_acid_beam.iff",
					--		"Beam Rifle", "object/weapon/ranged/rifle/rifle_beam.iff",
					"Flame Thrower", "object/weapon/ranged/rifle/rifle_flame_thrower.iff",
					--		"Heavy Acid Rifle", "object/weapon/ranged/rifle/rifle_acid_beam.iff",
					--		"Heavy Particle Beam Cannon", "object/weapon/ranged/heavy/heavy_particle_beam.iff",
					--		"Lightning Beam Cannon", "object/weapon/ranged/heavy/heavy_lightning_beam.iff",
					"Light Lightning Cannon", "object/weapon/ranged/rifle/rifle_lightning.iff",
				--		"Rocket Launcher", "object/weapon/ranged/heavy/heavy_rocket_launcher.iff"
				},
				"One-handed",
				{
					--		"Sword", "object/weapon/melee/sword/sword_01.iff",
					--		"Sword (noob)", "object/weapon/melee/sword/sword_01_noob.iff",
					--		"Curved Sword", "object/weapon/melee/sword/sword_02.iff",
					--		"Ryyk Blade", "object/weapon/melee/sword/sword_blade_ryyk.iff",
					--		"Curved Nyax Sword", "object/weapon/melee/sword/sword_curved_nyax.iff",
					--		"Nyax Sword", "object/weapon/melee/sword/sword_nyax.iff",
					--		"Rantok", "object/weapon/melee/sword/sword_rantok.iff",
					--		"Survival Knife", "object/weapon/melee/knife/knife_survival.iff",
					--		"Dagger", "object/weapon/melee/knife/knife_dagger.iff",
					--		"Donkuwah Knife", "object/weapon/melee/knife/knife_donkuwah.iff",
					--		"Janta Knife", "object/weapon/melee/knife/knife_janta.iff",
					--		"Stone Knife", "object/weapon/melee/knife/knife_stone.iff",
					--		"Stone Knife (noob)", "object/weapon/melee/knife/knife_stone_noob.iff",
					--		"Vibroblade", "object/weapon/melee/knife/knife_vibroblade.iff",
					--		"Jagged Vibroblade", "object/weapon/melee/knife/knife_vibroblade_quest.iff",
					"Stun Baton", "object/weapon/melee/baton/baton_stun.iff",
					"Gaderiffi Baton", "object/weapon/melee/baton/baton_gaderiffi.iff",
				--		"Black Metal Gaderiffi Baton", "object/weapon/melee/baton/victor_baton_gaderiffi.iff"
				},
				"Pistols",
				{
					--		"CDEF Pistol", "object/weapon/ranged/pistol/pistol_cdef.iff",
					--		"CDEF Pistol (noob)", "object/weapon/ranged/pistol/pistol_cdef_noob.iff",
					--		"Corsec CDEF Pistol", "object/weapon/ranged/pistol/pistol_cdef_corsec.iff",
					--		"D18 Pistol", "object/weapon/ranged/pistol/pistol_d18.iff",
					--		"DE-10 Pistol", "object/weapon/ranged/pistol/pistol_de_10.iff",
					--		"DH17 Pistol", "object/weapon/ranged/pistol/pistol_dh17.iff",
					--		"DL44 Pistol", "object/weapon/ranged/pistol/pistol_dl44.iff",
					--		"DL44 Metal Pistol", "object/weapon/ranged/pistol/pistol_dl44_metal.iff",
					--		"DX2 Pistol", "object/weapon/ranged/pistol/pistol_dx2.iff",
					"FWG5 Pistol", "object/weapon/ranged/pistol/pistol_fwg5.iff",
					--		"Featherweight FWG5 Pistol", "object/weapon/ranged/pistol/pistol_fwg5_quest.iff",
					--		"Geonosian Sonic Blaster", "object/weapon/ranged/pistol/pistol_geonosian_sonic_blaster_loot.iff",
					"Launcher Pistol", "object/weapon/ranged/pistol/pistol_launcher.iff",
					--		"Power5 Pistol", "object/weapon/ranged/pistol/pistol_power5.iff",
					"Republic Blaster", "object/weapon/ranged/pistol/pistol_republic_blaster.iff",
					--		"Modified Republic Blaster", "object/weapon/ranged/pistol/pistol_republic_blaster_quest.iff",
					"Scatter Pistol", "object/weapon/ranged/pistol/pistol_scatter.iff",
				--		"Scout Blaster", "object/weapon/ranged/pistol/pistol_scout_blaster.iff",
				--		"SR Combat Pistol", "object/weapon/ranged/pistol/pistol_srcombat.iff",
				--		"Striker Pistol", "object/weapon/ranged/pistol/pistol_striker.iff",
				--		"Striker Pistol (noob)", "object/weapon/ranged/pistol/pistol_striker_noob.iff",
				--		"Tangle Pistol", "object/weapon/ranged/pistol/pistol_tangle.iff"
				},
				"Polearms",
				{
					--		"Lance", "object/weapon/melee/polearm/lance_controllerfp.iff",
					--		"Nightsister Lance", "object/weapon/melee/polearm/lance_controllerfp_nightsister.iff",
					--		"Nightsister Energy Lance", "object/weapon/melee/polearm/lance_nightsister.iff",
					--		"Janta Staff", "object/weapon/melee/polearm/lance_staff_janta.iff",
					--		"Metal Staff", "object/weapon/melee/polearm/lance_staff_metal.iff",
					"Vibro Lance", "object/weapon/melee/polearm/lance_vibrolance.iff",
					"Long Vibro Axe", "object/weapon/melee/polearm/polearm_vibro_axe.iff",
				--		"Wood Staff", "object/weapon/melee/polearm/lance_staff_wood_s1.iff",
				--		"Reinforced Combat Staff", "object/weapon/melee/polearm/lance_staff_wood_s2.iff",
				},
				"Rifles",
				{
					--		"Berserker Rifle", "object/weapon/ranged/rifle/rifle_berserker.iff",
					--		"Bowcaster", "object/weapon/ranged/rifle/rifle_bowcaster.iff",
					--		"CDEF Rifle", "object/weapon/ranged/rifle/rifle_cdef.iff",
					--		"DLT20 Rifle", "object/weapon/ranged/rifle/rifle_dlt20.iff",
					--		"DLT20a Rifle", "object/weapon/ranged/rifle/rifle_dlt20a.iff",
					--		"E11 Rifle", "object/weapon/ranged/rifle/rifle_e11.iff",
					--		"An Ewok Crossbow", "object/weapon/ranged/rifle/rifle_ewok_crossbow.iff",
					"Jawa Ion Rifle", "object/weapon/ranged/rifle/rifle_jawa_ion.iff",
					--		"Laser Rifle", "object/weapon/ranged/rifle/rifle_laser.iff",
					--		"Laser Rifle (noob)", "object/weapon/ranged/rifle/rifle_laser_noob.iff",
					--		"Lithitanium Rifle", "object/weapon/ranged/rifle/rifle_victor_tusken.iff",
					--		"SG82 Rifle", "object/weapon/ranged/rifle/rifle_sg82.iff",
					--		"Spraystick", "object/weapon/ranged/rifle/rifle_spraystick.iff",
					--		"Tenloss DXR-6 Disruptor Rifle", "object/weapon/ranged/rifle/rifle_tenloss_dxr6_disruptor_loot.iff",
					"T21", "object/weapon/ranged/rifle/rifle_t21.iff",
				--		"Tusken Rifle", "object/weapon/ranged/rifle/rifle_tusken.iff"
				},
				"Two-handed",
				{
					"Two-handed Curved Sword", "object/weapon/melee/2h_sword/2h_sword_katana.iff",
					--		"Grooved Two-handed Sword", "object/weapon/melee/2h_sword/2h_sword_katana_quest.iff",
					--		"Scythe Blade", "object/weapon/melee/2h_sword/2h_sword_scythe.iff",
					"Power Hammer", "object/weapon/melee/2h_sword/2h_sword_maul.iff",
				--		"Two-handed Cleaver", "object/weapon/melee/2h_sword/2h_sword_cleaver.iff",
				--		"Black Sun Executioner's Hack", "object/weapon/melee/2h_sword/2h_sword_blacksun_hack.iff",
				--		"Gamorrean Battleaxe", "object/weapon/melee/2h_sword/2h_sword_battleaxe_quest.iff",
				--		"Heavy Axe", "object/weapon/melee/axe/axe_heavy_duty.iff",
				--		"Vibro Axe", "object/weapon/melee/axe/axe_vibroaxe.iff"
				},
				"Unarmed",
				{
					"Vibro Knuckler", "object/weapon/melee/special/vibroknuckler.iff",
				--		"A Blacksun Razor Knuckler", "object/weapon/melee/special/blacksun_razor.iff"
				}
			},
			--"Holocrons",
			--{
			--	"Dark Jedi Holocron", "object/tangible/jedi/jedi_holocron_dark.iff",
			--	"Light Jedi Holocron", "object/tangible/jedi/jedi_holocron_light.iff",
			--	"Dark Jedi Holocron (gift)", "object/tangible/jedi/no_drop_jedi_holocron_dark.iff",
			--	"Light Jedi Holocron (gift)", "object/tangible/jedi/no_drop_jedi_holocron_light.iff"
			--}
			"Village Testing (Temporary)",
			{
				"Resources",
				{
					"Ardanium II", "object/tangible/loot/quest/ardanium_ii.iff",
					"Endrine", "object/tangible/loot/quest/endrine.iff",
					"Ostrine", "object/tangible/loot/quest/ostrine.iff",
					"Rudic", "object/tangible/loot/quest/rudic.iff",
					"Wind Crystal", "object/tangible/loot/quest/wind_crystal.iff",
				},
				"Loot Kit Parts",
				{
					"Tracking Device Assembly Bracket - First Segment", "object/tangible/loot/collectible/collectible_parts/fs_tracking_device_assembly_bracket_01.iff",
					"Tracking Device Assembly Bracket - Second Segment", "object/tangible/loot/collectible/collectible_parts/fs_tracking_device_assembly_bracket_02.iff",
					"Tracking Device Assembly Bracket - Third Segment", "object/tangible/loot/collectible/collectible_parts/fs_tracking_device_assembly_bracket_03.iff",
					"Tracking Device Case - First Section", "object/tangible/loot/collectible/collectible_parts/fs_tracking_device_case_01.iff",
					"Tracking Device Case - Second Section", "object/tangible/loot/collectible/collectible_parts/fs_tracking_device_case_02.iff",
					"Tracking Device Case - Third Section", "object/tangible/loot/collectible/collectible_parts/fs_tracking_device_case_03.iff",
				},
			}
		}
	}
}

ObjectTemplates:addTemplate(object_tangible_terminal_terminal_character_builder, "object/tangible/terminal/terminal_character_builder.iff")
