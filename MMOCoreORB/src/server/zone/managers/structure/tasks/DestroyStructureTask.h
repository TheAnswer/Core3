/*
 * DestroyStructureTask.h
 *
 *  Created on: Jun 29, 2013
 *      Author: TheAnswer
 */

#ifndef DESTROYSTRUCTURETASK_H_
#define DESTROYSTRUCTURETASK_H_

#include "engine/engine.h"
#include "server/zone/objects/structure/StructureObject.h"
#include "server/zone/objects/cell/CellObject.h"
#include "server/zone/objects/tangible/components/vendor/VendorDataComponent.h"

class DestroyStructureTask : public Task {
protected:
	ManagedReference<StructureObject*> structureObject;

public:
	DestroyStructureTask(StructureObject* structure) {
		structureObject = structure;
	}

	void run() {
		Locker locker(structureObject);

		ManagedReference<Zone*> zone = structureObject->getZone();

		if (zone == NULL)
			return;

		ZoneServer* zoneServer = structureObject->getZoneServer();

		if (zoneServer != NULL && zoneServer->isServerLoading()) {
			schedule(1000);

			return;
		}

		float x = structureObject->getPositionX();
		float y = structureObject->getPositionY();
		float z = zone->getHeight(x, y);

		if (structureObject->isBuildingObject()) {
			ManagedReference<BuildingObject*> buildingObject =
					cast<BuildingObject*>(structureObject.get());

			for (uint32 i = 1; i <= buildingObject->getTotalCellNumber(); ++i) {
				ManagedReference<CellObject*> cellObject = buildingObject->getCell(
						i);

				int childObjects = cellObject->getContainerObjectsSize();

				if (cellObject == NULL || childObjects <= 0)
					continue;

				//Traverse the vector backwards since the size will change as objects are removed.
				for (int j = childObjects - 1; j >= 0; --j) {
					ManagedReference<SceneObject*> obj =
							cellObject->getContainerObject(j);

					if (obj->isPlayerCreature()) {
						CreatureObject* playerCreature =
								cast<CreatureObject*>(obj.get());

						structureObject->unlock();

						try {
							Locker plocker(playerCreature);

							playerCreature->teleport(x, z, y, 0);
						} catch (...) {
							playerCreature->error("unreported exception caught while teleporting");
						}

						structureObject->wlock();
					} else if (obj->isVendor()) {
						Reference<VendorDataComponent*> vendorData = cast<VendorDataComponent*>(obj->getDataObjectComponent());
						ManagedReference<SceneObject*> vendorOwner = zone->getZoneServer()->getObject(vendorData->getOwnerId());

						if (vendorOwner != NULL) {
							ManagedReference<SceneObject*> vendorOwnersGhost = vendorOwner->getSlottedObject("ghost");

							if (vendorOwnersGhost != NULL && vendorOwnersGhost->isPlayerObject()) {
								PlayerObject* playo = cast<PlayerObject*>(vendorOwnersGhost.get());

								structureObject->unlock();

								try {
									Locker plocker(playo);

									playo->removeVendor(obj);
								} catch (...) {
									playo->error("unreported exception caught while removing vendor");
								}

								structureObject->wlock();
							}
						}
					}
				}
			}

		}

		//Get the owner of the structure, and remove the structure from their possession.
		ManagedReference<SceneObject*> owner = zone->getZoneServer()->getObject(
				structureObject->getOwnerObjectID());

		if (owner != NULL) {
			ManagedReference<SceneObject*> ghost = owner->getSlottedObject("ghost");

			if (ghost != NULL && ghost->isPlayerObject()) {
				PlayerObject* playerObject = cast<PlayerObject*>(ghost.get());
				playerObject->removeOwnedStructure(structureObject);
			}
		}

		structureObject->destroyObjectFromWorld(true);
		structureObject->destroyObjectFromDatabase(true);
		structureObject->notifyObservers(ObserverEventType::OBJECTDESTRUCTION, structureObject, 0);
	}
};

#endif /* DESTROYSTRUCTURETASK_H_ */

