factional_neutral = {
	wanderRadius = 10,
	commandLevel = 0,

	type = LAIR,
	maxSpawnLimit = 0,

	lairSpawns = {
		{
			lairTemplateName = "criminal_base",
			spawnLimit = 0,
			minDifficulty = 7,
			maxDifficulty = 7,
			numberToSpawn = 0,
			weighting = 5,
			size = 15
		}
	}
}

addLairGroup("factional_neutral", factional_neutral);
