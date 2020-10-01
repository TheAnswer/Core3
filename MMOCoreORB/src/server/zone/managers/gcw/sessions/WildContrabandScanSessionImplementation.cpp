/*
 * WildContrabandScanSessionImplementation.cpp
 *
 *  Created on: Aug 30, 2020
 *      Author: loshult
 */

#include "server/zone/managers/creature/CreatureManager.h"
#include "server/zone/managers/gcw/GCWManager.h"
#include "server/zone/managers/gcw/sessions/WildContrabandScanSession.h"
#include "server/zone/managers/gcw/tasks/LambdaShuttleWithReinforcementsTask.h"
#include "server/zone/managers/gcw/tasks/WildContrabandScanTask.h"
#include "server/zone/managers/mission/MissionManager.h"
#include "server/zone/managers/planet/PlanetManager.h"
#include "server/zone/objects/creature/ai/AiAgent.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/packets/scene/PlayClientEffectLocMessage.h"
#include "server/zone/Zone.h"

int WildContrabandScanSessionImplementation::initializeSession() {
	ManagedReference<CreatureObject*> player = weakPlayer.get();

	if (player == nullptr || player->getZone() == nullptr || player->getZone()->getGCWManager() == nullptr) {
		return false;
	}

	if (wildContrabandScanTask == nullptr) {
		wildContrabandScanTask = new WildContrabandScanTask(player);
	}
	if (!wildContrabandScanTask->isScheduled()) {
		wildContrabandScanTask->schedule(TASKDELAY);
	}

	player->updateCooldownTimer("crackdown_scan", player->getZone()->getGCWManager()->getCrackdownPlayerScanCooldown());

	if (player->getActiveSession(SessionFacadeType::WILDCONTRABANDSCAN) != nullptr) {
		player->dropActiveSession(SessionFacadeType::WILDCONTRABANDSCAN);
	}
	player->addActiveSession(SessionFacadeType::WILDCONTRABANDSCAN, _this.getReferenceUnsafeStaticCast());

	landingCoordinates = getLandingCoordinates(player);

	return true;
}

int WildContrabandScanSessionImplementation::cancelSession() {
	ManagedReference<CreatureObject*> player = weakPlayer.get();

	if (player != nullptr) {
		player->dropActiveSession(SessionFacadeType::WILDCONTRABANDSCAN);
	}

	if (wildContrabandScanTask != nullptr) {
		wildContrabandScanTask->cancel();
	}

	return clearSession();
}

int WildContrabandScanSessionImplementation::clearSession() {
	return 0;
}

