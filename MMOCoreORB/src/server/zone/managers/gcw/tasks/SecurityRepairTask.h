#ifndef SECURITYREPAIRTASK_H_
#define SECURITYREPAIRTASK_H_

#include "server/zone/managers/gcw/GCWManager.h"

class SecurityRepairTask : public Task {

	ManagedReference<TangibleObject*> terminal;
	ManagedReference<GCWManager*> gcwManager;
	ManagedReference<CreatureObject*> creature;
	int count;

public:

	SecurityRepairTask(GCWManager* gcwMan, TangibleObject* term, CreatureObject* player, int cnt) {
		gcwManager = gcwMan;
		terminal = term;
		creature = player;
		count = cnt;
	}

	void run() {

		if (terminal == nullptr || gcwManager == nullptr || creature == nullptr)
			return;

		ManagedReference<BuildingObject*> building = terminal->getParentRecursively(SceneObjectType::FACTIONBUILDING).castTo<BuildingObject*>();

		if (building == nullptr)
			return;

		DataObjectComponentReference* data = building->getDataObjectComponent();

		if (data == nullptr)
			return;

		DestructibleBuildingDataComponent* baseData = cast<DestructibleBuildingDataComponent*>(data->get());

		if (baseData == nullptr)
			return;

		if (creature->isDead() || creature->isIncapacitated()) {
			creature->sendSystemMessage("You cannot repair the terminal in your current state. Aborting repairs...");
			baseData->setTerminalBeingRepaired(false);
			return;
		} else if (creature->isInCombat()) {
			creature->sendSystemMessage("You cannot slice the terminal while you are in combat!");
			baseData->setTerminalBeingRepaired(false);
			return;
		} else if (creature->getParentID() != terminal->getParentID()) {
			creature->sendSystemMessage("You cannot repair the terminal from that distance. Aborting repairs...");
			baseData->setTerminalBeingRepaired(false);
			return;
		}

		int maxDecrease = 1;

		if (creature->hasSkill("combat_smuggler_slicing_04"))
			maxDecrease = 4;
		else if (creature->hasSkill("combat_smuggler_slicing_03"))
			maxDecrease = 3;
		else if (creature->hasSkill("combat_smuggler_slicing_02"))
			maxDecrease = 2;

		count -= System::random(maxDecrease - 1) + 1;

		if (count <= 0) {
			creature->sendSystemMessage("It appears that the terminal has been repaired. You may begin slicing again.");

			Locker locker(building);

			baseData->setTerminalDamaged(false);
			baseData->setTerminalBeingRepaired(false);
		} else {
			creature->sendSystemMessage("Security terminal repairs continue...");
			reschedule(5000);
		}
	}
};

#endif /* SECURITYREPAIRTASK_H_ */
