/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef CREATUREOBJECTMESSAGE1_H_
#define CREATUREOBJECTMESSAGE1_H_

#include "server/zone/packets/BaseLineMessage.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/creature/variables/SkillList.h"
#include "engine/log/Logger.h"

class CreatureObjectMessage1 : public BaseLineMessage, public Logger {
public:
	CreatureObjectMessage1(const CreatureObject* creo)
			: BaseLineMessage(creo, 0x4352454F, 1, 0x4) {

		insertInt(creo->getBankCredits());
		
		insertInt(creo->getCashCredits());

		const DeltaVector<int>* baseHam = creo->getMaxHAM();
		baseHam->insertToMessage(this);

		const SkillList* skillList = creo->getSkillList(); // Needs to be skills confirmed.
		skillList->insertToMessage(this);

		setSize();

		setCompression(true);
	}

};

#endif /*CREATUREOBJECTMESSAGE1_H_*/
