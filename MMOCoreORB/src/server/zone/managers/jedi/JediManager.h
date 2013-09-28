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

#ifndef JEDIMANAGER_H_
#define JEDIMANAGER_H_

#include "engine/engine.h"
#include "server/zone/objects/creature/CreatureObject.h"

namespace server {
namespace zone {
namespace managers {
namespace jedi {

/**
 * Singleton class with a common interface for all different Jedi progression systems.
 */
class JediManager : public Singleton<JediManager>, public Logger, public Object, public ReadWriteLock {
private:
	/**
	 * The Jedi progression type currently configured.
	 */
	int jediProgressionType;

	/**
	 * The name of the jedi manager class in Lua.
	 */
	String jediManagerName;

public:
	/**
	 * Jedi progression not available.
	 */
	static const int NOJEDIPROGRESSION = 0;

	/**
	 * Jedi progression through the holocron system, i.e. master five random professions.
	 */
	static const int HOLOCRONJEDIPROGRESSION = 1;

	/**
	 * Jedi progression through the village system.
	 */
	static const int VILLAGEJEDIPROGRESSION = 2;

	/**
	 * Custom defined jedi progression system.
	 */
	static const int CUSTOMJEDIPROGRESSION = 3;

	/**
	 * Constructor for the Jedi Manager.
	 */
	JediManager();

	/**
	 * Destructor for the Jedi Manager.
	 */
	~JediManager();

	/**
	 * Load the Jedi Manager configuration.
	 * @param luaEngine the lua engine to use for loading the configuration.
	 */
	void loadConfiguration(Lua* luaEngine);

	/**
	 * Start the jedi manager.
	 * @param luaEngine the lua engine to use for loading the configuration.
	 */
	void start(Lua* luaEngine);

	/**
	 * On player creation.
	 * @param creature the creature/player that was created.
	 */
	void onPlayerCreation(CreatureObject* creature);

	/**
	 * On player login.
	 * @param creature the creature/player that logged in.
	 */
	void onPlayerLogin(CreatureObject* creature);

	/**
	 * On player logout.
	 * @param creature the creature/player that logged out.
	 */
	void onPlayerLogout(CreatureObject* creature);

	/**
	 * Check force status command.
	 * Calls the checkForceStatusCommand in the lua manager.
	 * @param creature the creature that performed the command.
	 */
	void checkForceStatusCommand(CreatureObject* creature);

	String getJediManagerName();
};

}
}
}
}

using namespace server::zone::managers::jedi;

#endif /* JEDIMANAGER_H_ */
