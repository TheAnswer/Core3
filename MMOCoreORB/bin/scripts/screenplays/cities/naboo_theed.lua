NabooTheedScreenPlay = ScreenPlay:new {
	numberOfActs = 1,

	screenplayName = "NabooTheedScreenPlay"
}

registerScreenPlay("NabooTheedScreenPlay", true)

function NabooTheedScreenPlay:start()
	if (isZoneEnabled("naboo")) then
		self:spawnMobiles()
	end
end

function NabooTheedScreenPlay:spawnMobiles()

	--Anything dashed out will need to be added here and removed from spawn manager once the spawnMobiles command is extended.

	--Outside
	--{"agriculturalist",60,-5827.81,6,4173.98,180.005,0, "conversation", ""},
	--{"bodyguard",300,-5258.93,6,4187.17,180.005,0, "conversation", ""},
	spawnMobile("naboo", "bounty_hunter",300,-5082.41,6,4261.15,180.005,0)
	--{"coa2_imperial_coordinator",0,-5431.8,6,4168.58,145.531,0, "npc_imperial", ""},
	spawnMobile("naboo", "commoner",60,-5394.06,6,4519.32,169.904,0)
	spawnMobile("naboo", "commoner",60,-5384.95,6,4283.38,71.1905,0)
	spawnMobile("naboo", "commoner",60,-5256.72,6,4254.43,243.769,0)
	spawnMobile("naboo", "commoner",60,-5319.27,6,4365.19,286.546,0)
	spawnMobile("naboo", "commoner",60,-5320.16,6,4236.39,175.933,0)
	spawnMobile("naboo", "commoner",60,-5327.58,6,4267.42,170.655,0)
	spawnMobile("naboo", "commoner",60,-5281.75,6,4325.98,47.2066,0)
	spawnMobile("naboo", "commoner",60,-5140.78,6,4404.92,212.214,0)
	spawnMobile("naboo", "commoner",60,-6036.18,6,4286.85,147.238,0)
	spawnMobile("naboo", "commoner",60,-6055.49,6,4319.64,186.501,0)
	spawnMobile("naboo", "commoner",60,-6040.72,6,4260.46,325.963,0)
	spawnMobile("naboo", "commoner",60,-6012.09,6,4241.61,83.2023,0)
	spawnMobile("naboo", "commoner",60,-5291.01,6,4428.53,322.605,0)
	spawnMobile("naboo", "commoner",60,-5282.92,6,4405.65,279.205,0)
	spawnMobile("naboo", "commoner",60,-5280.52,6,4372.79,226.645,0)
	spawnMobile("naboo", "commoner",60,-5306.24,6,4357.56,77.2615,0)
	spawnMobile("naboo", "commoner",60,-5326.21,6,4292.88,155.604,0)
	spawnMobile("naboo", "commoner",60,-5309.23,6,4307.01,78.4133,0)
	spawnMobile("naboo", "commoner",60,-5371.2,6,4337.4,57.5394,0)
	spawnMobile("naboo", "commoner",60,-5392.94,6,4361.06,74.0902,0)
	spawnMobile("naboo", "commoner",60,-5370.57,6,4384.5,150.505,0)
	spawnMobile("naboo", "commoner",60,-5399.67,6,4424.78,313.827,0)
	spawnMobile("naboo", "commoner",60,-5380.45,6,4628.71,65.0141,0)
	spawnMobile("naboo", "commoner",60,-5396.24,6,4492.81,306.733,0)
	spawnMobile("naboo", "commoner",60,-6023.52,6,4213.29,308.957,0)
	spawnMobile("naboo", "commoner",60,-5219.06,6,4291.75,225.099,0)
	spawnMobile("naboo", "commoner",60,-5194.95,6,4189.82,3.74494,0)
	spawnMobile("naboo", "commoner",60,-5135.69,6,4219.76,3.98419,0)
	spawnMobile("naboo", "commoner",60,-5124.32,6,4230.7,188.301,0)
	spawnMobile("naboo", "commoner",60,-5165.37,6,4173.32,44.4267,0)
	spawnMobile("naboo", "commoner",60,-5171.36,6,4148.39,89.3505,0)
	spawnMobile("naboo", "commoner",60,-5123.13,6,4201.32,268.516,0)
	spawnMobile("naboo", "commoner",60,-5101.22,6,4246.47,39.2932,0)
	spawnMobile("naboo", "commoner",60,-5106.97,6,4296.89,278.927,0)
	spawnMobile("naboo", "commoner",60,-5069.74,6,4262.45,304.456,0)
	spawnMobile("naboo", "commoner",60,-5075.93,6,4204.5,258.103,0)
	spawnMobile("naboo", "commoner",60,-5053.71,6,4151.61,236.952,0)
	spawnMobile("naboo", "commoner",60,-5090.67,6,4173.27,324.898,0)
	spawnMobile("naboo", "commoner",60,-5088.5,6,4152.21,228.099,0)
	spawnMobile("naboo", "commoner",60,-5076.56,6,4162.09,47.5912,0)
	spawnMobile("naboo", "commoner",60,-5032.44,6,4131.62,179.986,0)
	spawnMobile("naboo", "commoner",60,-5019.1,6,4125.83,354.895,0)
	spawnMobile("naboo", "commoner",60,-5023.92,6,4040.07,112.362,0)
	spawnMobile("naboo", "commoner",60,-5004.8,6,4067.54,285.879,0)
	spawnMobile("naboo", "commoner",60,-5037.78,6,4078.71,14.5782,0)
	spawnMobile("naboo", "commoner",60,-5007.98,6,4111.83,156.739,0)
	spawnMobile("naboo", "commoner",60,-5021.05,6,4230.04,13.8244,0)
	spawnMobile("naboo", "commoner",60,-4866.72,6,4160.45,346.733,0)
	spawnMobile("naboo", "commoner",60,-4991.86,6,4125.22,355.866,0)
	spawnMobile("naboo", "commoner",60,-4970.37,6,4069.78,230.918,0)
	spawnMobile("naboo", "commoner",60,-4924.84,6,4034.9,112.799,0)
	spawnMobile("naboo", "commoner",60,-4892.24,6,4083,131.075,0)
	spawnMobile("naboo", "commoner",60,-4902.73,6,4106.81,350.732,0)
	spawnMobile("naboo", "commoner",60,-4933.63,6,4090.17,355.935,0)
	spawnMobile("naboo", "commoner",60,-4921.08,6,4061.48,324.756,0)
	spawnMobile("naboo", "commoner",60,-4896.15,6,4167.85,352.84,0)
	spawnMobile("naboo", "commoner",60,-4897.7,6,4193.27,70.95,0)
	spawnMobile("naboo", "commoner",60,-4978.62,6,4119.77,158.522,0)
	spawnMobile("naboo", "commoner",60,-4941.05,6,4184.26,54.3125,0)
	spawnMobile("naboo", "commoner",60,-4968.55,6,4158.78,56.9517,0)
	spawnMobile("naboo", "commoner",60,-4978.84,6,4263.64,225.699,0)
	spawnMobile("naboo", "commoner",60,-4973.93,6,4220.31,28.5203,0)
	spawnMobile("naboo", "commoner",60,-4981.69,6,4244.41,37.6517,0)
	spawnMobile("naboo", "commoner",60,-4985.97,6,4212.75,54.7999,0)
	spawnMobile("naboo", "commoner",60,-4974.13,6,4196.91,163.185,0)
	spawnMobile("naboo", "commoner",60,-4973.2,6,4175.16,163.034,0)
	spawnMobile("naboo", "commoner",60,-4956.42,6,4205.06,106.528,0)
	spawnMobile("naboo", "commoner",60,-5054.68,6,4228.23,0,0)
	spawnMobile("naboo", "commoner",60,-5803.39,6,4101.34,219.092,0)
	spawnMobile("naboo", "commoner",60,-5858.79,6,4147.3,177.298,0)
	spawnMobile("naboo", "commoner",60,-5941.62,6,4339.4,156.272,0)
	spawnMobile("naboo", "commoner",60,-5996.82,6,4309.73,327.452,0)
	spawnMobile("naboo", "commoner",60,-5968.83,6,4287.92,169.112,0)
	spawnMobile("naboo", "commoner",60,-5996.87,6,4269.75,70.6282,0)
	spawnMobile("naboo", "commoner",60,-5928.72,6,4217.24,56.7839,0)
	spawnMobile("naboo", "commoner",60,-5889.07,6,4235.87,170.679,0)
	spawnMobile("naboo", "commoner",60,-5727.93,6,4316.95,342.377,0)
	spawnMobile("naboo", "commoner",60,-5741.57,6,4304.91,133.504,0)
	spawnMobile("naboo", "commoner",60,-5757.67,6,4411.04,296.772,0)
	spawnMobile("naboo", "commoner",60,-5818.26,6,4407.8,25.7365,0)
	spawnMobile("naboo", "commoner",60,-5751.98,6,4147.86,61.2467,0)
	spawnMobile("naboo", "commoner",60,-5729.03,6,4120.76,304.577,0)
	spawnMobile("naboo", "commoner",60,-5709.29,6,4134.09,24.3295,0)
	spawnMobile("naboo", "commoner",60,-5675.02,6,4238.39,193.294,0)
	spawnMobile("naboo", "commoner",60,-5686.06,6,4307.41,304.1,0)
	spawnMobile("naboo", "commoner",60,-5663.64,6,4136.03,34.2804,0)
	spawnMobile("naboo", "commoner",60,-5690.65,6,4124.1,313.987,0)
	spawnMobile("naboo", "commoner",60,-5632.25,6,4137.72,121.318,0)
	spawnMobile("naboo", "commoner",60,-5592.68,6,4154.97,125.654,0)
	spawnMobile("naboo", "commoner",60,-5600.12,6,4053.65,55.2097,0)
	spawnMobile("naboo", "commoner",60,-5418.26,6,4347.1,173.931,0)
	spawnMobile("naboo", "commoner",60,-5409.46,6,4386.55,51.6472,0)
	spawnMobile("naboo", "commoner",60,-5425.82,6,4371.62,158.964,0)
	spawnMobile("naboo", "commoner",60,-5416.72,6,4413.87,130.124,0)
	spawnMobile("naboo", "commoner",60,-5428.98,6,4447.99,285.357,0)
	spawnMobile("naboo", "commoner",60,-5449.78,6,4428.75,8.98008,0)
	spawnMobile("naboo", "commoner",60,-5457.38,6,4469.32,299.901,0)
	spawnMobile("naboo", "commoner",60,-5468.87,6,4432.8,25.9495,0)
	spawnMobile("naboo", "commoner",60,-5458.78,6,4343.19,264.89,0)
	spawnMobile("naboo", "commoner",60,-5485.31,6,4373.11,251.552,0)
	spawnMobile("naboo", "commoner",60,-5492.21,6,4337.69,94.7447,0)
	spawnMobile("naboo", "commoner",60,-5507.67,6,4309.38,330.512,0)
	spawnMobile("naboo", "commoner",60,-5520.11,6,4349.32,243.736,0)
	spawnMobile("naboo", "commoner",60,-5489.16,6,4404.33,229.081,0)
	spawnMobile("naboo", "commoner",60,-5525.24,6,4430.28,32.9667,0)
	spawnMobile("naboo", "commoner",60,-5537.28,6,4411.34,252.902,0)
	spawnMobile("naboo", "commoner",60,-5545.73,6,4466.12,324.904,0)
	spawnMobile("naboo", "commoner",60,-5586.88,6,4495.26,212.866,0)
	spawnMobile("naboo", "commoner",60,-5565.53,6,4469.02,122.256,0)
	spawnMobile("naboo", "commoner",60,-5541.82,6,4394.46,238.528,0)
	spawnMobile("naboo", "commoner",60,-5547.21,6,4383.24,155.163,0)
	spawnMobile("naboo", "commoner",60,-5554.8,6,4319.26,256.528,0)
	spawnMobile("naboo", "commoner",60,-5541.38,6,4302.99,60.5745,0)
	spawnMobile("naboo", "commoner",60,-5555.25,6,4241.44,63.404,0)
	spawnMobile("naboo", "commoner",60,-5544.22,6,4232.32,267.981,0)
	spawnMobile("naboo", "commoner",60,-5558.64,6,4283.8,12.7491,0)
	spawnMobile("naboo", "commoner",60,-5599.43,6,4261.62,241.361,0)
	spawnMobile("naboo", "commoner",60,-5583.5,6,4237.52,344.261,0)
	spawnMobile("naboo", "commoner",60,-5562.58,6,4208.34,326.3,0)
	spawnMobile("naboo", "commoner",60,-5507.69,6,4195.83,129.185,0)
	spawnMobile("naboo", "commoner",60,-5487.23,6,4193.99,298.896,0)
	spawnMobile("naboo", "commoner",60,-5453.08,6,4198.55,346.123,0)
	spawnMobile("naboo", "commoner",60,-5457.43,6,4156.44,21.7515,0)
	spawnMobile("naboo", "commoner",60,-5497.13,6,4148.57,166.606,0)
	spawnMobile("naboo", "commoner",60,-5404.2,6,4199.59,80.6257,0)
	spawnMobile("naboo", "commoner",60,-5437.58,6,4301.41,199.461,0)
	--{"commoner",60,-5778.43,6,4397.54,180.005,0, "conversation", ""},
	spawnMobile("naboo", "explorer",60,-5127.71,6,4336.34,0,0)
	spawnMobile("naboo", "explorer",60,-4870.86,6,4179.63,0,0)
	spawnMobile("naboo", "gungan_hunter",300,-5138.49,6,4229.36,180.005,0)
	--{"gungan_outcast",300,-5827.81,6,4172.98,0,0, "conversation", ""},
	spawnMobile("naboo", "imperial_recruiter",0,-4928,6,4231,174,0)
	spawnMobile("naboo", "imperial_recruiter",0,-4936,6,4231,174,0)
	spawnMobile("naboo", "informant_npc_lvl_1",0,-4833,6,4134,0,0)
	spawnMobile("naboo", "informant_npc_lvl_1",0,-4892,6,4216,0,0)
	spawnMobile("naboo", "informant_npc_lvl_1",0,-4962,6,4259,0,0)
	spawnMobile("naboo", "informant_npc_lvl_1",0,-5100,6,4146,0,0)
	spawnMobile("naboo", "informant_npc_lvl_1",0,-5455,6,4277,0,0)
	spawnMobile("naboo", "informant_npc_lvl_1",0,-5436,6,4133,0,0)
	spawnMobile("naboo", "informant_npc_lvl_1",0,-5369,6,4178,0,0)
	spawnMobile("naboo", "informant_npc_lvl_1",0,-5477,6,4089,0,0)
	spawnMobile("naboo", "j1_po",60,-4856.56,6,4158.12,237.016,0)
	--{"junk_dealer",0,-5222.4,6,4217.4,-137,0, "", "",JUNKGENERIC,JUNKCONVGENERIC},
	--{"junk_dealer",0,-5885.3,6,4214.7,83,0, "", "",JUNKGENERIC,JUNKCONVGENERIC},
	spawnMobile("naboo", "miner",60,-4965.4,6,4194.15,180.005,0)
	--{"miner",60,-5886.59,6,4369.23,180.005,0, "conversation", ""},
	spawnMobile("naboo", "naboo_police",300,-5138.49,6,4228.36,0,0)
	spawnMobile("naboo", "naboo_police",300,-4911.53,6,4089.7,127.784,0)
	--{"official",300,-5886.59,6,4368.23,0,0, "conversation", ""},
	spawnMobile("naboo", "stormtrooper",400,-5206.32,6,4182.8,329.254,0)
	spawnMobile("naboo", "stormtrooper",400,-4800,6,4113,121.097,0)
	spawnMobile("naboo", "stormtrooper",400,-5126.48,6.52214,4130.79,184.164,0)
	spawnMobile("naboo", "stormtrooper",400,-5132.44,6,4087.27,158.596,0)
	spawnMobile("naboo", "stormtrooper",400,-5016.16,6,4107,226.021,0)
	spawnMobile("naboo", "stormtrooper",400,-4848.1,6.55806,4172.25,222.445,0)
	spawnMobile("naboo", "stormtrooper",400,-4933.43,6,4233.84,32.6463,0)
	spawnMobile("naboo", "stormtrooper_groupleader",400,-4857.84,6.43289,4178.31,222.445,0)
	--{"stormtrooper_groupleader",400,-5969.83,6,4246.76,173.432,0, "", "VK-481"},
	spawnMobile("naboo", "stormtrooper_rifleman",400,-5160.99,6.52214,4159.54,216.801,0)
	spawnMobile("naboo", "stormtrooper_rifleman",400,-4845.17,6.43094,4167.68,222.445,0)
	spawnMobile("naboo", "trainer_1hsword",0,-5565,6,4304,84,0)
	spawnMobile("naboo", "trainer_2hsword",0,-5382,6,4327,180,0)
	spawnMobile("naboo", "trainer_architect",0,-4931,6,4020,39,0)
	spawnMobile("naboo", "trainer_artisan",0,-4946,6,4131,138,0)
	spawnMobile("naboo", "trainer_artisan",0,-5996.85,6,4287.56,69,0)
	spawnMobile("naboo", "trainer_bioengineer",0,-5017,6,4009,0,0)
	spawnMobile("naboo", "trainer_brawler",0,-5942,6,4253,169,0)
	spawnMobile("naboo", "trainer_brawler",0,-4858,6,4087,-47,0)
	spawnMobile("naboo", "trainer_brawler",0,-4684,6,3947,-71,0)
	spawnMobile("naboo", "trainer_chef",0,-4877,6,4065,0,0)
	spawnMobile("naboo", "trainer_doctor",0,-5038,6,4146,180,0)
	spawnMobile("naboo", "trainer_entertainer",0,-4840,6,4082,66,0)
	spawnMobile("naboo", "trainer_entertainer",0,-5834,6,4241,104,0)
	spawnMobile("naboo", "trainer_marksman",0,-4674,6,3995,180,0)
	spawnMobile("naboo", "trainer_marksman",0,-5982,6,4254,180,0)
	spawnMobile("naboo", "trainer_marksman",0,-4863,6,4079,0,0)
	spawnMobile("naboo", "trainer_medic",0,-4592,6,4125,217,0)
	spawnMobile("naboo", "trainer_medic",0,-5968,6,4277,-96,0)
	spawnMobile("naboo", "trainer_medic",0,-4934,6,4153,129,0)
	spawnMobile("naboo", "trainer_merchant",0,-5129,6,4238,112,0)
	spawnMobile("naboo", "trainer_polearm",0,-5375.06,6,4310.92,84,0)
	spawnMobile("naboo", "trainer_scout",0,-4909.36,6,4025.86,11,0)
	spawnMobile("naboo", "trainer_scout",0,-5986.5,6,4232.79,104,0)
	spawnMobile("naboo", "trainer_scout",0,-4796,6,4103,240,0)
	spawnMobile("naboo", "trainer_unarmed",0,-5649,6,4206,0,0)

	--Creatures
	spawnMobile("naboo", "flewt", 300, getRandomNumber(10) + -5195, 6, getRandomNumber(10) + 3988, getRandomNumber(360), 0)
	spawnMobile("naboo", "flewt", 300, getRandomNumber(10) + -5200, 6, getRandomNumber(10) + 3997, getRandomNumber(360), 0)
	spawnMobile("naboo", "flewt", 300, getRandomNumber(10) + -5207, 6, getRandomNumber(10) + 3992, getRandomNumber(360), 0)
	spawnMobile("naboo", "flewt", 300, getRandomNumber(10) + -5205, 6, getRandomNumber(10) + 3989, getRandomNumber(360), 0)
	spawnMobile("naboo", "flewt", 300, getRandomNumber(10) + -5201, 6, getRandomNumber(10) + 3981, getRandomNumber(360), 0)
	spawnMobile("naboo", "flewt", 300, getRandomNumber(10) + -5202, 6, getRandomNumber(10) + 3975, getRandomNumber(360), 0)

	spawnMobile("naboo", "nightspider", 300, getRandomNumber(10) + -5152.02, 6, getRandomNumber(10) + 3882.86, getRandomNumber(360), 0)
	spawnMobile("naboo", "nightspider", 300, getRandomNumber(10) + -5156.30, 6, getRandomNumber(10) + 3880.18, getRandomNumber(360), 0)
	spawnMobile("naboo", "nightspider", 300, getRandomNumber(10) + -5145.15, 6, getRandomNumber(10) + 3882.41, getRandomNumber(360), 0)
	spawnMobile("naboo", "nightspider", 300, getRandomNumber(10) + -5148.77, 6, getRandomNumber(10) + 3879.10, getRandomNumber(360), 0)
	spawnMobile("naboo", "nightspider", 300, getRandomNumber(10) + -5153.03, 6, getRandomNumber(10) + 3876.91, getRandomNumber(360), 0)
	spawnMobile("naboo", "nightspider", 300, getRandomNumber(10) + -5143.66, 6, getRandomNumber(10) + 3875.17, getRandomNumber(360), 0)

	spawnMobile("naboo", "nuna", 300, getRandomNumber(10) + -5054.71, 6, getRandomNumber(10) + 3959.16, getRandomNumber(360), 0)
	spawnMobile("naboo", "nuna", 300, getRandomNumber(10) + -5049.91, 6, getRandomNumber(10) + 3959.13, getRandomNumber(360), 0)
	spawnMobile("naboo", "nuna", 300, getRandomNumber(10) + -5056.38, 6, getRandomNumber(10) + 3954.13, getRandomNumber(360), 0)
	spawnMobile("naboo", "nuna", 300, getRandomNumber(10) + -5050.76, 6, getRandomNumber(10) + 3955.11, getRandomNumber(360), 0)
	spawnMobile("naboo", "nuna", 300, getRandomNumber(10) + -5046.74, 6, getRandomNumber(10) + 3956.10, getRandomNumber(360), 0)
	spawnMobile("naboo", "nuna", 300, getRandomNumber(10) + -5050.08, 6, getRandomNumber(10) + 3951.88, getRandomNumber(360), 0)

	spawnMobile("naboo", "mott_calf", 300, getRandomNumber(10) + -5068.63, 6, getRandomNumber(10) + 3874.93, getRandomNumber(360), 0)
	spawnMobile("naboo", "mott_calf", 300, getRandomNumber(10) + -5063.50, 6, getRandomNumber(10) + 3874.98, getRandomNumber(360), 0)
	spawnMobile("naboo", "mott_calf", 300, getRandomNumber(10) + -5059.21, 6, getRandomNumber(10) + 3873.08, getRandomNumber(360), 0)
	spawnMobile("naboo", "mott_calf", 300, getRandomNumber(10) + -5064.77, 6, getRandomNumber(10) + 3870.68, getRandomNumber(360), 0)
	spawnMobile("naboo", "mott_calf", 300, getRandomNumber(10) + -5069.20, 6, getRandomNumber(10) + 3867.40, getRandomNumber(360), 0)
	spawnMobile("naboo", "mott_calf", 300, getRandomNumber(10) + -5059.54, 6, getRandomNumber(10) + 3866.31, getRandomNumber(360), 0)

	spawnMobile("naboo", "hermit_spider", 300, getRandomNumber(10) + -5062.01, 6, getRandomNumber(10) + 3771.87, getRandomNumber(360), 0)
	spawnMobile("naboo", "hermit_spider", 300, getRandomNumber(10) + -5070.54, 6, getRandomNumber(10) + 3772.20, getRandomNumber(360), 0)
	spawnMobile("naboo", "hermit_spider", 300, getRandomNumber(10) + -5067.63, 6, getRandomNumber(10) + 3777.25, getRandomNumber(360), 0)
	spawnMobile("naboo", "hermit_spider", 300, getRandomNumber(10) + -5072.20, 6, getRandomNumber(10) + 3778.46, getRandomNumber(360), 0)
	spawnMobile("naboo", "hermit_spider", 300, getRandomNumber(10) + -5065.83, 6, getRandomNumber(10) + 3781.39, getRandomNumber(360), 0)
	spawnMobile("naboo", "hermit_spider", 300, getRandomNumber(10) + -5061.01, 6, getRandomNumber(10) + 3779.68, getRandomNumber(360), 0)

	spawnMobile("naboo", "nightspider", 300, getRandomNumber(10) + -4952.30, 6, getRandomNumber(10) + 3863.72, getRandomNumber(360), 0)
	spawnMobile("naboo", "nightspider", 300, getRandomNumber(10) + -4947.92, 6, getRandomNumber(10) + 3863.87, getRandomNumber(360), 0)
	spawnMobile("naboo", "nightspider", 300, getRandomNumber(10) + -4944.95, 6, getRandomNumber(10) + 3866.25, getRandomNumber(360), 0)
	spawnMobile("naboo", "nightspider", 300, getRandomNumber(10) + -4948.41, 6, getRandomNumber(10) + 3869.46, getRandomNumber(360), 0)
	spawnMobile("naboo", "nightspider", 300, getRandomNumber(10) + -4946.46, 6, getRandomNumber(10) + 3873.06, getRandomNumber(360), 0)
	spawnMobile("naboo", "nightspider", 300, getRandomNumber(10) + -4952.32, 6, getRandomNumber(10) + 3873.58, getRandomNumber(360), 0)

	spawnMobile("naboo", "hermit_spider", 300, getRandomNumber(10) + -4969.69, 6, getRandomNumber(10) + 3767.96, getRandomNumber(360), 0)
	spawnMobile("naboo", "hermit_spider", 300, getRandomNumber(10) + -4962.60, 6, getRandomNumber(10) + 3767.81, getRandomNumber(360), 0)
	spawnMobile("naboo", "hermit_spider", 300, getRandomNumber(10) + -4960.24, 6, getRandomNumber(10) + 3772.21, getRandomNumber(360), 0)
	spawnMobile("naboo", "hermit_spider", 300, getRandomNumber(10) + -4965.28, 6, getRandomNumber(10) + 3772.93, getRandomNumber(360), 0)
	spawnMobile("naboo", "hermit_spider", 300, getRandomNumber(10) + -4970.08, 6, getRandomNumber(10) + 3776.47, getRandomNumber(360), 0)
	spawnMobile("naboo", "hermit_spider", 300, getRandomNumber(10) + -4967.46, 6, getRandomNumber(10) + 3780.19, getRandomNumber(360), 0)

	spawnMobile("naboo", "flewt", 300, getRandomNumber(10) + -4862.57, 6, getRandomNumber(10) + 3768.61, getRandomNumber(360), 0)
	spawnMobile("naboo", "flewt", 300, getRandomNumber(10) + -4854.70, 6, getRandomNumber(10) + 3768.47, getRandomNumber(360), 0)
	spawnMobile("naboo", "flewt", 300, getRandomNumber(10) + -4858.64, 6, getRandomNumber(10) + 3773.44, getRandomNumber(360), 0)
	spawnMobile("naboo", "flewt", 300, getRandomNumber(10) + -4866.15, 6, getRandomNumber(10) + 3775.88, getRandomNumber(360), 0)
	spawnMobile("naboo", "flewt", 300, getRandomNumber(10) + -4860.42, 6, getRandomNumber(10) + 3778.87, getRandomNumber(360), 0)
	spawnMobile("naboo", "flewt", 300, getRandomNumber(10) + -4856.47, 6, getRandomNumber(10) + 3776.79, getRandomNumber(360), 0)

	spawnMobile("naboo", "nightspider", 300, getRandomNumber(10) + -5041, 6, getRandomNumber(10) + 3681, getRandomNumber(360), 0)
	spawnMobile("naboo", "nightspider", 300, getRandomNumber(10) + -5050, 6, getRandomNumber(10) + 3682, getRandomNumber(360), 0)
	spawnMobile("naboo", "nightspider", 300, getRandomNumber(10) + -5046, 6, getRandomNumber(10) + 3675, getRandomNumber(360), 0)
	spawnMobile("naboo", "nightspider", 300, getRandomNumber(10) + -5040, 6, getRandomNumber(10) + 3671, getRandomNumber(360), 0)
	spawnMobile("naboo", "nightspider", 300, getRandomNumber(10) + -5039, 6, getRandomNumber(10) + 3685, getRandomNumber(360), 0)
	spawnMobile("naboo", "nightspider", 300, getRandomNumber(10) + -5047, 6, getRandomNumber(10) + 3686, getRandomNumber(360), 0)

	spawnMobile("naboo", "mott_calf", 300, getRandomNumber(10) + -4946.98, 6, getRandomNumber(10) + 3644.18, getRandomNumber(360), 0)
	spawnMobile("naboo", "mott_calf", 300, getRandomNumber(10) + -4939.79, 6, getRandomNumber(10) + 3645.60, getRandomNumber(360), 0)
	spawnMobile("naboo", "mott_calf", 300, getRandomNumber(10) + -4942.72, 6, getRandomNumber(10) + 3647.44, getRandomNumber(360), 0)
	spawnMobile("naboo", "mott_calf", 300, getRandomNumber(10) + -4946.78, 6, getRandomNumber(10) + 3650.67, getRandomNumber(360), 0)
	spawnMobile("naboo", "mott_calf", 300, getRandomNumber(10) + -4943.30, 6, getRandomNumber(10) + 3652.44, getRandomNumber(360), 0)
	spawnMobile("naboo", "mott_calf", 300, getRandomNumber(10) + -4939.35, 6, getRandomNumber(10) + 3653.82, getRandomNumber(360), 0)

	--Med Center
	spawnMobile("naboo", "trainer_combatmedic",0,-16.4871,0.26,10.7738,176,1697364)
	spawnMobile("naboo", "trainer_combatmedic",0,26.5469,0.26,5.31169,90,1697360)
	spawnMobile("naboo", "trainer_doctor",0,16.0979,0.26,-0.105232,266,1697360)
	spawnMobile("naboo", "trainer_medic",0,13.7645,0.26,4.7703,149,1697360)
	spawnMobile("naboo", "trainer_medic",0,-17.1613,0.26,-0.82884,167,1697364)
	spawnMobile("naboo", "comm_operator",300,-32.8,0.3,13.892,92,1697365)
	spawnMobile("naboo", "commoner_technician",60,-5041.71,13.3,4193.94,180.005,0)
	spawnMobile("naboo", "commoner",60,-5041.71,13.3,4192.84,0,0)

	--Hotel
	spawnMobile("naboo", "commoner",60,2.04307,0.999959,21.6541,270.005,1697375)
	spawnMobile("naboo", "commoner",60,0.043072,0.999959,21.6541,89.9998,1697375)
	spawnMobile("naboo", "commoner",60,-21.2681,1.59776,11.3505,270.004,1697379)
	spawnMobile("naboo", "commoner",60,-23.2681,1.6,11.3505,90.0019,1697379)
	spawnMobile("naboo", "commoner_technician",60,-1.60874,0.999962,6.25947,360.011,1697377)
	spawnMobile("naboo", "explorer",60,-23.0798,1.59166,3.00121,180.006,1697379)
	spawnMobile("naboo", "gungan_hermit",300,-1.60874,0.999952,7.35947,180,1697377)
	spawnMobile("naboo", "naboo_monk",300,18.9004,1.28309,-6.40631,0,1697378)
	spawnMobile("naboo", "otolla_gungan",300,7.0973,1.00001,8.95992,0,1697377)
	spawnMobile("naboo", "otolla_gungan",300,-24.1798,1.5815,3.00112,135.006,1697379)
	--{"commoner",60,-24,1.6,-4,77.8869,1697383, "calm", "An Event Promoter"},
	spawnMobile("naboo", "explorer",60,-21.5772,1.6,-14.1795,180.023,1697383)
	spawnMobile("naboo", "explorer",60,-7.0383,1.6,-12.1532,360.011,1697381)
	spawnMobile("naboo", "miner",60,-7.0383,1.6,-11.0532,179.988,1697381)

	--Guild Hall -5450 4267
	--{"rsf_security_guard",300,6.3,1.2,-3.9,-89,1305892, "conversation", ""},
	--{"rsf_security_officer",300,5.4,1.2,-3.9,87,1305892, "conversation", ""},
	--{"bodyguard",300,11.7898,1.75,-1.89849,180.002,1305892, "conversation", ""},
	--{"commoner",60,-17.0001,2.25,17.4832,270.003,1305888, "conversation", ""},
	--{"commoner",60,-19.0001,2.25,17.4832,90.0053,1305888, "conversation", ""},

	--Guild Hall -5457 4122
	spawnMobile("naboo", "trainer_brawler",0,-11,1,-13,0,1692075)
	spawnMobile("naboo", "trainer_marksman",0,0,1.13306,-13,0,1692074)
	spawnMobile("naboo", "trainer_scout",0,-14.0781,1.13306,3.79797,142,1692072)

	--Guild Hall -5368 4138
	spawnMobile("naboo", "trainer_artisan",0,0.0417929,1.13306,-13.5584,2,1692084)

	--Guild Hall -5452 4014
	spawnMobile("naboo", "trainer_architect",0,11,1.133,-14.5,0,1692093)
	spawnMobile("naboo", "trainer_armorsmith",0,-15,1.1,0,90,1692092)
	spawnMobile("naboo", "trainer_droidengineer",0,-11,1.13306,-13,0,1692095)
	spawnMobile("naboo", "trainer_merchant",0,12,1.13306,6,180,1692091)
	spawnMobile("naboo", "trainer_weaponsmith",0,-3.1,1.1,-8.2,91,1692094)

	--Hotel
	--{"businessman",60,15.5641,1.28309,-2.37071,135.005,1677395, "conversation", ""},
	--{"businessman",60,-4.2087,0.999986,2.15452,179.993,1677394, "conversation", ""},
	--{"gambler",60,4.2931,1,-7.62435,360.011,1677394, "conversation", ""},
	--{"gambler",60,-11.7266,1.6,-16.4722,0,1677399, "conversation", ""},
	--{"naboo_nomad",300,16.6641,1.28309,-3.47071,360.011,1677395, "conversation", ""},
	--{"philosopher",300,-11.7266,1.6,-15.4722,180.001,1677399, "conversation", ""},
	spawnMobile("naboo", "scout_trooper",300,16.6641,1.28309,-2.37071,180.007,1677395)
	--{"scoundrel",300,-4.2087,0.999986,1.15452,0,1677394, "conversation", ""},
	--{"ysnide_omewror",60,4.2931,1.00001,-6.52435,180.012,1677394, "conversation", ""},
	--{"event_perk_vendor",0,-24,1.6,-4,240.351,1677400, "calm", "Pex (a storyteller vendor)"},
	--{"gungan_hermit",300,-22.6115,1.6,-10.3739,179.996,1677400, "conversation", ""},
	--{"naboo_holy_man",300,-22.6115,1.6,-11.4739,0,1677400, "conversation", ""},

	--Guild Hall/Theater
	--{"mercenary",60,-6.34119,0.6,-9.37965,360.011,5475480, "conversation", ""},
	--{"rsf_palace_guard",300,-6.34119,0.6,-8.27965,180.012,5475480, "conversation", ""},
	--{"brawler",60,19.6394,0.764492,42.4583,354.768,5475485, "conversation", ""},
	--{"commoner_technician",60,-22.5486,1.61223,33.2294,354.768,5475485, "conversation", ""},
	--{"entertainer",60,19.6394,0.664388,43.5491,180.018,5475485, "conversation", ""},
	--{"etheli_drenel",60,12.4716,2.31216,25.6024,180.001,5475485, "conversation", ""},
	--{"gungan_guard",300,12.4716,2.41226,24.5116,5.24304,5475485, "conversation", ""},
	--{"noble",60,-16.4118,1.02908,39.5837,179.999,5475485, "conversation", ""},
	--{"rsf_palace_guard",300,18.5394,0.664266,43.5491,134.79,5475485, "conversation", ""},
	--{"rsf_pilot",300,-16.4118,1.12919,38.4929,5.24439,5475485, "conversation", ""},
	--{"rsf_pilot",300,-22.5486,1.52122,34.221,180.016,5475485, "conversation", ""},
	--{"explorer",60,3.05224,2.12878,72.5469,180.005,5475487, "conversation", ""},
	--{"fringer",300,3.05224,2.12878,71.4469,0,5475487, "conversation", ""},
	--{"medic",60,-11.5446,2.12878,75.9709,0,5475487, "conversation", ""},
	spawnMobile("naboo", "trainer_dancer",0,18.2374,2.12871,53.9343,6,5475487)
	spawnMobile("naboo", "trainer_entertainer",0,28.3235,2.12854,73.5353,90,5475487)
	spawnMobile("naboo", "trainer_musician",0,28.3,2.12801,54,-39,5475486)
	--{"vendor",60,-11.5446,2.12878,76.8966,179.996,5475487, "conversation", ""},
	spawnMobile("naboo", "trainer_imagedesigner",0,-22.9,2.1287,74.7,121,5475488)

	--Starport
	--{"trainer_shipwright",0,0.2,0.7,-71.4,-177,1692101, "", "a shipwright trainer"},
	--{"bodyguard",300,-29.8622,7.9418,10.8957,180.008,1692104, "conversation", ""},
	--{"bodyguard",300,-37.5788,7.9418,22.3791,0,1692104, "conversation", ""},

end
