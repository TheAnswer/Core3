pfilbee_jhorn_missions =
{
	{ 
		missionType = "assassinate",
		silentTarget = "yes",
		primarySpawns = 
		{ 
			{ npcTemplate = "greater_desert_womp_rat", planetName = "tatooine", npcName = "a greater desert womp rat" } 
		}, 
		secondarySpawns = 
		{
				{ npcTemplate = "greater_desert_womp_rat", planetName = "tatooine", npcName = "a greater desert womp rat" }
		},
		itemSpawns = {}, 
		rewards = {}
	},
	{
		missionType = "assassinate",
		silentTarget = "yes",
		primarySpawns = 
		{ 
			{ npcTemplate = "tatooine_mynock", planetName = "tatooine", npcName = "a tatooine mynock" } 
		}, 
		secondarySpawns = 
		{
				{ npcTemplate = "tatooine_mynock", planetName = "tatooine", npcName = "a tatooine mynock" }
		},
		itemSpawns = {}, 
		rewards = {}
	},
	{
		missionType = "assassinate",
		silentTarget = "yes",
		primarySpawns = 
		{ 
			{ npcTemplate = "dragonet", planetName = "tatooine", npcName = "a dragonet" } 
		}, 
		secondarySpawns = 
		{
				{ npcTemplate = "dragonet", planetName = "tatooine", npcName = "a dragonet" },
				{ npcTemplate = "dragonet", planetName = "tatooine", npcName = "a dragonet" }
		},
		itemSpawns = {}, 
		rewards = {}
	},	
	{
		missionType = "assassinate",
		silentTarget = "yes",
		primarySpawns = 
		{ 
			{ npcTemplate = "dune_lizard", planetName = "tatooine", npcName = "a dune lizard" } 
		}, 
		secondarySpawns = 
		{
				{ npcTemplate = "dune_lizard", planetName = "tatooine", npcName = "a dune lizard" },
				{ npcTemplate = "dune_lizard", planetName = "tatooine", npcName = "a dune lizard" },
				{ npcTemplate = "dune_lizard", planetName = "tatooine", npcName = "a dune lizard" }				
		},
		itemSpawns = {}, 
		rewards = 
		{ 
			{ rewardType = "loot", lootGroup = "task_reward_pfilbee_jhorn" },
		}
	}	
}

npcMapPfilbeeJhorn = 
{ 
	{ 
		spawnData = { planetName = "tatooine", npcTemplate = "pfilbee_jhorn", x = 4.9, z = 0.1, y = -3.9, direction = -90, cellID = 1279923, position = STAND }, 
		worldPosition = { x = -1129.4, y = -3914.4 }, 
		npcNumber = 1, 
		stfFile = "@static_npc/tatooine/pfilbee_jhorn", 
		missions = pfilbee_jhorn_missions 
	},
}

PfilbeeJhorn = ThemeParkLogic:new {
	numberOfActs = 1,
	npcMap = npcMapPfilbeeJhorn,
	permissionMap = {},
	className = "PfilbeeJhorn",
	screenPlayState = "pfilbee_jhorn_quest",
	distance = 1000,
	missionDescriptionStf = "",
	missionCompletionMessageStf = "",
	faction = FACTIONIMPERIAL
}

registerScreenPlay("PfilbeeJhorn", true)

pfilbee_jhorn_mission_giver_conv_handler = mission_giver_conv_handler:new {
	themePark = PfilbeeJhorn
}
pfilbee_jhorn_mission_target_conv_handler = mission_target_conv_handler:new {
	themePark = PfilbeeJhorn
}
