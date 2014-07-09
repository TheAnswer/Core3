slooni_jong_missions =
	{
		{
			missionType = "retrieve",
			primarySpawns =
			{
				{ npcTemplate = "slooni_smuggler", planetName = "talus", npcName = "Smuggler" }
			},
			secondarySpawns =
			{
				{ npcTemplate = "thug", planetName = "talus", npcName = "a Thug" },
				{ npcTemplate = "thug", planetName = "talus", npcName = "a Thug" }
			},
			itemSpawns =
			{
				{ itemTemplate = "object/tangible/mission/quest_item/slooni_jong_q1_needed.iff", itemName = "Briefcase" }
			},
			rewards =
			{
				{ rewardType = "credits", amount = 50 },
				{ rewardType = "faction", faction = "fed_dub", amount = 5 },
			}
		},
		{
			missionType = "escort",
			primarySpawns =
			{
				{ npcTemplate = "slooni_slave", planetName = "talus", npcName = "Twi'lek slave" }
			},
			secondarySpawns =
			{
				{ npcTemplate = "slaver", planetName = "talus", npcName = "a Slaver" },
			},
			itemSpawns = {},
			rewards =
			{
				{ rewardType = "credits", amount = 50 },
				{ rewardType = "faction", faction = "fed_dub", amount = 5 },
			}
		},
		{
			missionType = "assassinate",
			primarySpawns =
			{
				{ npcTemplate = "slavemaster", planetName = "talus", npcName = "Slavemaster" }
			},
			secondarySpawns =
			{
				{ npcTemplate = "slaver", planetName = "talus", npcName = "a Slaver" },
				{ npcTemplate = "slaver", planetName = "talus", npcName = "a Slaver" }
			},
			itemSpawns = {},
			rewards =
			{
				{ rewardType = "credits", amount = 75 },
				{ rewardType = "faction", faction = "fed_dub", amount = 10 },
			}
		},
	}

npcMapSlooniJong =
	{
		{
			spawnData = { planetName = "talus", npcTemplate = "slooni_jong", x = 10.8, z = 1.7, y = -18, direction = -2, cellID = 4265440, position = STAND },
			worldPosition = { x = 4303, y = 5090 },
			npcNumber = 1,
			stfFile = "@static_npc/talus/talus_nashal_slooni_jong",
			missions = slooni_jong_missions
		},
	}

SlooniJong = ThemeParkLogic:new {
	numberOfActs = 1,
	npcMap = npcMapSlooniJong,
	className = "SlooniJong",
	screenPlayState = "slooni_jong_quest",
	distance = 800
}

registerScreenPlay("SlooniJong", true)

slooni_jong_mission_giver_conv_handler = mission_giver_conv_handler:new {
	themePark = SlooniJong
}
slooni_jong_mission_target_conv_handler = mission_target_conv_handler:new {
	themePark = SlooniJong
}