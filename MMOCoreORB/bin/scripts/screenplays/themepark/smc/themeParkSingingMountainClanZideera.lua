zideera_missions =
{
	{
		missionType = "escort",
		primarySpawns =
		{
			{ npcTemplate = "theme_park_commoner_old", planetName = "dathomir", npcName = "a Stranded Pirate" }
		},
		secondarySpawns =
		{
			{ npcTemplate = "lurking_shear_mite", planetName = "dathomir", npcName = "" },
			{ npcTemplate = "lurking_shear_mite", planetName = "dathomir", npcName = "" }
		},
		itemSpawns =
		{

		},
		rewards =
		{
			{ rewardType = "faction", faction = "mtn_clan", amount = 10 },
			{ rewardType = "credits", amount = 300 }
		}
	},
	{
		missionType = "escort",
		primarySpawns =
		{
			{ npcTemplate = "theme_park_commoner_old", planetName = "dathomir", npcName = "a Pirate Crewmate" }
		},
		secondarySpawns =
		{

		},
		itemSpawns =
		{

		},
		rewards =
		{
			{ rewardType = "faction", faction = "mtn_clan", amount = 10 },
			{ rewardType = "credits", amount = 450 }
		}
	}
}

npcMapSingingMountainClanZideera =
{
	{
		spawnData = { planetName = "dathomir", npcTemplate = "zideera", x = -14.2, z = 1.9, y = -21, direction = -20, cellID = 2665883, position = STAND },
		worldPosition = { x = 153.5, y = 4465.3 },
		npcNumber = 4,
		stfFile = "@static_npc/dathomir/dathomir_singingmountainclanstronghold_zideera",
		missions = zideera_missions
	}
}

ThemeParkSingingMountainClanZideera = ThemeParkLogic:new {
	numberOfActs = 1,
	npcMap = npcMapSingingMountainClanZideera,
	className = "ThemeParkSingingMountainClanZideera",
	screenPlayState = "singing_mountain_clan_zideera_theme_park",
	requiredFaction = "mtn_clan"
}

registerScreenPlay("ThemeParkSingingMountainClanZideera", true)

theme_park_singing_mountain_clan_zideera_mission_giver_conv_handler = mission_giver_conv_handler:new {
	themePark = ThemeParkSingingMountainClanZideera
}
theme_park_singing_mountain_clan_zideera_mission_target_conv_handler = mission_target_conv_handler:new {
	themePark = ThemeParkSingingMountainClanZideera
}