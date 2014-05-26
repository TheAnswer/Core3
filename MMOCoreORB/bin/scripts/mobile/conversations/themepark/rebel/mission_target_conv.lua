function createMissionTargetConvoTemplate(templateName, convoHandler)
	mission_target_convotemplate = ConvoTemplate:new {
		initialScreen = "npc_breech_n",
		templateType = "Lua",
		luaClassHandler = convoHandler,
		screens = {}
	}
	
	mission_target_npc_breech_n = ConvoScreen:new {
		id = "npc_breech_n",
		leftDialog = ":npc_breech_1",
		stopConversation = "false",
		options = {
			{ "@mission/mission_generic:yes", "missiontype" }
		}
	}
	
	mission_target_convotemplate:addScreen(mission_target_npc_breech_n);
	
	mission_target_missiontype = ConvoScreen:new {
		id = "missiontype",
		leftDialog = "",
		stopConversation = "false",
		options = {
		}
	}
	
	mission_target_convotemplate:addScreen(mission_target_missiontype);
	
	mission_target_npc_smuggle_n = ConvoScreen:new {
		id = "npc_smuggle_n",
		leftDialog = ":npc_smuggle_1",
		stopConversation = "true",
		options = {
		}
	}
	
	mission_target_convotemplate:addScreen(mission_target_npc_smuggle_n);
	
	mission_target_npc_takeme_n = ConvoScreen:new {
		id = "npc_takeme_n",
		leftDialog = ":npc_takeme_1",
		stopConversation = "true",
		options = {
		}
	}
	
	mission_target_convotemplate:addScreen(mission_target_npc_takeme_n);
	
	mission_target_dontknowyou_n = ConvoScreen:new {
		id = "dontknowyou_n",
		leftDialog = ":dontknowyou_1",
		stopConversation = "true",
		options = {
		}
	}
	
	mission_target_convotemplate:addScreen(mission_target_dontknowyou_n);
	
	mission_target_otherescort_n = ConvoScreen:new {
		id = "otherescort_n",
		leftDialog = ":otherescort_1",
		stopConversation = "true",
		options = {
		}
	}
	
	mission_target_convotemplate:addScreen(mission_target_otherescort_n);
	
	mission_target_notit_n = ConvoScreen:new {
		id = "notit_n",
		leftDialog = ":notit_1",
		stopConversation = "true",
		options = {
		}
	}
	
	mission_target_convotemplate:addScreen(mission_target_notit_n);
	
	addConversationTemplate(templateName, mission_target_convotemplate);
end


-- Themeparks
createMissionTargetConvoTemplate("theme_park_rebel_mission_target_convotemplate","theme_park_rebel_mission_target_conv_handler")
createMissionTargetConvoTemplate("theme_park_jabba_mission_target_convotemplate","theme_park_jabba_mission_target_conv_handler")
createMissionTargetConvoTemplate("theme_park_marauder_mission_target_convotemplate","theme_park_marauder_mission_target_conv_handler")
createMissionTargetConvoTemplate("theme_park_imperial_mission_target_convotemplate","theme_park_imperial_mission_target_conv_handler")


-- Tatooine
createMissionTargetConvoTemplate("hedon_istee_mission_target_convotemplate","hedon_istee_mission_target_conv_handler")
createMissionTargetConvoTemplate("kitster_banai_mission_target_convotemplate","kitster_banai_mission_target_conv_handler")
createMissionTargetConvoTemplate("pfilbee_jhorn_mission_target_convotemplate","pfilbee_jhorn_mission_target_conv_handler")
createMissionTargetConvoTemplate("rakir_banai_mission_target_convotemplate","rakir_banai_mission_target_conv_handler")
createMissionTargetConvoTemplate("tekil_barje_mission_target_convotemplate","tekil_barje_mission_target_conv_handler")
createMissionTargetConvoTemplate("vardias_tyne_mission_target_convotemplate","vardias_tyne_mission_target_conv_handler")
createMissionTargetConvoTemplate("nitra_vendallan_mission_target_convotemplate","nitra_vendallan_mission_target_conv_handler")
createMissionTargetConvoTemplate("aaph_koden_mission_target_convotemplate","aaph_koden_mission_target_conv_handler")


