/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef FINDTARGETTASK_H_
#define FINDTARGETTASK_H_

#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/mission/BountyMissionObjective.h"
#include "server/zone/managers/creature/CreatureTemplateManager.h"
#include "server/zone/managers/player/PlayerManager.h"
#include "server/zone/Zone.h"

namespace server {
namespace zone {
namespace objects {
namespace mission {
namespace bountyhunter {
namespace events {

class FindTargetTask : public Task, public Logger {
	ManagedWeakReference<CreatureObject*> weakDroid;
	ManagedWeakReference<CreatureObject*> weakPlayer;
	ManagedWeakReference<BountyMissionObjective*> weakObjective;
	int timeLeft;
	bool success;
	bool track;
	bool arakyd;
	int trackingsLeft;
	String zoneName;

	enum states { Init, DroidSent, Searching, Tracking, Completed};

	states state;

	void init(CreatureObject* player, CreatureObject* droid) {
		Locker locker(player);
		Locker clocker(droid, player);

		if (droid->getZone() == NULL)
			return;

		droid->setPosture(CreaturePosture::SITTING, true);

		if (arakyd) {
			player->sendSystemMessage("@mission/mission_generic:probe_droid_takeoff");
		} else {
			player->sendSystemMessage("Biological scan received. Commencing planetary search pattern.");
		}

		reschedule(5 * 1000);
		timeLeft -= 5;

		zoneName = droid->getZone()->getZoneName();

		state = DroidSent;
	}

	void droidSent(CreatureObject* droid) {
		Locker locker(droid);

		droid->destroyObjectFromWorld(true);

		reschedule(timeLeft * 1000);

		if (track) {
			state = Tracking;
		} else {
			state = Searching;
		}
	}

	void searching(CreatureObject* player, BountyMissionObjective* objective) {
		if (success) {
			findAndTrackSuccess(player, objective);
		} else {
			int randomNumber = System::random(5) + 1;
			player->sendSystemMessage("@mission/mission_generic:target_not_found_" + String::valueOf(randomNumber));
		}

		if (arakyd) {
			Locker locker(objective);
			objective->setArakydDroid(NULL);
		}
		state = Completed;
	}

	void tracking(CreatureObject* player, BountyMissionObjective* objective) {
		if (success) {
			if (findAndTrackSuccess(player, objective)) {
				if (trackingsLeft > 0) {
					player->sendSystemMessage("@mission/mission_generic:target_continue_tracking");
					reschedule(calculateTime(player) * 1000);
					trackingsLeft--;
					state = Tracking;
				} else {
					player->sendSystemMessage("@mission/mission_generic:target_track_lost");
					if (arakyd) {
						Locker locker(objective);
						objective->setArakydDroid(NULL);
					}
					state = Completed;
				}
			} else {
				state = Completed;
			}
		} else {
			int randomNumber = System::random(5) + 1;
			player->sendSystemMessage("@mission/mission_generic:target_not_found_" + String::valueOf(randomNumber));
			if (arakyd) {
				Locker locker(objective);
				objective->setArakydDroid(NULL);
			}
			state = Completed;
		}
	}

	bool findAndTrackSuccess(CreatureObject* player, BountyMissionObjective* objective) {
		Locker locker(player);

		if (objective->getPlayerOwner() == NULL) {
			return false;
		}

		objective->updateMissionStatus(2);

		if (arakyd) {
			player->sendSystemMessage("@mission/mission_generic:target_located_" + objective->getTargetZoneName());
		} else {
			if (objective->getTargetZoneName() == zoneName) {
				StringIdChatParameter message("@mission/mission_generic:assassin_target_location");
				message.setDI(getDistanceToTarget(player, objective));
				message.setTO("mission/mission_generic", getDirectionToTarget(player, objective));
				player->sendSystemMessage(message);
			} else {
				player->sendSystemMessage("@mission/mission_generic:target_not_on_planet");
				return false;
			}
		}

		return true;
	}

	bool getSuccess(CreatureObject* player, BountyMissionObjective* objective) {
		String skillToUse = "droid_find_chance";
		int maximumSkillMod = 155;
		if (track) {
			skillToUse = "droid_track_chance";
			maximumSkillMod = 125;
		}

		long long skillMods = player->getSkillMod(skillToUse) + player->getSkillModFromBuffs(skillToUse);

		int checkedSkillMods = skillMods;
		if (checkedSkillMods < 0) {
			checkedSkillMods = 0;
		} else if (checkedSkillMods > maximumSkillMod) {
			checkedSkillMods = maximumSkillMod;
		}

		int checkValue = checkedSkillMods - ((getTargetLevel(player, objective)) / 2);
		if (checkValue < 5) {
			checkValue = 5;
		} else if (checkValue > 95) {
			checkValue = 95;
		}

		int randomValue = System::random(100);

		return randomValue < checkValue;
	}

