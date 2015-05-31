/*
 * GarageInstallationImplementation.cpp
 *
 *  Created on: Dec 6, 2010
 *      Author: crush
 */



#include "server/zone/objects/installation/garage/GarageInstallation.h"
#include "server/zone/objects/area/ActiveArea.h"
#include "server/zone/ZoneServer.h"
#include "server/zone/Zone.h"

void GarageInstallationImplementation::createChildObjects() {
	float positionX = coordinates.getPositionX();
	float positionY = coordinates.getPositionY();
	float positionZ = coordinates.getPositionZ();

	uint32 garageAreaTemplateCRC = STRING_HASHCODE("object/garage_area.iff");
	ManagedReference<SceneObject*> obj = server->getZoneServer()->createObject(garageAreaTemplateCRC, 1);

	if (obj == NULL)
		return;

	ActiveArea* activeArea = cast<ActiveArea*>( obj.get());

	if (activeArea == NULL)
		return;

	Locker clocker(activeArea, _this.getReferenceUnsafeStaticCast());

	activeArea->setRadius(64);
	activeArea->initializePosition(getPositionX(), getPositionZ(), getPositionY());
	//activeArea->insertToZone(getZone());
	getZone()->transferObject(activeArea, -1, true);

	garageArea = activeArea;

	TangibleObjectImplementation::createChildObjects();
}

void GarageInstallationImplementation::notifyRemoveFromZone() {
	InstallationObjectImplementation::notifyRemoveFromZone();

	Reference<ActiveArea* > garageArea = this->garageArea;

	if (garageArea != NULL) {
		EXECUTE_TASK_1(garageArea, {
				Locker locker(garageArea_p);

				garageArea_p->destroyObjectFromWorld(false);
		});
	}
}

void GarageInstallationImplementation::destroyObjectFromDatabase(bool destroyContainedObjects) {
	InstallationObjectImplementation::destroyObjectFromDatabase(destroyContainedObjects);

	if (!destroyContainedObjects)
		return;

	if (garageArea != NULL)
		garageArea->destroyObjectFromDatabase(true);
}
