/*
 Copyright (C) 2007 <SWGEmu>

 This File is part of Core3.

 This program is free software; you can redistribute
 it and/or modify it under the terms of the GNU Lesser
 General Public License as published by the Free Software
 Foundation; either version 2 of the License,
 or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 See the GNU Lesser General Public License for
 more details.

 You should have received a copy of the GNU Lesser General
 Public License along with this program; if not, write to
 the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

 Linking Engine3 statically or dynamically with other modules
 is making a combined work based on Engine3.
 Thus, the terms and conditions of the GNU Lesser General Public License
 cover the whole combination.

 In addition, as a special exception, the copyright holders of Engine3
 give you permission to combine Engine3 program with free software
 programs or libraries that are released under the GNU LGPL and with
 code included in the standard release of Core3 under the GNU LGPL
 license (or modified versions of such code, with unchanged license).
 You may copy and distribute such a system following the terms of the
 GNU LGPL for Engine3 and the licenses of the other code concerned,
 provided that you include the source code of that other code when
 and as the GNU LGPL requires distribution of source code.

 Note that people who make modified versions of Engine3 are not obligated
 to grant this special exception for their modified versions;
 it is their choice whether to do so. The GNU Lesser General Public License
 gives permission to release a modified version without this exception;
 this exception also makes it possible to release a modified version
 which carries forward this exception.
 */

#ifndef STARTMUSICCOMMAND_H_
#define STARTMUSICCOMMAND_H_
#include "server/zone/ZoneProcessServer.h"
#include "server/zone/objects/scene/SceneObject.h"
#include "server/zone/objects/tangible/Instrument.h"
#include "server/zone/objects/player/sessions/EntertainingSession.h"
#include "server/zone/objects/player/sui/listbox/SuiListBox.h"
#include "server/zone/managers/skill/SkillManager.h"
#include "server/zone/managers/skill/PerformanceManager.h"
#include "server/zone/managers/skill/Performance.h"
#include "server/zone/objects/group/GroupObject.h"
#include "StartDanceCommand.h"

class StartMusicCommand: public QueueCommand {
public:

	StartMusicCommand(const String& name, ZoneProcessServer* server) :
		QueueCommand(name, server) {

	}

	static void startMusic(CreatureObject* creature, const String& song,
			const String& instrumentAnimation, int intid,
			bool targetInstrument = false) {
		ManagedReference<Facade*> facade = creature->getActiveSession(
				SessionFacadeType::ENTERTAINING);
		ManagedReference<EntertainingSession*> session =
				dynamic_cast<EntertainingSession*> (facade.get());

		if (session == NULL) {
			session = new EntertainingSession(creature);
			creature->addActiveSession(SessionFacadeType::ENTERTAINING, session);
		}

		session->setTargetInstrument(targetInstrument);
		session->startPlayingMusic(song, instrumentAnimation, intid);
	}

	static void sendAvailableSongs(CreatureObject* player, PlayerObject* ghost, uint32 suiType = SuiWindowType::MUSIC_START) {
			Reference<SuiListBox*> sui = new SuiListBox(player, suiType);
			sui->setPromptTitle("@performance:available_songs"); // Available Songs
			sui->setPromptText("@performance:select_song"); // Select a song to play.

			AbilityList* list = ghost->getAbilityList();

			for (int i = 0; i < list->size(); ++i) {
				Ability* ability = list->get(i);

				String abilityName = ability->getAbilityName();

				if (abilityName.indexOf("startMusic") != -1) {
					int args = abilityName.indexOf("+");

					if (args != -1) {
						String arg = abilityName.subString(args + 1);

						sui->addMenuItem(arg);
					}
				}
			}

			ghost->addSuiBox(sui);
			player->sendMessage(sui->generateMessage());

			return;
		}

