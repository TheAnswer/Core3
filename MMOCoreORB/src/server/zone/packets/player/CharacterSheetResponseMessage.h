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

#ifndef CHARACTERSHEETRESPONSEMESSAGE_H_
#define CHARACTERSHEETRESPONSEMESSAGE_H_

#include "engine/engine.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/building/BuildingObject.h"

class CharacterSheetResponseMessage : public BaseMessage {
public:
    CharacterSheetResponseMessage(CreatureObject* player) : BaseMessage() {
		insertShort(0x0D);
		insertInt(0x9B3A17C4); // CRC

		//We should be passing playerobject into this method, rather than PlayerCreature.
		ManagedReference<PlayerObject*> ghost = player->getPlayerObject();

		insertInt(0); //??
		insertInt(0); //??

		uint64 preDesignatedFacilityOid = ghost->getCloningFacility();
		ManagedReference<SceneObject*> cloningFacility = player->getZoneServer()->getObject(preDesignatedFacilityOid);

		if (cloningFacility != NULL && cloningFacility->getZone() != NULL) {
			insertFloat(cloningFacility->getPositionX());
			insertFloat(cloningFacility->getPositionZ());
			insertFloat(cloningFacility->getPositionY());
			insertAscii(cloningFacility->getZone()->getZoneName()); //note: the client ignores the tutorial zone
		} else {
			insertFloat(0); //Bind Location X
			insertFloat(0); //Bind Location Z
			insertFloat(0); //Bind Location Y
			insertAscii(""); //Bind Planet
		}

		insertFloat(0); //Bank Location X
		insertFloat(0); //Bank Location Z
		insertFloat(0); //Bank Location Y
		insertAscii(ghost->getBankLocation()); //Bank Planet

		uint64 declaredOidResidence = ghost->getDeclaredResidence();

		ManagedReference<SceneObject*> declaredResidence = player->getZoneServer()->getObject(declaredOidResidence);

		if (declaredResidence != NULL && declaredResidence->getZone() != NULL) {
			insertFloat(declaredResidence->getPositionX()); //Home Location X
			insertFloat(declaredResidence->getPositionZ()); //Home Location Z
			insertFloat(declaredResidence->getPositionY()); //Home Location Y
			insertAscii(declaredResidence->getZone()->getZoneName()); //Home Planet
		} else {
			insertFloat(0);
			insertFloat(0);
			insertFloat(0);
			insertAscii("");
		}

		insertUnicode(ghost->getSpouseName()); //Spouse Name
		insertInt(ghost->getLotsRemaining()); //Lots Remaining

		insertInt(player->getFaction()); //Faction CRC (or hashCode?)
		insertInt(ghost->getFactionStatus()); //Faction Status

		setCompression(true);
	}

};

#endif /*CHARACTERSHEETRESPONSEMESSAGE_H_*/
