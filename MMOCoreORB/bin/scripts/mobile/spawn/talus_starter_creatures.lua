talus_starter_creatures = {
	wanderRadius = 10,
	commandLevel = 0,

	type = LAIR,
	maxSpawnLimit = 512,

	lairSpawns = {
		{
			lairTemplateName = "talus_paralope_lair_neutral_medium",
			spawnLimit = -1,			
			minDifficulty = 2,
			maxDifficulty = 5,
			numberToSpawn = 0,
			weighting = 15,
			size = 25
		},
		{
			lairTemplateName = "talus_fynock_youth_lair_neutral_small",
			spawnLimit = -1,			
			minDifficulty = 2,
			maxDifficulty = 5,
			numberToSpawn = 0,
			weighting = 15,
			size = 25
		},
		{
			lairTemplateName = "talus_rasp_flite_large_neutral_none",
			spawnLimit = -1,
			minDifficulty = 4,
			maxDifficulty = 7,
			numberToSpawn = 0,
			weighting = 15,
			size = 25
		},
		{
			lairTemplateName = "talus_vynock_lair_neutral_medium",
			spawnLimit = -1,			
			minDifficulty = 4,
			maxDifficulty = 7,
			numberToSpawn = 0,
			weighting = 15,
			size = 25
		},
		{
			lairTemplateName = "talus_decay_mite_lair_neutral_small",
			spawnLimit = -1,			
			minDifficulty = 6,
			maxDifficulty = 9,
			numberToSpawn = 0,
			weighting = 15,
			size = 25
		},
		{
			lairTemplateName = "talus_rot_mite_lair_neutral_small",
			spawnLimit = -1,			
			minDifficulty = 6,
			maxDifficulty = 10,
			numberToSpawn = 0,
			weighting = 15,
			size = 25
		},
		{
			lairTemplateName = "talus_wrix_lair_neutral_medium",
			spawnLimit = -1,			
			minDifficulty = 7,
			maxDifficulty = 11,
			numberToSpawn = 0,
			weighting = 15,
			size = 25
		},
	}
}

addLairGroup("talus_starter_creatures", talus_starter_creatures);
