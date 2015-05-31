/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#include "server/zone/objects/tangible/terminal/gambling/GamblingTerminal.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/ZoneServer.h"
#include "server/zone/ZoneProcessServer.h"
#include "server/zone/managers/minigames/GamblingManager.h"
#include "server/chat/StringIdChatParameter.h"
#include "server/zone/templates/tangible/GamblingTerminalTemplate.h"
#include "server/zone/objects/player/sui/slotmachinebox/SuiSlotMachineBox.h"
//#include "server/zone/objects/player/sui/slotmachinebox/SuiSabaccStartBox.h"
#include "server/zone/managers/minigames/events/GamblingEvent.h"

int GamblingTerminalImplementation::handleObjectMenuSelect(CreatureObject* player, byte selectedID) {
	return 0;

	if (selectedID == 245 || selectedID == 20) {
		if (playersWindows.contains(player)) {
			leaveTerminal(player);
		} else {
			joinTerminal(player);
		}
	}

	return 0;
}

void GamblingTerminalImplementation::setEvent(GamblingEvent* event) {
	this->event = event;
}

bool GamblingTerminalImplementation::invalidPosture(CreatureObject* player) {
	if (player->isIncapacitated() || player->isInCombat() || player->isSwimming() || player->isRidingMount() || player->isDead() || player->isMeditating())
		return true;
	return false;
}



bool GamblingTerminalImplementation::checkJoin(CreatureObject* player) {
	bool returnValue = true;
	switch (machineType) {
		case SLOTMACHINE: {
			if (playersWindows.size() > 0) {

				returnValue = false;

				StringIdChatParameter textPlayer("gambling/default_interface","prose_station_full");
				textPlayer.setTT(getMachineTypeText());
				player->sendSystemMessage(textPlayer);

			} else if (!player->isInRange(_this.getReferenceUnsafeStaticCast(), 20.0)) {
				returnValue = false;

				StringIdChatParameter textPlayer("ui","radial_out_of_range_prose");
				textPlayer.setTT(getMachineTypeText());
				textPlayer.setTO("@gambling/default_interface:mnu_join");
				player->sendSystemMessage(textPlayer);

				info(String::valueOf(machineType),true);
				info(String::valueOf(getObjectID()),true);
			} else if (invalidPosture(player)) {
				player->sendSystemMessage("@error_message:wrong_state");
				returnValue = false;
			}

			break;
		}
		case ROULETTEMACHINE: {

			if (playersWindows.size() >= 4) {

				returnValue = false;

				StringIdChatParameter textPlayer("gambling/default_interface","prose_station_full");
				textPlayer.setTT(getMachineTypeText());
				player->sendSystemMessage(textPlayer);

			} else if (!player->isInRange(_this.getReferenceUnsafeStaticCast(), 20.0)) {
				returnValue = false;

				StringIdChatParameter textPlayer("ui","radial_out_of_range_prose");
				textPlayer.setTT(getMachineTypeText());
				textPlayer.setTO("@gambling/default_interface:mnu_join");
				player->sendSystemMessage(textPlayer);

				info(String::valueOf(machineType),true);
				info(String::valueOf(getObjectID()),true);
			}

			break;
		}
		// INSERT SWITCH CASES HERE FOR ADDITIONAL GAMES
		/*case SABACC: {
			if (playersWindows.size() > 5) {

				returnValue = false;

				StringIdChatParameter textPlayer("gambling/default_interface","prose_station_full");
				textPlayer.setTT(getMachineTypeText());
				player->sendSystemMessage(textPlayer);

			} else if (!player->isInRange(_this.getReferenceUnsafeStaticCast(), 20.0)) {
				returnValue = false;

				StringIdChatParameter textPlayer("ui","radial_out_of_range_prose");
				textPlayer.setTT(getMachineTypeText());
				textPlayer.setTO("@gambling/default_interface:mnu_join");
				player->sendSystemMessage(textPlayer);

			}
			break;
		}*/
	}
	return returnValue;
}

