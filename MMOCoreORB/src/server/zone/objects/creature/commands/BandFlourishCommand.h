/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef BANDFLOURISHCOMMAND_H_
#define BANDFLOURISHCOMMAND_H_

#include "server/zone/objects/creature/ai/DroidObject.h"
#include "server/zone/objects/tangible/Instrument.h"
#include "server/zone/objects/player/sessions/EntertainingSession.h"
#include "server/zone/objects/group/GroupObject.h"
#include "server/zone/objects/tangible/components/droid/DroidPlaybackModuleDataComponent.h"

class BandFlourishCommand : public QueueCommand {
public:

	BandFlourishCommand(const String& name, ZoneProcessServer* server)
		: QueueCommand(name, server) {

	}

	static void doBandFlourish(CreatureObject* player, String& number, bool musicflourish, int instrumentType) {
		ManagedReference<GroupObject*> group = player->getGroup();

		ManagedReference<Facade*> facade = player->getActiveSession(SessionFacadeType::ENTERTAINING);
		ManagedReference<EntertainingSession*> session = dynamic_cast<EntertainingSession*>(facade.get());

		ManagedReference<Instrument*> instrument = session->getInstrument(player);
		int leaderInstrument = instrument == NULL ? -1 : instrument->getInstrumentType();

		//Make the player flourish.
		if (group == NULL) { //player is not in a group.
			if (instrumentType > 0) { //the player specified a valid instrument.
				if (!player->isPlayingMusic() || leaderInstrument != instrumentType) {
					return;
				}
			}

			if (session->isAcceptingBandFlourishes()) {
				session->doFlourish(Integer::valueOf(number), true);
			}

			return;

		} else { //player is in a group.

			if (instrumentType > 0) { //the player specified a valid instrument.
				if (!musicflourish) {
					return;
				}
				player->sendSystemMessage("@performance:flourish_perform_band_self"); //"Your band performs a flourish."
				if (player->isPlayingMusic() && leaderInstrument == instrumentType && session->isAcceptingBandFlourishes()) {
					session->doFlourish(Integer::valueOf(number), true);
				}

			} else { //no instrument specified.
				player->sendSystemMessage("@performance:flourish_perform_band_self"); //"Your band performs a flourish."
				if (session->isAcceptingBandFlourishes()) {
					session->doFlourish(Integer::valueOf(number), true);
				}
			}

		}
		//Make group members flourish.
		StringIdChatParameter params;
		params.setTT(player->getFirstName());


		player->unlock();

		try {
			Locker locker(group);

			for (int i = 0; i < group->getGroupSize(); i++) {
				Reference<CreatureObject*> groupMember = group->getGroupMember(i);

				Locker clocker(groupMember, group);

				if (groupMember != player && groupMember->isPlayerCreature()) {
					ManagedReference<Facade*> pfacade = groupMember->getActiveSession(SessionFacadeType::ENTERTAINING);

					ManagedReference<EntertainingSession*> psession = dynamic_cast<EntertainingSession*>(pfacade.get());

					if (psession == NULL)
						continue;

					ManagedReference<Instrument*> pinstrument = psession->getInstrument(groupMember);
					int playerInstrumentType = pinstrument == NULL ? -1 : pinstrument->getInstrumentType();

					if (psession->isAcceptingBandFlourishes()) {

						//Handle dance flourish
						if (!musicflourish && psession->isDancing()) {
							params.setStringId("performance", "flourish_perform_band_member");
							groupMember->sendSystemMessage(params);
							psession->doFlourish(Integer::valueOf(number), false);
						}

						//Handle music flourish
						if (musicflourish && psession->isPlayingMusic()) {
							if (instrumentType < 1 || (playerInstrumentType == instrumentType)) {
								params.setStringId("performance", "flourish_perform_band_member");
								groupMember->sendSystemMessage(params);
								psession->doFlourish(Integer::valueOf(number), false);
							}
						}
					}
				}

				if (groupMember != player && groupMember->isDroidObject()) {
					// is the droid playing music?
					DroidObject* droid = cast<DroidObject*>(groupMember.get());
					auto module = droid->getModule("playback_module");

					if (module != NULL) {
						DroidPlaybackModuleDataComponent* entertainer = cast<DroidPlaybackModuleDataComponent*>(module.get());

						if (entertainer != NULL) {
							if (entertainer->isActive() && musicflourish && (instrumentType == entertainer->getCurrentInstrument() || instrumentType < 1)) {
								entertainer->doFlourish(Integer::valueOf(number));
							}
						}
					}
				}

			}
		} catch (Exception& e) {
			player->wlock();

			throw;
		}

		player->wlock();

		return;
	}


