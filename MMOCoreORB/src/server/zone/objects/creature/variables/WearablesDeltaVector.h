/*
 * WearablesDeltaVector.h
 *
 *  Created on: 09/04/2012
 *      Author: victor
 */

#ifndef WEARABLESDELTAVECTOR_H_
#define WEARABLESDELTAVECTOR_H_

#include "engine/engine.h"

#include "server/zone/objects/scene/variables/DeltaVector.h"
#include "server/zone/objects/tangible/TangibleObject.h"
#include "server/zone/objects/tangible/wearables/ArmorObject.h"

class WearablesDeltaVector : public DeltaVector<ManagedReference<TangibleObject*> > {
protected:
	VectorMap<uint8, Vector<ManagedReference<ArmorObject*> > > protectionArmorMap;

public:

	// hit locations (for serverside armor checks)
	enum SuitLocations { NOLOCATION = 0x0, CHEST = 0x1, ARMS = 0x2, LEGS = 0x4, HEAD = 0x8 };


	WearablesDeltaVector() : DeltaVector<ManagedReference<TangibleObject*> >() {
		protectionArmorMap.setAllowOverwriteInsertPlan();

		addSerializableVariable("protectionArmorMap", &protectionArmorMap);
	}

	void insertItemToMessage(ManagedReference<TangibleObject*>* item, BaseMessage* msg) {
		TangibleObject* object = item->get();

		String custString;
		object->getCustomizationString(custString);

		msg->insertAscii(custString);
		msg->insertInt(object->getContainmentType()); //Equipped
		msg->insertLong(object->getObjectID()); //object id
		msg->insertInt(object->getClientObjectCRC()); //CRC of the object
	}

	bool add(ManagedReference<TangibleObject*> element, DeltaMessage* message = NULL, int updates = 1) {
		if (element->isArmorObject()) {
			ManagedReference<ArmorObject*> armor = cast<ArmorObject*>(element.get());
			uint8 hitLocations = armor->getHitLocation();

			if (hitLocations & CHEST)
				addArmor(CHEST, armor);

			if (hitLocations & ARMS)
				addArmor(ARMS, armor);

			if (hitLocations & LEGS)
				addArmor(LEGS, armor);

			if (hitLocations & HEAD)
				addArmor(HEAD, armor);
		}

		return DeltaVector<ManagedReference<TangibleObject*> >::add(element, message, updates);
	}

	bool remove(int index, DeltaMessage* message = NULL, int updates = 1) {
		ManagedReference<TangibleObject*> element = get(index);

		if (element->isArmorObject()) {
			ManagedReference<ArmorObject*> armor = cast<ArmorObject*>(element.get());
			uint8 hitLocations = armor->getHitLocation();

			if (hitLocations & CHEST)
				removeArmor(CHEST, armor);

			if (hitLocations & ARMS)
				removeArmor(ARMS, armor);

			if (hitLocations & LEGS)
				removeArmor(LEGS, armor);

			if (hitLocations & HEAD)
				removeArmor(HEAD, armor);
		}

		return DeltaVector<ManagedReference<TangibleObject*> >::remove(index, message, updates);
	}


	Vector<ManagedReference<ArmorObject*> > getArmorAtHitLocation(uint8 hl) {

		// TODO: Migrate and remove this when the object versioning and migration system is in place

		// HIT_LOCATION has a circular dependency nightmare with CombatManager and CreatureObject
		switch(hl) {
		case 1: // HIT_BODY
			return protectionArmorMap.get((uint8)CHEST); // CHEST
		case 2: // HIT_LARM
		case 3: // HIT_RARM
		{
			Vector<ManagedReference<ArmorObject*> > armArmor = protectionArmorMap.get((uint8)ARMS); // ARMS
			Vector<ManagedReference<ArmorObject*> > armorAtLocation;

			if(armArmor.isEmpty())
				return armArmor;

			if(hl == 2) {
				for(int i=armArmor.size()-1; i>=0; i--) {
					ArmorObject *obj = armArmor.get(i);
					if(obj->hasArrangementDescriptor("bicep_l") || obj->hasArrangementDescriptor("bracer_upper_l") || obj->hasArrangementDescriptor("gloves"))
						armorAtLocation.add(obj);
				}
			} else {
				for(int i=armArmor.size()-1; i>=0; i--) {
					ArmorObject *obj = armArmor.get(i);

					if(obj->hasArrangementDescriptor("bicep_r") || obj->hasArrangementDescriptor("bracer_upper_r") || obj->hasArrangementDescriptor("gloves"))
						armorAtLocation.add(obj);
				}
			}

			if(armorAtLocation.isEmpty())
				return armArmor;
			else
				return armorAtLocation;
		}
		case 4: // HIT_LLEG
		case 5: // HIT_RLEG
			return protectionArmorMap.get((uint8)LEGS); // LEGS
		case 6: // HIT_HEAD
			return protectionArmorMap.get((uint8)HEAD); // HEAD
		}

		return protectionArmorMap.get((uint8)NOLOCATION);
	}



	void addArmor(uint8 hitLocation, ManagedReference<ArmorObject*> armor) {
		Vector<ManagedReference<ArmorObject*> > armors = protectionArmorMap.get(hitLocation);
		armors.add(armor);

		protectionArmorMap.drop(hitLocation);
		protectionArmorMap.put(hitLocation, armors);
	}

	void removeArmor(uint8 hitLocation, ManagedReference<ArmorObject*> armor) {
		Vector<ManagedReference<ArmorObject*> > armors = protectionArmorMap.get(hitLocation);
		armors.removeElement(armor);

		protectionArmorMap.drop(hitLocation);
		protectionArmorMap.put(hitLocation, armors);
	}
};


#endif /* WEARABLESDELTAVECTOR_H_ */