void GamblingTerminalImplementation::joinTerminal(CreatureObject* player) {
	ManagedReference<GamblingManager*> gamblingManager = server->getGamblingManager();

	Locker _locker(_this.getReferenceUnsafeStaticCast());
	switch (machineType) {
		case SLOTMACHINE: {
			if (state == SLOTGAMEENDED) {//begin new round

				setState(GAMESTARTING);
				incGameCount();

				playersWindows.drop(player);

				playersWindows.put(player, gamblingManager->createSlotWindow(player, 0));

			} else {
				if (checkJoin(player)) {

					setState(GAMESTARTING);
					incGameCount();

					gamblingManager->registerPlayer(_this.getReferenceUnsafeStaticCast(), player);

					playersWindows.put(player, gamblingManager->createWindow(_this.getReferenceUnsafeStaticCast(), player));

					statusUpdate(player, JOINTERMINAL);

				}
			}

			break;
		}
		case ROULETTEMACHINE: {
			if (checkJoin(player)) {

				winnings.put(player, 0);

				gamblingManager->registerPlayer(_this.getReferenceUnsafeStaticCast(), player);

				playersWindows.put(player, gamblingManager->createWindow(_this.getReferenceUnsafeStaticCast(), player));

				statusUpdate(player, JOINTERMINAL);

				if(!gameRunning()) {
					setState(GAMESTARTING);
					incGameCount();

					gamblingManager->startGame(_this.getReferenceUnsafeStaticCast());
				}

			}

			break;
		}
		// INSERT SWITCH CASES HERE FOR ADDITIONAL GAMES
		/*case SABACC: {
			if (checkJoin(player)) {
				if (playersWindows.size() == 0) {
					String prompt = "Please select the Sabacc variant you would like to play.\n\nEnter the Minimum Bet into the text field.\nMaximum Bets depend on the Sabacc variant.";
					ManagedReference<SuiSabaccStartBox*> xbox = new SuiSabaccStartBox(player, 115, 0);
					xbox->setPromptTitle("Start Sabacc Game");
					xbox->setPromptText(prompt);
					player->addSuiBox(xbox);
					player->sendMessage(xbox->generateMessage());
				} else {
					StringIdChatParameter textOthers("gambling/default_interface","prose_player_join_other");
					textOthers.setTT(player->getFirstName());
					notifyOthers(player, &textOthers);
				}
			}
			break;
		}*/

	}
}

void GamblingTerminalImplementation::closeMenu(CreatureObject* player, bool payout) {
	PlayerObject* ghost = player->getPlayerObject();
	uint32 boxID = playersWindows.get(player);

	if (ghost->hasSuiBox(boxID)) {

		ManagedReference<SuiSlotMachineBox*> box = ghost->getSuiBox(boxID).castTo<SuiSlotMachineBox*>();

		if (payout) {
			uint32 payoutBoxID = box->getPayoutBoxID();

			ManagedReference<SuiBox*> sui = ghost->getSuiBox(payoutBoxID);
			if (sui != NULL) {
				player->sendMessage(sui->generateCloseMessage());
				ghost->removeSuiBox(payoutBoxID);
			}
		}

		if (box != NULL)
			player->sendMessage(box->generateCloseMessage());

		ghost->removeSuiBox(boxID);
	}
}

void GamblingTerminalImplementation::closeAllMenus() {
	switch (machineType) {
		case SLOTMACHINE: {

			if (playersWindows.size() != 0)
				closeMenu(playersWindows.elementAt(0).getKey(),true);

			break;
		}
		case ROULETTEMACHINE: {

			for (int i = 0; i < playersWindows.size(); ++i) {
				closeMenu(playersWindows.elementAt(i).getKey(),false);
			}

			break;
		}
		// INSERT SWITCH CASES HERE FOR ADDITIONAL GAMES
	}
}

