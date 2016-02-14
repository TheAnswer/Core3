CorelliaKorVellaScreenPlay = CityScreenPlay:new {
	numberOfActs = 1,

	screenplayName = "CorelliaKorVellaScreenPlay",

	planet = "corellia",

	gcwMobs = {

	--swap spawns for imp base near starport at outskirts of city limits
		{"stormtrooper", "rebel_trooper", -3001.5, 31.0, 2930.4, 180, 0, "", ""},
		{"stormtrooper", "rebel_trooper", -3003.5, 31.0, 2930.4, 180, 0, "", ""},
		{"stormtrooper", "rebel_trooper", -3007.5, 31.0, 2930.4, 180, 0, "", ""},
		{"stormtrooper", "rebel_trooper", -3003.5, 31.0, 2926.4, 180, 0, "", ""},
		{"stormtrooper", "rebel_trooper", -3007.5, 31.0, 2926.4, 180, 0, "", ""},
		{"stormtrooper", "rebel_trooper", -3003.5, 31.0, 2922.4, 180, 0, "", ""},
		{"stormtrooper", "rebel_trooper", -2992.1, 30.9, 2913.5, -125, 0, "", ""},
		{"stormtrooper", "rebel_trooper", -2992.1, 30.9, 2903.5, -55, 0, "", ""},
		{"stormtrooper", "rebel_trooper", 3.7, 0.3, 1.1, 0, 1331934, "", ""},
		{"stormtrooper", "rebel_trooper", 3.7, 0.3, -2.3, 0, 1331934, "", ""},
		{"stormtrooper", "rebel_trooper", -3.4, -3.3, 5.2, 180, 1331936, "", ""},
		{"stormtrooper", "rebel_trooper", 1.3, -6.7, -6.0, 45, 1331937, "", ""},
		{"stormtrooper", "rebel_trooper", 7.3, -6.7, -6.0, -45, 1331937, "", ""},
		{"stormtrooper", "rebel_trooper", -5.1, -13.7, 7.5, 180, 1331939, "", ""},
		{"stormtrooper", "rebel_trooper", -1.9, -13.7, 7.5, 180, 1331939, "", ""},
		{"stormtrooper_medic", "rebel_medic", -5.7, -13.8, 13.8, -33, 1331940, "", ""},
		{"imperial_second_lieutenant", "rebel_second_lieutenant", 16.0, -13.8, 9.6, 180, 1331941, "", ""},
		{"imperial_trooper", "rebel_crewman", 17.0, -13.8, -2.5, -45, 1331941, "", ""},
		{"imperial_major_general", "rebel_major_general", 15.1, -11.5, -20.8, 0, 1331942, "", ""},
		{"stormtrooper_medic", "rebel_medic", 0.2, 0.1, -5.1, 0, 1331946, "", ""},
		{"imperial_major", "rebel_major", 4.6, 0.1, -2.1, -135, 1331948, "", ""},
		{"stormtrooper", "rebel_trooper", -0.2, 0.1, 1.3, -90, 1331953, "", ""},
		{"imperial_colonel", "rebel_colonel", 4.6, 0.1, -2.1, -135, 1331955, "", ""},

	--faction swap spawns for in the city itself
		{"imperial_noncom", "specforce_interrogator", -3660.31, 86, 3103.26, 39.0513, 0, "", ""},
		{"dark_trooper", "rebel_commando", -3773.66, 86, 3203.13, 22.9816, 0, "", ""},
		{"imperial_private", "rebel_corporal", -3387, 86, 2960, 95, 0, "", ""},
		{"imperial_private", "rebel_corporal", -3404.35, 74, 3347.14, 188, 0, "", ""},
		{"imperial_first_lieutenant", "rebel_first_lieutenant", -3662.58, 86, 3090.82, 101, 0, "", ""},
		{"imperial_first_lieutenant", "rebel_first_lieutenant", -3074, 31, 2825, 235, 0, "", ""},
		{"imperial_first_lieutenant", "rebel_first_lieutenant", -3674.98, 86, 3104.95, 180, 0, "", ""},
		{"imperial_sergeant", "rebel_sergeant", -3435, 86, 3034, 90, 0, "", ""},
		{"imperial_sergeant", "rebel_sergeant", -3424, 86, 3018, 30, 0, "", ""},
		{"imperial_sergeant", "rebel_sergeant", -3368.45, 86, 3242.5, 216, 0, "", ""},
		{"imperial_sergeant", "rebel_sergeant", -3366, 86, 3232, 166, 0, "", ""},
		{"imperial_sergeant", "rebel_sergeant", -3582, 86, 3143, 333, 0, "", ""},
		{"stormtrooper", "rebel_crewman", -3621.27, 86, 3142.55, 28.2333, 0, "", ""},
		{"stormtrooper", "rebel_crewman", -3631.32, 86, 3136.46, 252.779, 0, "", ""},
		{"stormtrooper", "rebel_crewman", -3410.43, 86, 3142.78, 151.613, 0, "", ""},
		{"stormtrooper", "rebel_crewman", -3406.24, 86, 3137.43, 204.684, 0, "", ""},
		{"stormtrooper", "rebel_crewman", -3395.4, 86, 3017.79, 72.2994, 0, "", ""},
		{"stormtrooper", "rebel_crewman", -3395.16, 86, 3023.07, 57.428, 0, "", ""},
		{"stormtrooper", "rebel_crewman", -3344.8, 80.6241, 3240.26, 200.272, 0, "", ""},
		{"stormtrooper", "rebel_crewman", -3343.2, 81.762, 3240.54, 177.569, 0, "", ""},
		{"stormtrooper", "rebel_crewman", -3767.99, 86, 3180.85, 154.813, 0, "", ""},
		{"stormtrooper", "rebel_crewman", -3761.11, 86, 3190.17, 63.2948, 0, "", ""},
		{"stormtrooper", "rebel_crewman", -3411.64, 74, 3344.7, 179.356, 0, "", ""},
		{"stormtrooper", "rebel_crewman", -3416.53, 74, 3346.57, 326.897, 0, "", ""},
		{"stormtrooper", "rebel_crewman", -3157.47, 31, 2797.69, 352.434, 0, "", ""},
		{"stormtrooper", "rebel_crewman", -3155.56, 31, 2792.15, 348.853, 0, "", ""},
		{"stormtrooper", "rebel_crewman", -3425, 86, 3034, 90, 0, "", ""},
		{"stormtrooper", "rebel_crewman", -3368.29, 86, 2949.6, 170.619, 0, "", ""},
		{"stormtrooper", "rebel_crewman", -3420, 86, 3034, 90, 0, "", ""},
		{"stormtrooper", "rebel_crewman", -3430, 86, 3070, 0, 0, "", ""},
		{"stormtrooper", "rebel_crewman", -3430, 86, 3065, 0, 0, "", ""},
		{"stormtrooper", "rebel_crewman", -3430, 86, 3060, 0, 0, "", ""},
		{"stormtrooper", "rebel_crewman", -3430, 86, 3050, 0, 0, "", ""},
		{"stormtrooper", "rebel_crewman", -3381.8, 86, 3298.1, -79, 0, "", ""},
		{"stormtrooper", "rebel_crewman", -3719.35, 86, 3175.2, 326.578, 0, "", ""},
		{"stormtrooper", "rebel_crewman", -3595.9, 86, 3172.4, 152, 0, "", ""},
		{"stormtrooper", "rebel_crewman", -3619.55, 86, 3121.89, 113.533, 0, "", ""},
		{"stormtrooper", "rebel_crewman", -3612.9, 86, 3130.8, 95, 0, "", ""},
		{"stormtrooper", "rebel_crewman", -3726.46, 86, 3185.97, 120.721, 0, "", ""},
		{"stormtrooper", "rebel_crewman", -3754, 86, 3172, -30, 0, "", ""},
		{"stormtrooper", "rebel_crewman", -3774.9, 86, 3203.38, 281.283, 0, "", ""},
		{"stormtrooper", "rebel_crewman", -3774.65, 86, 3204.62, 22.9721, 0, "", ""},
		{"stormtrooper_medic", "rebel_medic", -3112, 31, 2814, 168, 0, "", ""},
		{"stormtrooper_medic", "rebel_medic", -3386, 86, 2992, 354.507, 0, "", ""},
		{"stormtrooper_sniper", "rebel_scout", -3093, 31.0391, 2803, 347.01, 0, "", ""},
		{"stormtrooper_sniper", "rebel_scout", -3460.41, 78, 3112.21, 232.636, 0, "", ""},
		{"stormtrooper_sniper", "rebel_scout", -3430, 86, 3055, 0, 0, "", ""},
		{"stormtrooper_sniper", "rebel_scout", -3318.03, 86, 3230.13, 115.958, 0, "", ""},
		{"stormtrooper_sniper", "rebel_scout", -3326.35, 86, 3231.94, 52.9842, 0, "", ""},
		{"stormtrooper_sniper", "rebel_scout", -3602.47, 86, 3214.02, 44.6678, 0, "", ""},
		{"stormtrooper_sniper", "rebel_scout", -3613.06, 86, 3212.07, 47.9151, 0, "", ""},
		{"stormtrooper_sniper", "rebel_scout", -3715.75, 86.0549, 3169.75, 326.9, 0, "", ""},

	},
}

