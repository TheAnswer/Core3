GiantBarkMiteCaveScreenPlay = ScreenPlay:new {
	numberOfActs = 1,

	screenplayName = "GiantBarkMiteCaveScreenPlay",

	lootContainers = {
		5096066,
		5096043,
		5096047,
		5096065,
		5096064
        },
        
        lootLevel = 17, 
 
        lootGroups = {
                {
                        groups = {
                                {group = "color_crystals", chance = 100000},
                                {group = "junk", chance = 6900000},
                                {group = "melee_weapons", chance = 900000},
				{group = "pistols", chance = 300000},
				{group = "carbines", chance = 300000},
				{group = "rifles", chance = 300000},
                                {group = "clothing_attachments", chance = 600000},
                                {group = "armor_attachments", chance = 600000}
                        },
                        lootChance = 8000000
                }                                       
        },
        
        lootContainerRespawn = 1800 -- 30 minutes
}

registerScreenPlay("GiantBarkMiteCaveScreenPlay", true)

function GiantBarkMiteCaveScreenPlay:start()
	if (isZoneEnabled("rori")) then
                self:spawnMobiles()
                self:initializeLootContainers()
        end
end

function GiantBarkMiteCaveScreenPlay:registerLootObserver(mobile, type)
	if (mobile == nil) then
		return
	end
	createObserver(LOOTCREATURE, "RishaSinan", "notifyLoot" .. type, mobile)
end

