/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.
*/

#include "server/zone/objects/tangible/terminal/spaceship/SpaceshipTerminal.h"
#include "server/zone/objects/creature/CreatureObject.h"

int SpaceshipTerminalImplementation::handleObjectMenuSelect(CreatureObject* player, byte selectedID) {
	error() << "In SsTI::handleObjectMenuSelect, selectedId is: " << selectedID;
	return 0;
}
