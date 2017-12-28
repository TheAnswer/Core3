/*
 * 				Copyright <SWGEmu>
		See file COPYING for copying conditions. */

#ifndef DROIDEFFECTSMODULEDATACOMPONENT_H_
#define DROIDEFFECTSMODULEDATACOMPONENT_H_

#include <algorithm>

#include "BaseDroidModuleComponent.h"
#include "system/lang/String.h"
#include "system/platform.h"
#include "system/util/Vector.h"
#include "system/util/VectorMap.h"

namespace server {
namespace zone {
namespace objects {
namespace creature {
class CreatureObject;
}  // namespace creature
namespace intangible {
class PetControlDevice;
}  // namespace intangible
namespace scene {
class SceneObject;
}  // namespace scene
}  // namespace objects
namespace packets {
namespace object {
class ObjectMenuResponse;
}  // namespace object
namespace scene {
class AttributeListMessage;
}  // namespace scene
}  // namespace packets
}  // namespace zone
}  // namespace server
namespace sys {
namespace io {
class ObjectInputStream;
class ObjectOutputStream;
}  // namespace io
}  // namespace sys

namespace server {
namespace zone {
namespace objects {
namespace tangible {
namespace components {
namespace droid {

class DroidEffectsModuleDataComponent : public BaseDroidModuleComponent {

protected:
	bool active;

	// Key:   Effect Name
	// Value: Effect Animation
	VectorMap<String,String> installedEffects;

	Vector<String> configuredEffects; // Effect name
	Vector<int> configuredDelays; // Delay in seconds

	int currentEffectIndex;

public:
	DroidEffectsModuleDataComponent();
	~DroidEffectsModuleDataComponent();
	String getModuleName();
	void initializeTransientMembers();
	void fillAttributeList(AttributeListMessage* msg, CreatureObject* droid);
	void fillObjectMenuResponse(SceneObject* droidObject, ObjectMenuResponse* menuResponse, CreatureObject* player);
	int handleObjectMenuSelect(CreatureObject* player, byte selectedID, PetControlDevice* controller);
	int getBatteryDrain();
	void deactivate();
	String toString();
	void onCall();
	void onStore();
	bool isActive(){ return active; }
	virtual bool isStackable() { return true; }
	virtual void addToStack(BaseDroidModuleComponent* other);
	bool nextEffect();
	String getCurrentAnimation();
	int getCurrentDelay();
	void setEffect( String effectName, int delay, int slotIndex );
	bool toBinaryStream(ObjectOutputStream* stream);
	bool parseFromBinaryStream(ObjectInputStream* stream);

private:

	int writeObjectMembers(ObjectOutputStream* stream);
	bool readObjectMember(ObjectInputStream* stream, const String& name);
};

} // droid
} // components
} // tangible
} // objects
} // zone
} // server
using namespace server::zone::objects::tangible::components::droid;

#endif /* DROIDAUTOREPAIRMODULEDATACOMPONENT_H_ */
