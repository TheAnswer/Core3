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

#ifndef CREATUREPOSTURE_H_
#define CREATUREPOSTURE_H_

#include "engine/engine.h"
#include "server/zone/templates/datatables/DataTableIff.h"
#include "server/zone/templates/datatables/DataTableRow.h"
#include "server/zone/managers/templates/TemplateManager.h"
#include "server/zone/objects/scene/SceneObjectType.h"

#include "CreatureLocomotion.h"

class CreatureMovementEntry : public Variable {
public:
	UnsignedCharacter posture;
	UnsignedCharacter stationary;
	UnsignedCharacter slow;
	UnsignedCharacter fast;
	Float movementScale;
	Float accelerationScale;
	Float turnScale;
	Float canSeeHeightMod;

	CreatureMovementEntry() : Variable() {
		posture = 0;
		stationary = 0;
		slow = 0;
		fast = 0;
		movementScale = 0;
		accelerationScale = 0;
		turnScale = 0;
		canSeeHeightMod = 0;
	}

	CreatureMovementEntry(const CreatureMovementEntry& m) : Variable() {
		posture = m.posture;
		stationary = m.stationary;
		slow = m.slow;
		fast = m.fast;
		movementScale = m.movementScale;
		accelerationScale = m.accelerationScale;
		turnScale = m.turnScale;
		canSeeHeightMod = m.canSeeHeightMod;
	}

	bool operator==(CreatureMovementEntry entry) {
		return posture == entry.posture; // only need posture here because I am sure that these are unique to entries
	}

	bool toString(String& str) {
		return posture.toString(str) && stationary.toString(str) && slow.toString(str) && fast.toString(str) && movementScale.toString(str) && accelerationScale.toString(str) && turnScale.toString(str) && canSeeHeightMod.toString(str);
	}

	bool parseFromString(const String& str, int version = 0) {
		return posture.parseFromString(str, version) && stationary.parseFromString(str, version) && slow.parseFromString(str, version) && fast.parseFromString(str, version) && movementScale.parseFromString(str, version) && accelerationScale.parseFromString(str, version) && turnScale.parseFromString(str, version) && canSeeHeightMod.parseFromString(str, version);
	}

	bool toBinaryStream(ObjectOutputStream* stream) {
		posture.toBinaryStream(stream);
		stationary.toBinaryStream(stream);
		slow.toBinaryStream(stream);
		fast.toBinaryStream(stream);
		movementScale.toBinaryStream(stream);
		accelerationScale.toBinaryStream(stream);
		turnScale.toBinaryStream(stream);
		canSeeHeightMod.toBinaryStream(stream);

		return true;
	}

	bool parseFromBinaryStream(ObjectInputStream* stream) {
		posture.parseFromBinaryStream(stream);
		stationary.parseFromBinaryStream(stream);
		slow.parseFromBinaryStream(stream);
		fast.parseFromBinaryStream(stream);
		movementScale.parseFromBinaryStream(stream);
		accelerationScale.parseFromBinaryStream(stream);
		turnScale.parseFromBinaryStream(stream);
		canSeeHeightMod.parseFromBinaryStream(stream);

		return true;
	}
};

class CreaturePosture : public Singleton<CreaturePosture>, public Object, public Logger {
public:
	
	enum {
		INVALID        = 0xFF,
		UPRIGHT        = 0,
		CROUCHED       = 1,
		PRONE          = 2,
		SNEAKING       = 3,
		BLOCKING       = 4,
		CLIMBING       = 5,
		FLYING         = 6,
		LYINGDOWN      = 7,
		SITTING        = 8,
		SKILLANIMATING = 9,
		DRIVINGVEHICLE = 10,
		RIDINGCREATURE = 11,
		KNOCKEDDOWN    = 12,
		INCAPACITATED  = 13,
		DEAD           = 14
	};

	HashTable<uint8, CreatureMovementEntry> movementTable;
	HashTable<uint8, int> rangedAttackMod;
	HashTable<uint8, int> rangedDefenseMod;
	HashTable<uint8, int> meleeAttackMod;
	HashTable<uint8, int> meleeDefenseMod;

