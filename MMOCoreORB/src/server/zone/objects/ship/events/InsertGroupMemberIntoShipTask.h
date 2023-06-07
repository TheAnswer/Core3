
#ifndef INSERTGROUPMEMBERINTOSHIPTASK_H_
#define INSERTGROUPMEMBERINTOSHIPTASK_H_

#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/ship/ShipObject.h"
#include "server/zone/objects/ship/PobShipObject.h"
#include "server/zone/SpaceZone.h"

class InsertGroupMembertIntoShipTask : public Task, public Logger {
	ManagedWeakReference<PobShipObject*> pobObject;
	uint64 memberID;

public:
	InsertGroupMembertIntoShipTask(PobShipObject* shipObj, uint64 memID) : pobObject(shipObj), memberID(memID) {
		setLoggingName("InsertPlayerToShipTask");
	}

	void run() {
		auto pobShip = pobObject.get();

		if (pobShip == nullptr)
			return;

		auto spaceZone = pobShip->getSpaceZone();

		if (spaceZone == nullptr) {
			return;
		}

		auto zoneServer = pobShip->getZoneServer();

		if (zoneServer == nullptr)
			return;

		auto memberObject = zoneServer->getObject(memberID).get();

		if (memberObject == nullptr || !memberObject->isPlayerCreature())
			return;

		CreatureObject* groupMember = memberObject->asCreatureObject();

		if (groupMember == nullptr)
			return;

		Locker lock(pobShip);

		String randomCell = pobShip->getRandomLaunchCell();
		Vector3 launchLoc(pobShip->getLaunchPointInCell(randomCell));
		auto cell = pobShip->getCell(randomCell);

		if (cell == nullptr)
			return;

		Locker clock(groupMember, pobShip);

		groupMember->switchZone(spaceZone->getZoneName(), launchLoc.getX(), launchLoc.getZ(), launchLoc.getY(), cell->getObjectID());
		groupMember->setState(CreatureState::SHIPINTERIOR);
	}
};

#endif /* INSERTGROUPMEMBERINTOSHIPTASK_H_ */