function GiantBarkMiteCaveScreenPlay:spawnMobiles()
	local mob = spawnMobile("rori", "mutant_bark_mite_hatchling", 480,  -16.3, -4.3, -6.6, -27, 5095990)
	self:registerLootObserver(mob, "Hatchling")
	mob = spawnMobile("rori", "mutant_bark_mite_worker", 480,  -12.0, -5.9, -4.8, -45, 5095990)
	self:registerLootObserver(mob, "Worker")
	mob = spawnMobile("rori", "mutant_bark_mite_hatchling", 480,  23.5, -26.2, -5.7, -97, 5095990)
	self:registerLootObserver(mob, "Hatchling")
	mob = spawnMobile("rori", "mutant_bark_mite_worker", 480,  22.6, -27.4, -8.1, -77, 5095990)
	self:registerLootObserver(mob, "Worker")
	mob = spawnMobile("rori", "giant_mutant_bark_mite", 480,  24.1, -42.2, -61.3, -4, 5095991)
	self:registerLootObserver(mob, "Mite")
	mob = spawnMobile("rori", "giant_mutant_bark_mite", 480,  45.9, -47.9, -56.4, -138, 5095991)
	self:registerLootObserver(mob, "Mite")
	mob = spawnMobile("rori", "mutant_bark_mite_worker", 480,  49.3, -51.4, -91.8, -9, 5095999)
	self:registerLootObserver(mob, "Worker")
	mob = spawnMobile("rori", "mutant_bark_mite_hatchling", 480,  49.1, -51.7, -90.4, -15, 5095999)
	self:registerLootObserver(mob, "Hatchling")
	mob = spawnMobile("rori", "mutant_bark_mite_hatchling", 480,  46.8, -51.3, -90.6, 6, 5095999)
	self:registerLootObserver(mob, "Hatchling")
	mob = spawnMobile("rori", "mutant_bark_mite_hatchling", 480,  47.8, -51.8, -88.3, -173, 5095999)
	self:registerLootObserver(mob, "Hatchling")
	mob = spawnMobile("rori", "mutant_bark_mite_worker", 480,  58.3, -48.5, -104.3, -71, 5095999)
	self:registerLootObserver(mob, "Worker")
	mob = spawnMobile("rori", "mutant_bark_mite_hatchling", 480,  57.6, -48.7, -105.9, -50, 5095999)
	self:registerLootObserver(mob, "Hatchling")
	mob = spawnMobile("rori", "mutant_bark_mite_hatchling", 480,  57.6, -48.7, -104.7, -47, 5095999)
	self:registerLootObserver(mob, "Hatchling")
	mob = spawnMobile("rori", "giant_mutant_bark_mite", 480,  74.2, -46.5, -109.0, -88, 5095994)
	self:registerLootObserver(mob, "Mite")
	mob = spawnMobile("rori", "mutant_bark_mite_worker", 480,  95.6, -45.7, -94.7, -149, 5095994)
	self:registerLootObserver(mob, "Worker")
	mob = spawnMobile("rori", "mutant_bark_mite_hatchling", 480,  96.8, -45.5, -98.1, 172, 5095994)
	self:registerLootObserver(mob, "Hatchling")
	mob = spawnMobile("rori", "mutant_bark_mite_hatchling", 480,  93.5, -45.8, -97.6, -150, 5095994)
	self:registerLootObserver(mob, "Hatchling")
	mob = spawnMobile("rori", "mutant_bark_mite_worker", 480,  94.6, -46.4, -139.8, -4, 5096000)
	self:registerLootObserver(mob, "Worker")
	mob = spawnMobile("rori", "mutant_bark_mite_worker", 480,  92.2, -46.9, -137.3, -8, 5096000)
	self:registerLootObserver(mob, "Worker")
	mob = spawnMobile("rori", "giant_mutant_bark_mite", 480,  78.6, -46.5, -140.4, 79, 5096000)
	self:registerLootObserver(mob, "Mite")
	mob = spawnMobile("rori", "giant_mutant_bark_mite", 480,  79.7, -46.2, -145.3, 56, 5096000)
	self:registerLootObserver(mob, "Mite")
	mob = spawnMobile("rori", "giant_mutant_bark_mite", 480,  42.6, -46.0, -25.6, 172, 5095991)
	self:registerLootObserver(mob, "Mite")
	mob = spawnMobile("rori", "mutant_bark_mite_worker", 480,  91.9, -61.6, -12.1, -96, 5095992)
	self:registerLootObserver(mob, "Worker")
	mob = spawnMobile("rori", "mutant_bark_mite_hatchling", 480,  90.2, -61.7, -10.4, -94, 5095992)
	self:registerLootObserver(mob, "Hatchling")
	mob = spawnMobile("rori", "mutant_bark_mite_hatchling", 480,  88.0, -62.1, -12.3, -83, 5095992)
	self:registerLootObserver(mob, "Hatchling")
	mob = spawnMobile("rori", "mutant_bark_mite_hatchling", 480,  91.1, -61.9, -13.9, -87, 5095992)
	self:registerLootObserver(mob, "Hatchling")
	mob = spawnMobile("rori", "giant_mutant_bark_mite", 480,  85.0, -68.8, -41.9, -9, 5095992)
	self:registerLootObserver(mob, "Mite")
	mob = spawnMobile("rori", "giant_mutant_bark_mite", 480,  54.1, -68.0, -46.4, 29, 5095991)
	self:registerLootObserver(mob, "Mite")
	mob = spawnMobile("rori", "giant_mutant_bark_mite", 480,  51.4, -67.5, -43.4, 59, 5095991)
	self:registerLootObserver(mob, "Mite")
	mob = spawnMobile("rori", "mutant_bark_mite_hatchling", 480,  54.0, -68.1, -43.7, -59, 5095991)
	self:registerLootObserver(mob, "Hatchling")
	mob = spawnMobile("rori", "mutant_bark_mite_hatchling", 480,  56.9, -68.3, -44.9, 17, 5095991)
	self:registerLootObserver(mob, "Hatchling")
	mob = spawnMobile("rori", "mutant_bark_mite_hatchling", 480,  95.8, -74.0, -57.0, -58, 5095993)
	self:registerLootObserver(mob, "Hatchling")
	mob = spawnMobile("rori", "mutant_bark_mite_hatchling", 480,  94.0, -74.6, -57.7, -54, 5095993)
	self:registerLootObserver(mob, "Hatchling")
	mob = spawnMobile("rori", "mutant_bark_mite_hatchling", 480,  64.9, -76.4, -72.9, 29, 5095993)
	self:registerLootObserver(mob, "Hatchling")
	mob = spawnMobile("rori", "mutant_bark_mite_hatchling", 480,  63.0, -75.8, -70.8, 29, 5095993)
	self:registerLootObserver(mob, "Hatchling")
	mob = spawnMobile("rori", "giant_mutant_bark_mite", 480,  82.5, -76.6, -88.1, -4, 5095993)
	self:registerLootObserver(mob, "Mite")
	mob = spawnMobile("rori", "giant_mutant_bark_mite", 480,  78.4, -76.7, -88.0, -16, 5095993)
	self:registerLootObserver(mob, "Mite")
	mob = spawnMobile("rori", "mutant_bark_mite_worker", 480,  54.3, -72.4, -111.0, 0, 5095995)
	self:registerLootObserver(mob, "Worker")
	mob = spawnMobile("rori", "giant_mutant_bark_mite", 480,  52.4, -72.8, -109.7, 3, 5095995)
	self:registerLootObserver(mob, "Mite")
	mob = spawnMobile("rori", "giant_mutant_bark_mite", 480,  56.6, -73.3, -109.1, -17, 5095995)
	self:registerLootObserver(mob, "Mite")
	mob = spawnMobile("rori", "mutant_bark_mite_soldier", 480,  78.3, -65.3, -142.2, -77, 5095996)
	self:registerLootObserver(mob, "Soldier")
	mob = spawnMobile("rori", "giant_mutant_bark_mite", 480,  92.1, -66.4, -118.7, -168, 5095994)
	self:registerLootObserver(mob, "Mite")
	mob = spawnMobile("rori", "giant_mutant_bark_mite", 480,  95.9, -66.2, -119.7, -165, 5095994)
	self:registerLootObserver(mob, "Mite")
	mob = spawnMobile("rori", "mutant_bark_mite_soldier", 480,  114.4, -67.1, -92.0, -175, 5095997)
	self:registerLootObserver(mob, "Soldier")
	mob = spawnMobile("rori", "giant_mutant_bark_mite", 480,  140.8, -67.0, -95.5, -116, 5095997)
	self:registerLootObserver(mob, "Mite")
	mob = spawnMobile("rori", "giant_mutant_bark_mite", 480,  137.5, -67.1, -90.2, -108, 5095997)
	self:registerLootObserver(mob, "Mite")
	mob = spawnMobile("rori", "giant_mutant_bark_mite", 480,  125.9, -66.7, -107.8, -99, 5095997)
	self:registerLootObserver(mob, "Mite")
	mob = spawnMobile("rori", "mutant_bark_mite_worker", 480,  123.4, -66.6, -104.5, -106, 5095997)
	self:registerLootObserver(mob, "Worker")
	mob = spawnMobile("rori", "mutant_bark_mite_worker", 480,  124.9, -66.6, -111.4, -87, 5095997)
	self:registerLootObserver(mob, "Worker")
	mob = spawnMobile("rori", "mutant_bark_mite_soldier", 480,  138.2, -66.5, -119.8, -119, 5095997)
	self:registerLootObserver(mob, "Soldier")
	mob = spawnMobile("rori", "mutant_bark_mite_soldier", 480,  150.4, -66.2, -128.8, -92, 5095997)
	self:registerLootObserver(mob, "Soldier")
	mob = spawnMobile("rori", "mutant_bark_mite_soldier", 480,  167.6, -66.6, -97.3, -96, 5095998)
	self:registerLootObserver(mob, "Soldier")
	mob = spawnMobile("rori", "mutant_bark_mite_hatchling", 480,  196.6, -65.9, -96.5, -46, 5095998)
	self:registerLootObserver(mob, "Hatchling")
	mob = spawnMobile("rori", "mutant_bark_mite_hatchling", 480,  196.9, -65.8, -95.3, -66, 5095998)
	self:registerLootObserver(mob, "Hatchling")
	mob = spawnMobile("rori", "mutant_bark_mite_hatchling", 480,  195.2, -66.3, -96.4, -14, 5095998)
	self:registerLootObserver(mob, "Hatchling")
	mob = spawnMobile("rori", "mutant_bark_mite_hatchling", 480,  193.8, -66.5, -95.9, 57, 5095998)
	self:registerLootObserver(mob, "Hatchling")
	mob = spawnMobile("rori", "mutant_bark_mite_hatchling", 480,  195.7, -66.0, -94.0, -140, 5095998)
	self:registerLootObserver(mob, "Hatchling")
	mob = spawnMobile("rori", "mutant_bark_mite_hatchling", 480,  193.9, -66.0, -93.6, 144, 5095998)
	self:registerLootObserver(mob, "Hatchling")
	mob = spawnMobile("rori", "mutant_bark_mite_queen", 600,  188.3, -65.4, -109.4, -22, 5095998)
	self:registerLootObserver(mob, "Queen")
	mob = spawnMobile("rori", "mutant_bark_mite_worker", 480,  190.3, -66.1, -108.3, -32, 5095998)
	self:registerLootObserver(mob, "Worker")
	mob = spawnMobile("rori", "mutant_bark_mite_worker", 480,  186.1, -65.4, -109.4, -37, 5095998)
	self:registerLootObserver(mob, "Worker")
end