	CreaturePosture() {
		rangedAttackMod.put(CreatureLocomotion::STANDING, 0);
		rangedAttackMod.put(CreatureLocomotion::SNEAKING, -6);
		rangedAttackMod.put(CreatureLocomotion::WALKING, -40);
		rangedAttackMod.put(CreatureLocomotion::RUNNING, -60);
		rangedAttackMod.put(CreatureLocomotion::KNEELING, 15);
		rangedAttackMod.put(CreatureLocomotion::CROUCHSNEAKING, 0);
		rangedAttackMod.put(CreatureLocomotion::CROUCHWALKING, 0);
		rangedAttackMod.put(CreatureLocomotion::PRONE, 30);
		rangedAttackMod.put(CreatureLocomotion::CRAWLING, -80);
		rangedAttackMod.put(CreatureLocomotion::CLIMBINGSTATIONARY, 0);
		rangedAttackMod.put(CreatureLocomotion::CLIMBING, 0);
		rangedAttackMod.put(CreatureLocomotion::HOVERING, 0);
		rangedAttackMod.put(CreatureLocomotion::FLYING, -10);
		rangedAttackMod.put(CreatureLocomotion::LYINGDOWN, 0);
		rangedAttackMod.put(CreatureLocomotion::SITTING, 0);
		rangedAttackMod.put(CreatureLocomotion::SKILLANIMATING, 0);
		rangedAttackMod.put(CreatureLocomotion::DRIVINGVEHICLE, 0);
		rangedAttackMod.put(CreatureLocomotion::RIDINGCREATURE, 0);
		rangedAttackMod.put(CreatureLocomotion::KNOCKEDDOWN, 0);
		rangedAttackMod.put(CreatureLocomotion::INCAPACITATED, 0);
		rangedAttackMod.put(CreatureLocomotion::BLOCKING, 0);
		rangedAttackMod.put(CreatureLocomotion::DEAD, 0);
		rangedAttackMod.put(CreatureLocomotion::INVALID, 0);

		rangedDefenseMod.put(CreatureLocomotion::STANDING, -10);
		rangedDefenseMod.put(CreatureLocomotion::SNEAKING, 5);
		rangedDefenseMod.put(CreatureLocomotion::WALKING, 25);
		rangedDefenseMod.put(CreatureLocomotion::RUNNING, 45);
		rangedDefenseMod.put(CreatureLocomotion::KNEELING, 0);
		rangedDefenseMod.put(CreatureLocomotion::CROUCHSNEAKING, 0);
		rangedDefenseMod.put(CreatureLocomotion::CROUCHWALKING, 0);
		rangedDefenseMod.put(CreatureLocomotion::PRONE, 25);
		rangedDefenseMod.put(CreatureLocomotion::CRAWLING, 5);
		rangedDefenseMod.put(CreatureLocomotion::CLIMBINGSTATIONARY, 0);
		rangedDefenseMod.put(CreatureLocomotion::CLIMBING, 0);
		rangedDefenseMod.put(CreatureLocomotion::HOVERING, -5);
		rangedDefenseMod.put(CreatureLocomotion::FLYING, -10);
		rangedDefenseMod.put(CreatureLocomotion::LYINGDOWN, 0);
		rangedDefenseMod.put(CreatureLocomotion::SITTING, 0);
		rangedDefenseMod.put(CreatureLocomotion::SKILLANIMATING, 0);
		rangedDefenseMod.put(CreatureLocomotion::DRIVINGVEHICLE, 0);
		rangedDefenseMod.put(CreatureLocomotion::RIDINGCREATURE, 0);
		rangedDefenseMod.put(CreatureLocomotion::KNOCKEDDOWN, -15);
		rangedDefenseMod.put(CreatureLocomotion::INCAPACITATED, 0);
		rangedDefenseMod.put(CreatureLocomotion::DEAD, 0);
		rangedDefenseMod.put(CreatureLocomotion::BLOCKING, 0);
		rangedDefenseMod.put(CreatureLocomotion::INVALID, 0);

		meleeAttackMod.put(CreatureLocomotion::STANDING, 0);
		meleeAttackMod.put(CreatureLocomotion::SNEAKING, -20);
		meleeAttackMod.put(CreatureLocomotion::WALKING, 10);
		meleeAttackMod.put(CreatureLocomotion::RUNNING, 25);
		meleeAttackMod.put(CreatureLocomotion::KNEELING, -20);
		meleeAttackMod.put(CreatureLocomotion::CROUCHSNEAKING, 0);
		meleeAttackMod.put(CreatureLocomotion::CROUCHWALKING, 0);
		meleeAttackMod.put(CreatureLocomotion::PRONE, -90);
		meleeAttackMod.put(CreatureLocomotion::CRAWLING, -90);
		meleeAttackMod.put(CreatureLocomotion::CLIMBINGSTATIONARY, 0);
		meleeAttackMod.put(CreatureLocomotion::CLIMBING, 0);
		meleeAttackMod.put(CreatureLocomotion::HOVERING, 0);
		meleeAttackMod.put(CreatureLocomotion::FLYING, -10);
		meleeAttackMod.put(CreatureLocomotion::LYINGDOWN, 0);
		meleeAttackMod.put(CreatureLocomotion::SITTING, 0);
		meleeAttackMod.put(CreatureLocomotion::SKILLANIMATING, 0);
		meleeAttackMod.put(CreatureLocomotion::DRIVINGVEHICLE, 0);
		meleeAttackMod.put(CreatureLocomotion::RIDINGCREATURE, 0);
		meleeAttackMod.put(CreatureLocomotion::KNOCKEDDOWN, 0);
		meleeAttackMod.put(CreatureLocomotion::INCAPACITATED, 0);
		meleeAttackMod.put(CreatureLocomotion::BLOCKING, 0);
		meleeAttackMod.put(CreatureLocomotion::DEAD, 0);
		meleeAttackMod.put(CreatureLocomotion::INVALID, 0);

		meleeDefenseMod.put(CreatureLocomotion::STANDING, 0);
		meleeDefenseMod.put(CreatureLocomotion::SNEAKING, 0);
		meleeDefenseMod.put(CreatureLocomotion::WALKING, 10);
		meleeDefenseMod.put(CreatureLocomotion::RUNNING, 25);
		meleeDefenseMod.put(CreatureLocomotion::KNEELING, -20);
		meleeDefenseMod.put(CreatureLocomotion::CROUCHSNEAKING, 0);
		meleeDefenseMod.put(CreatureLocomotion::CROUCHWALKING, 0);
		meleeDefenseMod.put(CreatureLocomotion::PRONE, -80);
		meleeDefenseMod.put(CreatureLocomotion::CRAWLING, -95);
		meleeDefenseMod.put(CreatureLocomotion::CLIMBINGSTATIONARY, 0);
		meleeDefenseMod.put(CreatureLocomotion::CLIMBING, 0);
		meleeDefenseMod.put(CreatureLocomotion::HOVERING, -5);
		meleeDefenseMod.put(CreatureLocomotion::FLYING, -40);
		meleeDefenseMod.put(CreatureLocomotion::LYINGDOWN, 0);
		meleeDefenseMod.put(CreatureLocomotion::SITTING, 0);
		meleeDefenseMod.put(CreatureLocomotion::SKILLANIMATING, 0);
		meleeDefenseMod.put(CreatureLocomotion::DRIVINGVEHICLE, 0);
		meleeDefenseMod.put(CreatureLocomotion::RIDINGCREATURE, 0);
		meleeDefenseMod.put(CreatureLocomotion::KNOCKEDDOWN, -15);
		meleeDefenseMod.put(CreatureLocomotion::INCAPACITATED, 0);
		meleeDefenseMod.put(CreatureLocomotion::DEAD, 0);
		meleeDefenseMod.put(CreatureLocomotion::BLOCKING, 0);
		meleeDefenseMod.put(CreatureLocomotion::INVALID, 0);
	}