registerScreenPlay("CorelliaKorVellaScreenPlay", true)

function CorelliaKorVellaScreenPlay:start()
	if (isZoneEnabled(self.planet)) then
		self:spawnMobiles()
		self:spawnSceneObjects()
		self:spawnGcwMobiles()
	end
end

function CorelliaKorVellaScreenPlay:spawnSceneObjects()

	--outside starport
	spawnSceneObject(self.planet, "object/tangible/crafting/station/public_space_station.iff", -3118.55, 31, 2783.4, 0, math.rad(90) )

end

function CorelliaKorVellaScreenPlay:spawnMobiles()

	--side starport room
	local pNpc = spawnMobile(self.planet, "bounty_hunter",60,-33.5,1.6,49.2,-88,4255428)
	self:setMoodString(pNpc, "sad")
	--starport interior
	pNpc = spawnMobile(self.planet, "trainer_shipwright",60,-0.1,0.6,67.4,176,4255423)
	self:setMoodString(pNpc, "neutral")

	--corsec not in buildings
	spawnMobile(self.planet, "chiss_male", 360, -3101.09, 31, 2905.97, 180, 0)
	spawnMobile(self.planet, "corellia_times_reporter", 360, -3100.88, 31, 2890.25, 180, 0)
	spawnMobile(self.planet, "corsec_detective", 360, -3408.26, 86, 3049.35, 180, 0)
	spawnMobile(self.planet, "corsec_inspector_sergeant", 360, -3520.73, 78, 3125.57, 0, 0)
	spawnMobile(self.planet, "corsec_inspector_sergeant", 360, -3102.24, 31, 2988.47, 266, 0)
	spawnMobile(self.planet, "corsec_lieutenant", 360, -3587.81, 86, 3135.7, 180, 0)
	spawnMobile(self.planet, "corsec_lieutenant", 360, -3450.64, 78, 3031.31, 180, 0)
	spawnMobile(self.planet, "ithorian_male", 360, -3177.78, 31, 2889.93, 0, 0)
	spawnMobile(self.planet, "corsec_major", 360, -3150.16, 31.1, 2773.0, 282, 0)
	spawnMobile(self.planet, "medic", 360, -3172.38, 31, 2880.85, 0, 0)
	spawnMobile(self.planet, "rogue_corsec_trooper", 360, -3340.49, 86, 3292.76, 100.151, 0)
	spawnMobile(self.planet, "rogue_corsec_trooper", 360, -3332.89, 86, 3303.69, 68.6738, 0)
	spawnMobile(self.planet, "rogue_corsec_trooper", 360, -3330.1, 86, 3290.98, 104.099, 0)
	spawnMobile(self.planet, "sullustan_male", 360, -3177.78, 31, 2891.03, 180, 0)
	spawnMobile(self.planet, "corsec_sergeant", 360, -3149.45, 31, 2771.7, 282.9, 0)
	spawnMobile(self.planet, "corsec_trooper", 360, -3151.32, 31, 2772.13, 282.871, 0)
	spawnMobile(self.planet, "corsec_trooper", 360, -3103.98, 31, 2988.38, 266.9, 0)
	spawnMobile(self.planet, "corsec_trooper", 360, -3102.95, 31, 2989.56, 266.9, 0)

	--misc
	spawnMobile(self.planet, "cll8_binary_load_lifter", 60, -3654.48, 86, 3081.81, 26.971, 0)
	spawnMobile(self.planet, "cll8_binary_load_lifter", 60, -3444.06, 78, 3384.03, 359.013, 0)
	spawnMobile(self.planet, "cll8_binary_load_lifter", 60, -3095.09, 31, 2975.46, 81.028, 0)
	spawnMobile(self.planet, "eg6_power_droid", 60, -3766.44, 86, 3180.9, 54.4, 0)
	spawnMobile(self.planet, "informant_npc_lvl_1", 10, -3563, 86, 3138, 60, 0)
	spawnMobile(self.planet, "informant_npc_lvl_1", 10, -3571, 86, 3176, 60, 0)
	spawnMobile(self.planet, "informant_npc_lvl_1", 10, -3574, 86, 3154, 315, 0)
	spawnMobile(self.planet, "informant_npc_lvl_1", 10, -3604, 86, 3172, 90, 0)
	spawnMobile(self.planet, "informant_npc_lvl_1", 10, -3665, 86, 3139, 170, 0)
	spawnMobile(self.planet, "informant_npc_lvl_1", 10, -3375, 86, 3281, 90, 0)
	spawnMobile(self.planet, "informant_npc_lvl_1", 10, -3358, 86, 3307, 140, 0)
	spawnMobile(self.planet, "informant_npc_lvl_1", 10, -3315, 86, 3234, 270, 0)
	spawnMobile(self.planet, "informant_npc_lvl_1", 10, -3386, 85.6, 3184, 90, 0)
	spawnMobile(self.planet, "informant_npc_lvl_1", 10, -3444, 78, 3231, 225, 0)
	spawnMobile(self.planet, "informant_npc_lvl_1", 10, -3458, 85.9, 3165, 90, 0)
	spawnMobile(self.planet, "informant_npc_lvl_1", 10, -3416, 86, 3078, 0, 0)
	spawnMobile(self.planet, "informant_npc_lvl_1", 10, -3401, 86, 3034, 270, 0)
	spawnMobile(self.planet, "informant_npc_lvl_1", 10, -3124, 31, 2808, 180, 0)
	spawnMobile(self.planet, "informant_npc_lvl_1", 10, -3128, 31, 3008, 180, 0)
	--spawnMobile(self.planet, "junk_dealer", 10,-3133.8,31,2975.5,7,0)
	spawnMobile(self.planet, "medic", 60, -3591.74, 86, 3179.01, 0, 0)
		--Left these in corellia.lua because of the custom name
	--{"r2",60,-3697.43,87.5185,3141.62,327.074,0, "calm", "R2-Z1"},
	--{"r3",60,-3423.05,80.6846,3333.75,253.023,0, "calm", "R3-U1"},
	--{"r3",60,-3327.95,86,3233.64,315.946,0, "calm", "R3-N0"},
	--{"r3",60,-3151.77,31,2800,265.951,0, "calm", "R3-L8"},
	--{"r5",60,-3094.48,31,2972.63,268.321,0, "calm", "R5-W1"},
	--{"r5",60,-3134.32,31,2801.71,82.3858,0, "calm", "R5-U7"},

	--commoners
	spawnMobile(self.planet, "bodyguard", 300, -3587.81, 86, 3134.7, 0, 0)
	spawnMobile(self.planet, "bodyguard", 300, -3162.17, 31, 2902.28, 0, 0)
	spawnMobile(self.planet, "info_broker", 60, -3088.9, 31, 2902.08, 0, 0)
	spawnMobile(self.planet, "brawler", 60, -3459.55, 78, 3226.02, 180, 0)
	spawnMobile(self.planet, "businessman", 60, -3591.74, 86, 3180.01, 180, 0)
	spawnMobile(self.planet, "businessman", 60, -3159.35, 31, 2813.02, 0, 0)
	spawnMobile(self.planet, "businessman", 60, -3152.32, 31, 2971.71, 0, 0)
	spawnMobile(self.planet, "commoner", 60, -3522.13, 70, 3234.57, 298, 0)
	spawnMobile(self.planet, "commoner", 60, -3572.52, 70, 3208.39, 214, 0)
	spawnMobile(self.planet, "commoner", 60, -3509.45, 78, 3148.97, 307, 0)
	spawnMobile(self.planet, "commoner", 60, -3447.57, 78, 3219.08, 209, 0)
	spawnMobile(self.planet, "commoner", 60, -3455.84, 78, 3343.36, 116, 0)
	spawnMobile(self.planet, "commoner", 60, -3427.62, 86, 3029.21, 72, 0)
	spawnMobile(self.planet, "commoner", 60, -3423.54, 86, 3043.61, 131, 0)
	spawnMobile(self.planet, "commoner", 60, -3403.44, 86, 2997.65, 182, 0)
	spawnMobile(self.planet, "commoner", 60, -3403.21, 86, 3111.2, 47, 0)
	spawnMobile(self.planet, "commoner", 60, -3408.19, 86, 3125.55, 199, 0)
	spawnMobile(self.planet, "commoner", 60, -3426.15, 86, 3134.26, 343, 0)
	spawnMobile(self.planet, "commoner", 60, -3444.77, 86, 3143.75, 343, 0)
	spawnMobile(self.planet, "commoner", 60, -3435.7, 86, 3119.29, 67, 0)
	spawnMobile(self.planet, "commoner", 60, -3416.7, 86, 3033.89, 318, 0)
	spawnMobile(self.planet, "commoner", 60, -3458.31, 78, 3027.47, 275, 0)
	spawnMobile(self.planet, "commoner", 60, -3461.07, 86, 3156, 54, 0)
	spawnMobile(self.planet, "commoner", 60, -3421.3, 86, 3104.47, 296, 0)
	spawnMobile(self.planet, "commoner", 1,-3361.13,86,3217.26,124.012,0)
	spawnMobile(self.planet, "commoner", 1,-3324.59,86,3261.86,293.486,0)
	spawnMobile(self.planet, "commoner", 1,-3322.1,86,3230.47,113.222,0)
	spawnMobile(self.planet, "commoner", 1,-3342.65,86,3268.3,214.421,0)
	spawnMobile(self.planet, "commoner", 1,-3365.89,86,3284.68,46.8022,0)
	spawnMobile(self.planet, "commoner", 1,-3374.97,86,3265.02,47.337,0)
	spawnMobile(self.planet, "commoner", 1,-3351.81,86,3303.25,24.0851,0)
	spawnMobile(self.planet, "commoner", 1,-3332.16,86,3307.2,264.653,0)
	spawnMobile(self.planet, "commoner", 1,-3322.3,86,3283.63,275.608,0)
	spawnMobile(self.planet, "commoner", 1,-3393.47,86,3007.94,286.244,0)
	spawnMobile(self.planet, "commoner", 1,-3378.96,86,2985.53,285.729,0)
	spawnMobile(self.planet, "commoner", 1,-3384.17,86,2958.24,11.3093,0)
	spawnMobile(self.planet, "commoner", 1,-3311.8,69.6204,2886.94,40.0687,0)
	spawnMobile(self.planet, "commoner", 1,-3378.65,86,3173.54,240.512,0)
	spawnMobile(self.planet, "commoner", 1,-3396.89,86,3131.64,135.762,0)
	spawnMobile(self.planet, "commoner", 1,-3391.66,86,3140.05,231.255,0)
	spawnMobile(self.planet, "commoner", 1,-3296.84,66.0938,2877.86,186.227,0)
	spawnMobile(self.planet, "commoner", 1,-3225.11,32.2567,2816.51,354.534,0)
	spawnMobile(self.planet, "commoner", 1,-3276.7,86,3089.82,195.888,0)
	spawnMobile(self.planet, "commoner", 1,-3256,86,3124.3,90.73,0)
	spawnMobile(self.planet, "commoner", 1,-3272.53,86,3071.96,247.995,0)
	spawnMobile(self.planet, "commoner", 1,-3199.88,30.6149,2786.24,349.225,0)
	spawnMobile(self.planet, "commoner", 1,-3160.26,30.6615,2762.28,0.966738,0)
	spawnMobile(self.planet, "commoner", 1,-3178.84,31,2775.92,288.531,0)
	spawnMobile(self.planet, "commoner", 1,-3115.47,31,2800.28,30.7435,0)
	spawnMobile(self.planet, "commoner", 1,-3112.88,31,2984.36,125.035,0)
	spawnMobile(self.planet, "commoner", 1,-3131.04,31,2995.05,134.779,0)
	spawnMobile(self.planet, "commoner", 1,-3157.04,101,3077.75,219.742,0)
	spawnMobile(self.planet, "commoner", 1,-3191.67,101,3052.4,294.559,0)
	spawnMobile(self.planet, "commoner", 1,-3190.54,101,3073.46,88.2138,0)
	spawnMobile(self.planet, "commoner", 1,-3185.75,101,3104.64,308.461,0)
	spawnMobile(self.planet, "commoner", 1,-3153.75,101,3112.41,31.6067,0)
	spawnMobile(self.planet, "commoner", 1,-3188.79,101,3156.71,306.911,0)
	spawnMobile(self.planet, "commoner", 1,-3068.52,30.6373,2772.26,154.712,0)
	spawnMobile(self.planet, "commoner", 1,-3028.15,29.9485,2793.83,126.824,0)
	spawnMobile(self.planet, "commoner", 1,-3066.9,31,2925.13,132.488,0)
	spawnMobile(self.planet, "commoner", 1,-3087.72,31,2984.93,5.4138,0)
	spawnMobile(self.planet, "entertainer", 60, -3602.67, 86, 3221.17, 0, 0)
	spawnMobile(self.planet, "entertainer", 60, -16.9034, 1.6, 11.8646, 180, 3005672)
	spawnMobile(self.planet, "entertainer", 60, -3100.88, 31, 2889.15, 0, 0)
	spawnMobile(self.planet, "businessman", 60, -3172.38, 31, 2881.95, 180, 0)
	spawnMobile(self.planet, "bounty_hunter", 60, -3101.09, 31, 2904.87, 0, 0)
	spawnMobile(self.planet, "farmer", 60, -3713.14, 86, 3105.66, 0, 0)
	spawnMobile(self.planet, "farmer", 60, -3610.27, 86, 3198.4, 0, 0)
	spawnMobile(self.planet, "farmer", 60, -3163.27, 31, 2903.38, 135.0, 0)
	spawnMobile(self.planet, "farmer_rancher", 60, -3159.35, 31, 2814.02, 180, 0)
	spawnMobile(self.planet, "fringer", 300, -3713.14, 86, 3106.66, 180.0, 0)
	spawnMobile(self.planet, "gambler", 60, -3408.26, 86, 3048.35, 0, 0)
	spawnMobile(self.planet, "contractor", 60, -3090, 31, 2903.18, 135.0, 0)
	spawnMobile(self.planet, "mercenary", 60, -3520.73, 78, 3126.57, 180, 0)
	spawnMobile(self.planet, "twilek_slave", 60, -3162.17, 31, 2903.38, 180, 0)
	spawnMobile(self.planet, "entertainer", 60, -3163.27, 31, 2902.28, 45, 0)
	spawnMobile(self.planet, "noble", 60, -3610.27, 86, 3199.4, 180, 0)
	spawnMobile(self.planet, "noble", 60, -3459.55, 78, 3225.02, 0, 0)
	spawnMobile(self.planet, "noble", 60, -3095.48, 31, 2971.63, 249, 0)
	spawnMobile(self.planet, "pilot", 60, -3384.54, 86, 3008.15, 180, 0)
	spawnMobile(self.planet, "slicer", 60, -3450.64, 78, 3030.31, 0, 0)
	spawnMobile(self.planet, "slicer", 60, -3384.54, 86, 3007.15, 0, 0)
	spawnMobile(self.planet, "slicer", 60, -3152.32, 31, 2972.71, 180, 0)
	spawnMobile(self.planet, "mercenary", 60, -3088.9, 31, 2903.18, 180, 0)
	--thugs
	spawnMobile(self.planet, "ragtag_kook", 300, -3816.4, 86, 3114.2, 103, 0)
	spawnMobile(self.planet, "ragtag_kook", 300, -3814.6, 86, 3109.6, 58, 0)
	spawnMobile(self.planet, "ragtag_kook", 300, -3809.9, 86, 3111.1, -46, 0)
	spawnMobile(self.planet, "ragtag_kook", 300, -3809.5, 86, 3117.4, -115, 0)
	spawnMobile(self.planet, "ragtag_kook", 300, -3801.2, 86, 3251.2, 12, 0)
	spawnMobile(self.planet, "ragtag_kook", 300, -3797.2, 86, 3251.1, 67, 0)
	spawnMobile(self.planet, "ragtag_kook", 300, -3793.2, 86, 3255.3, -73, 0)
	spawnMobile(self.planet, "ragtag_kook", 300, -3797.9, 86, 3259.9, -170, 0)
	spawnMobile(self.planet, "ragtag_kook", 300, -3717.2, 86, 3074.9, 29, 0)
	spawnMobile(self.planet, "ragtag_kook", 300, -3712.6, 86, 3073.8, 104, 0)
	spawnMobile(self.planet, "ragtag_kook", 300, -3711.0, 86, 3077.6, -38, 0)
	spawnMobile(self.planet, "ragtag_kook", 300, -3714.8, 86, 3079.9, 19, 0)
	spawnMobile(self.planet, "ragtag_kook", 300, -3602.3, 86, 3105.7, -69, 0)
	spawnMobile(self.planet, "ragtag_kook", 300, -3602.5, 86, 3107.6, 2, 0)
	spawnMobile(self.planet, "ragtag_kook", 300, -3598.4, 86, 3107.5, -90, 0)
	spawnMobile(self.planet, "ragtag_kook", 300, -3599.5, 86, 3112.2, 132, 0)
	spawnMobile(self.planet, "ragtag_kook", 300, -3602.1, 86, 3115.1, -170, 0)
	spawnMobile(self.planet, "ragtag_kook", 300, -3632.6, 86, 3232.5, 51, 0)
	spawnMobile(self.planet, "ragtag_kook", 300, -3630.2, 86, 3236.7, 104, 0)
	spawnMobile(self.planet, "ragtag_kook", 300, -3630.0, 86, 3232.6, -57, 0)
	spawnMobile(self.planet, "ragtag_kook", 300, -3500.5, 78, 3148.0, 107, 0)
	spawnMobile(self.planet, "ragtag_kook", 300, -3500.8, 78, 3145.0, 114, 0)
	spawnMobile(self.planet, "ragtag_kook", 300, -3494.1, 78, 3140.4, -53, 0)
	spawnMobile(self.planet, "ragtag_kook", 300, -3495.8, 78, 3146.1, -88, 0)
	spawnMobile(self.planet, "ragtag_kook", 300, -3496.7, 78, 3138.3, 2, 0)
	spawnMobile(self.planet, "ragtag_kook", 300, -3499.1, 70, 3239.0, 1, 0)
	spawnMobile(self.planet, "ragtag_kook", 300, -3495.3, 70, 3240.8, 8, 0)
	spawnMobile(self.planet, "ragtag_kook", 300, -3495.8, 70, 3243.1, -100, 0)
	spawnMobile(self.planet, "ragtag_kook", 300, -3497.5, 70, 3245.9, -83, 0)
	spawnMobile(self.planet, "ragtag_kook", 300, -3501.7, 70, 3244.5, 162, 0)
	spawnMobile(self.planet, "ragtag_kook", 300, -3467.7, 85.6, 3182.2, 74, 0)
	spawnMobile(self.planet, "ragtag_kook", 300, -3467.6, 85.9, 3175.1, 15, 0)
	spawnMobile(self.planet, "ragtag_kook", 300, -3461.5, 85.4, 3179.1, -53, 0)
	spawnMobile(self.planet, "ragtag_kook", 300, -3462.3, 85.1, 3186.0, 18, 0)
	spawnMobile(self.planet, "ragtag_kook", 300, -3309.4, 86, 3285.4, -141, 0)
	spawnMobile(self.planet, "ragtag_kook", 300, -3309.9, 86, 3281.3, -103, 0)
	spawnMobile(self.planet, "ragtag_kook", 300, -3309.5, 86, 3276.9, -99, 0)
	spawnMobile(self.planet, "ragtag_kook", 300, -3314.0, 86, 3277.9, 16, 0)
	spawnMobile(self.planet, "ragtag_kook", 300, -3631.5, 86, 3117.4, -67, 0)
	spawnMobile(self.planet, "ragtag_kook", 300, -3637.5, 86, 3114.4, 22, 0)
	spawnMobile(self.planet, "ragtag_kook", 300, -3634.5, 86, 3119.4, -167, 0)
	spawnMobile(self.planet, "ragtag_kook", 300, -3630.5, 86, 3119.4, -30, 0)
	spawnMobile(self.planet, "ragtag_kook", 300, -3162.3, 31, 2999.6, 64, 0)
	spawnMobile(self.planet, "ragtag_kook", 300, -3160.5, 31, 3001.3, 47, 0)
	spawnMobile(self.planet, "ragtag_kook", 300, -3157.2, 31, 2999.4, 145, 0)
	spawnMobile(self.planet, "ragtag_kook", 300, -3158.8, 31, 2994.4, -29, 0)
	spawnMobile(self.planet, "ragtag_kook", 300, -3161.8, 31, 2991.5, -109, 0)
	spawnMobile(self.planet, "ragtag_kook", 300, -3417.7, 86, 3118.9, 112, 0)
	spawnMobile(self.planet, "ragtag_kook", 300, -3421.1, 86, 3120.9, 157, 0)
	spawnMobile(self.planet, "ragtag_kook", 300, -3424.7, 86, 3115.9, -25, 0)
	spawnMobile(self.planet, "ragtag_kook", 300, -3420.1, 86, 3112.9, 148, 0)

	--Creatures
	spawnMobile(self.planet, "minor_gubbur", 300, getRandomNumber(10) + -3286, 101,getRandomNumber(10) + 2986, getRandomNumber(360), 0)
	spawnMobile(self.planet, "minor_gubbur", 300, getRandomNumber(10) + -3286, 101,getRandomNumber(10) + 2986, getRandomNumber(360), 0)
	spawnMobile(self.planet, "minor_gubbur", 300, getRandomNumber(10) + -3286, 101,getRandomNumber(10) + 2986, getRandomNumber(360), 0)
	spawnMobile(self.planet, "minor_gubbur", 300, getRandomNumber(10) + -3286, 101,getRandomNumber(10) + 2986, getRandomNumber(360), 0)
	spawnMobile(self.planet, "minor_gubbur", 300, getRandomNumber(10) + -3286, 101,getRandomNumber(10) + 2986, getRandomNumber(360), 0)
	spawnMobile(self.planet, "minor_gubbur", 300, getRandomNumber(10) + -3286, 101,getRandomNumber(10) + 2986, getRandomNumber(360), 0)
	spawnMobile(self.planet, "durni", 300, getRandomNumber(10) + -3177, 101,getRandomNumber(10) + 3084, getRandomNumber(360), 0)
	spawnMobile(self.planet, "durni", 300, getRandomNumber(10) + -3177, 101,getRandomNumber(10) + 3084, getRandomNumber(360), 0)
	spawnMobile(self.planet, "durni", 300, getRandomNumber(10) + -3177, 101,getRandomNumber(10) + 3084, getRandomNumber(360), 0)
	spawnMobile(self.planet, "durni", 300, getRandomNumber(10) + -3177, 101,getRandomNumber(10) + 3084, getRandomNumber(360), 0)
	spawnMobile(self.planet, "durni", 300, getRandomNumber(10) + -3177, 101,getRandomNumber(10) + 3084, getRandomNumber(360), 0)
	spawnMobile(self.planet, "durni", 300, getRandomNumber(10) + -3177, 101,getRandomNumber(10) + 3084, getRandomNumber(360), 0)
	spawnMobile(self.planet, "paralope", 300, getRandomNumber(10) + -3249, 101, getRandomNumber(10) + 3308, getRandomNumber(360), 0)
	spawnMobile(self.planet, "paralope", 300, getRandomNumber(10) + -3249, 101, getRandomNumber(10) + 3308, getRandomNumber(360), 0)
	spawnMobile(self.planet, "paralope", 300, getRandomNumber(10) + -3249, 101, getRandomNumber(10) + 3308, getRandomNumber(360), 0)
	spawnMobile(self.planet, "paralope", 300, getRandomNumber(10) + -3249, 101, getRandomNumber(10) + 3308, getRandomNumber(360), 0)
	spawnMobile(self.planet, "paralope", 300, getRandomNumber(10) + -3249, 101, getRandomNumber(10) + 3308, getRandomNumber(360), 0)
	spawnMobile(self.planet, "paralope", 300, getRandomNumber(10) + -3249, 101, getRandomNumber(10) + 3308, getRandomNumber(360), 0)
	spawnMobile(self.planet, "corellian_butterfly_drone", 300, getRandomNumber(10) + -3335, 101, getRandomNumber(10) + 3383, getRandomNumber(360), 0)
	spawnMobile(self.planet, "corellian_butterfly_drone", 300, getRandomNumber(10) + -3335, 101, getRandomNumber(10) + 3383, getRandomNumber(360), 0)
	spawnMobile(self.planet, "corellian_butterfly_drone", 300, getRandomNumber(10) + -3335, 101, getRandomNumber(10) + 3383, getRandomNumber(360), 0)
	spawnMobile(self.planet, "corellian_butterfly_drone", 300, getRandomNumber(10) + -3335, 101, getRandomNumber(10) + 3383, getRandomNumber(360), 0)
	spawnMobile(self.planet, "corellian_butterfly_drone", 300, getRandomNumber(10) + -3335, 101, getRandomNumber(10) + 3383, getRandomNumber(360), 0)
	spawnMobile(self.planet, "corellian_butterfly_drone", 300, getRandomNumber(10) + -3335, 101, getRandomNumber(10) + 3383, getRandomNumber(360), 0)
	spawnMobile(self.planet, "verne", 300, getRandomNumber(10) + -3389, 37, getRandomNumber(10) + 3531, getRandomNumber(360), 0)
	spawnMobile(self.planet, "verne", 300, getRandomNumber(10) + -3389, 37, getRandomNumber(10) + 3531, getRandomNumber(360), 0)
	spawnMobile(self.planet, "verne", 300, getRandomNumber(10) + -3389, 37, getRandomNumber(10) + 3531, getRandomNumber(360), 0)
	spawnMobile(self.planet, "verne", 300, getRandomNumber(10) + -3389, 37, getRandomNumber(10) + 3531, getRandomNumber(360), 0)
	spawnMobile(self.planet, "verne", 300, getRandomNumber(10) + -3389, 37, getRandomNumber(10) + 3531, getRandomNumber(360), 0)
	spawnMobile(self.planet, "verne", 300, getRandomNumber(10) + -3389, 37, getRandomNumber(10) + 3531, getRandomNumber(360), 0)
	spawnMobile(self.planet, "corellian_butterfly_drone", 300, getRandomNumber(10) + -3519, 101, getRandomNumber(10) + 3469, getRandomNumber(360), 0)
	spawnMobile(self.planet, "corellian_butterfly_drone", 300, getRandomNumber(10) + -3519, 101, getRandomNumber(10) + 3469, getRandomNumber(360), 0)
	spawnMobile(self.planet, "corellian_butterfly_drone", 300, getRandomNumber(10) + -3519, 101, getRandomNumber(10) + 3469, getRandomNumber(360), 0)
	spawnMobile(self.planet, "corellian_butterfly_drone", 300, getRandomNumber(10) + -3519, 101, getRandomNumber(10) + 3469, getRandomNumber(360), 0)
	spawnMobile(self.planet, "corellian_butterfly_drone", 300, getRandomNumber(10) + -3519, 101, getRandomNumber(10) + 3469, getRandomNumber(360), 0)
	spawnMobile(self.planet, "corellian_butterfly_drone", 300, getRandomNumber(10) + -3519, 101, getRandomNumber(10) + 3469, getRandomNumber(360), 0)
	spawnMobile(self.planet, "vrelt", 300, getRandomNumber(10) + -3698, 101, getRandomNumber(10) + 3397, getRandomNumber(360), 0)
	spawnMobile(self.planet, "vrelt", 300, getRandomNumber(10) + -3698, 101, getRandomNumber(10) + 3397, getRandomNumber(360), 0)
	spawnMobile(self.planet, "vrelt", 300, getRandomNumber(10) + -3698, 101, getRandomNumber(10) + 3397, getRandomNumber(360), 0)
	spawnMobile(self.planet, "vrelt", 300, getRandomNumber(10) + -3698, 101, getRandomNumber(10) + 3397, getRandomNumber(360), 0)
	spawnMobile(self.planet, "vrelt", 300, getRandomNumber(10) + -3698, 101, getRandomNumber(10) + 3397, getRandomNumber(360), 0)
	spawnMobile(self.planet, "vrelt", 300, getRandomNumber(10) + -3698, 101, getRandomNumber(10) + 3397, getRandomNumber(360), 0)
	spawnMobile(self.planet, "minor_gubbur", 300, getRandomNumber(10) + -3677, 101,getRandomNumber(10) + 3294, getRandomNumber(360), 0)
	spawnMobile(self.planet, "minor_gubbur", 300, getRandomNumber(10) + -3677, 101,getRandomNumber(10) + 3294, getRandomNumber(360), 0)
	spawnMobile(self.planet, "minor_gubbur", 300, getRandomNumber(10) + -3677, 101,getRandomNumber(10) + 3294, getRandomNumber(360), 0)
	spawnMobile(self.planet, "minor_gubbur", 300, getRandomNumber(10) + -3677, 101,getRandomNumber(10) + 3294, getRandomNumber(360), 0)
	spawnMobile(self.planet, "minor_gubbur", 300, getRandomNumber(10) + -3677, 101,getRandomNumber(10) + 3294, getRandomNumber(360), 0)
	spawnMobile(self.planet, "minor_gubbur", 300, getRandomNumber(10) + -3677, 101,getRandomNumber(10) + 3294, getRandomNumber(360), 0)
	spawnMobile(self.planet, "durni", 300, getRandomNumber(10) + -3808, 101,getRandomNumber(10) + 3347, getRandomNumber(360), 0)
	spawnMobile(self.planet, "durni", 300, getRandomNumber(10) + -3808, 101,getRandomNumber(10) + 3347, getRandomNumber(360), 0)
	spawnMobile(self.planet, "durni", 300, getRandomNumber(10) + -3808, 101,getRandomNumber(10) + 3347, getRandomNumber(360), 0)
	spawnMobile(self.planet, "durni", 300, getRandomNumber(10) + -3808, 101,getRandomNumber(10) + 3347, getRandomNumber(360), 0)
	spawnMobile(self.planet, "durni", 300, getRandomNumber(10) + -3808, 101,getRandomNumber(10) + 3347, getRandomNumber(360), 0)
	spawnMobile(self.planet, "durni", 300, getRandomNumber(10) + -3808, 101,getRandomNumber(10) + 3347, getRandomNumber(360), 0)
	spawnMobile(self.planet, "corellian_butterfly_drone", 300, getRandomNumber(10) + -3797, 101, getRandomNumber(10) + 2958, getRandomNumber(360), 0)
	spawnMobile(self.planet, "corellian_butterfly_drone", 300, getRandomNumber(10) + -3797, 101, getRandomNumber(10) + 2958, getRandomNumber(360), 0)
	spawnMobile(self.planet, "corellian_butterfly_drone", 300, getRandomNumber(10) + -3797, 101, getRandomNumber(10) + 2958, getRandomNumber(360), 0)
	spawnMobile(self.planet, "corellian_butterfly_drone", 300, getRandomNumber(10) + -3797, 101, getRandomNumber(10) + 2958, getRandomNumber(360), 0)
	spawnMobile(self.planet, "corellian_butterfly_drone", 300, getRandomNumber(10) + -3797, 101, getRandomNumber(10) + 2958, getRandomNumber(360), 0)
	spawnMobile(self.planet, "corellian_butterfly_drone", 300, getRandomNumber(10) + -3797, 101, getRandomNumber(10) + 2958, getRandomNumber(360), 0)
	spawnMobile(self.planet, "vrelt", 300, getRandomNumber(10) + -3659, 101, getRandomNumber(10) + 2855, getRandomNumber(360), 0)
	spawnMobile(self.planet, "vrelt", 300, getRandomNumber(10) + -3659, 101, getRandomNumber(10) + 2855, getRandomNumber(360), 0)
	spawnMobile(self.planet, "vrelt", 300, getRandomNumber(10) + -3659, 101, getRandomNumber(10) + 2855, getRandomNumber(360), 0)
	spawnMobile(self.planet, "vrelt", 300, getRandomNumber(10) + -3659, 101, getRandomNumber(10) + 2855, getRandomNumber(360), 0)
	spawnMobile(self.planet, "vrelt", 300, getRandomNumber(10) + -3659, 101, getRandomNumber(10) + 2855, getRandomNumber(360), 0)
	spawnMobile(self.planet, "vrelt", 300, getRandomNumber(10) + -3659, 101, getRandomNumber(10) + 2855, getRandomNumber(360), 0)
	spawnMobile(self.planet, "paralope", 300, getRandomNumber(10) + -3532, 101, getRandomNumber(10) + 2821, getRandomNumber(360), 0)
	spawnMobile(self.planet, "paralope", 300, getRandomNumber(10) + -3532, 101, getRandomNumber(10) + 2821, getRandomNumber(360), 0)
	spawnMobile(self.planet, "paralope", 300, getRandomNumber(10) + -3532, 101, getRandomNumber(10) + 2821, getRandomNumber(360), 0)
	spawnMobile(self.planet, "paralope", 300, getRandomNumber(10) + -3532, 101, getRandomNumber(10) + 2821, getRandomNumber(360), 0)
	spawnMobile(self.planet, "paralope", 300, getRandomNumber(10) + -3532, 101, getRandomNumber(10) + 2821, getRandomNumber(360), 0)
	spawnMobile(self.planet, "paralope", 300, getRandomNumber(10) + -3532, 101, getRandomNumber(10) + 2821, getRandomNumber(360), 0)
	spawnMobile(self.planet, "corellian_butterfly_drone", 300, -3217.9, 29.2, 2732.2, 87, 0)
	spawnMobile(self.planet, "corellian_butterfly_drone", 300, -3220.2, 29.2, 2731.6, -111, 0)
	spawnMobile(self.planet, "corellian_butterfly_drone", 300, -3220.7, 29.2, 2735.2, 1, 0)
	spawnMobile(self.planet, "corellian_butterfly_drone", 300, -3217.6, 29.3, 2737.8, 57, 0)
	spawnMobile(self.planet, "corellian_butterfly_drone", 300, -3219.4, 29.4, 2742.1, -50, 0)
	spawnMobile(self.planet, "corellian_butterfly_drone", 300, -3223.1, 29.3, 2742.4, -87, 0)
	spawnMobile(self.planet, "minor_gubbur", 300, -3005.4, 25.5, 2731.1, 60, 0)
	spawnMobile(self.planet, "minor_gubbur", 300, -3001.2, 25.8, 2728.8, 118, 0)
	spawnMobile(self.planet, "minor_gubbur", 300, -2998.3, 25.9, 2732.4, -18, 0)
	spawnMobile(self.planet, "minor_gubbur", 300, -3001.0, 25.7, 2734.2, -60, 0)
	spawnMobile(self.planet, "minor_gubbur", 300, -2996.4, 25.9, 2735.7, 77, 0)
	spawnMobile(self.planet, "minor_gubbur", 300, -2994.2, 26.2, 2730.8, 177, 0)
	spawnMobile(self.planet, "corellian_butterfly_drone", 300, getRandomNumber(10) + -2981, 31, getRandomNumber(10) + 2988, getRandomNumber(360), 0)
	spawnMobile(self.planet, "corellian_butterfly_drone", 300, getRandomNumber(10) + -2981, 31, getRandomNumber(10) + 2988, getRandomNumber(360), 0)
	spawnMobile(self.planet, "corellian_butterfly_drone", 300, getRandomNumber(10) + -2981, 31, getRandomNumber(10) + 2988, getRandomNumber(360), 0)
	spawnMobile(self.planet, "corellian_butterfly_drone", 300, getRandomNumber(10) + -2981, 31, getRandomNumber(10) + 2988, getRandomNumber(360), 0)
	spawnMobile(self.planet, "corellian_butterfly_drone", 300, getRandomNumber(10) + -2981, 31, getRandomNumber(10) + 2988, getRandomNumber(360), 0)
	spawnMobile(self.planet, "corellian_butterfly_drone", 300, getRandomNumber(10) + -2981, 31, getRandomNumber(10) + 2988, getRandomNumber(360), 0)

	--Trainers not in buildings
	spawnMobile(self.planet, "trainer_1hsword", 1, -3320, 86, 3220, 0, 0)
	spawnMobile(self.planet, "trainer_2hsword", 1, -3374, 86, 3285, 87, 0)
	spawnMobile(self.planet, "trainer_artisan", 1, -3164, 31, 2791, 0, 0)
	spawnMobile(self.planet, "trainer_brawler", 1, -3124, 31, 2791, 0, 0)
	spawnMobile(self.planet, "trainer_brawler", 1, -3193, 31, 2799, -129, 0)
	spawnMobile(self.planet, "trainer_chef", 1, -3147, 31, 2792, 0, 0)
	spawnMobile(self.planet, "trainer_commando", 1, -3344, 86, 3292, 0, 0)
	spawnMobile(self.planet, "trainer_entertainer", 1, -3152, 31, 2807, 178, 0)
	spawnMobile(self.planet, "trainer_marksman", 1, -3130, 31, 2791, 0, 0)
	spawnMobile(self.planet, "trainer_medic", 1, -3139, 31, 2792, 0, 0)
	spawnMobile(self.planet, "trainer_polearm", 1, -3412, 86, 3084, 0, 0)
	spawnMobile(self.planet, "trainer_scout", 1, -3164.42, 31, 2802.08, 78, 0)
	spawnMobile(self.planet, "trainer_unarmed", 1, -3570, 86, 3146, 233, 0)
	--named npc's
	spawnMobile(self.planet, "crowley", 60, -3404.73, 86, 3092.64, 323.239, 0)
	spawnMobile(self.planet, "dac_axfow", 60, -3143.7, 31, 2803.73, 233.156, 0)
	spawnMobile(self.planet, "eryk_chinn", 60, -3145.32, 31, 2803.95, 208.286, 0)
	spawnMobile(self.planet, "logro_elsin", 60, -3144.67, 31, 2806.06, 174, 0)

	--Building Interiors
		--Starport
	spawnMobile(self.planet, "contractor", 60, 0.68378, 0.639421, 62.1666, 180, 4255423)
	spawnMobile(self.planet, "sullustan_male", 60, 45.99, -0.52, 35.85, 180, 4255426)
	spawnMobile(self.planet, "medic", 60, 45.99, -0.52, 34.75, 0, 4255426)
	spawnMobile(self.planet, "mercenary", 60, 44.8927, -0.52, 35.85, 135, 4255426)
	spawnMobile(self.planet, "medic", 60, -62.23, 2.64, 40.83, 180, 4255431)
	spawnMobile(self.planet, "corellia_times_reporter", 300, -62.23, 2.64, 39.73, 0.19, 4255431)
	spawnMobile(self.planet, "chassis_dealer", 60, -56.71, 0.97, 8.33, 12.61, 4255433)

		--Hotel
	spawnMobile(self.planet, "zhanks", 60, 1.25, 1.0, 6.9, 340, 3005670)
	spawnMobile(self.planet, "ta_667", 60, -0.52, 1.0, 6.4, 13.75, 3005670)
	spawnMobile(self.planet, "commoner", 1, 20.5, 1.28, 10, 135, 3005671)
	spawnMobile(self.planet, "entertainer", 60, 21.6, 1.28, 8.94, 0, 3005671)
	spawnMobile(self.planet, "farmer_rancher", 60, 21.6, 1.28, 10.04, 180, 3005671)
		--left in corellia.lua because of custom name
	--spawnMobile(self.planet, "commoner", 1,-24,1.6,-4,124.701,3005676, "calm", "An Event Promoter"},
		--Cantina
	spawnMobile(self.planet, "comm_operator", 400, 48.13, 0.1, 2.47, 292, 3005397)
	spawnMobile(self.planet, "artisan", 60, 34.4, 0.1, -8.04, 0, 3005398)
	spawnMobile(self.planet, "mercenary", 60, 34.4, 0.1, -6.9, 180, 3005398)
	spawnMobile(self.planet, "trainer_entertainer", 1, 26.2, -0.9, 10.25, 260, 3005399)
	spawnMobile(self.planet, "bartender", 60, 6.4, -0.9, -2.3, 156, 3005399)
	spawnMobile(self.planet, "corellia_times_investigator", 300, 20.6, -0.9, -18.6, 180, 3005400)
	spawnMobile(self.planet, "commoner_technician", 60, 20.6, -0.9, -19.6, 0, 3005400)
	spawnMobile(self.planet, "brawler", 60, 22.6, -0.9, 19.6, 0, 3005401)
	spawnMobile(self.planet, "commoner", 60, 21.5, -0.9, 20.7, 135, 3005401)
	spawnMobile(self.planet, "farmer_rancher", 300, 22.6, -0.9, 20.7, 180, 3005401)
	spawnMobile(self.planet, "farmer", 60, 10.9, -0.9, -13.9, 180, 3005402)
	spawnMobile(self.planet, "mercenary", 60, 10.9, -0.9, -15.0, 0, 3005402)
	spawnMobile(self.planet, "skinkner", 60, 8.8, -0.9, 21.4, 104, 3005403)
	spawnMobile(self.planet, "brawler", 60, -2.4, -0.9, 24.5, 180, 3005405)
	spawnMobile(self.planet, "shadowy_figure", 60, -2.4, -0.9, 23.4, 0, 3005405)
	spawnMobile(self.planet, "commoner_tatooine", 300, -25.7, -0.5, 10.5, 0, 3005409)
	spawnMobile(self.planet, "bounty_hunter", 60, -25.7, -0.5, 11.5, 180, 3005409)
	spawnMobile(self.planet, "commoner_naboo", 300, -45.2, 0.1, -20.7, 180, 3005411)
	spawnMobile(self.planet, "commoner", 60, -45.2, 0.1, -21.7, 0, 3005411)

		--Guild Hall (-3435,3196)
	spawnMobile(self.planet, "trainer_tailor", 0, 11, 1.1, -14, 0, 2955415)
	spawnMobile(self.planet, "mercenary", 60, 3.29, 1.1, -9.58, 249, 2955416)
	spawnMobile(self.planet, "trainer_artisan", 0, 0, 1.1, -14, 0, 2955416)
	spawnMobile(self.planet, "trainer_chef", 0, -11, 1.1, -14, 0, 2955417)
		--Guild Hall (-3412,3364)
	--spawnMobile(self.planet, "junk_dealer", 0, -14.5, 1.1, 2.5, 88, 2955424)
	spawnMobile(self.planet, "trainer_scout", 0, -12, 1.13, 5.5, 180, 2955424)
	spawnMobile(self.planet, "trainer_marksman", 0, 0, 1.13, -14, 0, 2955426)
	spawnMobile(self.planet, "trainer_brawler", 0, -11, 1.13, -14, 0, 2955427)

		--Cloning Facility
	spawnMobile(self.planet, "info_broker", 60, 4.47, -0.05, 2.9, 135, 3005428)
	spawnMobile(self.planet, "medic", 60, 5.57, -0.05, 2.9, 180, 3005428)
	spawnMobile(self.planet, "medic", 300, 3.9, -0.05, -3.15, 0, 3005428)
	spawnMobile(self.planet, "scientist", 60, 5.57, -0.05, 1.84, 0, 3005428)
	spawnMobile(self.planet, "farmer", 60, 3.98, -0.05, -2.04, 180, 3005428)
	spawnMobile(self.planet, "contractor", 60, 3.19, -5.5, -6.32, 180, 3005430)
	spawnMobile(self.planet, "brawler", 60, 2.09, -5.5, -6.32, 135, 3005430)
	spawnMobile(self.planet, "businessman", 60, 3.19, -5.5, -7.42, 0, 3005430)
	spawnMobile(self.planet, "artisan", 300, -5.69, -5.5, -9.21, 180, 3005430)
	spawnMobile(self.planet, "farmer_rancher", 60, -5.69, -5.5, -10.31, 0, 3005430)

		--Guild Hall (-3680,3081)
	spawnMobile(self.planet, "trainer_merchant", 0, 12, 1.13, 6, 180, 2955398)
	spawnMobile(self.planet, "trainer_armorsmith", 0, -12, 1.1, 5, 180, 2955399)
	spawnMobile(self.planet, "trainer_architect", 0, 11, 1.13, -14, 0, 2955400)
	spawnMobile(self.planet, "trainer_weaponsmith", 0, -2.5, 1.13, -8.4, 91, 2955401)
	spawnMobile(self.planet, "trainer_droidengineer", 0, -11, 1.13, -14, 0, 2955402)

		--Medical Center
	spawnMobile(self.planet, "trainer_doctor", 0, 12, 0.18, -1, 0, 3375392)
	spawnMobile(self.planet, "trainer_medic", 0, -12.65, 0.18, 2.17, 111, 3375392)
	spawnMobile(self.planet, "commoner", 60, -0.84, 0.18, 1.59, 270, 3375392)
	spawnMobile(self.planet, "medic", 60, -2.84, 0.18, 1.59, 90, 3375392)
	spawnMobile(self.planet, "twilek_slave", 300, 0.28, 0.18, 7.39, 0, 3375392)
	spawnMobile(self.planet, "chiss_female", 60, 0.28, 0.18, 8.39, 180, 3375392)
	spawnMobile(self.planet, "commoner_old", 400, -11.7, 0.18, -13.7, 90, 3375393)
	spawnMobile(self.planet, "ithorian_male", 400, -9.9, 0.18, -13.1, 168, 3375393)

end
