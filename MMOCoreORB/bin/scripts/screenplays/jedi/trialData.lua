TRIAL_LIGHTSABER = 1
TRIAL_TALK = 2
TRIAL_KILL = 3
TRIAL_HUNT = 4

padawanTrialQuests = {
	{
		trialName = "architect",
		trialType = TRIAL_TALK,
		trialNpc = "devaronian_male",
		trialNpcName = "Kant Graf (an architect)",
		targetNpc = "gungan_captain",
		targetKillable = true
	},
	{
		trialName = "artist",
		trialType = TRIAL_TALK,
		trialNpc = "bestine_rumor12",
		trialNpcName = "Sathme Forr (an artist)",
		targetNpc = "commoner",
		targetKillable = false
	},
	{
		trialName = "bad_cat",
		trialType = TRIAL_KILL,
		trialNpc = "bestine_capitol02",
		trialNpcName = "Yvana Bailer (an actor)",
		targetNpc = "bloodstained_prowler",
		targetKillable = true
	},
	{
		trialName = "chef",
		trialType = TRIAL_TALK,
		trialNpc = "dannik_malaan",
		trialNpcName = "Seevi Nyed (a chef)",
		targetNpc = "neo_cobral_overlord",
		targetKillable = true
	},
	{
		trialName = "kill_baz_nitch",
		trialType = TRIAL_HUNT,
		trialNpc = "sullustan_male",
		trialNpcName = "Menchi (an environmentalist)",
		huntTarget = "baz_nitch",
		huntGoal = 20
	},
	{
		trialName = "kill_falumpaset",
		trialType = TRIAL_HUNT,
		trialNpc = "sullustan_male",
		trialNpcName = "Braganta (a naturalist)",
		huntTarget = "falumpaset",
		huntGoal = 20
	},
	{
		trialName = "kill_sludge_panther",
		trialType = TRIAL_HUNT,
		trialNpc = "sullustan_male",
		trialNpcName = "Luha Kellaro (an ecologist)",
		huntTarget = "sludge_panther",
		huntGoal = 20
	},
	{
		trialName = "peoples_soldier",
		trialType = TRIAL_KILL,
		trialNpc = "marco_vahn",
		trialNpcName = "Torin Gundo (an old soldier)",
		targetNpc = "brigand_leader",
		targetKillable = true
	},
	{
		trialName = "politician",
		trialType = TRIAL_KILL,
		trialNpc = "dorn_gestros",
		trialNpcName = "Kaul Dysen (a politician)",
		targetNpc = "bloodseeker_mite",
		targetKillable = true
	},
	{
		trialName = "sob_story",
		trialType = TRIAL_TALK,
		trialNpc = "karena_keer",
		trialNpcName = "Erim Thelcar",
		targetNpc = "object/tangible/jedi/padawan_trials_skeleton.iff",
		targetNpcName = "The remains of Josef Thelcar",
		targetKillable = false
	},
	{
		trialName = "spice_mom",
		trialType = TRIAL_TALK,
		trialNpc = "bestine_rumor10",
		trialNpcName = "Sola Nosconda",
		targetNpc = "devaronian_male",
		targetNpcName = "Evif Sulp",
		targetKillable = false
	},
	{
		trialName = "surveyor",
		trialType = TRIAL_KILL,
		trialNpc = "bestine_rumor08",
		trialNpcName = "Par Doiae (a surveyor)",
		targetNpc = "sharnaff_bull",
		targetKillable = true
	},
}

trialsCivilizedPlanets = { "corellia", "naboo", "rori", "talus", "tatooine" }

trialsCivilizedPlanetCities = {
	corellia = { "coronet", "tyrena", "kor_vella", "doaba_guerfel", "bela_vistal" },
	naboo = { "theed", "moenia", "keren", "kaadara", "deeja_peak" },
	rori = { "narmle", "restuss" },
	talus = { "dearic", "nashal" },
	tatooine = { "bestine", "mos_espa", "mos_eisley", "mos_entha" }
}

