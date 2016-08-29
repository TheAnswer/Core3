blerx_tango_missions =
	{
		{
			missionType = "escort",
			primarySpawns =
			{
				{ npcTemplate = "jakko_mixon", npcName = "Jakko Mixon" }
			},
			secondarySpawns = {},
			itemSpawns = {},
			rewards =
			{
				{ rewardType = "credits", amount = 1500 },
			}
		},
		{
			missionType = "escort",
			primarySpawns =
			{
				{ npcTemplate = "mellaw_doowon", npcName = "Mellaw Doowon" }
			},
			secondarySpawns =
			{
				{ npcTemplate = "stormtrooper", npcName = "Imperial Stormtrooper" }
			},
			itemSpawns = {},
			rewards =
			{
				{ rewardType = "credits", amount = 3000 },
			}
		},
		{
			missionType = "escort",
			primarySpawns =
			{
				{ npcTemplate = "baze_joz", npcName = "Baze Joz" }
			},
			secondarySpawns =
			{
				{ npcTemplate = "imperial_sergeant", npcName = "Imperial Sergeant" },
				{ npcTemplate = "imperial_sergeant", npcName = "Imperial Sergeant" },
				{ npcTemplate = "imperial_sergeant", npcName = "Imperial Sergeant" }
			},
			itemSpawns = {},
			rewards =
			{
				{ rewardType = "loot",  lootGroup = "task_reward_blerx_tango" },
			}
		},

	}

npcMapBlerxTango =
	{
		{
			spawnData = { npcTemplate = "blerx_tango", x = -19.161, z = 7.01, y = -8.1, direction = 25, cellID = 1153497, position = STAND },
			worldPosition = { x = 1692, y = 3038 },
			npcNumber = 1,
			stfFile = "@static_npc/tatooine/blerx_tango",
			missions = blerx_tango_missions
		},
	}

BlerxTango = ThemeParkLogic:new {
	npcMap = npcMapBlerxTango,
	className = "BlerxTango",
	screenPlayState = "blerx_tango_quest",
	planetName = "tatooine",
	distance = 800,
	faction = FACTIONREBEL

}

registerScreenPlay("BlerxTango", true)

blerx_tango_mission_giver_conv_handler = mission_giver_conv_handler:new {
	themePark = BlerxTango
}
blerx_tango_mission_target_conv_handler = mission_target_conv_handler:new {
	themePark = BlerxTango
}