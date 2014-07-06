TatooineMosEspaScreenPlay = ScreenPlay:new {
	numberOfActs = 1,

	screenplayName = "TatooineMosEspaScreenPlay"
}

registerScreenPlay("TatooineMosEspaScreenPlay", true)

function TatooineMosEspaScreenPlay:start()
	if (isZoneEnabled("tatooine")) then
		self:spawnMobiles()
	end
end

function TatooineMosEspaScreenPlay:spawnMobiles()

	--Anything dashed out will need to be added here and removed from spawn manager once the spawnMobiles command is extended.

	--Starport
	--{"chassis_broker",60,2.29774,0.639422,67.5349,157.943,1261655, "neutral", ""},
	--{"ecoospoo",60,1.1,0.639421,66.3,33.001,1261655, "conversation", "Ecoospoo"},
	--{"poki_wirqi",60,-29.0928,1.63942,52.3406,180.009,1261660, "conversation", "Poki Wirqi"},
	--{"awwaschlu_witubrarq",60,-29.0928,1.63942,51.3406,0,1261660, "conversation", "Awwaschlu Witubrarq"},
	--{"farmer",60,-50.0859,2.63942,33.6967,135.008,1261663, "conversation", ""},
	--{"tatooine_militiaman",300,-48.9859,2.63942,33.6967,180.011,1261663, "conversation", ""},

	--Guild Hall -2857 1987
	spawnMobile("tatooine", "contractor",60,-3.69,1.13306,-6,11.0003,1256306)
	spawnMobile("tatooine", "miner",60,3.29,1.13306,-9.58,249.007,1256011)
	spawnMobile("tatooine", "trainer_artisan",0,-0.0885271,1.13306,-14.0283,2,1256011)
	spawnMobile("tatooine", "coa3_information_rebel",60,14.2091,1.13306,-10.1408,205.638,1256010)

	--Cantina
	spawnMobile("tatooine", "bartender",60,-11.1703,-0.894992,1.53149,33.4278,1256058)
	--{"dorn_gestros",60,-6.00754,-0.894992,-5.35219,231.068,1256058, "calm", "Dorn Gestros"},
	--{"medic",60,12.1732,-0.894991,3.93609,180.003,1256058, "conversation", ""},
	--{"squatter",300,12.1732,-0.894992,2.93609,360.011,1256058, "conversation", ""},
	--{"tatooine_militiaman",300,2.1656,-0.894992,-15.9672,360.011,1256061, "conversation", ""},
	--{"gambler",60,2.1656,-0.894992,-14.9672,180.001,1256061, "conversation", ""},
	--{"gambler",60,-20.6545,-0.894989,25.0112,0,1256067, "conversation", ""},
	--{"dim_u_preacher",300,-20.6545,-0.894989,26.0112,180.01,1256067, "conversation", ""},
	--{"da_la_socuna",60,-29.7168,-0.519991,7.77728,54.7476,1256068, "neutral", ""},

	--Guild Hall -3063 2079
	spawnMobile("tatooine", "trainer_architect",0,11,1.133,-14.5,0,1256021)
	spawnMobile("tatooine", "trainer_armorsmith",0,-12.5142,1.13306,3.63893,165,1256020)
	spawnMobile("tatooine", "trainer_droidengineer",0,-11.5362,1.13306,-12.6351,1,1256023)
	spawnMobile("tatooine", "trainer_weaponsmith",0,-2.95975,1.13306,-8.77207,109,1256022)
	spawnMobile("tatooine", "trainer_merchant",0,12,1.13306,6,180,1256019)

	--Med Center
	spawnMobile("tatooine", "trainer_combatmedic",0,7.93319,0.184067,5.54261,86,4005424)

	--Hotel
	--{"commoner_technician",60,20.2715,1.28309,-1.14578,360.011,1261018, "conversation", ""},
	--{"noble",60,20.2715,1.28309,-0.0457823,179.998,1261018, "conversation", ""},
	--{"rodian_clan_medic",300,19.1715,1.28309,-0.0457823,134.998,1261018, "conversation", ""},
	spawnMobile("tatooine", "scientist",60,20.2668,1.56616,15.1105,241.986,1261018)
	--{"tatooine_militiaman",300,7.41432,1.00001,-5.80893,180.003,1261017, "conversation", ""},
	--{"commoner_technician",60,7.41432,1.00001,-6.90893,0,1261017, "conversation", ""},
	--{"commoner",60,-24,1.6,-4,47.5304,1261023, "calm", "An Event Promoter"},

	--Junk Shop
	--{"junk_nado",60,4.4,-0.5,1.3,-133,1255997, "", "Nado",JUNKGENERIC,JUNKCONVNADOWATTOS},

	--Guild Hall -2997 2426
	spawnMobile("tatooine", "trainer_brawler",0,-11,1.1,-14,0,1255994)
	spawnMobile("tatooine", "trainer_marksman",0,0,1.13306,-13,0,1255993)
	spawnMobile("tatooine", "trainer_scout",0,-13,1.1,4.8,180,1255991)

	--Outside
	--{"anim_kahn",60,-2907.07,5,2130.14,71.993,0, "neutral", ""},
	--{"brawler",60,-2918.94,5,2169.6,0,0, "conversation", ""},
	spawnMobile("tatooine", "businessman",60,-2881.87,5,2142.69,18.8035,0)
	spawnMobile("tatooine", "businessman",60,-2787.25,5,1962.04,14.8999,0)
	spawnMobile("tatooine", "businessman",60,-2939.1,5,1962.9,15.9578,0)
	spawnMobile("tatooine", "businessman",60,-2912.23,5,1928.7,337.934,0)
	spawnMobile("tatooine", "businessman",60,-2881.87,5,1974.41,329.04,0)
	spawnMobile("tatooine", "businessman",60,-3019.69,5,2114.97,73.4692,0)
	spawnMobile("tatooine", "businessman",60,-2881.8,5,2267.4,180,0)
	spawnMobile("tatooine", "businessman",60,-2897.39,5,2323.06,54.7434,0)
	spawnMobile("tatooine", "businessman",60,-2976.72,5.52789,2333.09,215.86,0)
	spawnMobile("tatooine", "businessman",60,-2931.98,5,2495.44,266.081,0)
	spawnMobile("tatooine", "businessman",60,-2774.25,5,2018.1,237.899,0)
	spawnMobile("tatooine", "businessman",60,-2739.74,5,2044.75,99.9046,0)
	spawnMobile("tatooine", "businessman",60,-2722.49,5,2007.18,217.488,0)
	spawnMobile("tatooine", "businessman",60,-2767.35,5,2232.76,164.701,0)
	spawnMobile("tatooine", "businessman",60,-2766.88,5,2314.77,201.13,0)
	--{"businessman",60,-2901.2,5,2360.3,241,0, "npc_sitting_chair", ""},
	--{"businessman",60,-2868.4,5,2493.9,238,0, "npc_sitting_chair", ""},
	spawnMobile("tatooine", "cll8_binary_load_lifter",60,-2922.62,5,2143.4,19.5832,0)
	spawnMobile("tatooine", "coa2_rebel_coordinator",60,-2927.64,5,2574.17,304.433,0)
	spawnMobile("tatooine", "commoner_fat",60,-2954.74,5,2235.4,232.852,0)
	spawnMobile("tatooine", "commoner_fat",60,-2877.18,5,2473.48,293.853,0)
	spawnMobile("tatooine", "commoner_fat",60,-2886.35,5,2377.12,323.001,0)
	spawnMobile("tatooine", "commoner_naboo",60,-3046.72,5,2133.45,249.763,0)
	spawnMobile("tatooine", "commoner_naboo",60,-2890.19,5,2251.37,190.438,0)
	spawnMobile("tatooine", "commoner_naboo",60,-2839.04,5,2492.89,55.3765,0)
	spawnMobile("tatooine", "commoner_naboo",60,-2874.25,5,2546.29,122.899,0)
	spawnMobile("tatooine", "commoner_naboo",60,-2881.23,5,2004.94,233.054,0)
	spawnMobile("tatooine", "commoner_naboo",60,-2818.45,5,2407.94,238.346,0)
	spawnMobile("tatooine", "commoner_old",60,-2952.26,5,2087.58,46.4769,0)
	spawnMobile("tatooine", "commoner_old",60,-2859.3,5,2288.53,351.513,0)
	spawnMobile("tatooine", "commoner_old",60,-3007.6,5,2372.2,-133,0)
	spawnMobile("tatooine", "commoner_tatooine",60,-2914.38,5,1984.92,26.6556,0)
	spawnMobile("tatooine", "commoner_tatooine",60,-3100.44,5,2148.76,158.885,0)
	spawnMobile("tatooine", "commoner_tatooine",60,-2860.36,5,2212.37,343.92,0)
	spawnMobile("tatooine", "commoner_tatooine",60,-2914.09,5,2466.26,163.417,0)
	spawnMobile("tatooine", "commoner_tatooine",60,-2715.87,5,2040.78,144.662,0)
	spawnMobile("tatooine", "commoner_tatooine",60,-2754.28,5,1981.64,94.3056,0)
	spawnMobile("tatooine", "commoner_tatooine",60,-2831.34,5,1912.49,217.51,0)
	spawnMobile("tatooine", "commoner_tatooine",60,-2807.11,5,1953.71,320.134,0)
	spawnMobile("tatooine", "commoner_tatooine",60,-3115.34,5,2084.32,284.484,0)
	spawnMobile("tatooine", "commoner_tatooine",60,-3000.81,5,2213.95,148.773,0)
	spawnMobile("tatooine", "commoner_tatooine",60,-2978.76,5,2473.24,5.89742,0)
	spawnMobile("tatooine", "commoner_tatooine",60,-2884.43,5,2575.51,325.132,0)
	spawnMobile("tatooine", "commoner_tatooine",60,-2894.24,5,2492.22,84.423,0)
	spawnMobile("tatooine", "commoner_tatooine",60,-2810.06,5,2318.02,268.996,0)
	spawnMobile("tatooine", "commoner_tatooine",60,-2770.48,5,2245.4,214.817,0)
	spawnMobile("tatooine", "commoner_tatooine",60,-2731.39,5,2226.03,196.441,0)

	--Imperial Npc's Outside
	--{"crackdown_comm_operator",400,-2960.4,5,1973.9,74,0, "npc_imperial", ""},
	spawnMobile("tatooine", "crackdown_dark_trooper",400,-3028.58,5.11515,2618.21,39.6237,0)
	spawnMobile("tatooine", "crackdown_elite_dark_trooper",400,-2636.4,5,2254.3,81,0)
	--{"crackdown_elite_dark_trooper",400,-2953.88,5,1933.16,344.533,0, "npc_imperial", ""},
	--{"crackdown_imperial_master_sergeant",400,-2954.65,5,1935.93,344.531,0, "npc_imperial", ""},
	--{"crackdown_imperial_master_sergeant",400,-3115,5,2199.8,-75,0, "npc_imperial", ""},
	spawnMobile("tatooine", "crackdown_imperial_master_sergeant",400,-3204.52,5,2091.93,204.135,0)
	spawnMobile("tatooine", "crackdown_imperial_master_sergeant",400,-3029.53,5.07697,2617.06,39.7527,0)
	--{"crackdown_imperial_noncom",400,-2648.9,5,2290.2,67,0, "npc_imperial", ""},
	--{"crackdown_imperial_noncom",400,-3027.01,5.06395,2617.66,289.047,0, "npc_imperial", ""},
	spawnMobile("tatooine", "crackdown_imperial_warrant_officer_ii",400,-3207,5,2097.52,347.895,0)
	--{"crackdown_sand_trooper",400,-2985.75,5,1978.7,74.49,0, "npc_imperial", ""},
	--{"crackdown_sand_trooper",400,-2963.4,5,1978.6,75,0, "npc_imperial", ""},
	--{"crackdown_sand_trooper",400,-2988.1,5,1978.04,74.4903,0, "npc_imperial", ""},
	--{"crackdown_sand_trooper",400,-2960.1,5,1968.6,67,0, "npc_imperial", ""},
	spawnMobile("tatooine", "crackdown_sand_trooper",400,-2695.6,5,1982.9,122,0)
	spawnMobile("tatooine", "crackdown_sand_trooper",400,-3170.7,5,2137.7,-30,0)
	spawnMobile("tatooine", "crackdown_sand_trooper",400,-3028.33,5.01162,2616.08,294.927,0)
	spawnMobile("tatooine", "crackdown_scout_trooper",400,-2982.5,5,1956.8,73,0)
	spawnMobile("tatooine", "crackdown_scout_trooper",400,-3029.2,5,2614.8,-52,0)
	spawnMobile("tatooine", "crackdown_stormtrooper",400,-3207.39,5,2099.33,347.898,0)
	spawnMobile("tatooine", "crackdown_stormtrooper",400,-3203.5,5,2091.5,-159,0)
	--{"crackdown_stormtrooper_captain",400,-3026.34,5.10198,2618.73,289.047,0, "npc_imperial", ""},
	--{"crackdown_stormtrooper_squad_leader",400,-2979.8,5,1957.6,67,0, "npc_imperial", ""},
	spawnMobile("tatooine", "mos_espa_police_officer",300,-3022.29,5.02759,2618.41,268.2,0)
	spawnMobile("tatooine", "mos_espa_police_officer",300,-2976.17,5,2009.31,171.013,0)
	spawnMobile("tatooine", "mos_espa_police_officer",300,-3023.4,5,2618.4,86,0)
	spawnMobile("tatooine", "mos_espa_police_officer",300,-2977.32,5,2010.1,171.012,0)
	spawnMobile("tatooine", "mos_espa_police_officer",300,-3022.7,5.1,2619.6,-171,0)
	spawnMobile("tatooine", "mos_espa_police_officer",300,-2976.54,5,2011.63,171.014,0)

	--More misc outside
	spawnMobile("tatooine", "criminal",300,-2960.96,5,2163.64,79.2954,0)
	spawnMobile("tatooine", "criminal",300,-2856.83,5,2473.55,184.191,0)
	spawnMobile("tatooine", "criminal",300,-2976.77,5,2539.28,36.1265,0)
	spawnMobile("tatooine", "criminal",300,-2837.3,5,2425.05,254.096,0)
	spawnMobile("tatooine", "criminal",300,-2760.75,5,2038.95,263.963,0)
	spawnMobile("tatooine", "criminal",300,-2834.04,5,2462.23,193.677,0)
	spawnMobile("tatooine", "criminal",300,-2846.59,5,2497.64,332.133,0)
	spawnMobile("tatooine", "desert_demon",300,-2821.38,5,2432.95,176.513,0)
	spawnMobile("tatooine", "desert_demon",300,-2861.27,5,2393.34,146.859,0)
	spawnMobile("tatooine", "desert_demon",300,-2823.03,5,2448.69,64.4102,0)
	spawnMobile("tatooine", "eg6_power_droid",300,-2833.22,5,1899.71,245.01,0)
	spawnMobile("tatooine", "eg6_power_droid",300,-2928.04,5,1977.68,330.633,0)
	spawnMobile("tatooine", "eg6_power_droid",300,-2945.98,5,2470.9,186.376,0)
	--{"giaal_itotr",60,-3102.7,5,2185,118,0, "npc_sitting_chair", ""},
	spawnMobile("tatooine", "informant_npc_lvl_1",0,-2804,5,2237,315,0)
	spawnMobile("tatooine", "informant_npc_lvl_1",0,-2836,5,2275,45,0)
	spawnMobile("tatooine", "informant_npc_lvl_1",0,-2935,5,2393,90,0)
	spawnMobile("tatooine", "informant_npc_lvl_2",0,-2879,5,2555,135,0)

	--Jawa's
	--{"jawa",60,-2930.03,5,2038.02,270.008,0, "conversation", ""},
	--{"jawa",60,-2932.03,5,2038.02,90.0027,0, "conversation", ""},
	--{"jawa",60,-2931.03,5,2037.02,0,0, "conversation", ""},
	spawnMobile("tatooine", "jawa",60,-3033.6,5,2245.5,80,0)
	spawnMobile("tatooine", "jawa",60,-3028.7,5,2246.4,-99,0)
	spawnMobile("tatooine", "jawa",60,-2997.5,5,2246.5,-83,0)
	spawnMobile("tatooine", "jawa",60,-2921.8,5,2561,-17,0)
	spawnMobile("tatooine", "jawa",60,-2922.7,5,2565.1,167,0)
	spawnMobile("tatooine", "jawa",60,-2918.3,5,2587.5,-147,0)
	spawnMobile("tatooine", "jawa",60,-2923.6,5,2579.2,24,0)
	spawnMobile("tatooine", "jawa",60,-2888,5,2591,90.7367,0)
	spawnMobile("tatooine", "jawa",60,-2886.7,5,2591,-90,0)
	spawnMobile("tatooine", "jawa",60,-2854.7,5,2461.1,-77,0)
	spawnMobile("tatooine", "jawa",60,-2857,5,2461.6,112,0)
	spawnMobile("tatooine", "jawa",60,-2820.6,5,2414.3,-6,0)
	spawnMobile("tatooine", "jawa",60,-2821,5,2417.3,154,0)
	spawnMobile("tatooine", "jawa",60,-2842.4,5,2317.4,-132,0)
	spawnMobile("tatooine", "jawa",60,-2844.9,5,2315.3,49,0)
	spawnMobile("tatooine", "jawa",60,-2866.7,5,2311.8,-51,0)
	spawnMobile("tatooine", "jawa",60,-2869.2,5,2314,140,0)
	spawnMobile("tatooine", "jawa",60,-2737.2,5,2179.2,-12,0)
	spawnMobile("tatooine", "jawa",60,-2736.3,5,2181.5,-97,0)
	spawnMobile("tatooine", "jawa",60,-2737.7,5,2183.6,169,0)
	spawnMobile("tatooine", "jawa_engineer",300,-2834.7,5,2399,76,0)
	spawnMobile("tatooine", "jawa_engineer",300,-2771.4,5,1934.2,84,0)
	spawnMobile("tatooine", "jawa_engineer",300,-2735,5,2189.5,-116,0)
	spawnMobile("tatooine", "jawa_engineer",300,-2742.4,5,2175.1,78,0)
	spawnMobile("tatooine", "jawa_healer",300,-2866.1,5,2465.9,-33,0)
	spawnMobile("tatooine", "jawa_healer",300,-2830.7,5,2400,-111,0)
	spawnMobile("tatooine", "jawa_healer",300,-2775.7,5,1925,2,0)
	spawnMobile("tatooine", "jawa_healer",300,-2775.8,5,1931.4,54,0)
	spawnMobile("tatooine", "jawa_healer",300,-2773.3,5,1932.7,-122,0)
	spawnMobile("tatooine", "jawa_healer",300,-2775.5,5,1927.9,-176,0)
	spawnMobile("tatooine", "jawa_healer",300,-2766.8,5,1934,-95,0)
	spawnMobile("tatooine", "jawa_henchman",300,-2906.1,5,2594.7,103,0)
	spawnMobile("tatooine", "jawa_smuggler",300,-3002.1,5,2245.9,77,0)
	spawnMobile("tatooine", "jawa_smuggler",300,-2775.8,5,1918.2,152,0)
	spawnMobile("tatooine", "jawa_smuggler",300,-2776.8,5,1913.8,78,0)
	spawnMobile("tatooine", "jawa_thief",300,-2903,5,2594.2,-80,0)
	spawnMobile("tatooine", "jawa_thief",300,-2867.9,5,2468,144,0)
	spawnMobile("tatooine", "jawa_thief",300,-2867.6,5,2421.4,-130,0)
	spawnMobile("tatooine", "jawa_thief",300,-2848.8,5,2430.5,103,0)
	spawnMobile("tatooine", "jawa_thief",300,-2739.1,5,2175.5,-96,0)
	spawnMobile("tatooine", "jawa_thief",300,-2736.1,5,2188.86,69.1613,0)

	--More misc outside
	--{"junk_dealer",0,-2750.9,5,2057,132,0, "", "",JUNKGENERIC,JUNKCONVGENERIC},
	--{"klepa_laeel",60,-2769.5,5,2111.1,104,0, "npc_sitting_chair", ""},
	--{"miner",60,-2918.94,5,2170.6,180.005,0, "conversation", ""},
	spawnMobile("tatooine", "noble",60,-2924.82,5,2143.27,127.261,0)
	spawnMobile("tatooine", "noble",60,-3081.03,5,2104.52,341.726,0)
	spawnMobile("tatooine", "noble",60,-3064.8,5,2095.01,-130,0)
	spawnMobile("tatooine", "noble",60,-2999.41,5.52789,2350.79,244.987,0)
	spawnMobile("tatooine", "noble",60,-2847.77,5,2393.35,42.0583,0)
	spawnMobile("tatooine", "noble",60,-2810.65,5,2195.86,318.004,0)
	spawnMobile("tatooine", "noble",60,-3091.67,5,2165.61,286.75,0)
	spawnMobile("tatooine", "noble",60,-3062.85,5,2195.92,156.151,0)
	--{"prost_roberts",60,-2906.52,5,2128.41,75.633,0, "neutral", ""},
	--{"r2",60,-2928.79,5,2111.03,245.497,0, "calm", "R2-N3"},
	--{"r2",60,-2946.98,5,2470.9,350.917,0, "calm", "R2-U4"},
	--{"r4",60,-2959.44,5,2110.92,112.101,0, "calm", "R4-S7"},
	--{"r4",60,-2871.38,6.12074,1985.32,252.493,0, "calm", "R4-X2"},
	--{"r5",60,-2935.94,5,2275.75,205.483,0, "calm", "R5-L7"},
	--{"r5",60,-2915.25,5,2163.42,154.028,0, "calm", "R5-E0"},
	--{"r5",60,-2919.36,5,2262.73,15.6789,0, "calm", "R5-F4"},
	--{"rel_keteris",60,-2905.83,5,2127.11,255.625,0, "neutral", ""},
	spawnMobile("tatooine", "scientist",60,-2969.77,5,2194.82,198.34,0)
	spawnMobile("tatooine", "scientist",60,-3055.28,5,2098.76,237.173,0)
	spawnMobile("tatooine", "scientist",60,-2913.31,5,2566.89,167.24,0)
	spawnMobile("tatooine", "scientist",60,-2969.78,5,2469.4,58.2554,0)
	spawnMobile("tatooine", "scientist",60,-2860.55,5,2534.77,227.149,0)
	spawnMobile("tatooine", "thug",300,-2857.9,5,2250.18,156.767,0)
	spawnMobile("tatooine", "thug",300,-2731.61,5,2242.41,339.94,0)
	spawnMobile("tatooine", "thug",300,-2748.23,5,2031.47,337.983,0)
	spawnMobile("tatooine", "thug",300,-2794.64,5,2327.09,109.693,0)
	spawnMobile("tatooine", "thug",300,-2927.54,5,2013.85,322.05,0)
	spawnMobile("tatooine", "thug",300,-2994.3,5.5,2331.4,165,0)
	spawnMobile("tatooine", "thug",300,-2964.35,5,2536.74,244.766,0)
	spawnMobile("tatooine", "thug",300,-3065.25,5,2142.42,155.455,0)
	spawnMobile("tatooine", "thug",300,-2919.38,5,2143.08,304.531,0)
	--{"trace_lyson",60,-2909.17,5,2180.03,180,0, "calm", "Trace Lyson"},
	spawnMobile("tatooine", "trainer_1hsword",0,-2964,5,2021,33,0)
	spawnMobile("tatooine", "trainer_2hsword",0,-2971,5,2032,174,0)
	spawnMobile("tatooine", "trainer_architect",0,-2872,5,2159,180,0)
	spawnMobile("tatooine", "trainer_artisan",0,-2890,5,2142,19,0)
	spawnMobile("tatooine", "trainer_brawler",0,-2933,5,2125,97,0)
	spawnMobile("tatooine", "trainer_creaturehandler",0,-2994,5,2530,0,0)
	spawnMobile("tatooine", "trainer_doctor",0,-3163.63,5,2122.39,18,0)
	spawnMobile("tatooine", "trainer_entertainer",0,-2889,5,2178,161,0)
	spawnMobile("tatooine", "trainer_marksman",0,-2931,5,2121,91,0)
	spawnMobile("tatooine", "trainer_medic",0,-3158,5,2122,5,0)
	spawnMobile("tatooine", "trainer_medic",0,-2931,5,2116,65,0)
	spawnMobile("tatooine", "trainer_musician",0,-2882,5,2171,180,0)
	spawnMobile("tatooine", "trainer_polearm",0,-2931,5,2386,0,0)
	spawnMobile("tatooine", "trainer_scout",0,-2912.86,5,2106.67,174,0)
	spawnMobile("tatooine", "trainer_unarmed",0,-2812,5,1973,180,0)
	spawnMobile("tatooine", "trainer_weaponsmith",0,-2869,5,2146,8,0)
	--{"watto_leg_syndil",60,-2866.54,5,1856.86,242.669,0, "calm", "Syndil"},

	--Creatures
	spawnMobile("tatooine", "kreetle",300,-3217.87,6.7851,2010.86,329.47,0)
	spawnMobile("tatooine", "kreetle",300,-3205.35,6.01035,2016.06,32.4216,0)
	spawnMobile("tatooine", "kreetle",300,-3208.98,6.12053,2017.03,118.154,0)
	spawnMobile("tatooine", "kreetle",300,-3246.28,5.42038,2218.4,130.138,0)
	spawnMobile("tatooine", "kreetle",300,-3248.94,5.4228,2116.77,343.61,0)
	spawnMobile("tatooine", "kreetle",300,-2789.62,6.17824,1763.98,98.5487,0)
	spawnMobile("tatooine", "kreetle",300,-2797.01,5.38467,1764.85,345.893,0)
	spawnMobile("tatooine", "kreetle",300,-2713.99,5.73621,1815.47,199.28,0)
	spawnMobile("tatooine", "kreetle",300,-2705.64,5.75105,1820.1,188.657,0)
	spawnMobile("tatooine", "kreetle",300,-2720.95,5.12037,1838.72,69.0059,0)
	spawnMobile("tatooine", "kreetle",300,-3114.39,5.75482,1906.64,235.346,0)
	spawnMobile("tatooine", "kreetle",300,-3093.11,5.06432,1912.45,117.788,0)
	spawnMobile("tatooine", "kreetle",300,-3006.66,7.83559,2697.94,246.037,0)
	spawnMobile("tatooine", "kreetle",300,-2868.66,5.92974,2659.77,280.728,0)
	spawnMobile("tatooine", "kreetle",300,-2879.16,5.3938,2642.48,208.517,0)
	spawnMobile("tatooine", "kreetle",300,-2957.17,6.65151,2663.78,200.677,0)
	spawnMobile("tatooine", "kreetle",300,-2958.99,6.05473,2651.86,35.5292,0)
	spawnMobile("tatooine", "kreetle",300,-2954.14,5.93401,2648.89,80.4338,0)
	spawnMobile("tatooine", "kreetle",300,-2959.73,6.48665,2662.19,351.646,0)
	spawnMobile("tatooine", "kreetle",300,-2599.04,5.20143,2338.05,44.6433,0)
	spawnMobile("tatooine", "kreetle",300,-2592.54,5.37076,2333.19,191.631,0)
	spawnMobile("tatooine", "kreetle",300,-2593.9,5,2038.48,159.546,0)
	spawnMobile("tatooine", "kreetle",300,-2599.47,5,2034.19,249.173,0)
	spawnMobile("tatooine", "kreetle",300,-2626.73,5.0133,1962.54,9.62294,0)
	spawnMobile("tatooine", "kreetle",300,-3248.78,5.22997,2122.05,130.343,0)
	spawnMobile("tatooine", "lesser_desert_womprat",300,-3210.83,6.40006,2005.31,323.114,0)
	spawnMobile("tatooine", "lesser_desert_womprat",300,-3244.16,6.56426,2232.65,151.289,0)
	spawnMobile("tatooine", "lesser_desert_womprat",300,-3095.72,5.05761,1915.26,68.165,0)
	spawnMobile("tatooine", "lesser_desert_womprat",300,-2594.47,5,2219.41,202.753,0)
	spawnMobile("tatooine", "lesser_desert_womprat",300,-2613.3,5.16549,1956.6,191.904,0)
	spawnMobile("tatooine", "lesser_desert_womprat",300,-3113.6,5.65245,1908.34,35.3904,0)
	spawnMobile("tatooine", "minor_worrt",300,-3241.57,5.39865,2216.06,297.723,0)
	spawnMobile("tatooine", "minor_worrt",300,-3258.59,5.19927,2132.91,298.091,0)
	spawnMobile("tatooine", "minor_worrt",300,-3096.16,5.08975,1913.67,167.708,0)
	spawnMobile("tatooine", "minor_worrt",300,-3108.76,5.45396,1910.17,6.74715,0)
	spawnMobile("tatooine", "minor_worrt",300,-3012.17,7.90734,2691.38,235.115,0)
	spawnMobile("tatooine", "minor_worrt",300,-2998.58,7.10669,2701.67,51.6869,0)
	spawnMobile("tatooine", "minor_worrt",300,-3074.46,6.07262,2617.97,336.772,0)
	spawnMobile("tatooine", "minor_worrt",300,-3069.16,5.91533,2614.47,71.6097,0)
	spawnMobile("tatooine", "minor_worrt",300,-2878.72,5.73483,2655.63,201.074,0)
	spawnMobile("tatooine", "minor_worrt",300,-2961.46,5.95319,2651.28,282.92,0)
	spawnMobile("tatooine", "minor_worrt",300,-2590.93,5,2218.28,295.223,0)
	spawnMobile("tatooine", "minor_worrt",300,-2586.74,5.44835,2327.16,227.993,0)
	spawnMobile("tatooine", "minor_worrt",300,-2579.03,5.26702,2042.14,89.1564,0)
	spawnMobile("tatooine", "minor_worrt",300,-2633.43,5.01395,1952.51,234.757,0)
	spawnMobile("tatooine", "minor_worrt",300,-2618.22,5.11747,1957.43,96.8282,0)
	spawnMobile("tatooine", "minor_worrt",300,-3257.35,5.46191,2118.72,178.577,0)
	spawnMobile("tatooine", "minor_worrt",300,-3068.78,6.01769,2624.45,350.333,0)
	spawnMobile("tatooine", "minor_worrt",300,-3073.79,6.05983,2617.18,335.476,0)
	spawnMobile("tatooine", "mound_mite",300,-3174.62,8.58522,1889.76,275.616,0)
	spawnMobile("tatooine", "mound_mite",300,-3321.03,6.78999,2187.04,121.37,0)
	spawnMobile("tatooine", "mound_mite",300,-3224.53,4.79097,1931.72,159.513,0)
	spawnMobile("tatooine", "mound_mite",300,-2922.41,8.03007,2711.74,325.617,0)
	spawnMobile("tatooine", "mound_mite",300,-2921.7,7.7918,2717.75,338.017,0)
	spawnMobile("tatooine", "mound_mite",300,-2909.38,7.75577,2704.39,256.038,0)
	spawnMobile("tatooine", "mound_mite",300,-2791.98,6.24406,1759.43,250.163,0)
	spawnMobile("tatooine", "mound_mite",300,-2485.1,7.2,2148.3,-33,0)
	spawnMobile("tatooine", "mound_mite",300,-2492.4,7,2150.2,-5,0)
	spawnMobile("tatooine", "mound_mite",300,-2977.98,5.61491,2651.49,216.494,0)
	spawnMobile("tatooine", "mound_mite",300,-2591.43,5.32223,2342.79,267.897,0)
	spawnMobile("tatooine", "mound_mite",300,-2595.01,5,2036.52,108.932,0)
	spawnMobile("tatooine", "mound_mite",300,-2488.3,6.82684,2159.7,-178,0)
	spawnMobile("tatooine", "mound_mite",300,-2491.39,6.83135,2156.44,337.917,0)
	spawnMobile("tatooine", "mound_mite",300,-2485.06,7.15469,2153.94,143.47,0)
	spawnMobile("tatooine", "rill",300,-3204.8,5.93603,2007.91,60.4885,0)
	spawnMobile("tatooine", "rill",300,-3207.4,5.68534,2028.55,27.7763,0)
	spawnMobile("tatooine", "rill",300,-3249.68,5.837,2224.78,186.126,0)
	spawnMobile("tatooine", "rill",300,-3232.15,6.18538,2232.39,115.466,0)
	spawnMobile("tatooine", "rill",300,-3252.78,6.52437,2230.5,245.072,0)
	spawnMobile("tatooine", "rill",300,-3250.93,5.68002,2105.16,193.916,0)
	spawnMobile("tatooine", "rill",300,-2795.61,5.42368,1767.23,216.636,0)
	spawnMobile("tatooine", "rill",300,-2795.65,6.41057,1752.01,160.826,0)
	spawnMobile("tatooine", "rill",300,-2723.39,5.02906,1842.49,350.515,0)
	spawnMobile("tatooine", "rill",300,-2724.42,5.34057,1826.85,328.592,0)
	spawnMobile("tatooine", "rill",300,-3064.46,5.7272,2615.22,74.3472,0)
	spawnMobile("tatooine", "rill",300,-2872.34,5.55843,2645.44,257.186,0)
	spawnMobile("tatooine", "rill",300,-2956.31,5.85355,2647.42,135.445,0)
	spawnMobile("tatooine", "rill",300,-2593.87,5,2226.87,289.139,0)
	spawnMobile("tatooine", "rill",300,-2590.52,5,2218.13,127.958,0)
	spawnMobile("tatooine", "rill",300,-2585.79,5.0883,2035.52,136.404,0)
	spawnMobile("tatooine", "rill",300,-3108.03,5.62114,1896.46,169.483,0)
	spawnMobile("tatooine", "rockmite",300,-3238.87,6.19092,2231.04,313.88,0)
	spawnMobile("tatooine", "rockmite",300,-3238.91,6.14088,2230.72,103.109,0)
	spawnMobile("tatooine", "rockmite",300,-3246.74,5.19249,2121.66,161.991,0)
	spawnMobile("tatooine", "rockmite",300,-3316.94,8.07518,2198.91,167.512,0)
	spawnMobile("tatooine", "rockmite",300,-3321.67,8.2476,2207.22,280.722,0)
	spawnMobile("tatooine", "rockmite",300,-2909.86,7.8253,2692.86,105.782,0)
	spawnMobile("tatooine", "rockmite",300,-2714.58,5.37967,1828.84,245.32,0)
	spawnMobile("tatooine", "rockmite",300,-2863.93,5.69244,2645.23,193.813,0)
	spawnMobile("tatooine", "rockmite",300,-2871.32,5.43329,2638.84,167.571,0)
	spawnMobile("tatooine", "rockmite",300,-2883.89,5.34137,2636.44,218.56,0)
	spawnMobile("tatooine", "rockmite",300,-2590.42,5,2212.03,168.759,0)
	spawnMobile("tatooine", "rockmite",300,-2601.23,5.0058,2320.44,217.298,0)
	spawnMobile("tatooine", "rockmite",300,-2591.62,5,2039.11,328.105,0)
	spawnMobile("tatooine", "rockmite",300,-2591.7,5.28616,2031.4,-72,0)
	spawnMobile("tatooine", "rockmite",300,-2633.87,5,1974.12,180.868,0)
	spawnMobile("tatooine", "rockmite",300,-3080.79,6.03743,2611.32,274.602,0)
	spawnMobile("tatooine", "tatooine_mynock",300,-2775.96,5,2483.96,160.007,0)
	spawnMobile("tatooine", "tatooine_mynock",300,-2795.9,5,2465.26,234.054,0)
	spawnMobile("tatooine", "tatooine_mynock",300,-2738.08,7.0857,2593.59,10.0957,0)
	spawnMobile("tatooine", "tatooine_mynock",300,-2814.11,5,2564.44,351.252,0)
	spawnMobile("tatooine", "tatooine_mynock",300,-2704.69,5,2447,144.669,0)
	spawnMobile("tatooine", "tatooine_mynock",300,-2681.63,5,2446.35,49.6666,0)
	spawnMobile("tatooine", "tatooine_mynock",300,-2776.92,5,2519.58,127.489,0)
	spawnMobile("tatooine", "tatooine_mynock",300,-2727.05,5,2452.36,233.461,0)
	spawnMobile("tatooine", "tatooine_mynock",300,-2708.25,5,2502.65,33.3582,0)
	spawnMobile("tatooine", "tatooine_mynock",300,-2712.84,5,2531.4,66.9802,0)
	spawnMobile("tatooine", "tatooine_mynock",300,-2724.24,5,2493.81,125.127,0)
	spawnMobile("tatooine", "tatooine_mynock",300,-2759.53,5,2452.82,285.438,0)
	spawnMobile("tatooine", "tatooine_mynock",300,-2725.86,5,2448.69,319.256,0)
	spawnMobile("tatooine", "tatooine_mynock",300,-2672.59,5,2453.01,205.972,0)
	spawnMobile("tatooine", "tatooine_mynock",300,-2702.54,5,2484.93,263.207,0)
	spawnMobile("tatooine", "tatooine_mynock",300,-2686.76,5,2433.69,241.59,0)
	spawnMobile("tatooine", "tatooine_mynock",300,-2723.47,5,2516.85,226.256,0)
	spawnMobile("tatooine", "tatooine_mynock",300,-2723.69,5,2481,27.9293,0)
	spawnMobile("tatooine", "tatooine_mynock",300,-2674.03,5.08911,2516.33,146.077,0)
	spawnMobile("tatooine", "tatooine_mynock",300,-2753.85,5,2515.57,287.643,0)
	spawnMobile("tatooine", "tatooine_mynock",300,-2689.61,5,2510.66,127.468,0)
	spawnMobile("tatooine", "tatooine_mynock",300,-2712.54,5,2478.41,328.789,0)
	spawnMobile("tatooine", "tatooine_mynock",300,-2783.56,5,2547.41,274.367,0)
	spawnMobile("tatooine", "tatooine_mynock",300,-2770.34,5,2507.88,282.073,0)
	spawnMobile("tatooine", "tatooine_mynock",300,-2688.24,5.18783,2540.25,306.662,0)
	spawnMobile("tatooine", "tatooine_mynock",300,-2697.2,5.68001,2579.61,60.858,0)
	spawnMobile("tatooine", "tatooine_mynock",300,-2660.7,5.26786,2500.87,151.217,0)
	spawnMobile("tatooine", "tatooine_mynock",300,-2674.79,5,2495.31,90.1322,0)
	spawnMobile("tatooine", "tempest_rill",300,-2800,6.1,1756.1,-76,0)
	spawnMobile("tatooine", "womprat",300,-3204.49,5.96378,2015.95,36.2798,0)
	spawnMobile("tatooine", "womprat",300,-3265.58,5.20889,2129.95,295.349,0)
	spawnMobile("tatooine", "womprat",300,-2963.31,6.41839,2664.35,19.3742,0)
	spawnMobile("tatooine", "womprat",300,-2593.13,5,2213.37,141.78,0)
	spawnMobile("tatooine", "womprat",300,-2581.69,5.15777,2031.96,201.278,0)
	spawnMobile("tatooine", "womprat",300,-2621.59,5.02486,1967.45,345.653,0)
	spawnMobile("tatooine", "womprat",300,-3103.38,5.15501,1918.82,259.068,0)
	spawnMobile("tatooine", "worrt",300,-3177.72,8.08039,1907.34,336.463,0)
	spawnMobile("tatooine", "worrt",300,-3172.01,8.51885,1888.24,357.926,0)
	spawnMobile("tatooine", "worrt",300,-3173.18,8.36948,1901.38,300.936,0)
	spawnMobile("tatooine", "worrt",300,-3213.79,6.5217,2012.58,328.593,0)
	spawnMobile("tatooine", "worrt",300,-3253.53,5.24487,2125.54,105.424,0)
	spawnMobile("tatooine", "worrt",300,-3337.76,7.3219,2207.57,236.021,0)
	spawnMobile("tatooine", "worrt",300,-3242.18,5.11691,1949.38,349.239,0)
	spawnMobile("tatooine", "worrt",300,-3227.98,4.76563,1939.4,297.068,0)
	spawnMobile("tatooine", "worrt",300,-3243.03,5.88032,1922.89,185.988,0)
	spawnMobile("tatooine", "worrt",300,-2716.18,5.32138,1830.71,264.114,0)
	spawnMobile("tatooine", "worrt",300,-3010.7,8.06339,2696.54,355.815,0)
	spawnMobile("tatooine", "worrt",300,-3075.74,6.01681,2609.75,341.395,0)
	spawnMobile("tatooine", "worrt",300,-2867.04,5.66638,2645.57,311.095,0)
	spawnMobile("tatooine", "worrt",300,-2594.32,5,2215.81,184.003,0)
	spawnMobile("tatooine", "worrt",300,-2589.82,5,2226.13,110.267,0)
	spawnMobile("tatooine", "worrt",300,-2613.31,5.08118,1969.82,347.531,0)
	spawnMobile("tatooine", "worrt",300,-2628.18,5.04651,1955.15,351.895,0)
end