-- x, y, radius
trialsCivilizedNpcSpawnPoints = {
	corellia = {
		coronet = {
			{ -14, -4619, 150 },
			{ -170, -4462, 200 },
			{ -327, -4596, 150 },
			{ -107, -4320, 150 }
		},
		tyrena = {
			{ -5132, -2426, 175 },
			{ -5535, -2701, 125 }
		},
		kor_vella = {
			{ -3130, 2798, 20 },
			{ -3332, 3271, 30 },
			{ -3473, 3149, 60 },
			{ -3403, 3026, 20 },
			{ -3673, 3107, 30 },
			{ -3771, 3207, 30 }
		},
		doaba_guerfel = {
			{ 3325, 5504, 40 },
			{ 3214, 5348, 100 },
			{ 3137, 5172, 80 },
			{ 3180, 4985, 30 }
		},
		bela_vistal = {
			{ 6707, -5894, 30 },
			{ 6793, -5733, 100 },
			{ 6712, -5574, 20 },
			{ 6929, -5551, 20 }
		}
	},

	naboo = {
		theed = {
			{ -4675, 3970, 50 },
			{ -4607, 4110, 40 },
			{ -4926, 4027, 40 },
			{ -5173, 4225, 40 },
			{ -5426, 4150, 75 },
			{ -5538, 4307, 60 },
			{ -5948, 4308, 50 },
			{ -5801, 4131, 45 }
		},
		moenia = {
			{ 4892, -4842, 50 },
			{ 4961, -4948, 40 },
			{ 4755, -4948, 60 },
			{ 4698, -4889, 30 }
		},
		keren = {
			{ 1677, 2950, 20 },
			{ 1714, 2664, 75 },
			{ 1780, 2519, 60 },
			{ 1934, 2711, 50 },
			{ 1986, 2479, 60 }
		},
		kaadara = {
			{ 5138, 6630, 40 },
			{ 4987, 6761, 40 },
			{ 5164, 6776, 60 }
		},
		deeja_peak = {
			{ 5114, -1503, 30 },
			{ 5027, -1444, 30 },
			{ 4959, -1517, 30 }
		}
	},

	rori = {
		narmle = {
			{ -5354, -2079, 20 },
			{ -5412, -2238, 20 },
			{ -5215, -2252, 50 },
			{ -5067, -2303, 75 },
			{ -5151, -2461, 60 }
		},
		restuss = {
			{ 5434, 5589, 60 },
			{ 5216, 5627, 80 },
			{ 5186, 5751, 60 },
			{ 5394, 5840, 40 }
		}
	},

	talus = {
		dearic = {
			{ 202, 2846, 30 },
			{ 195, -3040, 40 },
			{ 435, -3063, 100 },
			{ 526, -2946, 100 },
			{ 667, -3066, 80 }
		},
		nashal = {
			{ 4364, 5340, 100 },
			{ 4491, 5188, 40 },
			{ 4365, 5163, 15 },
			{ 4223, 5132, 25 },
			{ 4095, 5280, 25 }
		}
	},

	tatooine = {
		bestine = {
			{ -1401, -3725, 20 },
			{ -1344, -3911, 20 },
			{ -1219, -3565, 100 },
			{ -1095, -3584, 75 },
			{ -1064, -3692, 40 }
		},
		mos_espa = {
			{ -2896, 2009, 150 },
			{ -3057, 2161, 100 },
			{ -2905, 2340, 200 }
		},
		mos_eisley = {
			{ 3495, -4828, 100 },
			{ 3434, -4952, 75 },
			{ 3387, -4715, 200 }
		},
		mos_entha = {
			{ 1332, 3114, 100 },
			{ 1352, 3259, 150 },
			{ 1413, 3341, 150 },
			{ 1507, 3097, 100 },
			{ 1702, 3090, 50 }
		}
	}
}
