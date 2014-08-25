CorelliaTyrenaScreenPlay = ScreenPlay:new {
	numberOfActs = 1,

	screenplayName = "CorelliaTyrenaScreenPlay"
}

registerScreenPlay("CorelliaTyrenaScreenPlay", true)

function CorelliaTyrenaScreenPlay:start()
	if (isZoneEnabled("corellia")) then
		self:spawnMobiles()
	end
end

function CorelliaTyrenaScreenPlay:spawnMobiles()

	--Anything dashed out will need to be added here and removed from spawn manager once the spawnMobiles command is extended.

	--Starport
	--{"bodyguard",300,-62.089,2.63942,41.8477,179.999,1935695, "conversation", ""},
	--{"chassis_broker",60,-56.6393,0.974563,8.33815,26.9298,1935697, "neutral", ""},
	--{"corsec_agent",300,57.1873,-0.521137,36.2187,180.007,1935690, "conversation", ""},
	--{"corsec_trooper",300,57.1873,-0.521137,35.1187,360.011,1935690, "conversation", ""},
	--{"entertainer",60,-63.189,2.63942,41.8477,134.996,1935695, "conversation", ""},
	--{"farmer",60,-62.089,2.63942,40.7477,0,1935695, "conversation", ""},
	--{"bodyguard",300,31.7104,0.639212,63.2998,354.769,1935688, "conversation", ""},
	--{"corellia_times_investigator",60,31.6347,0.63915,64.1249,174.754,1935688, "conversation", ""},
	--{"corsec_detective",300,-1.85381,0.639422,56.3013,180.012,1935687, "conversation", ""},
	--{"corsec_master_sergeant",300,-1.85381,0.639422,55.3013,360.011,1935687, "conversation", ""},
	--{"commoner",60,15.2091,0.639422,56.8933,284.328,1935687, "calm", "Amarent Loren"},

	--Med Center
	--{"brawler",60,24.4668,0.26,4.56238,180.004,1935831, "conversation", ""},
	--{"entertainer",60,24.4668,0.26,3.56238,0,1935831, "conversation", ""},
	spawnMobile("corellia", "trainer_doctor", 0,21,0.26,-5,0,1935831)
	spawnMobile("corellia", "trainer_medic", 0,13.8906,0.26,3.8275,183,1935831)
	spawnMobile("corellia", "surgical_droid_21b", 60,-25.52,0.26,-3.48,5.78928,1935835)
	spawnMobile("corellia", "trainer_combatmedic", 0,-17,0.26,10,180,1935835)
	spawnMobile("corellia", "trainer_doctor", 0,-24.2179,0.26,-3.63691,195,1935835)
	spawnMobile("corellia", "trainer_medic", 0,-15.9755,0.26,0.30427,15,1935835)
	--2nd floor
	--{"corsec_trooper",300,-5.68393,7.26,0.077741,180.455,1935837, "conversation", ""},
	--{"mercenary",60,-5.6917,7.26,-0.915899,0.445885,1935837, "conversation", ""},
	--3rd floor
	--{"corsec_sergeant",300,29.3506,13.25,10.8359,180.011,1935841, "conversation", ""},
	--{"farmer",60,29.3506,13.25,9.83587,360.011,1935841, "conversation", ""},

	--Hotel
	--{"herald_biogenic_03",60,21.3645,1.28309,9.48413,142.744,1935823, "calm", "Teraeza Furloti"},
	--{"corsec_chief",300,15.7701,1.28309,-8.57401,180.007,1935823, "conversation", ""},
	--{"bothan_diplomat",60,15.7701,1.28309,-9.57401,360.011,1935823, "conversation", ""},
	--{"entertainer",60,-6.74614,1.6,-12.8336,0,1935826, "conversation", ""},
	--{"commoner",60,-12.4312,1.6,-16.4443,270.002,1935827, "conversation", ""},
	--{"commoner",60,-14.4312,1.6,-16.4443,89.9986,1935827, "conversation", ""},
	--{"r2",60,-13.4312,1.6,-15.4443,7.68027,1935827, "calm", "R2-P9"},
	--{"bodyguard",300,-23.3263,1.59523,3.86229,0.250236,1935824, "conversation", ""},
	--{"slicer",60,-23.3263,1.6,4.96228,180.004,1935824, "conversation", ""},
	--{"commoner",60,-24,1.6,-4,337.557,1935828, "calm", "Pex (an elite storyteller vendor)"},

	--Cantina
	--{"stormtrooper",400,48.13,0.105,2.47,248.001,2625353, "neutral", ""},
	--{"crev_bombaasa",60,45.2285,0.104999,0.017077,135.448,2625353, "calm", "Crev Bombaasa"},
	--{"bartender",60,8.6,-0.9,0.6,90,2625355, "neutral", ""},
	spawnMobile("corellia", "trainer_dancer", 0,16.7961,-0.894993,-10.1031,3,2625355)
	spawnMobile("corellia", "trainer_musician", 0,21.1399,-0.894993,8.20648,120,2625355)
	--{"acun_solari",60,19.8219,-0.894996,-19.5533,0,2625356, "calm", "Acun Solari"},
	spawnMobile("corellia", "trainer_entertainer", 0,6.15345,-0.894992,-19.3905,0,2625358)
	--{"artisan",60,-8.04223,-0.894908,-17.7225,360.011,2625360, "conversation", ""},
	--{"corsec_agent",300,-8.04223,-0.894941,-16.6225,180.002,2625360, "conversation", ""},
	--{"shalera_the_hutt",60,-19.2041,-0.894989,22.0094,239.061,2625364, "calm", "Shalera the Hutt"},
	--{"vrak",60,-26.3961,-0.894991,23.1611,112,2625364, "nervous", ""},
	--{"commoner",60,-26.4983,-0.519991,10.8543,179.992,2625365, "conversation", ""},
	--{"corsec_commissioner",300,-26.4983,-0.519991,9.85428,360.011,2625365, "conversation", ""},
	spawnMobile("corellia", "rebel_recruiter", 60,-27.6,-0.9,0.6,89,2625363)
	spawnMobile("corellia", "corellia_times_investigator", 60,-32.0578,0.10501,-20.4764,246.868,2625366)
	spawnMobile("corellia", "mercenary", 60,-32.0578,0.10501,-21.5764,262.741,2625366)


	--2nd part of the city.

	--Guild Hall -5417 -2723
	spawnMobile("corellia", "miner", 60,3.29,1.13306,-9.58,249.007,1935454)
	spawnMobile("corellia", "trainer_artisan", 0,0,1.13306,-14,0,1935454)
	--spawnMobile("corellia", "coa3_information_rebel", 60,14.1936,1.13306,-8.97227,335.228,1935453)

	--Guild Hall -5463 -2722
	--spawnMobile("corellia", "coa3_tactical_rebel", 60,9.53575,1.13306,0.330446,310.824,1935441)
	spawnMobile("corellia", "trainer_marksman", 0,0,1.13306,-14,0,1935444)
	spawnMobile("corellia", "businessman", 60,3.32,1.13306,-8.49,228.007,1935444)
	spawnMobile("corellia", "hunter", 300,-14.01,1.13306,-8.53,120.004,1935445)
	spawnMobile("corellia", "trainer_brawler", 0,-11,1.13306,-14,0,1935445)
	spawnMobile("corellia", "trainer_scout", 0,-12,1.13306,5.5,180,1935442)

	--Guild Hall -5502 -2678
	spawnMobile("corellia", "trainer_merchant", 0,12,1.13306,6,180,1935461)
	spawnMobile("corellia", "trainer_architect", 0,11,1.13306,-14,0,1935463)
	spawnMobile("corellia", "trainer_weaponsmith", 0,-2.5,1.13306,-8.4,91,1935464)
	spawnMobile("corellia", "trainer_droidengineer", 0,-11,1.13306,-14,0,1935465)
	spawnMobile("corellia", "trainer_armorsmith", 0,-12,1.1,5,180,1935462)

	--Guild Hall -5502 -2799
	--{"entertainer",60,-3.65637,1.1544,-8.77307,0,1935476, "conversation", ""},
	--{"farmer",60,10.1683,1.75,-5.25457,135.008,1935476, "conversation", ""},
	--{"farmer_rancher",60,11.2683,1.75,-5.25457,180.017,1935476, "conversation", ""},
	--{"medic",60,11.2683,1.75,-6.35457,360.011,1935476, "conversation", ""},
	--{"mercenary",60,-3.65637,1.1544,-7.67307,179.999,1935476, "conversation", ""},
	--{"noble",60,5.78383,1.1544,-1.95638,180.012,1935476, "conversation", ""},
	--{"slicer",60,5.78383,1.1544,-3.05638,360.011,1935476, "conversation", ""},
	--{"bodyguard",300,1.29504,1.75,-19.8672,180.011,1935477, "conversation", ""},
	--{"miner",60,1.29504,1.75,-20.9672,360.011,1935477, "conversation", ""},
	--{"commoner",60,-15.1386,2.25,17.5614,270.01,1935472, "conversation", ""},
	--{"commoner",60,-17.1386,2.25,17.5614,90.0005,1935472, "conversation", ""},
	--{"corsec_deserter",300,-19.1229,9,14.3195,135.007,1935472, "conversation", ""},
	--{"corsec_detective",300,-18.0229,9,14.3195,180.008,1935472, "conversation", ""},
	--{"r3",60,-16.1386,2.25,18.5614,173.343,1935472, "calm", "R3-X8"},
	--{"slicer",60,-18.0229,9,13.2195,360.011,1935472, "conversation", ""},

	--Hotel
	--{"businessman",60,17.3056,1.28309,-7.47174,0,1935527, "conversation", ""},
	--{"businessman",60,23.1824,1.28309,8.37019,0,1935527, "conversation", ""},
	--{"corsec_investigator",300,17.3056,1.28309,-6.37174,180.003,1935527, "conversation", ""},
	--{"pilot",60,23.1824,1.28309,9.37019,179.994,1935527, "conversation", ""},
	--{"commoner",60,-16.9721,1,-4.12621,360.011,1935528, "conversation", ""},
	--{"rogue_corsec_trooper",300,-16.9721,1.00014,-3.02621,180.01,1935528, "conversation", ""},
	--{"commoner",60,-24,1.6,-4,354.16,1935532, "calm", "Pex (an elite storyteller vendor)"},

	--Theater/Guild Hall
	--{"artisan",60,2.65839,2.6,4.15034,180.003,2365434, "conversation", ""},
	--{"bothan_information_broker",60,-14.2562,2.4869,23.6983,5.24379,2365434, "conversation", ""},
	--{"explorer",60,1.55839,2.6,4.15034,135.004,2365434, "conversation", ""},
	--{"farmer",60,-22.8235,0.609014,44.1007,180.008,2365434, "conversation", ""},
	--{"farmer_rancher",60,-14.2562,2.3868,24.7891,180.003,2365434, "conversation", ""},
	--{"info_broker",60,25.7618,0.784388,42.249,5.24275,2365434, "conversation", ""},
	spawnMobile("corellia", "noble", 60,28.93,2.12878,58.19,222.007,2365435)
	spawnMobile("corellia", "noble", 60,19.26,2.12847,56.13,266.008,2365436)
	--{"corsec_major",300,5.33572,2.12878,71.6096,0,2365436, "conversation", ""},
	--{"gambler",60,5.33572,2.12878,72.7096,180.003,2365436, "conversation", ""},
	spawnMobile("corellia", "trainer_dancer", 0,17.6008,2.12878,53.4279,4,2365436)
	spawnMobile("corellia", "trainer_musician", 0,21.9985,2.12818,63.5,0,2365436)
	spawnMobile("corellia", "trainer_imagedesigner", 0,-21.8004,2.12878,74.6487,184,2365437)

	--Outside
	--{"adar",60,-5448.03,21,-2674.92,253.163,0, "calm", "Adar Tallon"},
	--{"artisan",60,-5537.5,21,-2614.06,180.005,0, "conversation", ""},
	--{"artisan",60,-5596.05,21,-2761.83,180.005,0, "conversation", ""},
	--{"bodyguard",300,-5537.5,21,-2615.06,0,0, "conversation", ""},
	--{"bothan_diplomat",60,-5146.7,21,-2548.18,0,0, "conversation", ""},
	--{"bothan_diplomat",60,-5009.39,28.1,-2497.19,0,0, "conversation", ""},
	--{"bothan_information_broker",60,-5398.52,21,-2676.03,0,0, "conversation", ""},
	--{"brawler",60,-5000.34,28.1,-2521.71,45.0013,0, "conversation", ""},
	spawnMobile("corellia", "businessman", 60,-5091.15,21,-2315.33,227.834,0)
	spawnMobile("corellia", "cll_8", 60,-5050.13,21,-2288.73,145.718,0)
	--{"coa2_rebel_coordinator",60,-5218.4,21,-2602.17,331.695,0, "calm", ""},
	--{"comm_operator",300,-5467.56,21,-2792.59,0,0, "conversation", ""},
	spawnMobile("corellia", "commoner", 60,-5633.73,21,-2757.46,93.1038,0)
	spawnMobile("corellia", "commoner", 60,-5583.79,21,-2764.8,215.273,0)
	spawnMobile("corellia", "commoner", 60,-5538.94,21,-2600.55,4.08279,0)
	spawnMobile("corellia", "commoner", 60,-5500.8,21,-2778.27,175.737,0)
	spawnMobile("corellia", "commoner", 60,-5409.42,21,-2700.49,244.27,0)
	spawnMobile("corellia", "commoner", 60,-5411.79,21,-2655.41,7.76214,0)
	spawnMobile("corellia", "commoner", 60,-5465.78,21,-2601.9,287.819,0)
	spawnMobile("corellia", "commoner", 60,-5314.83,21,-2461.07,267.814,0)
	spawnMobile("corellia", "commoner", 60,-5291.55,21,-2479.74,323.878,0)
	spawnMobile("corellia", "commoner", 60,-5274.74,21,-2574.04,268.911,0)
	spawnMobile("corellia", "commoner", 60,-5259.48,21,-2446.51,34.7297,0)
	spawnMobile("corellia", "commoner", 60,-5278.98,21,-2390.6,94.9464,0)
	spawnMobile("corellia", "commoner", 60,-5208.22,21,-2333.68,118.094,0)
	spawnMobile("corellia", "commoner", 60,-5247.09,21,-2623.54,285.684,0)
	spawnMobile("corellia", "commoner", 60,-5242.79,21,-2421.66,30.7465,0)
	spawnMobile("corellia", "commoner", 60,-5110.54,21,-2505.6,104.325,0)
	spawnMobile("corellia", "commoner", 60,-5200.63,21,-2505.59,277.113,0)
	spawnMobile("corellia", "commoner", 60,-5200.12,21,-2594.79,332.894,0)
	spawnMobile("corellia", "commoner", 60,-5162.69,21,-2277.81,212.16,0)
	spawnMobile("corellia", "commoner", 60,-5114.89,21,-2424.44,187.746,0)
	spawnMobile("corellia", "commoner", 60,-5200.74,21,-2638.47,308.492,0)
	spawnMobile("corellia", "commoner", 60,-5164.3,21,-2639.01,192.357,0)
	--{"commoner",60,-5192.9,21,-2545.55,46.5164,0, "calm", "Excited Journalist"},
	spawnMobile("corellia", "commoner", 60,-5144.16,21,-2413.14,25.4291,0)
	spawnMobile("corellia", "commoner", 60,-5156.26,21,-2376.59,186.561,0)
	spawnMobile("corellia", "commoner", 60,-5003.67,21,-2342.15,284.215,0)
	spawnMobile("corellia", "commoner", 60,-5090.54,21,-2580.35,208.738,0)
	spawnMobile("corellia", "commoner", 60,-5079.26,21,-2445.52,106.736,0)
	spawnMobile("corellia", "commoner", 60,-5064.44,21,-2392.07,128.202,0)
	spawnMobile("corellia", "commoner", 60,-5089.39,21,-2344.25,247.178,0)
	spawnMobile("corellia", "commoner", 60,-5073.15,21,-2521.26,351.018,0)
	spawnMobile("corellia", "commoner", 60,-5048.34,21,-2490.91,295.017,0)
	spawnMobile("corellia", "commoner", 60,-5014.32,21,-2560.74,19.5735,0)
	spawnMobile("corellia", "commoner", 60,-5025.37,21,-2453.92,175.818,0)
	spawnMobile("corellia", "commoner", 60,-4942.72,21,-2445.55,99.4923,0)
	spawnMobile("corellia", "commoner", 60,-4960.23,21,-2357.14,295.225,0)
	spawnMobile("corellia", "commoner", 60,-4965.54,21,-2383.55,227.39,0)
	--{"commoner",60,-5146.7,21,-2547.18,180.005,0, "conversation", ""},
	--{"corellia_times_investigator",60,-5568.35,21,-2664.99,0,0, "conversation", ""},
	--{"corellia_times_investigator",60,-5491.88,30.0036,-2809.63,0,0, "conversation", ""},
	spawnMobile("corellia", "corsec_agent", 60,-5412.5,20.8978,-2794.47,215.747,0)
	spawnMobile("corellia", "corsec_cadet", 60,-4945.07,21,-2489.35,89.6921,0)
	--{"corsec_chief",300,-5000.34,28.1,-2520.6,135.004,0, "conversation", ""},
	--{"corsec_deserter",300,-5467.56,21,-2791.59,180.005,0, "conversation", ""},
	--{"corsec_detective",300,-4999.24,28.1,-2520.6,180.005,0, "conversation", ""},
	--{"corsec_inspector_sergeant",300,-5061.36,21,-2532.53,0,0, "conversation", ""},
	--{"corsec_investigator",300,-5491.88,30.0036,-2808.63,180.005,0, "conversation", ""},
	--{"corsec_investigator",300,-5159.41,21,-2569.03,0,0, "conversation", ""},
	--{"corsec_major",300,-5010.49,28.1,-2496.09,135.004,0, "conversation", ""},
	--{"rogue_corsec_trooper",300,-5059.06,21,-2399.08,180.005,0, "conversation", ""},
	spawnMobile("corellia", "corsec_sergeant", 300,-5059.87,21,-2606.63,195.299,0)
	spawnMobile("corellia", "corsec_trooper", 300,-5506.2,21,-2634.5,102,0)
	spawnMobile("corellia", "corsec_trooper", 300,-5626.49,21,-2812.58,261.279,0)
	spawnMobile("corellia", "corsec_trooper", 300,-5470.84,21,-2565.16,107.104,0)
	spawnMobile("corellia", "corsec_trooper", 300,-5470.71,21,-2566.52,107.123,0)
	--{"corsec_trooper",300,-5061.36,21,-2531.53,180.005,0, "conversation", ""},
	spawnMobile("corellia", "corsec_trooper", 300,-5060.41,21,-2604.37,195.315,0)
	spawnMobile("corellia", "corsec_trooper", 300,-4944.48,21,-2488.58,42.466,0)
	spawnMobile("corellia", "corsec_trooper", 300,-5363.23,24.87,-2595.04,64.2702,0)
	spawnMobile("corellia", "corsec_trooper", 300,-5069.93,21,-2474.8,135.004,0)
	spawnMobile("corellia", "corsec_trooper", 300,-5401.3,21,-2613.1,58,0)
	spawnMobile("corellia", "corsec_trooper", 300,-5058.83,21,-2579.08,135.145,0)

	--Imperials Npcs outside
	spawnMobile("corellia", "command_security_guard", 300,-5281.9,21,-2574.6,80,0)
	spawnMobile("corellia", "command_security_guard", 300,-5112.1,21,-2279.3,79.0023,0)
	spawnMobile("corellia", "command_security_guard", 300,-5048.7,21,-2299.5,256.201,0)
	spawnMobile("corellia", "dark_trooper", 400,-4941.2,20.8719,-2522,214.459,0)
	spawnMobile("corellia", "dark_trooper", 400,-5381.8,20.7451,-2741.1,130.004,0)
	spawnMobile("corellia", "dark_trooper", 400,-5565.3,20.6891,-2852.06,298.056,0)
	spawnMobile("corellia", "dark_trooper", 400,-4975.9,20.9989,-2323.7,76.0022,0)
	--{"dark_trooper",400,-5347.92,21.0054,-2440.31,351.468,0, "npc_imperial", ""},
	spawnMobile("corellia", "dark_trooper", 400,-5654.8,21.1047,-2805.7,191.006,0)
	--{"elite_sand_trooper",400,-5613.95,21,-2585.97,41.9991,0, "npc_imperial", ""},
	spawnMobile("corellia", "elite_sand_trooper", 400,-5606,21.6,-2777.2,82,0)
	spawnMobile("corellia", "imperial_army_captain", 400,-5298.7,21,-2545.6,32,0)
	--{"imperial_corporal",400,-5590.48,20.3776,-2854.51,311.192,0, "npc_imperial", ""},
	--{"imperial_noncom",400,-5207.41,21.425,-2247.49,237.999,0, "npc_imperial", ""},
	spawnMobile("corellia", "scout_trooper", 400,-5128.84,21,-2652.86,83.2245,0)
	spawnMobile("corellia", "scout_trooper", 400,-5208.06,21.6077,-2246.46,296.757,0)
	spawnMobile("corellia", "scout_trooper", 400,-5194.27,20.9887,-2653.85,274.172,0)
	spawnMobile("corellia", "scout_trooper", 400,-5468.4,21.1828,-2841.6,175.005,0)
	--{"specialist_noncom",400,-5342.53,21,-2468.78,360.011,0, "npc_imperial", ""},
	spawnMobile("corellia", "storm_commando", 400,-5055.1,21,-2616,137.004,0)
	spawnMobile("corellia", "storm_commando", 400,-5250.6,21,-2327.7,42.0012,0)
	spawnMobile("corellia", "stormtrooper", 400,-4944.3,21,-2387.8,257.008,0)
	spawnMobile("corellia", "stormtrooper", 400,-4945.7,21,-2383.1,249.007,0)
	spawnMobile("corellia", "stormtrooper", 400,-4936.1,21,-2486.2,197.971,0)
	spawnMobile("corellia", "stormtrooper", 400,-5031.2,21,-2614.9,278.725,0)
	spawnMobile("corellia", "stormtrooper", 400,-5092.3,21,-2648.2,175.005,0)
	spawnMobile("corellia", "stormtrooper", 400,-5135.07,20.9878,-2659.19,86.9316,0)
	spawnMobile("corellia", "stormtrooper", 400,-5174.6,20.9876,-2656.7,185.005,0)
	spawnMobile("corellia", "stormtrooper", 400,-5212.9,20.9913,-2650.9,194.006,0)
	spawnMobile("corellia", "stormtrooper", 400,-5343.5,21,-2471.7,271.008,0)
	spawnMobile("corellia", "stormtrooper", 400,-5338.7,21,-2427.9,309.009,0)
	spawnMobile("corellia", "stormtrooper", 400,-5195.9,21,-2253.9,336.01,0)
	spawnMobile("corellia", "stormtrooper", 400,-5270.9,21,-2360.7,133.004,0)
	spawnMobile("corellia", "stormtrooper", 400,-5281.8,21,-2349.8,314.009,0)
	spawnMobile("corellia", "stormtrooper", 400,-5398.3,21,-2761.7,146.004,0)
	spawnMobile("corellia", "stormtrooper", 400,-5565.5,20.9967,-2842.9,340.795,0)
	spawnMobile("corellia", "stormtrooper", 400,-5633.8,20.9535,-2824.4,197.006,0)
	spawnMobile("corellia", "stormtrooper", 400,-5485.9,21,-2585.8,89.0026,0)
	spawnMobile("corellia", "stormtrooper", 400,-4942.5,21,-2438.9,106.003,0)
	spawnMobile("corellia", "stormtrooper", 400,-5556.4,21.6,-2611.8,99.0029,0)
	--{"stormtrooper",400,-5112.07,21,-2248.89,97.4325,0, "npc_imperial", ""},
	spawnMobile("corellia", "stormtrooper", 400,-5044.7,21,-2303,200.405,0)
	spawnMobile("corellia", "stormtrooper", 400,-5131.1,21,-2417.7,6.00023,0)
	spawnMobile("corellia", "stormtrooper", 400,-5301.8,21,-2572,244.007,0)
	spawnMobile("corellia", "stormtrooper", 400,-5306.8,21,-2561.2,247.007,0)
	spawnMobile("corellia", "stormtrooper", 400,-5399.6,21,-2608.8,241.007,0)
	spawnMobile("corellia", "stormtrooper", 400,-5396.8,21,-2614.9,251.007,0)
	spawnMobile("corellia", "stormtrooper", 400,-5459.6,21.2742,-2841.6,185.005,0)
	--{"stormtrooper_bombardier",400,-5086.49,21,-2255.77,216.35,0, "npc_imperial", ""},
	spawnMobile("corellia", "stormtrooper_medic", 400,-5302.1,21,-2379.2,225.007,0)
	spawnMobile("corellia", "stormtrooper_rifleman", 400,-5418.2,20.9379,-2799.6,243.757,0)
	spawnMobile("corellia", "stormtrooper_rifleman", 400,-5556.4,21.6,-2643.5,79.0023,0)
	--{"stormtrooper_rifleman",400,-5097.16,21,-2271.05,130.264,0, "npc_imperial", ""},
	spawnMobile("corellia", "stormtrooper_rifleman", 400,-5123,21,-2425.3,67.002,0)
	spawnMobile("corellia", "stormtrooper_rifleman", 400,-5245.9,20.9324,-2636.5,228.007,0)
	spawnMobile("corellia", "stormtrooper_sniper", 400,-5227.3,21,-2272.3,331.01,0)
	spawnMobile("corellia", "stormtrooper_sniper", 400,-5574.1,20.98,-2843.3,314.848,0)
	spawnMobile("corellia", "stormtrooper_sniper", 400,-4932.5,20.978,-2427.7,123.004,0)
	--{"stormtrooper_sniper",400,-5094.14,21,-2270.16,45.8873,0, "npc_imperial", ""},
	--{"stormtrooper_squad_leader",400,-5613.85,21,-2584,33.2059,0, "npc_imperial", ""},
	--{"stormtrooper_squad_leader",400,-5103.81,21,-2266.4,126.218,0, "npc_imperial", ""},

	--Misc Outside
	spawnMobile("corellia", "eg6_power_droid", 60,-5605.25,21,-2721.9,155.92,0)
	spawnMobile("corellia", "eg6_power_droid", 60,-5295.6,21,-2396.69,237.183,0)
	--{"entertainer",60,-5539.77,21,-2683.65,0,0, "conversation", ""},
	--{"entertainer",60,-5539.77,21,-2682.65,180.005,0, "conversation", ""},
	--{"entertainer",60,-5196.06,21,-2408.57,0,0, "conversation", ""},
	--{"entertainer",60,-5009.39,28.1,-2496.09,180.005,0, "conversation", ""},
	--{"explorer",60,-5568.35,21,-2663.99,180.005,0, "conversation", ""},
	--{"explorer",60,-5596.05,21,-2762.83,0,0, "conversation", ""},
	--{"explorer",60,-5308.12,21,-2493.27,0,0, "conversation", ""},
	--{"farmer",60,-5574.26,21,-2683.38,180.005,0, "conversation", ""},
	--{"farmer",60,-5159.41,21,-2568.03,180.005,0, "conversation", ""},
	--{"farmer",60,-5196.06,21,-2407.57,180.005,0, "conversation", ""},
	--{"farmer",60,-4999.24,28.1,-2521.71,0,0, "conversation", ""},
	spawnMobile("corellia", "gubbur", 300,-4826,32.1,-2229.2,18,0)
	spawnMobile("corellia", "gubbur", 300,-4827.25,31.6625,-2225.7,157.208,0)
	--{"info_broker",60,-5574.26,21,-2684.38,0,0, "conversation", ""},
	--{"info_broker",60,-5095.02,21,-2513.56,180.005,0, "conversation", ""},
	spawnMobile("corellia", "informant_npc_lvl_1", 0,-5024,21,-2345,180,0)
	spawnMobile("corellia", "informant_npc_lvl_1", 0,-5136,21,-2294,270,0)
	spawnMobile("corellia", "informant_npc_lvl_1", 0,-5587,21,-2809,315,0)
	spawnMobile("corellia", "informant_npc_lvl_1", 0,-5480,21,-2777,225,0)
	spawnMobile("corellia", "informant_npc_lvl_1", 0,-5504,21,-2699,180,0)
	spawnMobile("corellia", "informant_npc_lvl_1", 0,-5539,21,-2667,270,0)
	spawnMobile("corellia", "informant_npc_lvl_1", 0,-5592,21,-2684,90,0)
	spawnMobile("corellia", "informant_npc_lvl_1", 0,-5450,21,-2630,45,0)
	spawnMobile("corellia", "informant_npc_lvl_1", 0,-5308,21,-2512,90,0)
	spawnMobile("corellia", "informant_npc_lvl_1", 0,-5097,21,-2344,270,0)
	--{"j_pai_brek",60,-5073.17,21,-2342.77,33.2325,0, "neutral", ""},
	--{"jom_irimore",60,-5520,21.3,-2630.99,0,0, "calm", "Jom Irimore"},
	--{"junk_dealer",0,-5639.2,21,-2799.2,-40,0, "", "",JUNKCLOTHESANDJEWELLERY+JUNKARMOUR,JUNKCONVFINARY},
	--{"junk_dealer",0,-5203.6,21,-2285.9,-11,0, "", "",JUNKGENERIC,JUNKCONVGENERIC},
	--{"larek_tatham",60,-5050.23,21,-2291.69,113.059,0, "neutral", ""},
	--{"medic",60,-5095.02,21,-2514.56,0,0, "conversation", ""},
	--{"mercenary",60,-5308.12,21,-2492.27,180.005,0, "conversation", ""},
	--{"mercenary",60,-5398.52,21,-2675.03,180.005,0, "conversation", ""},
	--{"mercenary",60,-5059.06,21,-2400.08,0,0, "conversation", ""},
	spawnMobile("corellia", "noble", 60,-5441.11,21.0895,-2624.13,269.845,0)
	--{"r2",60,-5295.88,21,-2528.24,196.589,0, "calm", "R2-B0"},
	--{"r2",60,-5297.6,21,-2396.69,271.4,0, "calm", "R2-X2"},
	--{"r2",60,-4954.63,21,-2376.82,26.4361,0, "calm", "R2-E3"},
	--{"r3",60,-5312.13,25.7208,-2570.41,64.2735,0, "calm", "R3-X5"},
	--{"r5",60,-5400.32,21,-2614.17,236.816,0, "calm", "R5-S7"},
	--{"r5",60,-4954.81,21,-2379.55,166.051,0, "calm", "R5-C7"},

	--Ragtag's
	spawnMobile("corellia", "ragtag_kook", 300,-5215.15,29.033,-2198.86,133.12,0)
	spawnMobile("corellia", "ragtag_kook", 300,-5220.6,30.4428,-2186.49,348.738,0)
	spawnMobile("corellia", "ragtag_kook", 300,-5211.84,28.4788,-2194.09,10.766,0)
	spawnMobile("corellia", "ragtag_kook", 300,-5219.46,30.4446,-2185.86,7.83436,0)
	spawnMobile("corellia", "ragtag_kook", 300,-4872.65,11.1341,-2390.99,302.831,0)
	spawnMobile("corellia", "ragtag_kook", 300,-4868.58,11.257,-2387.94,83.5642,0)
	spawnMobile("corellia", "ragtag_kook", 300,-5735.79,28.6208,-2610.4,123.649,0)
	spawnMobile("corellia", "ragtag_kook", 300,-5737.79,28.5516,-2607.4,132.784,0)
	spawnMobile("corellia", "ragtag_kook", 300,-5734.79,28.7267,-2609.4,139.039,0)
	spawnMobile("corellia", "ragtag_kook", 300,-5752.38,16.7564,-2798.05,339.808,0)
	spawnMobile("corellia", "ragtag_kook", 300,-5738.79,28.2523,-2614.4,291.78,0)
	spawnMobile("corellia", "ragtag_kook", 300,-5759.38,16.8412,-2798.05,36.2043,0)
	spawnMobile("corellia", "ragtag_kook", 300,-5755.38,16.8078,-2791.05,5.21744,0)
	spawnMobile("corellia", "ragtag_kook", 300,-5757.38,16.8277,-2792.05,209.008,0)
	spawnMobile("corellia", "ragtag_kook", 300,-5750.38,16.684,-2794.05,244.293,0)
	spawnMobile("corellia", "ragtag_loon", 300,-4855.73,11.1455,-2525.71,169.83,0)
	spawnMobile("corellia", "ragtag_loon", 300,-4855.73,11.1789,-2528.71,285.482,0)
	spawnMobile("corellia", "ragtag_loon", 300,-4855.73,11.1489,-2521.71,326.785,0)
	spawnMobile("corellia", "ragtag_loon", 300,-4865.73,10.5102,-2521.71,326.643,0)
	spawnMobile("corellia", "ragtag_mercenary", 300,-5691.26,15.4606,-2887.12,356.964,0)
	spawnMobile("corellia", "ragtag_mercenary", 300,-5672.95,15.312,-2893.61,357.718,0)
	spawnMobile("corellia", "ragtag_mercenary", 300,-5694.23,15.4638,-2884.47,185.304,0)
	spawnMobile("corellia", "ragtag_thug", 300,-5683.98,16.7263,-2843.44,81.9355,0)
	spawnMobile("corellia", "ragtag_thug", 300,-5685.49,17.6827,-2825.55,256.078,0)
	spawnMobile("corellia", "ragtag_thug", 300,-5683.2,16.9368,-2840.36,90.595,0)

	--More misc
	spawnMobile("corellia", "scientist", 60,-5431.41,21,-2685.81,106.943,0)
	--{"slicer",60,-5564.23,21,-2777.78,0,0, "conversation", ""},
	spawnMobile("corellia", "swooper_leader", 300,-5416.8,4.57113,-2286.37,76.0594,0)
	spawnMobile("corellia", "swooper_leader", 300,-5415.8,5.33258,-2277.08,224.956,0)
	spawnMobile("corellia", "swooper_leader", 300,-5420.58,3.58008,-2285.59,200.562,0)

	--Trainers
	spawnMobile("corellia", "trainer_armorsmith", 0,-5118,21,-2337,131,0)
	spawnMobile("corellia", "trainer_artisan", 0,-5081,21,-2307,134,0)
	spawnMobile("corellia", "trainer_artisan", 0,-5300.07,21,-2383,184,0)
	spawnMobile("corellia", "trainer_bountyhunter", 0,-5130,21,-2302,180,0)
	spawnMobile("corellia", "trainer_carbine", 0,-5225,21,-2488,180,0)
	spawnMobile("corellia", "trainer_combatmedic", 0,-4996.29,21,-2451.61,129,0)
	spawnMobile("corellia", "trainer_dancer", 0,34.5107,0.105,1.79681,89,3005398)
	spawnMobile("corellia", "trainer_doctor", 0,-5002,21,-2459,180,0)
	spawnMobile("corellia", "trainer_entertainer", 0,-5661,21.6,-2599.57,14,0)
	spawnMobile("corellia", "trainer_entertainer", 0,-5062,21,-2291,124,0)
	spawnMobile("corellia", "trainer_marksman", 0,6770,21,-5714,180,0)
	spawnMobile("corellia", "trainer_marksman", 0,-5041,21,-2324,91,0)
	spawnMobile("corellia", "trainer_marksman", 0,-5249,21,-2306,180,0)
	spawnMobile("corellia", "trainer_medic", 0,-5027,21,-2311,266,0)
	spawnMobile("corellia", "trainer_musician", 0,-5067,21,-2296,158,0)
	spawnMobile("corellia", "trainer_pistol", 0,-5220,21,-2488,180,0)
	spawnMobile("corellia", "trainer_rifleman", 0,-5234,21,-2488,176,0)
	spawnMobile("corellia", "trainer_scout", 0,-5061.86,21,-2322.52,36,0)
	spawnMobile("corellia", "trainer_weaponsmith", 0,-5106,21,-2324,180,0)

end
