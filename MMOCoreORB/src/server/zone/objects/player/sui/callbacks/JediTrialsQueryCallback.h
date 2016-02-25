#ifndef JEDITRIALSQUERYCALLBACK_H_
#define JEDITRIALSQUERYCALLBACK_H_

#include "server/zone/objects/player/sui/SuiCallback.h"
#include "server/zone/managers/player/PlayerManager.h"
#include "server/zone/managers/jedi/JediManager.h"
#include "engine/engine.h"

class JediTrialsQuerySuiCallback: public SuiCallback {

public:
	JediTrialsQuerySuiCallback(ZoneServer* serv) :
		SuiCallback(serv) {
	}

	void run(CreatureObject* creature, SuiBox* sui, bool cancelPressed,
			Vector<UnicodeString>* args) {
		if (!sui->isMessageBox() || cancelPressed)
			return;

		SuiMessageBox* messageBox = cast<SuiMessageBox*>(sui);

		if (!creature->isPlayerCreature())
			return;

		if (!cancelPressed) {
			PlayerManager* pMan = creature->getZoneServer()->getPlayerManager();
			int trialNumberInit = System::random(JediManager::PADAWAN_TRIALS_TOTAL - 1);

			// Start.
			pMan->startJediPadawanTrial(creature, trialNumberInit);
			creature->setScreenPlayState("padawan:trials:started", 1);
		}
	}
};


#endif