	static int determineInstrumentType(const String& instrument) {
		if (instrument == "traz") {
			return InstrumentImplementation::TRAZ;
		} else if (instrument == "slitherhorn") {
			return InstrumentImplementation::SLITHERHORN;
		} else if (instrument == "fanfar") {
			return InstrumentImplementation::FANFAR;
		} else if (instrument == "chidinkalu") {
			return InstrumentImplementation::FLUTEDROOPY;
		} else if (instrument == "kloohorn") {
			return InstrumentImplementation::KLOOHORN;
		} else if (instrument == "fizz") {
			return InstrumentImplementation::FIZZ;
		} else if (instrument == "bandfill") {
			return InstrumentImplementation::BANDFILL;
		} else if (instrument == "omnibox") {
			return InstrumentImplementation::OMNIBOX;
		} else if (instrument == "nalargon") {
			return InstrumentImplementation::NALARGON;
		} else if (instrument == "mandoviol") {
			return InstrumentImplementation::MANDOVIOL;
		} else {
			return -1;
		}
	}

	static bool parseOptions(CreatureObject* player, const String& options, String& stringA, String& stringB) {
		if (options.isEmpty()) {
			player->sendSystemMessage("@performance:band_flourish_format");
			return false;
		}

		stringA = "";
		stringB = "";

		StringTokenizer tokenizer(options);
		tokenizer.setDelimeter(" ");
		tokenizer.getStringToken(stringA);
		stringA.toLowerCase();

		if (tokenizer.hasMoreTokens()) {
			tokenizer.getStringToken(stringB);
			stringB.toLowerCase();
		}

		if (tokenizer.hasMoreTokens()) {
			player->sendSystemMessage("@performance:band_flourish_format");
			return false;
		}

		return true;
	}

	static void changeStatus(CreatureObject* player, bool newstatus) {
		ManagedReference<Facade*> facade = player->getActiveSession(SessionFacadeType::ENTERTAINING);
		ManagedReference<EntertainingSession*> session = dynamic_cast<EntertainingSession*>(facade.get());

		if (session->isAcceptingBandFlourishes()) {
			if (newstatus == true) {
				player->sendSystemMessage("@performance:band_flourish_status_on");
			} else {
				session->setAcceptingBandFlourishes(false);
				player->sendSystemMessage("@performance:band_flourish_off");
			}
		} else {
			if (newstatus == false) {
				player->sendSystemMessage("@performance:band_flourish_status_off");
			} else {
				session->setAcceptingBandFlourishes(true);
				player->sendSystemMessage("@performance:band_flourish_on");
			}
		}

		return;
	}

	void printStatus(CreatureObject* player) const {
		ManagedReference<Facade*> facade = player->getActiveSession(SessionFacadeType::ENTERTAINING);
		ManagedReference<EntertainingSession*> session = dynamic_cast<EntertainingSession*>(facade.get());

		if (session->isAcceptingBandFlourishes()) {
			player->sendSystemMessage("@performance:band_flourish_status_on");
		} else {
			player->sendSystemMessage("@performance:band_flourish_status_off");
		}
		return;
	}

	int doQueueCommand(CreatureObject* creature, const uint64& target, const UnicodeString& arguments) const {

		if (!checkStateMask(creature))
			return INVALIDSTATE;

		if (!checkInvalidLocomotions(creature))
			return INVALIDLOCOMOTION;

		if (!creature->isPlayerCreature()) {
			return GENERALERROR;
		}

		String stringA, stringB;

		CreatureObject* player = cast<CreatureObject*>(creature);

		if (!creature->isEntertaining()) {
			player->sendSystemMessage("@performance:flourish_not_performing");
			return GENERALERROR;
		}

		//Parse the options from the input string. This is the text the player entered after "/bandFlourish"
		if (!parseOptions(player, arguments.toString(), stringA, stringB))
			return GENERALERROR;

		//Check if leader specified an instrument to flourish.
		int instrumentType = 0;
		if (!stringB.isEmpty()) {
			instrumentType = determineInstrumentType(stringB);

			if (instrumentType < 0) {
				player->sendSystemMessage("@performance:flourish_instrument_unknown");
				return GENERALERROR;
			}
		}

		//Check if the player is doing a flourish.
		int number = Integer::valueOf(stringA);

		if (number > 0 && number < 9) {
			if (player->isDancing()) {
				doBandFlourish(player, stringA, false, instrumentType);

			} else if (player->isPlayingMusic()) {
				doBandFlourish(player, stringA, true, instrumentType);

			} else {
				player->sendSystemMessage("@performance:flourish_not_performing");
				return GENERALERROR;
			}

			//Check if the player is changing an option.
		} else if (stringA == "on") {
			changeStatus(player, true);

		} else if (stringA == "off") {
			changeStatus(player, false);

		} else if (stringA == "status") {
			printStatus(player);

		} else if (number > 8) {
			player->sendSystemMessage("@performance:flourish_not_valid");
			return GENERALERROR;

		} else {
			player->sendSystemMessage("@performance:band_flourish_format");
			return GENERALERROR;
		}

		return SUCCESS;
	}

};

#endif //BANDFLOURISHCOMMAND_H_
