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

#ifndef COMBATACTION_H_
#define COMBATACTION_H_

#include "ObjectControllerMessage.h"

class CombatAction : public ObjectControllerMessage {
	int defenderSize;


public:
	// set the new posture of attacker and defender BEFORE constructing a CombatAction
	CombatAction(CreatureObject* attacker, CreatureObject* defender, uint32 animcrc, uint8 hit, uint8 trails, long weaponID = 0) :
		ObjectControllerMessage(attacker->getObjectID(), 0x1B, 0xCC) {

		insertInt(animcrc);

		insertLong(attacker->getObjectID());
		if (weaponID == 0)
			insertLong(attacker->getWeaponID());
		else
			insertLong(weaponID);

		insertByte(attacker->getPosture()); // AttackerEndPosture: 0x0-Standing 0x1-Kneeling 0x2-Prone 0xD-Incapacitated
		insertByte(trails); // Trails: 0x01-left_foot 0x02-right_foot 0x04-left_hand 0x08-right_hand 0x10-weapon
		insertByte(0x00); // AttackerSpecialMoveEffect: disabled in the client

		defenderSize = 1;

		insertShort(1);
		insertLong(defender->getObjectID());
		insertByte(defender->getPosture()); // DefenderEndPosture: 0x0-Standing 0x1-Kneeling 0x2-Prone 0xD-Incapacitated
		insertByte(hit); // HitType: 0x0-MISS 0x1-HIT 0x2-BLOCK 0x3-DODGE 0x5-COUNTER 0x7-RICOCHET 0x8-REFLECT 0x9-REFLECT_TO_TARGET
		insertByte(0x00); // DefenderSpecialMoveEffect: disabled in the client
	}

	CombatAction(CreatureObject* attacker, TangibleObject* defender, uint32 animcrc, uint8 hit, uint8 trails, long weaponID = 0) :
				ObjectControllerMessage(attacker->getObjectID(), 0x1B, 0xCC) {

		insertInt(animcrc);

		insertLong(attacker->getObjectID());
		if (weaponID == 0)
			insertLong(attacker->getWeaponID());
		else
			insertLong(weaponID);

		insertByte(attacker->getPosture());
		insertByte(trails);
		insertByte(0x00);

		defenderSize = 1;

		insertShort(1);
		insertLong(defender->getObjectID());
		insertByte(0);
		insertByte(hit);
		insertByte(0);
	}

	CombatAction(CreatureObject* attacker, uint32 animcrc) :
		ObjectControllerMessage(attacker->getObjectID(), 0x1B, 0xCC) {
		insertInt(animcrc);

		insertLong(attacker->getObjectID());
		insertLong(attacker->getWeaponID());

		insertByte(attacker->getPosture());
		insertByte(0xFF);
		insertByte(0xFF);

		defenderSize = 0;
		insertShort(0); // number of defenders
	}

	CombatAction(CreatureObject* attacker, uint32 animcrc, long weaponID) :
		ObjectControllerMessage(attacker->getObjectID(), 0x1B, 0xCC) {
		insertInt(animcrc);

		insertLong(attacker->getObjectID());
		insertLong(weaponID);

		insertByte(attacker->getPosture());
		insertByte(0xFF);
		insertByte(0xFF);

		defenderSize = 0;
		insertShort(0); // number of defenders
	}


	CombatAction(TangibleObject* attacker, CreatureObject* defender, uint32 animcrc,uint8 hit, long wpnID = 0) :
		ObjectControllerMessage(attacker->getObjectID(), 0x1B, 0xCC) {
			insertInt(animcrc);

			insertLong(attacker->getObjectID());
			insertLong(wpnID);

			insertByte(0x0);
			insertByte(0xFF);
			insertByte(0xFF);

			defenderSize = 1;

			insertShort(1);
			insertLong(defender->getObjectID());
			insertByte(defender->getPosture());
			insertByte(hit);
			insertByte(0);
	}


	CombatAction(CreatureObject* attacker, CreatureObject* defender,
			uint32 animcrc,uint8 hit, long wpnID) :
		ObjectControllerMessage(attacker->getObjectID(), 0x1B, 0xCC) {
		insertInt(animcrc);

		insertLong(attacker->getObjectID());
		insertLong(wpnID);

		insertByte(attacker->getPosture());
		insertByte(0xFF);
		insertByte(0xFF);

		defenderSize = 1;

		insertShort(1);
		insertLong(defender->getObjectID());
		insertByte(defender->getPosture());
		insertByte(hit);
		insertByte(0);
	}

	void updateDefenderSize() {
		insertShort(53, ++defenderSize);
	}

	void addDefender(CreatureObject* defender, uint8 hit) {
		insertLong(defender->getObjectID());
		insertByte(defender->getPosture());
		insertByte(hit);
		insertByte(0);

		updateDefenderSize();
	}
};

#endif /*COMBATACTION_H_*/