void GamblingTerminalImplementation::leaveTerminal(CreatureObject* player) {
	ManagedReference<GamblingManager*> gamblingManager = server->getGamblingManager();

	Locker _locker(_this.getReferenceUnsafeStaticCast());
	switch (machineType) {
		case SLOTMACHINE: {

			closeMenu(player, true);

			statusUpdate(player, LEAVETERMINAL);

			playersWindows.drop(player);
			gamblingManager->stopGame(_this.getReferenceUnsafeStaticCast(), true);

			break;
		}
		case ROULETTEMACHINE: {

			closeMenu(player, false);

			statusUpdate(player, LEAVETERMINAL);

			playersWindows.drop(player);

			winnings.drop(player);

			if (playersWindows.size() == 0) {
				gamblingManager->stopGame(_this.getReferenceUnsafeStaticCast(), true);
			}

			break;
		}
		// INSERT SWITCH CASES HERE FOR ADDITIONAL GAMES
	}

}

void GamblingTerminalImplementation::statusUpdate(int event) {
	switch (machineType) {
		case SLOTMACHINE: {

			statusUpdate(playersWindows.elementAt(0).getKey(), event);

			break;
		}
		case ROULETTEMACHINE: {

			statusUpdate(NULL, event);

			break;
		}
	}
}

