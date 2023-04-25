#ifndef SHIPDELTAVECTOR_H_
#define SHIPDELTAVECTOR_H_

#include "server/zone/objects/scene/SceneObject.h"
#include "server/zone/packets/DeltaMessage.h"

class ShipDeltaVector : public Object {
protected:
	VectorMap<uint32, DeltaMessage*> deltaVector;

	uint64 objectID;
	uint64 playerID;

	enum DeltaTypeID : uint32 {
		None	= 0,
		Delta1	= 1,
		Delta3	= 3,
		Delta4	= 4,
		Delta6	= 6,
	};

	enum DeltaType : uint32 {
		Private	= 1,
		Public	= 2,
	};

	int getDeltaType(uint32 deltaID) {
		switch (deltaID) {
			case DeltaTypeID::Delta1: return DeltaType::Private;
			case DeltaTypeID::Delta3: return DeltaType::Public;
			case DeltaTypeID::Delta4: return DeltaType::Private;
			case DeltaTypeID::Delta6: return DeltaType::Public;
		}

		return DeltaTypeID::None;
	}

public:
	ShipDeltaVector(SceneObject* ship, SceneObject * player = nullptr) : Object() {
		deltaVector.setNoDuplicateInsertPlan();
		deltaVector.setNullValue(nullptr);

		objectID = ship->getObjectID();
		playerID = player != nullptr ? player->getObjectID() : 0;
	}

	~ShipDeltaVector() {
		reset();
	}

	void reset(SceneObject* player = nullptr) {
		if (player != nullptr && player->getObjectID() != playerID) {
			playerID = player->getObjectID();
		}

		for (int i = 0; i < deltaVector.size(); ++i) {
			auto message = deltaVector.elementAt(i).getValue();
			if (message == nullptr) {
				continue;
			}

			delete message;
			message = nullptr;
		}

		deltaVector.removeAll();
	}

	DeltaMessage* getMessage(uint32 deltaID) {
		int deltaType = getDeltaType(deltaID);
		if (deltaType == DeltaTypeID::None || (deltaType == DeltaType::Private && playerID == DeltaTypeID::None)) {
			return nullptr;
		}

		auto delta = deltaVector.get(deltaID);
		if (delta != nullptr) {
			return delta;
		}

		delta = new DeltaMessage(objectID, 'SHIP', deltaID);
		deltaVector.put(deltaID, delta);

		return delta;
	}

	void sendMessages(SceneObject* ship, SceneObject* player = nullptr) {
		for (int i = 0; i < deltaVector.size(); ++ i) {
			auto deltaID = deltaVector.elementAt(i).getKey();
			auto message = deltaVector.elementAt(i).getValue();

			if (message == nullptr) {
				continue;
			}

			message->close();

			int deltaType = getDeltaType(deltaID);

			if (deltaType == DeltaType::Private && player != nullptr) {
				player->sendMessage(message->clone());
			} else if (deltaType == DeltaType::Public) {
				ship->broadcastMessage(message->clone(), false);
			}
		}

		reset(player);
	}

	int size() {
		return deltaVector.size();
	}
};

#endif // SHIPDELTAVECTOR_H_
