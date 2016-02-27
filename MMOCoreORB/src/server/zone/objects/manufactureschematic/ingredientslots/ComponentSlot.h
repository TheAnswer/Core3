/*
 				Copyright <SWGEmu>
		See file COPYING for copying conditions. */

#ifndef COMPONENTSLOT_H_
#define COMPONENTSLOT_H_

#include "IngredientSlot.h"
#include "server/zone/managers/object/ObjectManager.h"
#include "server/zone/objects/factorycrate/FactoryCrate.h"
#include "server/zone/packets/tangible/TangibleObjectDeltaMessage3.h"

class ComponentSlot: public IngredientSlot {

	/// Indexed by <object, parent>
	Vector<ManagedReference<TangibleObject*> > contents;


public:
	ComponentSlot() : IngredientSlot() {

		clientSlotType = 2;
	}

	ComponentSlot(const ComponentSlot& slot) : Object(), IngredientSlot(slot) {
		contents = slot.contents;

		setLoggingName("ComponentSlot");
		setLogging(false);
	}

	~ComponentSlot() {

	}

	Object* clone() {
		return new ComponentSlot(*this);
	}

	bool add(CreatureObject* player, SceneObject* satchel, ManagedReference<TangibleObject*> incomingTano) {

		int currentQuantity = getSlotQuantity();
		FactoryCrate* crate = NULL;

		/// If Full, don't add
		if (currentQuantity >= requiredQuantity)
			return false;

		/// Get template
		Reference<SharedObjectTemplate*> baseTemplate = incomingTano->getObjectTemplate();

		/// Get prototype's template for check
		if (incomingTano->isFactoryCrate()) {

			crate = cast<FactoryCrate*>(incomingTano.get());

			TangibleObject* prototype = crate->getPrototype();

			if (prototype == NULL) {
				error("Factory crate doesn't contain a prototype");
				return false;
			}

			/// Check if prototype is correct type
			baseTemplate = prototype->getObjectTemplate();
		}

		/// Check types
		if (!baseTemplate->isDerivedFrom(contentType))
			return false;

		// Serial Number check
		if (requiresIdentical() && !contents.isEmpty()) {
			TangibleObject* tano = contents.elementAt(0);

			if(tano == NULL) {
				error("Null items in contents when checking serial number");
				return false;
			}

			if (crate != NULL && crate->getPrototype()->getSerialNumber() != tano->getSerialNumber()) {
				return false;
			} else {

				if (incomingTano->getSerialNumber() != tano->getSerialNumber())
					return false;
			}
		}

		/// How much do we need
		int slotNeeds = requiredQuantity - currentQuantity;

		/// Extract tano from crate and set it to the incoming object
		if (crate != NULL) {

			if (crate->getUseCount() >= slotNeeds)
				incomingTano = crate->extractObject(slotNeeds);
			else
				incomingTano = crate->extractObject(crate->getUseCount());
		}

		if(incomingTano == NULL) {
			error("Incoming object is NULL");
			return false;
		}

		incomingTano->sendAttributeListTo(player);

		ObjectManager* objectManager = ObjectManager::instance();
		ManagedReference<TangibleObject*> itemToUse = NULL;

		if(incomingTano->getUseCount() > slotNeeds) {

			incomingTano->decreaseUseCount(slotNeeds);

			itemToUse = cast<TangibleObject*>( objectManager->cloneObject(incomingTano));

			Locker ilocker(itemToUse);

			if (itemToUse->hasAntiDecayKit()) {
				itemToUse->removeAntiDecayKit();
			}

			itemToUse->setUseCount(slotNeeds, false);
			itemToUse->setParent(NULL);
			itemToUse->sendAttributeListTo(player);

		} else {

			itemToUse = incomingTano;
		}

		Vector<ManagedReference<TangibleObject*> > itemsToAdd;

		itemsToAdd.add(itemToUse);
		while(itemToUse->getUseCount() > 1) {

			ManagedReference<TangibleObject*> newTano = cast<TangibleObject*>( objectManager->cloneObject(itemToUse));

			Locker ilocker(newTano);

			if (newTano->hasAntiDecayKit()) {
				newTano->removeAntiDecayKit();
			}

			newTano->setParent(NULL);
			newTano->setUseCount(1, false);
			itemsToAdd.add(newTano);

			ilocker.release();

			Locker itemToUseLocker(itemToUse);

			itemToUse->decreaseUseCount();
		}

		while(itemsToAdd.size() > 0) {
			ManagedReference<TangibleObject*> tano = itemsToAdd.remove(0);
			if(!satchel->transferObject(tano, -1, true)) {
				error("cant transfer crafting component Has Items: " + String::valueOf(satchel->getContainerObjectsSize()));
				return false;
			}

			contents.add(tano);
		}

		return true;
	}

	bool returnToParents(CreatureObject* player) {

		for(int i = 0; i < contents.size(); ++i) {
			TangibleObject* object = contents.get(i);

			if(object == NULL) {
				warning("Can't return object, object is null");
				continue;
			}

			SceneObject* parent = player->getSlottedObject("inventory");

			if(parent == NULL) {
				warning("Can't return object, inventory is null");
				continue;
			}

			parent->transferObject(object, -1, true, true);
			parent->broadcastObject(object, true);
		}

		contents.removeAll();

		return true;
	}


	int getSlotQuantity() {
		int quantity = 0;
		for(int i = 0; i < contents.size(); ++i) {
			TangibleObject* tano =  contents.elementAt(i);
			if(tano != NULL)
				quantity += tano->getUseCount();
		}
		return quantity;
	}

	bool isFull() {
		return requiredQuantity == getSlotQuantity();
	}

	bool isEmpty() {
		return getSlotQuantity() == 0;
	}

	TangibleObject* getPrototype() {

		if(contents.isEmpty())
			return NULL;

		return contents.elementAt(0);
	}

	SceneObject* getFactoryIngredient() {
		return getPrototype();
	}

	bool isComponentSlot() {
		return true;
	}

	Vector<uint64> getOIDVector() {
		Vector<uint64> oid;
		for(int i = 0; i < getSlotQuantity(); ++i) {
			oid.add(getPrototype()->getObjectID());
		}
		return oid;
	}

	// We add 1 for each item in the slot
	// NOT the quantity of each item, it's a vector
	// of 1's that equals how many items are in the slot
	Vector<int> getQuantityVector() {
		Vector<int> quant;
		for(int i = 0; i < getSlotQuantity(); ++i) {
			quant.add(1);
		}
		return quant;
	}
};
#endif /*COMPONENTSLOT_H_*/