-- Naboo
createMissionTargetConvoTemplate("boss_nass_mission_target_convotemplate","boss_nass_mission_target_conv_handler")
createMissionTargetConvoTemplate("gavyn_sykes_mission_target_convotemplate","gavyn_sykes_mission_target_conv_handler")
createMissionTargetConvoTemplate("leb_slesher_mission_target_convotemplate","leb_slesher_mission_target_conv_handler")
createMissionTargetConvoTemplate("tamvar_senzen_mission_target_convotemplate","tamvar_senzen_mission_target_conv_handler")


-- Talus
createMissionTargetConvoTemplate("kathikiis_ruwahurr_mission_target_convotemplate","kathikiis_ruwahurr_mission_target_conv_handler")


-- Dantooine
createMissionTargetConvoTemplate("drakka_judarrl_mission_target_convotemplate","drakka_judarrl_mission_target_conv_handler")
createMissionTargetConvoTemplate("jatrian_lytus_mission_target_convotemplate","jatrian_lytus_mission_target_conv_handler")
createMissionTargetConvoTemplate("kelvus_naria_mission_target_convotemplate","kelvus_naria_mission_target_conv_handler")
createMissionTargetConvoTemplate("luthik_uwyr_mission_target_convotemplate","luthik_uwyr_mission_target_conv_handler")
createMissionTargetConvoTemplate("lx_466_mission_target_convotemplate","lx_466_mission_target_conv_handler")
createMissionTargetConvoTemplate("sg_567_mission_target_convotemplate","sg_567_mission_target_conv_handler")
createMissionTargetConvoTemplate("stoos_olko_mission_target_convotemplate","stoos_olko_mission_target_conv_handler")
createMissionTargetConvoTemplate("xaan_talmaron_mission_target_convotemplate","xaan_talmaron_mission_target_conv_handler")


-- Dathomir
createMissionTargetConvoTemplate("singular_nak_mission_target_convotemplate", "singular_nak_mission_target_conv_handler")
createMissionTargetConvoTemplate("wallaw_loowobbli_mission_target_convotemplate", "wallaw_loowobbli_mission_target_conv_handler")

-- Rori
createMissionTargetConvoTemplate("ajuva_vanasterin_mission_target_convotemplate","ajuva_vanasterin_mission_target_conv_handler")
createMissionTargetConvoTemplate("biribas_tarun_mission_target_convotemplate","biribas_tarun_mission_target_conv_handler")
createMissionTargetConvoTemplate("booto_lubble_mission_target_convotemplate","booto_lubble_mission_target_conv_handler")
createMissionTargetConvoTemplate("draya_korbinari_mission_target_convotemplate","draya_korbinari_mission_target_conv_handler")
createMissionTargetConvoTemplate("hefsen_zindalai_mission_target_convotemplate","hefsen_zindalai_mission_target_conv_handler")
createMissionTargetConvoTemplate("indintra_imbru_yerevan_mission_target_convotemplate","indintra_imbru_yerevan_mission_target_conv_handler")
createMissionTargetConvoTemplate("jaleela_bindoo_mission_target_convotemplate","jaleela_bindoo_mission_target_conv_handler")
createMissionTargetConvoTemplate("oxil_sarban_mission_target_convotemplate","oxil_sarban_mission_target_conv_handler")
createMissionTargetConvoTemplate("raxa_binn_mission_target_convotemplate","raxa_binn_mission_target_conv_handler")
createMissionTargetConvoTemplate("sidoras_bey_mission_target_convotemplate","sidoras_bey_mission_target_conv_handler")
createMissionTargetConvoTemplate("sindra_lintikoor_mission_target_convotemplate","sindra_lintikoor_mission_target_conv_handler")
createMissionTargetConvoTemplate("sloan_rusper_mission_target_convotemplate","sloan_rusper_mission_target_conv_handler")
createMissionTargetConvoTemplate("vordin_sildor_mission_target_convotemplate","vordin_sildor_mission_target_conv_handler")
createMissionTargetConvoTemplate("zeelius_kraymunder_mission_target_convotemplate","zeelius_kraymunder_mission_target_conv_handler")

-- Yavin4
createMissionTargetConvoTemplate("gins_darone_mission_target_convotemplate","gins_darone_mission_target_conv_handler")
createMissionTargetConvoTemplate("lian_byrne_mission_target_convotemplate","lian_byrne_mission_target_conv_handler")
createMissionTargetConvoTemplate("ruwan_tokai_mission_target_convotemplate","ruwan_tokai_mission_target_conv_handler")

-- Corellia
createMissionTargetConvoTemplate("diktatTargetConvo","diktatTargetHandler")