	int calculateTime(CreatureObject* player) {
		String skillToUse = "droid_find_speed";
		int maximumSkillMod = 115;
		if (track) {
			skillToUse = "droid_track_speed";
			maximumSkillMod = 105;
		}

		long long skillMod = player->getSkillMod(skillToUse) + player->getSkillModFromBuffs(skillToUse);

		int checkedSkillMod = skillMod;
		if (checkedSkillMod < 0) {
			checkedSkillMod = 0;
		} else if (checkedSkillMod > maximumSkillMod) {
			checkedSkillMod = maximumSkillMod;
		}

		int time = 150 - checkedSkillMod;

		return time + System::random(time / 2);
	}

	int getTargetLevel(CreatureObject* player, BountyMissionObjective* objective) {
		ManagedReference<MissionObject*> strong = objective->getMissionObject();
		String targetTemplateName = strong->getTargetOptionalTemplate();

		if (targetTemplateName != "") {
			CreatureTemplate* creoTempl = CreatureTemplateManager::instance()->getTemplate(targetTemplateName.hashCode());

			if (creoTempl != NULL) {
				return creoTempl->getLevel();
			} else {
				error("Could not find template for target.");
				return 0;
			}
		} else {
			ManagedReference<CreatureObject*> target = player->getZoneServer()->getObject(strong->getTargetObjectId()).castTo<CreatureObject*>();

			if (target != NULL) {
				//TODO: modify this to better suit the calculation.
				return player->getZoneServer()->getPlayerManager()->calculatePlayerLevel(target);
			}
		}

		return 0;
	}

	int getDistanceToTarget(CreatureObject* player, BountyMissionObjective* objective) {
		Vector3 playerCoordinate;
		playerCoordinate.setX(player->getWorldPositionX());
		playerCoordinate.setY(player->getWorldPositionY());

		Vector3 targetCoordinate = objective->getTargetPosition();

		return playerCoordinate.distanceTo(targetCoordinate);
	}

	String getDirectionToTarget(CreatureObject* player, BountyMissionObjective* objective) {
		Vector3 targetCoordinate = objective->getTargetPosition();
		float dx = targetCoordinate.getX() - player->getWorldPositionX();
		float dy = targetCoordinate.getY() - player->getWorldPositionY();

		if (dx > 0) {
			if (dy > 0) {
				if (dx < dy * 0.5) {
					return "north";
				} else if (dx > dy * 2) {
					return "east";
				} else {
					return "northeast";
				}
			} else {
				if (dx < -dy * 0.5) {
					return "south";
				} else if (dx > -dy * 2) {
					return "east";
				} else {
					return "southeast";
				}
			}
		} else {
			if (dy > 0) {
				if (-dx < dy * 0.5) {
					return "north";
				} else if (-dx > dy * 2) {
					return "west";
				} else {
					return "northwest";
				}
			} else {
				if (-dx < -dy * 0.5) {
					return "south";
				} else if (-dx > -dy * 2) {
					return "west";
				} else {
					return "southwest";
				}
			}
		}
	}

public:
	FindTargetTask(CreatureObject* droid, CreatureObject* player, BountyMissionObjective* objective, bool track, bool arakyd) :
		Logger("FindTargetTask") {
		weakDroid = droid;
		weakPlayer = player;
		weakObjective = objective;
		this->track = track;
		this->arakyd = arakyd;
		state = Init;

		timeLeft = calculateTime(player);

		success = getSuccess(player, objective);

		trackingsLeft = 0;
		if (track) {
			trackingsLeft = player->getSkillMod("droid_tracks");
		}
	}

	~FindTargetTask() {
		cancel();
	}

	bool isArakydTask() {
		return arakyd;
	}

	bool isCompleted() {
		return state & Completed;
	}

	void run() {
		ManagedReference<CreatureObject*> droid = weakDroid.get();
		ManagedReference<CreatureObject*> player = weakPlayer.get();
		ManagedReference<BountyMissionObjective*> objective = weakObjective.get();

		if (player == NULL || objective == NULL) {
			return;
		}

		if (state != Init && !arakyd && player->getZone() != NULL && player->getZone()->getZoneName() != zoneName) {
			//Fail seekers if player leaves zone.
			success = false;
		}

		switch(state) {
		case Init:
			if (droid == NULL || droid->getZone() == NULL) {
				return;
			}
			init(player, droid);
			break;
		case DroidSent:
			if (droid == NULL) {
				return;
			}
			droidSent(droid);
			break;
		case Searching:
			searching(player, objective);
			break;
		case Tracking:
			tracking(player, objective);
			break;
		case Completed:
			break;
		default:
			error("Incorrect state.");
			break;
		}
	}
};

} // namespace events
} // namespace bountyhunter
} // namespace mission
} // namespace objects
} // namespace zone
} // namespace server

using namespace server::zone::objects::mission::bountyhunter::events;

#endif /* FINDTARGETTASK_H_ */