	~CreaturePosture() {}

	uint8 getLocomotion(uint8 pos, uint8 speed) {
		CreatureMovementEntry* move = &movementTable.get(pos);

		switch (speed) {
		case CreatureLocomotion::STATIONARY:
			return move->stationary;
			break;
		case CreatureLocomotion::SLOW:
			return move->slow;
			break;
		case CreatureLocomotion::FAST:
			return move->fast;
			break;
		default:
			return CreatureLocomotion::INVALID;
		}

		return CreatureLocomotion::INVALID;
	}

	uint8 getSpeed(uint8 pos, uint8 loc) {
		CreatureMovementEntry* move = &movementTable.get(pos);

		if (loc == move->stationary)
			return CreatureLocomotion::STATIONARY;

		if (loc == move->slow)
			return CreatureLocomotion::SLOW;

		if (loc == move->fast)
			return CreatureLocomotion::FAST;

		return CreatureLocomotion::INVALID;
	}

	float getMovementScale(uint8 pos) {
		return movementTable.get(pos).movementScale;
	}

	float getAccelerationScale(uint8 pos) {
		return movementTable.get(pos).accelerationScale;
	}

	float getTurnScale(uint8 pos) {
		return movementTable.get(pos).turnScale;
	}

	float getCanSeeHeightMod(uint8 pos) {
		return movementTable.get(pos).canSeeHeightMod;
	}

