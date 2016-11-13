/*
 * ContrabandScanTask.h
 *
 *  Created on: Nov 3, 2016
 *      Author: vagrant
 */

#ifndef CONTRABANDSCANTASK_H_
#define CONTRABANDSCANTASK_H_
#include "engine/engine.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/creature/ai/AiAgent.h"
#include "server/zone/managers/collision/CollisionManager.h"

class ContrabandScanTask : public Task {
	const unsigned int SCANINITIATECHANCE = 8; // 1/8 chance to initiate scan - 12.5 % chance.
	const unsigned int TASKDELAY = 1000; // 1 s delay.
	const unsigned int PLAYERSCANCOOLDOWN = 30 * 1000; // 30 s cooldown
	const unsigned int SCANTIME = 5; // 5 s
	const unsigned int RETURNCHECKDELAY = 1 * 1000; // 1 s cooldown
	const unsigned int TIMETORETURNFORSCAN = 10; // 10 s
	const float RANAWAYFACTIONFINE = 100;

	ManagedWeakReference<AiAgent*> weakScanner;
	ManagedWeakReference<CreatureObject*> weakPlayer;

	enum ScanState {
		SCANCHANCE,
		INITIATESCAN,
		AVOIDINGSCAN,
		SCANDELAY,
		FINISHED
	};

	ScanState scanState;
	ScanState previousScanState;

	int timeLeft;
	int previousTimeLeft;

	String getFactionStringId(AiAgent* scanner, const String& imperial, const String& rebel) {
		const String stringId = "@imperial_presence/contraband_search:";
		if (scanner->getFaction() == Factions::FACTIONIMPERIAL || rebel == "") {
			return stringId + imperial;
		} else {
			return stringId + rebel;
		}
	}

	void sendScannerChatMessage(Zone* zone, AiAgent* scanner, CreatureObject* player, const String& imperial, const String& rebel = "") {
		StringIdChatParameter chatMessage;
		chatMessage.setStringId(getFactionStringId(scanner, imperial, rebel));
		zone->getZoneServer()->getChatManager()->broadcastChatMessage(scanner, chatMessage, player->getObjectID(), 0, 0);
	}

	void sendSystemMessage(AiAgent* scanner, CreatureObject* player, const String& imperial, const String& rebel = "") {
		StringIdChatParameter systemMessage;
		systemMessage.setStringId(getFactionStringId(scanner, imperial, rebel));
		player->sendSystemMessage(systemMessage);
	}

	bool scanPrerequisitesMet(AiAgent* scanner, CreatureObject* player) {
		return scanner != NULL && player != NULL && !scanner->isDead() && !player->isDead() && !player->isFeigningDeath()
				&& !player->isIncapacitated() && !scanner->isInCombat() && !player->isInCombat();
	}

	bool playerTriesToAvoidScan(AiAgent* scanner, CreatureObject* player) {
		return scanState != AVOIDINGSCAN && (!scanner->isInRange(player, 16) || !CollisionManager::checkLineOfSight(scanner, player));
	}

	void scannerRequestsPlayerToReturn(Zone* zone, AiAgent* scanner, CreatureObject* player) {
		previousScanState = scanState;
		scanState = AVOIDINGSCAN;
		previousTimeLeft = timeLeft;
		timeLeft = TIMETORETURNFORSCAN;

		sendScannerChatMessage(zone, scanner, player, "return_request_imperial", "return_request_rebel");
	}

	void playerReturned(Zone* zone, AiAgent* scanner, CreatureObject* player) {
		timeLeft--;
		if (scanner->isInRange(player, 16) && CollisionManager::checkLineOfSight(scanner, player)) {
			scanState = previousScanState;
			timeLeft = previousTimeLeft;

			sendScannerChatMessage(zone, scanner, player, "return_thank_imperial", "return_thank_rebel");

			this->reschedule(TASKDELAY);
		} else if (timeLeft < 0) {
			sendScannerChatMessage(zone, scanner, player, "return_false_imperial", "return_false_rebel");
			sendSystemMessage(scanner, player, "ran_away_imperial", "ran_away_rebel");
			player->getPlayerObject()->decreaseFactionStanding(scanner->getFactionString(), RANAWAYFACTIONFINE);

			scanState = FINISHED;
		}
		this->reschedule(TASKDELAY);
	}

	void performScan(Zone* zone, AiAgent* scanner, CreatureObject* player) {
		timeLeft--;
		if (timeLeft < 0) {
			sendScannerChatMessage(zone, scanner, player, "clean_target_imperial", "clean_target_rebel");

			sendSystemMessage(scanner, player, "probe_scan_done");

			scanState = FINISHED;
		}
		this->reschedule(TASKDELAY);
	}

	void scanChance(CreatureObject* player) {
		if (System::random(SCANINITIATECHANCE) >= SCANINITIATECHANCE - 1) {
			this->reschedule(TASKDELAY);
			scanState = INITIATESCAN;
			player->updateCooldownTimer("crackdown_scan", PLAYERSCANCOOLDOWN);
		}
	}

	void initiateScan(Zone* zone, AiAgent* scanner, CreatureObject* player) {
		sendSystemMessage(scanner, player, "dismount_imperial", "dismount_rebel");

		if (player->isRidingMount()) {
			player->dismount();

			sendSystemMessage(scanner, player, "dismount");
		}

		sendScannerChatMessage(zone, scanner, player, "scan_greeting_imperial", "scan_greeting_rebel");

		scanState = SCANDELAY;
		timeLeft = SCANTIME;
	}

public:
	ContrabandScanTask(AiAgent* scanner, CreatureObject* player){
		weakScanner = scanner;
		weakPlayer = player;
		previousScanState = SCANCHANCE;
		scanState = SCANCHANCE;
		previousTimeLeft = 0;
		timeLeft = 0;
	}

	void run() {
		ManagedReference<AiAgent*> scanner = weakScanner.get();
		ManagedReference<CreatureObject*> player = weakPlayer.get();

		if (!scanPrerequisitesMet(scanner, player)){
			return;
		}

		Locker locker(player);
		Locker crossLocker(scanner, player);

		ManagedReference<Zone* > zone = scanner->getZone();

		if (zone == NULL) {
			return;
		}

		if (playerTriesToAvoidScan(scanner, player)) {
			scannerRequestsPlayerToReturn(zone, scanner, player);
		}

		switch (scanState) {
		case SCANCHANCE:
			scanChance(player);
			break;
		case INITIATESCAN:
			initiateScan(zone, scanner, player);
			break;
		case SCANDELAY:
			performScan(zone, scanner, player);
			break;
		case AVOIDINGSCAN:
			playerReturned(zone, scanner, player);
			break;
		default:
			break;
		}
	}
};

#endif /* CONTRABANDSCANTASK_H_ */
