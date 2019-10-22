/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef CITYSPAWNMAP_H_
#define CITYSPAWNMAP_H_

#include "NpcSpawnPoint.h"

namespace server {
namespace zone {
namespace managers {
namespace mission {
namespace spawnmaps {

/**
 * Container for all spawn points in a city and function to find a suitable spawn point.
 */
class CitySpawnMap : public Object, Logger {
protected:
	/**
	 * The coordinates of the city center.
	 */
	Vector3 cityCenter;

	/**
	 * The city radius.
	 */
	float radius;

	/**
	 * The city name.
	 */
	String cityName;

	/**
	 * Vector with all spawn points in the city.
	 */
	Vector<Reference<NpcSpawnPoint* > > npcSpawnMap;

	/**
	 * Checks if a spawn point fulfills certain requirements (spawn type, distance and is free to use).
	 * @param npc the spawn point to check.
	 * @param position the position to check distance from.
	 * @param minDistance the minimum distance between the spawn point and the supplied position.
	 * @param minDistance the maximum distance between the spawn point and the supplied position.
	 * @param spawnType spawn type bitmask that must be fulfilled.
	 * @return true if the spawn fulfills spawn type, distance and is free, false otherwise.
	 */
	bool fulfillsRequirements(NpcSpawnPoint* npc, const Vector3* position,
			const float minDistance, const float maxDistance, int spawnType) const {
		if (npc != nullptr) {
			if (((npc->getSpawnType() & spawnType) == spawnType)) {
				float squaredDistance = npc->getPosition()->squaredDistanceTo(*position);
				if ((squaredDistance <= maxDistance * maxDistance) &&
					(squaredDistance >= minDistance * minDistance)) {
					//NPC matches requirements.
					return true;
				}
			}
		}

		return false;
	}

public:
	/**
	 * Read the object from a LuaObject.
	 * @param luaObject the object to load from.
	 */
	void readObject(LuaObject* luaObject) {
		cityName = luaObject->getStringAt(2);
		cityCenter.setX(luaObject->getFloatAt(3));
		cityCenter.setY(luaObject->getFloatAt(4));
		radius = luaObject->getFloatAt(5);
	}

	/**
	 * Get the city center coordinates.
	 * @return city center position.
	 */
	inline const Vector3* getCityCenter() const {
		return &cityCenter;
	}

	/**
	 * Returns a reandom NPC spawn point of the requested spawn type within the minimum and maximum distance
	 * from the given position if it exists.
	 * @param position The position to measure distance from.
	 * @param spawnType the spawn type bit mask needed on the spawn point.
	 * @param minDistance minimum distance between the spawn point and the given position.
	 * @param maxDistance maximum distance between the spawn point and the given position.
	 * @return random spawn point matching the requirements or nullptr if none can be found.
	 */
	NpcSpawnPoint* getRandomNpcSpawnPoint(const Vector3* position, const int spawnType,
			const float minDistance = 0.0, const float maxDistance = 100000.0) const {
		if (npcSpawnMap.size() == 0) {
			return nullptr;
		}

		//Try 100 random npc spawn points, return the first that fulfills the requirements.
		int maximumNumberOfTries = (npcSpawnMap.size() / 4) + 1;
		while (maximumNumberOfTries > 0) {
			int npcNumber = System::random(npcSpawnMap.size() - 1);

			auto npc = npcSpawnMap.get(npcNumber);

			if (fulfillsRequirements(npc, position, minDistance, maxDistance, spawnType)) {
				return npc;
			}

			maximumNumberOfTries--;
		}

		//100 random npc spawn points failed, do a full iteration and pick the first npc that match.
		for (int i = 0; i < npcSpawnMap.size(); ++i) {
			auto npc = npcSpawnMap.get(i);

			if (fulfillsRequirements(npc, position, minDistance, maxDistance, spawnType)) {
				return npc;
			}
		}

		//No npc matches the requirements.
		return nullptr;
	}

	/**
	 * Add a NPC to the spawn map for the city.
	 * @param npc the NPC to add.
	 */
	void addNpc(NpcSpawnPoint* npc) {
		npcSpawnMap.emplace(npc);
	}

	/**
	 * Finds the nearest NPC spawn point.
	 * @param position the position to search from.
	 * @return the nearest NPC spawn point.
	 */
	NpcSpawnPoint* getNearestNpcSpawnPoint(const Vector3* position) const {
		float minimumSquaredDistance = 100000.0f * 100000.0f;
		NpcSpawnPoint* nearestNpcSpawnPoint = nullptr;
		//Iterate over all spawn points.
		for (int i = 0; i < npcSpawnMap.size(); i++) {
			//Calculate distance between spawn point and supplied position and store the smallest distance.
			float squaredDistance = npcSpawnMap.get(i)->getPosition()->squaredDistanceTo(*position);
			if (minimumSquaredDistance > squaredDistance) {
				minimumSquaredDistance = squaredDistance;
				nearestNpcSpawnPoint = npcSpawnMap.get(i);
			}
		}

		return nearestNpcSpawnPoint;
	}

	/**
	 * Finds a spawn point on a certain location.
	 * @param position the position to search.
	 * @return the spawn point on the position or nullptr if none exist.
	 */
	NpcSpawnPoint* findSpawnAt(const Vector3* position) const {
		for (int i = 0; i < npcSpawnMap.size(); i++) {
			if (npcSpawnMap.get(i)->getPosition()->squaredDistanceTo(*position) < 25.0f) {
				return npcSpawnMap.get(i);
			}
		}

		return nullptr;
	}

	/**
	 * Load the object from a stream.
	 * @param stream the stream to load the object from.
	 * @return true if successful.
	 */
	bool parseFromBinaryStream(ObjectInputStream* stream) {
		bool result = cityCenter.parseFromBinaryStream(stream);
		return result & npcSpawnMap.parseFromBinaryStream(stream);
	}

	/**
	 * Write the object to a stream.
	 * @param stream the stream to write the object to.
	 * @return true if successful.
	 */
	bool toBinaryStream(ObjectOutputStream* stream) {
		bool result = cityCenter.toBinaryStream(stream);
		return result & npcSpawnMap.toBinaryStream(stream);
	}

	/**
	 * Saves the spawn points to a file.
	 * @param file the file stream to save the spawn points to.
	 * @param itemsBefore indicates if items has been added before from another city.
	 * @return number of npc spawns written.
	 */
	int saveSpawnPoints(std::ofstream& file, bool itemsBefore) const {
		for (int i = 0; i < npcSpawnMap.size(); i++) {
			if (i > 0 || itemsBefore) {
				file << "," << std::endl;
			}
			npcSpawnMap.get(i)->saveSpawnPoint(file);
		}

		return npcSpawnMap.size();
	}
};

} // namespace spawnmaps
} // namespace mission
} // namespace managers
} // namespace zone
} // namespace server

using namespace server::zone::managers::mission::spawnmaps;

#endif /* CITYSPAWNMAP_H_ */