void GamblingTerminalImplementation::statusUpdate(CreatureObject* player, int event) {
	switch (machineType) {
		case SLOTMACHINE: {

			switch (event) {
				case JOINTERMINAL: {

					if (player != NULL) {

						StringIdChatParameter textPlayer("gambling/default_interface","prose_player_join");
						textPlayer.setTO(getMachineTypeText());
						player->sendSystemMessage(textPlayer);
					}

					break;
				}
				case LEAVETERMINAL: {

					if (player != NULL) {
						StringIdChatParameter textPlayer("gambling/default_interface","prose_player_leave");
						textPlayer.setTO(getMachineTypeText());
						player->sendSystemMessage(textPlayer);
					}

					break;
				}
				case GAMESTARTED: {

					if (player != NULL) {
						player->sendSystemMessage("@gambling/default_interface:wheel_spin");
					}

					break;
				}
				case TWOLEFT: {
					//Locker _locker(_this.getReferenceUnsafeStaticCast()); Unnecessary, called from continue which locks
					first = System::random(7);

					if (player != NULL) {
						player->sendSystemMessage("Slot Machine Pay Line");
						player->sendSystemMessage("-- | " + String::valueOf(first) + " | | Rolling... | | Rolling... | --");
					}

					break;
				}
				case ONELEFT: {
					//Locker _locker(_this.getReferenceUnsafeStaticCast());
					second = System::random(7);

					if (player != NULL) {
						player->sendSystemMessage("Slot Machine Pay Line");
						player->sendSystemMessage("-- | " + String::valueOf(first) + " | | " + String::valueOf(second) + " | | Rolling... | --");
					}

					break;
				}
				case END: {
					//Locker _locker(_this.getReferenceUnsafeStaticCast());
					third = System::random(7);

					if (player != NULL) {
						player->sendSystemMessage("Slot Machine Pay Line");
						player->sendSystemMessage("-- | " + String::valueOf(first) + " | | " + String::valueOf(second) + " | | " + String::valueOf(third) + "| --");
					}

					break;
				}
			}

			break;
		}
		case ROULETTEMACHINE: {

			switch (event) {
				case JOINTERMINAL: {

					if (player != NULL) {
						StringIdChatParameter textPlayer("gambling/default_interface","prose_player_join");
						textPlayer.setTO(getMachineTypeText());
						player->sendSystemMessage(textPlayer);
					}

					StringIdChatParameter textOthers("gambling/default_interface","prose_player_join_other");
					textOthers.setTT(player->getFirstName());
					notifyOthers(player, &textOthers);

					break;
				}
				case LEAVETERMINAL: {

					if (player != NULL) {
						StringIdChatParameter textPlayer("gambling/default_interface","prose_player_leave");
						textPlayer.setTO(getMachineTypeText());
						player->sendSystemMessage(textPlayer);
					}

					StringIdChatParameter textOthers("gambling/default_interface","prose_player_leave_other");
					textOthers.setTT(player->getFirstName());
					notifyOthers(player, &textOthers);

					break;
				}
				case GAMESTARTING: {
					//120 seconds left

					StringIdChatParameter body("gambling/default_interface","prose_starting_in");
					body.setDI(120);

					notifyAll(&body);

					break;
				}
				case NINETY: {
					//90 seconds left

					StringIdChatParameter body("gambling/default_interface","prose_starting_in");
					body.setDI(90);

					notifyAll(&body);

					break;
				}
				case SIXTY: {
					// 60 seconds left

					StringIdChatParameter body("gambling/default_interface","prose_starting_in");
					body.setDI(60);

					notifyAll(&body);

					break;
				}
				case THIRTY: {
					// 30 seconds left

					StringIdChatParameter body("gambling/default_interface","prose_starting_in");
					body.setDI(30);

					notifyAll(&body);

					break;
				}
				case TWENTY: {
					// 20 seconds left

					StringIdChatParameter body("gambling/default_interface","prose_starting_in");
					body.setDI(20);

					notifyAll(&body);

					break;
				}
				case FIFTEEN: {
					// 15 seconds left

					StringIdChatParameter body("gambling/default_interface","prose_starting_in");
					body.setDI(15);

					notifyAll(&body);

					break;
				}
				case TEN: {
					// 10 seconds left

					StringIdChatParameter body("gambling/default_interface","prose_starting_in");
					body.setDI(10);

					notifyAll(&body);

					break;
				}
				case FIVE: {
					// 5 seconds left

					StringIdChatParameter body("gambling/default_interface","prose_starting_in");
					body.setDI(5);

					notifyAll(&body);

					break;
				}
				case WHEELSTART: {
					// wheels begin to spin

					StringIdChatParameter body("gambling/default_interface","wheel_spin");
					notifyAll(&body);

					break;
				}
				case WHEELBEGINSLOW: {
					// wheel begins to slow down

					StringIdChatParameter body("gambling/default_interface","wheel_begin_slow");
					notifyAll(&body);

					break;
				}
				case WHEELSLOW: {
					// wheel slows down, seems to be.
					//Locker _locker(_this.getReferenceUnsafeStaticCast());

					first = System::random(37);

					StringIdChatParameter body("gambling/default_interface","prose_wheel_slow");
					body.setTT(String::valueOf(first));

					String terminalName;
					_this.getReferenceUnsafeStaticCast()->getObjectName()->getFullPath(terminalName);
					body.setTO(terminalName);

					notifyAll(&body);

					break;
				}
				case WHEELSTOP: {
					// wheel stops, it is X

					int random = System::random(1);

					if (random == 0) {

						StringIdChatParameter body("gambling/default_interface","prose_result_same");
						body.setTT(String::valueOf(first));

						String terminalName;
						_this.getReferenceUnsafeStaticCast()->getObjectName()->getFullPath(terminalName);

						body.setTO(terminalName);

						notifyAll(&body);

					} else {

						first = System::random(37);

						StringIdChatParameter body("gambling/default_interface","prose_result_change");
						body.setTT(String::valueOf(first));

						String terminalName;
						_this.getReferenceUnsafeStaticCast()->getObjectName()->getFullPath(terminalName);

						body.setTO(terminalName);

						notifyAll(&body);
					}

					break;
				}
			}

			break;
		}
		// INSERT SWITCH CASES HERE FOR ADDITIONAL GAMES
	}
}

void GamblingTerminalImplementation::notifyAll(StringIdChatParameter* text) {
	for (int i = 0; i < playersWindows.size(); ++i) {
		playersWindows.elementAt(i).getKey()->sendSystemMessage(*text);
	}
}

void GamblingTerminalImplementation::notifyOthers(CreatureObject* player, StringIdChatParameter* text) {
	if (player != NULL) {
		for (int i = 0; i < playersWindows.size(); ++i) {
			if (playersWindows.elementAt(i).getKey() != player) {
				playersWindows.elementAt(i).getKey()->sendSystemMessage(*text);
			}
		}
	} else {
		notifyAll(text);
	}
}