void WildContrabandScanSessionImplementation::runWildContrabandScan() {
	ManagedReference<CreatureObject*> player = weakPlayer.get();

	if (!scanPrerequisitesMet(player)) {
		cancelSession();
	}

	Locker plocker(player);

	ManagedReference<Zone*> zone = player->getZone();

	if (zone == nullptr) {
		cancelSession();
	}

	timeLeft--;

	switch (scanState) {
	case LANDING:
		landProbeDroid(player);
		scanState = HEADTOPLAYER;
		break;
	case HEADTOPLAYER:
		if (timeLeft <= 0) {
			weakDroid = cast<AiAgent*>(player->getZone()->getCreatureManager()->spawnCreature(STRING_HASHCODE("probot"), 0, landingCoordinates.getX(),
																							  landingCoordinates.getZ(), landingCoordinates.getY(), 0));
			AiAgent* droid = getDroid();
			if (droid != nullptr) {
				Locker clocker(droid, player);
				droid->activateLoad("stationary");
				droid->setFollowObject(player);
				scanState = CLOSINGIN;
				timeLeft = 30;
			} else {
				error("Probot is missing.");
				scanState = FINISHED;
			}
		}
		break;
	case CLOSINGIN:
		if (timeLeft > 0) {
			AiAgent* droid = getDroid();
			if (droid != nullptr) {
				if (player->getWorldPosition().distanceTo(droid->getWorldPosition()) < 32) {
					scanState = INITIATESCAN;
				}
			} else {
				error("Probot is missing.");
				scanState = FINISHED;
			}
		} else {
			scanState = TAKEOFF; // Probot has not reached the player in 30 s, take off again.
		}
		break;
	case INITIATESCAN: {
		sendSystemMessage(player, "dismount_imperial");

		if (player->isRidingMount()) {
			player->dismount();
			sendSystemMessage(player, "dismount");
		}
		sendSystemMessage(player, "probe_scan");

		AiAgent* droid = getDroid();
		if (droid != nullptr) {
			Locker clocker(droid, player);
			droid->showFlyText("imperial_presence/contraband_search", "probe_scan_fly", 255, 0, 0);
			droid->setFollowObject(player);
			timeLeft = SCANTIME;
			scanState = SCANDELAY;
		} else {
			error("Probot is missing.");
			scanState = FINISHED;
		}
	} break;
	case SCANDELAY:
		if (timeLeft <= 0) {
			int numberOfContrabandItems = 0;
			GCWManager* gcwManager = zone->getGCWManager();
			if (gcwManager != nullptr) {
				numberOfContrabandItems = gcwManager->countContrabandItems(player);
			}
			if (numberOfContrabandItems > 0) {
				sendSystemMessage(player, "probe_scan_positive");
				scanState = WAITFORSHUTTLE;

				MissionManager* missionManager = player->getZoneServer()->getMissionManager();
				auto spawnPoint = missionManager->getFreeNpcSpawnPoint(player->getPlanetCRC(), player->getWorldPositionX(), player->getWorldPositionY(),
																	   NpcSpawnPoint::LAMBDASHUTTLESPAWN);
				if (spawnPoint != nullptr) {
					Reference<Task*> lambdaTask = new LambdaShuttleWithReinforcementsTask(player, Factions::FACTIONIMPERIAL, 1, "", *spawnPoint->getPosition(),
																						  *spawnPoint->getDirection(), false);
					lambdaTask->schedule(1);
				} else {
					Reference<Task*> lambdaTask = new LambdaShuttleWithReinforcementsTask(player, Factions::FACTIONIMPERIAL, 1, "", landingCoordinates,
																						  Quaternion(Vector3(0, 1, 0), player->getDirection()->getRadians() + 3.14f), false);
					lambdaTask->schedule(1);
				}
			} else {
				sendSystemMessage(player, "probe_scan_negative");
				scanState = TAKEOFF;
			}
		}
		break;
	case WAITFORSHUTTLE:
		scanState = TAKEOFF;
		break;
	case TAKEOFF: {
		scanState = CLEANUP;
		timeLeft = 10;
		AiAgent* droid = getDroid();
		if (droid != nullptr) {
			Locker dlocker(droid);
			droid->setPosture(CreaturePosture::SITTING, true); // Takeoff
		} else {
			scanState = FINISHED; // Probot is destroyed.
		}
	} break;
	case CLEANUP:
		if (timeLeft <= 0) {
			AiAgent* droid = getDroid();
			if (droid != nullptr) {
				Locker dlocker(droid);
				droid->destroyObjectFromWorld(true);
			}
			scanState = FINISHED;
		}
		break;
	default:
		error("Incorrect state");
		break;
	}

	if (scanState != FINISHED) {
		wildContrabandScanTask->reschedule(TASKDELAY);
	} else {
		cancelSession();
	}
}

bool WildContrabandScanSessionImplementation::scanPrerequisitesMet(CreatureObject* player) {
	return player != nullptr && player->isPlayerCreature() && !player->isDead() && !player->isFeigningDeath() && !player->isIncapacitated() && !player->isInCombat();
}

void WildContrabandScanSessionImplementation::landProbeDroid(CreatureObject* player) {
	PlayClientEffectLoc* effect = new PlayClientEffectLoc("clienteffect/probot_delivery.cef", player->getZone()->getZoneName(), landingCoordinates.getX(),
														  landingCoordinates.getZ(), landingCoordinates.getY(), 0, 0);
	player->sendMessage(effect);
	timeLeft = 3;
}

Vector3 WildContrabandScanSessionImplementation::getLandingCoordinates(CreatureObject* player) {
	if (player->getZone() == nullptr || player->getZone()->getPlanetManager() == nullptr) {
		return player->getPosition();
	}

	PlanetManager* planetManager = player->getZone()->getPlanetManager();

	return planetManager->getInSightSpawnPoint(player, 30, 120, 15);
}

void WildContrabandScanSessionImplementation::sendSystemMessage(CreatureObject* player, const String& messageName) {
	StringIdChatParameter systemMessage;
	systemMessage.setStringId("@imperial_presence/contraband_search:" + messageName);
	player->sendSystemMessage(systemMessage);
}

AiAgent* WildContrabandScanSessionImplementation::getDroid() {
	return weakDroid.get();
}