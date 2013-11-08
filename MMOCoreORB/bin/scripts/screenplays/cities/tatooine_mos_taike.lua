TatooineMosTaikeScreenPlay = ScreenPlay:new {
	numberOfActs = 1,

	screenplayName = "TatooineMosTaikeScreenPlay"
}

registerScreenPlay("TatooineMosTaikeScreenPlay", true)

function TatooineMosTaikeScreenPlay:start()
	if (isZoneEnabled("tatooine")) then
		self:spawnMobiles()
	end
end

function TatooineMosTaikeScreenPlay:spawnMobiles()

		--misc
	--spawnMobile("tatooine", "junk_dealer", 1, 3905.3, 33, 2362.9, -90, 0)
	--spawnMobile("tatooine", "junk_nathan", 1, 3817.7, 16.2, 2340.5, 0, 0)
	spawnMobile("tatooine", "informant_npc_lvl_1", 1, 3775, 8.7, 2371, 180, 0)
	spawnMobile("tatooine", "informant_npc_lvl_2", 1, 0.93374, 1.00421, 9.03511, 180, 1154123)
	spawnMobile("tatooine", "informant_npc_lvl_3", 1, -1.97807, -9.54192, 9.62469, -45, 1154131)

		--commoners
	spawnMobile("tatooine", "commoner_tatooine", 1, 3860.7, 28.0, 2361.1, 180, 0)
	spawnMobile("tatooine", "commoner_tatooine", 1, 3775.0, 8.6, 2369.7, 0, 0)
	spawnMobile("tatooine", "commoner_tatooine", 1, 4.9, -0.4, -3.8, -150, 1400866)
	spawnMobile("tatooine", "commoner_tatooine", 1, 3.1, -0.4, -5.8, -45, 1400866)

		--thugs
	spawnMobile("tatooine", "scoundrel", 24, 3867.0, 27.5, 2307.5, -90, 0)
	spawnMobile("tatooine", "spice_fiend", 24, 3865.0, 27.5, 2307.5, 90, 0)
	spawnMobile("tatooine", "dune_stalker", 40, math.random(13) + 3734.0, 6.2, math.random(13) + 2466.5, math.random(360), 0)
	spawnMobile("tatooine", "dune_stalker", 40, math.random(13) + 3734.0, 6.2, math.random(13) + 2466.5, math.random(360), 0)
	spawnMobile("tatooine", "dune_stalker", 40, math.random(13) + 3734.0, 6.2, math.random(13) + 2466.5, math.random(360), 0)
	spawnMobile("tatooine", "dune_stalker_scavenger", 40, math.random(13) + 3734.0, 6.2, math.random(13) + 2466.5, math.random(360), 0)
	spawnMobile("tatooine", "dune_stalker_scavenger", 40, math.random(13) + 3734.0, 6.2, math.random(13) + 2466.5, math.random(360), 0)
	spawnMobile("tatooine", "dune_stalker_scavenger", 40, math.random(13) + 3734.0, 6.2, math.random(13) + 2466.5, math.random(360), 0)

	spawnMobile("tatooine", "swooper", 40, math.random(13) + 3781, 6.9, math.random(13) + 2220, math.random(360), 0)
	spawnMobile("tatooine", "swooper", 40, math.random(13) + 3781, 6.9, math.random(13) + 2220, math.random(360), 0)
	spawnMobile("tatooine", "swooper", 40, math.random(13) + 3781, 6.9, math.random(13) + 2220, math.random(360), 0)
	spawnMobile("tatooine", "swooper", 40, math.random(13) + 3781, 6.9, math.random(13) + 2220, math.random(360), 0)
	spawnMobile("tatooine", "swooper", 40, math.random(13) + 3781, 6.9, math.random(13) + 2220, math.random(360), 0)
	spawnMobile("tatooine", "swooper", 40, math.random(13) + 3781, 6.9, math.random(13) + 2220, math.random(360), 0)

		--jawas
	spawnMobile("tatooine", "jawa_engineer", 210, 3971.5, 8.9, 2496.9, -97, 0)
	spawnMobile("tatooine", "jawa_engineer", 210, 3966.1, 9.1, 2502.9, 40, 0)
	spawnMobile("tatooine", "jawa_smuggler", 210, 3967.6, 9.0, 2503.3, -90, 0)
	spawnMobile("tatooine", "jawa_smuggler", 210, 3966.9, 9.1, 2504.5, -155, 0)
	spawnMobile("tatooine", "jawa_engineer", 210, 3974.4, 7.3, 2517.1, -161, 0)
	spawnMobile("tatooine", "jawa_smuggler", 210, 3980.9, 7.7, 2507.0, 25, 0)

		--creatures
	spawnMobile("tatooine", "rockmite", 40, math.random(13) + 3992, 7.4, math.random(13) + 2576, math.random(360), 0)
	spawnMobile("tatooine", "rockmite", 40, math.random(13) + 3992, 7.4, math.random(13) + 2576, math.random(360), 0)
	spawnMobile("tatooine", "rockmite", 40, math.random(13) + 3992, 7.4, math.random(13) + 2576, math.random(360), 0)
	spawnMobile("tatooine", "rockmite", 40, math.random(13) + 3992, 7.4, math.random(13) + 2576, math.random(360), 0)
	spawnMobile("tatooine", "rockmite", 40, math.random(13) + 3992, 7.4, math.random(13) + 2576, math.random(360), 0)
	spawnMobile("tatooine", "rockmite", 40, math.random(13) + 3992, 7.4, math.random(13) + 2576, math.random(360), 0)

	spawnMobile("tatooine", "tatooine_mynock", 40, math.random(13) + 3660, 7.6, math.random(13) + 2407, math.random(360), 0)
	spawnMobile("tatooine", "tatooine_mynock", 40, math.random(13) + 3660, 7.6, math.random(13) + 2407, math.random(360), 0)
	spawnMobile("tatooine", "tatooine_mynock", 40, math.random(13) + 3660, 7.6, math.random(13) + 2407, math.random(360), 0)
	spawnMobile("tatooine", "tatooine_mynock", 40, math.random(13) + 3660, 7.6, math.random(13) + 2407, math.random(360), 0)
	spawnMobile("tatooine", "tatooine_mynock", 40, math.random(13) + 3660, 7.6, math.random(13) + 2407, math.random(360), 0)
	spawnMobile("tatooine", "tatooine_mynock", 40, math.random(13) + 3660, 7.6, math.random(13) + 2407, math.random(360), 0)

	spawnMobile("tatooine", "rockmite", 40, math.random(13) + 3660, 8.1, math.random(13) + 2258, math.random(360), 0)
	spawnMobile("tatooine", "rockmite", 40, math.random(13) + 3660, 8.1, math.random(13) + 2258, math.random(360), 0)
	spawnMobile("tatooine", "rockmite", 40, math.random(13) + 3660, 8.1, math.random(13) + 2258, math.random(360), 0)
	spawnMobile("tatooine", "rockmite", 40, math.random(13) + 3660, 8.1, math.random(13) + 2258, math.random(360), 0)
	spawnMobile("tatooine", "rockmite", 40, math.random(13) + 3660, 8.1, math.random(13) + 2258, math.random(360), 0)
	spawnMobile("tatooine", "rockmite", 40, math.random(13) + 3660, 8.1, math.random(13) + 2258, math.random(360), 0)

	spawnMobile("tatooine", "rill", 40, math.random(13) + 3889, 6.6, math.random(13) + 2139, math.random(360), 0)
	spawnMobile("tatooine", "rill", 40, math.random(13) + 3889, 6.6, math.random(13) + 2139, math.random(360), 0)
	spawnMobile("tatooine", "rill", 40, math.random(13) + 3889, 6.6, math.random(13) + 2139, math.random(360), 0)
	spawnMobile("tatooine", "rill", 40, math.random(13) + 3889, 6.6, math.random(13) + 2139, math.random(360), 0)
	spawnMobile("tatooine", "rill", 40, math.random(13) + 3889, 6.6, math.random(13) + 2139, math.random(360), 0)
	spawnMobile("tatooine", "rill", 40, math.random(13) + 3889, 6.6, math.random(13) + 2139, math.random(360), 0)

	spawnMobile("tatooine", "minor_worrt", 40, math.random(13) + 4102, 12.3, math.random(13) + 2199, math.random(360), 0)
	spawnMobile("tatooine", "minor_worrt", 40, math.random(13) + 4102, 12.3, math.random(13) + 2199, math.random(360), 0)
	spawnMobile("tatooine", "minor_worrt", 40, math.random(13) + 4102, 12.3, math.random(13) + 2199, math.random(360), 0)
	spawnMobile("tatooine", "minor_worrt", 40, math.random(13) + 4102, 12.3, math.random(13) + 2199, math.random(360), 0)
	spawnMobile("tatooine", "minor_worrt", 40, math.random(13) + 4102, 12.3, math.random(13) + 2199, math.random(360), 0)
	spawnMobile("tatooine", "minor_worrt", 40, math.random(13) + 4102, 12.3, math.random(13) + 2199, math.random(360), 0)

end


