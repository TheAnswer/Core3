rakir_banai_missions =
	{
		{
			missionType = "assassinate",
			primarySpawns =
			{
				{ npcTemplate = "triggo_binz", planetName = "tatooine", npcName = "Triggo Binz" }
			},
			secondarySpawns = {
				{ npcTemplate = "thug", planetName = "tatooine", npcName = "" },
				{ npcTemplate = "thug", planetName = "tatooine", npcName = "" }
			},
			itemSpawns = {},
			rewards =
			{
				{ rewardType = "credits", amount = 1000 }
			}
		},
		{
			missionType = "retrieve",
			primarySpawns =
			{
				{ npcTemplate = "silman_voueen", planetName = "tatooine", npcName = "Silm'an Voueen" }
			},
			secondarySpawns = {
				{ npcTemplate = "thug", planetName = "tatooine", npcName = "" },
				{ npcTemplate = "jabba_thug", planetName = "tatooine", npcName = "" }
			},
			itemSpawns =
			{
				{ itemTemplate = "object/tangible/mission/quest_item/rakir_banai_q2_needed.iff", itemName = "" }
			},
			rewards =
			{
				{ rewardType = "credits", amount = 2000 },
			}
		},
		{
			missionType = "escort",
			primarySpawns =
			{
				{ npcTemplate = "deela_quellor", planetName = "tatooine", npcName = "Deela Quellor" }
			},
			secondarySpawns = {
				{ npcTemplate = "jabba_thug", planetName = "tatooine", npcName = "" },
				{ npcTemplate = "jabba_thug", planetName = "tatooine", npcName = "" }
			},
			itemSpawns = {
			},
			rewards =
			{
				{ rewardType = "credits", amount = 3000 }
			}
		},
		{
			missionType = "confiscate",
			primarySpawns =
			{
				{ npcTemplate = "wizzel", planetName = "tatooine", npcName = "Wizzel" }
			},
			secondarySpawns = {
				{ npcTemplate = "mercenary_aggro", planetName = "tatooine", npcName = "Mercenary" },
				{ npcTemplate = "mercenary_aggro", planetName = "tatooine", npcName = "Mercenary" }
			},
			itemSpawns =
			{
				{ itemTemplate = "object/tangible/mission/quest_item/rakir_banai_q4_needed.iff", itemName = "" }
			},
			rewards =
			{
				{ rewardType = "loot",  lootGroup = "task_reward_rakir_banai" },
				{ rewardType = "credits", amount = 4000 }
			}
		}
	}

npcMapRakirBanai =
	{
		{
			spawnData = { planetName = "tatooine", npcTemplate = "rakir_banai", x = -5043, z = 75, y = -6600.3, direction = 214, cellID = 0, position = SIT },
			npcNumber = 1,
			stfFile = "@static_npc/tatooine/rakir_banai",
			missions = rakir_banai_missions
		},
	}

RakirBanai = ThemeParkLogic:new {
	numberOfActs = 1,
	npcMap = npcMapRakirBanai,
	permissionMap = {},
	className = "RakirBanai",
	screenPlayState = "rakir_banai_task",
	distance = 600
}

registerScreenPlay("RakirBanai", true)

rakir_banai_mission_giver_conv_handler = mission_giver_conv_handler:new {
	themePark = RakirBanai
}
rakir_banai_mission_target_conv_handler = mission_target_conv_handler:new {
	themePark = RakirBanai
}
