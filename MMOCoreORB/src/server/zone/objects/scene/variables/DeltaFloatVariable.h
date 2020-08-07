/*
 * DeltaFloatVariable.h
 *
 *  Created on: 18/10/2013
 *      Author: victor
 */

#ifndef DELTAFLOATVARIABLE_H_
#define DELTAFLOATVARIABLE_H_

#include "DeltaBasicVariable.h"

#include "server/zone/packets/DeltaMessage.h"

#include "server/zone/objects/scene/SceneObject.h"

template<int BaselineName, uint8 Type, int DeltaID>
class DeltaFloatVariable : public DeltaBasicVariable<float> {
public:
	void update(int newValue, bool broadcastStandalone = false, SceneObject* obj = nullptr) {
		set(newValue);

		if (broadcastStandalone) {
			broadcastStandaloneDeltaMessage(obj);
		}
	}
	// brakiss: ToDo: check if this was restructured and needs to be changed to fit the new structure
    void update(int newValue, bool broadcastStandalone = false, SceneObject* obj = NULL, DeltaMessage* message = NULL) {
        set(newValue);

        if (message) {
            addToDeltaMessage(message);
        } else if (broadcastStandalone) {
            broadcastStandaloneDeltaMessage(obj);
        }
    }

	void broadcastStandaloneDeltaMessage(SceneObject* obj) {
		DeltaMessage* msg = new DeltaMessage(obj->getObjectID(), BaselineName, Type);
		addToDeltaMessage(msg);
		msg->close();

		obj->broadcastMessage(msg, true);
	}

	void addToDeltaMessage(DeltaMessage* msg) {
		msg->addFloatUpdate(DeltaID, object);
	}
};


#endif /* DELTAFLOATVARIABLE_H_ */
