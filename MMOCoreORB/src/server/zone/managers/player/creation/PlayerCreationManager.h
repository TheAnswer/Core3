/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.
*/

#ifndef PLAYERCREATIONMANAGER_H_
#define PLAYERCREATIONMANAGER_H_

#include "engine/lua/Lua.h"
#include "server/zone/packets/charcreation/ClientCreateCharacterCallback.h"

namespace server {
namespace zone {
	class ZoneServer;
}
}

using namespace server::zone;

namespace server {
namespace zone {
namespace objects {
namespace creature {
	class CreatureObject;
}
}
}
}

using namespace server::zone::objects::creature;

namespace server {
namespace zone {
namespace objects {
namespace scene {
	class SceneObject;
}
}
}
}

using namespace server::zone::objects::scene;

namespace server {
namespace zone {
namespace managers {
namespace player {
namespace creation {

class RacialCreationData;
class HairStyleInfo;
class ProfessionDefaultsInfo;

class PlayerCreationManager : public Singleton<PlayerCreationManager>, public Logger, public Object {
	ManagedReference<ZoneServer*> zoneServer;

	VectorMap<String, Reference<RacialCreationData*> > racialCreationData;
	VectorMap<String, Reference<ProfessionDefaultsInfo*> > professionDefaultsInfo;
	VectorMap<String, SortedVector<String> > defaultCharacterEquipment;
	VectorMap<String, Reference<HairStyleInfo*> > hairStyleInfo;

	/**
	 * Vector containing starting items common for all races and professions.
	 */
	Vector<String> commonStartingItems;

	int startingCash;
	int startingBank;
	int skillPoints;

	bool freeGodMode;

	HashTable<uint32, Time> lastCreatedCharacter;
	Mutex charCountMutex;

	void loadLuaConfig();
	void loadRacialCreationData();
	void loadDefaultCharacterItems();
	void loadProfessionDefaultsInfo();
	void loadHairStyleInfo();

	/**
	 * Loads all starting items (profession specific, race specific, common) and
	 * starting clothes.
	 * @param lua pointer to a Lua object which contains the starting items and clothes.
	 */
	void loadLuaStartingItems(Lua* lua);

	void addCustomization(CreatureObject* creature, const String& customizationString, const String& appearanceFilename);
	void addHair(CreatureObject* creature, const String& hairTemplate, const String& hairCustomization);
	void addRacialMods(CreatureObject* creature, const String& race, Vector<String>* startingSkills, Vector<String>* startingItems, bool equipmentOnly);
	void addStartingItems(CreatureObject* creature, const String& clientTemplate, bool equipmentOnly);
	void addProfessionStartingItems(CreatureObject* creature, const String& profession, const String& clientTemplate, bool equipmentOnly);
	//void generateHologrindProfessions(CreatureObject* creature);

public:
	PlayerCreationManager();
	~PlayerCreationManager();

	/**
	 * Validates the character's name.
	 * @param characterName The character's name.
	 */
	bool validateCharacterName(const String& characterName);
	/**
	 * Attempts to create a character, validating the information passed back by the client.
	 */
	bool createCharacter(ClientCreateCharacterCallback* callback);

	/**
	 * Returns the requested maximum attribute limit for the specified race.
	 * @param race the race of interest.
	 * @param attributeNumber The attribute number (starting from 0).
	 * @return maximum attribute limit.
	 */
	int getMaximumAttributeLimit(const String& race, int attributeNumber);

	/**
	 * Returns the requested minimum attribute limit for the specified race.
	 * @param race the race of interest.
	 * @param attributeNumber The attribute number (starting from 0).
	 * @return minimum attribute limit.
	 */
	int getMinimumAttributeLimit(const String& race, int attributeNumber);

	/**
	 * Returns the total attribute limit for the specified race.
	 * @param race the race of interest.
	 * @return total attribute limit.
	 */
	int getTotalAttributeLimit(const String& race);

	/**
	 * Adds starting Weapons into the target container
	 * @param creature the player creature
	 * @param container the target container
	 */
	void addStartingWeaponsInto(CreatureObject* creature, SceneObject* container);

	/**
	 * Adds starting Items except Weapons into the target container
	 * @param creature the player creature
	 * @param container the target container
	 */
	void addStartingItemsInto(CreatureObject* creature, SceneObject* container);
};

}
}
}
}
}

using namespace server::zone::managers::player::creation;

#endif // PLAYERCREATIONMANAGER_H_
