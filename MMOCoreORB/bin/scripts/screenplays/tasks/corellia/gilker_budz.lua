poacherHideout =
	{
		type = "destructible",
		buildingTemplate = "object/building/military/pirate_warehouse.iff",
		terminal = { template = "object/tangible/terminal/terminal_destructible_building.iff", vectorCellID = 3, x = 0, z = 0.3, y = -6.0 },
		childNpcs =
		{
			{ npcTemplate = "poacher", npcName = "Poacher Leader", vectorCellID = 3, x = 0, z = 0.3, y = -3.0 }
		}
	}

gilker_budz_missions =
{
	{
		missionType = "retrieve",
		primarySpawns =
		{
			{ npcTemplate = "sneeroos", npcName = "Snee-Roos" }
		},
		secondarySpawns =	
		{
			{ npcTemplate = "thug", npcName = "a thug" }
		},
		itemSpawns =
		{
			{ itemTemplate = "object/tangible/mission/quest_item/gilker_budz_q1_needed.iff", itemName = "Datadisc" }
		},
		rewards =
		{
			{ rewardType = "credits", amount = 250 }
		}
	},
	{
		missionType = "destroy",
		buildingSpawn = poacherHideout,
		primarySpawns =
		{
			{ npcTemplate = "poacher", npcName = "a Poacher pelt-skinner" }
		},
		secondarySpawns =
		{
			{ npcTemplate = "poacher", npcName = "" },
			{ npcTemplate = "poacher", npcName = "" },
			{ npcTemplate = "poacher", npcName = "" },
			{ npcTemplate = "poacher", npcName = "" },
			{ npcTemplate = "poacher", npcName = "" },
			{ npcTemplate = "war_gronda", npcName = "a wounded Gronda" },
			{ npcTemplate = "probot", npcName = "a reprogrammed Probe" }
		},
		staticObjects =
			{
				{ objectTemplate = "object/tangible/furniture/decorative/foodcart.iff", objectName = "Gronda Meat Storage" },
				{ objectTemplate = "object/tangible/furniture/decorative/tanning_hide_s01.iff", objectName = "Gronda Hide" },
				{ objectTemplate = "object/tangible/furniture/decorative/tanning_hide_s02.iff", objectName = "Gronda Pelt" },
				{ objectTemplate = "object/static/structure/general/streetlamp_small_style_02_on.iff", objectName = "" }
			},
		itemSpawns =
		{

		},
		rewards =
		{
			{ rewardType = "credits", amount = 500 }
		}
	},
	{
		missionType = "assassinate",
		primarySpawns =
		{
			{ npcTemplate = "slice_hound", npcName = "" }
		},
		secondarySpawns =
		{
			{ npcTemplate = "vicious_slice_hound", npcName = "a vicious slice hound" },
			{ npcTemplate = "vicious_slice_hound", npcName = "a vicious slice hound" },
			{ npcTemplate = "vicious_slice_hound", npcName = "a vicious slice hound" },
			{ npcTemplate = "vicious_slice_hound", npcName = "a vicious slice hound" },
			{ npcTemplate = "vicious_slice_hound", npcName = "a vicious slice hound" },
			{ npcTemplate = "commoner", npcName = "a scared Citizen" },
			{ npcTemplate = "commoner_fat", npcName = "a scared Citizen" }
		},
		itemSpawns =
		{

		},
		rewards =
		{
			{ rewardType = "credits", amount = 750 }
		}
	}
}

npcMapGilkerBudz =
{
	{
		spawnData = { npcTemplate = "gilker_budz", x = -3336.21, z = 72.4231, y = 2926.19, direction = 277.144, cellID = 0, position = STAND },
		worldPosition = { x = -3336, y = 2926 },
		npcNumber = 1,
		stfFile = "@static_npc/corellia/gilker_budz",
		missions = gilker_budz_missions
	}
}

GilkerBudz = ThemeParkLogic:new {
	npcMap = npcMapGilkerBudz,
	className = "GilkerBudz",
	screenPlayState = "gilker_budz_quest",
	planetName = "corellia",
	distance = 1100
}

registerScreenPlay("GilkerBudz", true)

gilker_budz_mission_giver_conv_handler = mission_giver_conv_handler:new {
	themePark = GilkerBudz
}

gilker_budz_mission_target_conv_handler = mission_target_conv_handler:new {
	themePark = GilkerBudz
}