	void loadMovementData() {
		IffStream* iffStream = TemplateManager::instance()->openIffFile("datatables/movement/movement_human.iff");

		if (iffStream == NULL) {
			error("Could not load movement data.");
			return;
		}

		DataTableIff dtiff;
		dtiff.readObject(iffStream);

		delete iffStream;

		for (int i = 0; i < dtiff.getTotalRows(); i++) {
			DataTableRow* row = dtiff.getRow(i);
			CreatureMovementEntry entry;
			int value;
			float scale;

			row->getValue(0, value);
			if (value < 0) entry.posture = 0xFF;
			else entry.posture = (uint8)value;

			row->getValue(1, value);
			if (value < 0) entry.stationary = 0xFF;
			else entry.stationary = (uint8)value;

			row->getValue(2, value);
			if (value < 0) entry.slow = 0xFF;
			else entry.slow = (uint8)value;

			row->getValue(3, value);
			if (value < 0) entry.fast = 0xFF;
			else entry.fast = (uint8)value;

			row->getValue(4, scale);
			entry.movementScale = scale;
			row->getValue(5, scale);
			entry.accelerationScale = scale;
			row->getValue(6, scale);
			entry.turnScale = scale;
			row->getValue(7, scale);
			entry.canSeeHeightMod = scale;

			movementTable.put(entry.posture, entry);
		}
	}

	float getWeaponLocomotionMod(int weaponObjectType) {
		// this only works because the weapon types that don't have specific SCOTs also don't have mods
		// otherwise, we'd need to get the string and compare off that
		switch (weaponObjectType) {
		case SceneObjectType::RIFLE:
			return 2.5f;
		case SceneObjectType::PISTOL:
			return 1.5f;
		case SceneObjectType::CARBINE:
			return 2.f;
		case SceneObjectType::MINE:
		case SceneObjectType::SPECIALHEAVYWEAPON:
		case SceneObjectType::HEAVYWEAPON:
			return 3.f;
		default:
			return 1.f;
		}
	}

	int getRangedAttackMod(uint loc) {
		return rangedAttackMod.get(loc);
	}

	int getRangedDefenseMod(uint loc) {
		return rangedDefenseMod.get(loc);
	}

	int getMeleeAttackMod(uint loc) {
		return meleeAttackMod.get(loc);
	}

	int getMeleeDefenseMod(uint loc) {
		return meleeDefenseMod.get(loc);
	}
};

#endif /* CREATUREPOSTURE_H_ */