	int doQueueCommand(CreatureObject* creature, const uint64& target,
			const UnicodeString& arguments) const {

		if (!checkStateMask(creature))
			return INVALIDSTATE;

		if (!checkInvalidLocomotions(creature))
			return INVALIDLOCOMOTION;

		if (!creature->isPlayerCreature())
			return GENERALERROR;

		CreatureObject* player = cast<CreatureObject*> (creature);

		ManagedReference<Facade*> facade = creature->getActiveSession(SessionFacadeType::ENTERTAINING);
		ManagedReference<EntertainingSession*> session = dynamic_cast<EntertainingSession*> (facade.get());

		if (session != NULL) {
			if (session->isDancing()) {
				session->stopDancing();
			}

			if (session->isPlayingMusic()) {
				creature->sendSystemMessage("@performance:already_performing_self"); // You are already performing.

				return GENERALERROR;
			}
		}

		Reference<PlayerObject*> ghost = creature->getSlottedObject("ghost").castTo<PlayerObject*> ();

		Reference<Instrument*> instrument = creature->getSlottedObject("hold_r").castTo<Instrument*> ();
		bool targetedInstrument = false;

		if (instrument == NULL) {
			ManagedReference<SceneObject*> nala = server->getZoneServer()->getObject(creature->getTargetID());

			if (nala != NULL && dynamic_cast<Instrument*> (nala.get())) {

				targetedInstrument = true;
				instrument = cast<Instrument*> (nala.get());
				ManagedReference<SceneObject*> creatureParent = creature->getParent();

				if (creature->getDistanceTo(nala) >= 3 || nala->getZone()
						== NULL || (creatureParent == NULL && NULL
						!= nala->getParent().get())) {
					creature->sendSystemMessage("@elevator_text:too_far"); // You are too far away to use that.

					return GENERALERROR;
				}

				if (instrument->getSpawnerPlayer() != NULL
						&& instrument->getSpawnerPlayer() != creature) {
					creature->sendSystemMessage("You must be the owner of the instrument");

					return GENERALERROR;
				}

				if (instrument->isBeingUsed()) {
					creature->sendSystemMessage("Someone else is using this instrument");

					return GENERALERROR;
				}

				if (instrument->getParent() != NULL || instrument->getSpawnerPlayer() != NULL) {
					instrument->setDirection(*creature->getDirection());
					instrument->teleport(creature->getPositionX(), creature->getPositionZ(), creature->getPositionY(), creature->getParentID());
				}
			} else {
				creature->sendSystemMessage("@performance:music_no_instrument"); // You must have an instrument equipped to play music.

				return GENERALERROR;
			}
		}

		PerformanceManager* performanceManager =
				SkillManager::instance()->getPerformanceManager();
		String instr = performanceManager->getInstrument(
				instrument->getInstrumentType());

		if (!ghost->hasAbility(instr)) {
			creature->sendSystemMessage("@performance:music_lack_skill_instrument"); // You do not have the skill to use the currently equipped instrument.

			return GENERALERROR;
		}

		String args = arguments.toString();

		ManagedReference<GroupObject*> group = creature->getGroup();

		if (group != NULL) {
			String bandSong = group->getBandSong();

			if (args.length() < 2) {
				if (bandSong == "") {
					sendAvailableSongs(player, ghost);

					return SUCCESS;
				} else {
					args = bandSong;

					String fullString = String("startMusic") + "+" + args;
					if (!ghost->hasAbility(fullString)) {
						creature->sendSystemMessage("@performance:music_lack_skill_song_band"); // You do not have the skill to perform the song the band is performing.
						return GENERALERROR;
					}
				}
			} else {
				if (bandSong != "" && args != bandSong) {
					creature->sendSystemMessage("@performance:music_join_band_stop"); // You must play the same song as the band.
					return GENERALERROR;
				}
			}
		} else {
			if (args.length() < 2) {
				sendAvailableSongs(player, ghost);

				return SUCCESS;
			}
		}

		if (!performanceManager->hasInstrumentId(args)) {
			creature->sendSystemMessage("@performance:music_invalid_song"); // That is not a valid song name.
			return GENERALERROR;
		}

		String fullString = String("startMusic") + "+" + args;

		if (!ghost->hasAbility(fullString)) {
			creature->sendSystemMessage("@performance:music_lack_skill_song_self"); // You do not have the skill to perform that song.
			return GENERALERROR;
		}

		Locker lockerInstr(instrument);

		if (instrument->isBeingUsed()) {
			creature->sendSystemMessage("Someone else is using this instrument");
			return GENERALERROR;
		} else
			instrument->setBeingUsed(true);

		String instrumentAnimation;
		int instrid = performanceManager->getInstrumentId(args);
		instrid += performanceManager->getInstrumentAnimation(
				instrument->getInstrumentType(), instrumentAnimation);

		startMusic(creature, args, instrumentAnimation, instrid,
				targetedInstrument);

		return SUCCESS;
	}

};

#endif //STARTMUSICCOMMAND_H_
