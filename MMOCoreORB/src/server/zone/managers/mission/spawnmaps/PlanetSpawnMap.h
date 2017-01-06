/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef PLANETSPAWNMAP_H_
#define PLANETSPAWNMAP_H_

#include "CitySpawnMap.h"

namespace server {
namespace zone {
namespace managers {
namespace mission {
namespace spawnmaps {

/**
 * Container for all cities on a planet and their mission NPC spawn points.
 */
class PlanetSpawnMap : public Object, Logger {
protected:
	/**
	 * Vector of cities.
	 */
	Vector<Reference<CitySpawnMap*> > citySpawnMaps;

	/**
	 * Name of the planet.
	 */
	String planetName;

	/**
	 * Finds the city number closest to the supplied position on the planet.
	 * @param position the position to search from.
	 * @return closest city number or -1 if none is found.
	 */
	int getClosestCityNumber(const Vector3* position) {
		int city = -1;
		float minSquaredDistance = 100000.0 * 100000.0;
		for (int i = 0; i < citySpawnMaps.size(); i++) {
			float squaredDistance = citySpawnMaps.get(i)->getCityCenter()->distanceTo(*position);
			if (minSquaredDistance >= squaredDistance) {
				minSquaredDistance = squaredDistance;
				city = i;
			}
		}

		return city;
	}

public:
	/**
	 * Default constructor.
	 */
	PlanetSpawnMap() {}

	/**
	 * Constructor.
	 * @param planetName the name of the planet.
	 */
	PlanetSpawnMap(const String& planetName) {
		this->planetName = planetName;
	}

	/**
	 * Loads the object from a LuaObject.
	 * @param luaObject object to load from.
	 */
	void readObject(LuaObject* luaObject) {
		planetName = luaObject->getStringField("name");

		//Load npc's.
		LuaObject npcSpawns = luaObject->getObjectField("npcs");

		for (int numberOfSpawns = 1; numberOfSpawns <= npcSpawns.getTableSize(); ++numberOfSpawns) {
			lua_rawgeti(luaObject->getLuaState(), -1, numberOfSpawns);

			LuaObject luaSpawnObj(luaObject->getLuaState());

			Reference<NpcSpawnPoint*> npc = new NpcSpawnPoint();
			npc->readObject(&npcSpawns);

			addToClosestCity(npc, false);

			luaSpawnObj.pop();
		}
		npcSpawns.pop();
	}

	/**
	 * Adds a city on the planet.
	 * @param city the city to add.
	 */
	void addCity(Reference<CitySpawnMap*> city) {
		citySpawnMaps.add(city);
	}

	/**
	 * Get the planet name.
	 * @return planet name.
	 */
	inline String getPlanetName() {
		return planetName;
	}

	/**
	 * Finds the city closest to the supplied position on the planet.
	 * @param position the position to search from.
	 * @return closest city or NULL if no city is available on the planet.
	 */
	CitySpawnMap* getClosestCity(const Vector3* position) {
		int cityNumber = getClosestCityNumber(position);

		if (cityNumber >= 0) {
			return citySpawnMaps.get(cityNumber);
		} else {
			return NULL;
		}
	}

	CitySpawnMap* getRandomCityNotCloseTo(const Vector3* position) {
		if (citySpawnMaps.size() == 1) {
			//Only one city, return it.
			return citySpawnMaps.get(0);
		} else if (citySpawnMaps.size() > 1) {
			//Get city number of city closest to the supplied position.
			int closestCityNumber = getClosestCityNumber(position);

			//Generate a random city number.
			int randomCityNumber = System::random(citySpawnMaps.size() - 2);
			if (randomCityNumber >= closestCityNumber) {
				//Add one to the city number to skip the closest city.
				randomCityNumber++;
			}

			return citySpawnMaps.get(randomCityNumber);
		} else {
			//No cities.
			return NULL;
		}
	}

	/**
	 * Add a NPC to the closest city.
	 * @param npc the NPC to add.
	 * @param checkDistanceToOtherSpawnPoints if true the distance to all other spawn points in the city
	 * is checked and the new spawn point is only added if it is at least a certain distance from all
	 * other spawn points.
	 * @return the added npc spawn point or the nearest existing spawn point if the supplied spawn point is to close.
	 */
	NpcSpawnPoint* addToClosestCity(Reference<NpcSpawnPoint* > npc, bool checkDistanceToOtherSpawnPoints) {
		int closestCityNumber = getClosestCityNumber(npc->getPosition());

		if (closestCityNumber >= 0) {
			NpcSpawnPoint* closestNpc = citySpawnMaps.get(closestCityNumber)->getNearestNpcSpawnPoint(npc->getPosition());
			if (!checkDistanceToOtherSpawnPoints || closestNpc == NULL || (closestNpc->getPosition()->distanceTo(*npc->getPosition()) > 5.0f)) {
				citySpawnMaps.get(closestCityNumber)->addNpc(npc);
				return npc;
			} else {
				return closestNpc;
			}
		}

		return NULL;
	}

	/**
	 * Finds a spawn point on a certain location.
	 * @param position the position to search.
	 * @return the spawn point on the position or NULL if none exist.
	 */
	NpcSpawnPoint* findSpawnAt(Vector3* position) {
		int closestCityNumber = getClosestCityNumber(position);
		if (closestCityNumber >= 0) {
			return citySpawnMaps.get(closestCityNumber)->findSpawnAt(position);
		}

		return NULL;
	}

	/**
	 * Load the object from a stream.
	 * @param stream the stream to load the object from.
	 * @return true if successful.
	 */
	bool parseFromBinaryStream(ObjectInputStream* stream) {
		bool result = planetName.parseFromBinaryStream(stream);
		return result & citySpawnMaps.parseFromBinaryStream(stream);
	}

	/**
	 * Write the object to a stream.
	 * @param stream the stream to write the object to.
	 * @return true if successful.
	 */
	bool toBinaryStream(ObjectOutputStream* stream) {
		bool result = planetName.toBinaryStream(stream);
		return result & citySpawnMaps.toBinaryStream(stream);
	}

	/**
	 * Saves the spawn points to a file.
	 * @param file the file stream to save the spawn points to.
	 */
	void saveSpawnPoints(std::ofstream& file) {
		file << "planet_" << planetName.toCharArray() << " = PlanetSpawnMap:new {" << std::endl;
		file << "\tname = \"" << planetName.toCharArray() << "\"," << std::endl;
		file << "\tnpcs = {" << std::endl;

		int numberOfNpcSpawnPointsInCity = 0;
		bool itemsBefore = false;

		for (int i = 0; i < citySpawnMaps.size(); i++) {
			if (numberOfNpcSpawnPointsInCity > 0) {
				itemsBefore = true;
			}
			numberOfNpcSpawnPointsInCity = citySpawnMaps.get(i)->saveSpawnPoints(file, itemsBefore);
		}

		file << std::endl << "\t}" << std::endl;
		file << "}" << std::endl << std::endl;
		file << "universe:addPlanet(planet_" << planetName.toCharArray() << ");" << std::endl << std::endl;
	}
};

} // namespace spawnmaps
} // namespace mission
} // namespace managers
} // namespace zone
} // namespace server

using namespace server::zone::managers::mission::spawnmaps;

#endif /* PLANETSPAWNMAP_H_ */
