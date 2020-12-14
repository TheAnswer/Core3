/* Copyright <SWGEmu> See file COPYING for copying conditions.*/

#ifndef CREATUREOBJECTMESSAGE6_H_
#define CREATUREOBJECTMESSAGE6_H_

#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/packets/tangible/TangibleObjectMessage6.h"

class CreatureObjectMessage6 : public TangibleObjectMessage6 {
public:
	CreatureObjectMessage6(const CreatureObject* creo)
			: TangibleObjectMessage6(creo, 0x4352454F, 0x16) {

		insertShort((uint16)creo->getLevel());
		insertAscii(creo->getPerformanceAnimation());
		insertAscii(creo->getMoodString());
		insertLong(creo->getWeaponID());
		insertLong(creo->getGroupID());
		insertLong(creo->getGroupInviterID());	
		insertLong(creo->getGroupInviteCounter());
		insertInt(creo->getGuildID());
		insertLong(creo->getTargetID());
		insertByte(creo->getMoodID());
		insertInt(creo->getPerformanceCounter());
		insertInt(creo->getInstrumentID());
	
		const DeltaVector<int>* baseHam = creo->getBaseHAM();
		baseHam->insertToMessage(this);

		insertEquipmentList(creo);

		insertAscii(creo->getAlternateAppearance());

		insertByte(creo->getFrozen());
		setSize();
	}

	void insertEquipmentList(const CreatureObject* creo) {
		const DeltaVector<ManagedReference<TangibleObject*> >* wearables = creo->getWearablesDeltaVector();
		wearables->insertToMessage(this);

	}
};

#endif /*CREATUREOBJECTMESSAGE6_H_*/
