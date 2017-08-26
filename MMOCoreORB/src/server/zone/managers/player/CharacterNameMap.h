/*
 * CharacterNameMap.h
 *
 *  Created on: 03/02/2010
 *      Author: victor
 */

#ifndef CHARACTERNAMEMAP_H_
#define CHARACTERNAMEMAP_H_

#include "server/zone/objects/creature/CreatureObject.h"

class CharacterNameMap : public Object {
	HashTable<String, uint64> names;
	HashTable<uint64, String> reverseTable;

	ReadWriteLock guard;

public:
	CharacterNameMap() : names(3000), reverseTable(3000) {
	}

	void put(CreatureObject* player) {
		Locker locker(&guard);

		String firstName = player->getFirstName().toLowerCase();
		uint64 oid = player->getObjectID();

		names.put(firstName, oid);
		reverseTable.put(oid, firstName);
	}

	bool put(const String& name, uint64 oid) {
		Locker locker(&guard);

		if (names.put(name, oid) != names.getNullValue())
			return false;

		reverseTable.put(oid, name);

		return true;
	}

	void remove(const String& name) {
		Locker locker(&guard);

		uint64 oid = names.remove(name.toLowerCase());
		reverseTable.remove(oid);
	}

	uint64 get(const String& name) {
		ReadLocker locker(&guard);

		return names.get(name);
	}

	String get(uint64 oid) {
		ReadLocker locker(&guard);

		return reverseTable.get(oid);
	}

	bool containsKey(const String& name) {
		ReadLocker locker(&guard);

		return names.containsKey(name);
	}

	int size() {
		return names.size();
	}

	HashTable<String, uint64> getNames() {
		ReadLocker locker(&guard);

		return names;
	}
};

#endif /* CHARACTERNAMEMAP_H_ */
