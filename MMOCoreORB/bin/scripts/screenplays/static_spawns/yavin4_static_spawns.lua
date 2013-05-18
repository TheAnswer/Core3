--This is to be used for static spawns that are NOT part of caves, cities, dungeons, poi's, or other large screenplays.
Yavin4StaticSpawnsScreenPlay = ScreenPlay:new 
{
	numberOfActs = 1,

	screenplayName = "Yavin4StaticSpawnsScreenPlay",
}

registerScreenPlay("Yavin4StaticSpawnsScreenPlay", true)

function Yavin4StaticSpawnsScreenPlay:start()
	if (isZoneEnabled("yavin4")) then
		self:spawnMobiles()
	end
end

function Yavin4StaticSpawnsScreenPlay:spawnMobiles()
	
	-- Light Jedi Enclave (-5575, 4910)
	spawnMobile("yavin4", "light_jedi_sentinel", 1800, -5579.0, 87.7, 4908.5, -179, 0)
	spawnMobile("yavin4", "light_jedi_sentinel", 1800, -5572.6, 87.7, 4908.2, -179, 0)

	-- Dark Jedi Enclave (5068, 310)
	spawnMobile("yavin4", "dark_jedi_sentinel", 1800, 5074.2, 78.8, 313.9, 90, 0)
	spawnMobile("yavin4", "dark_jedi_sentinel", 1800, 5074.3, 79.2, 306.4, 90, 0)	

end
