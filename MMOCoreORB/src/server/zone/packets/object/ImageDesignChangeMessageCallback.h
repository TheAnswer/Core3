/*
 * ImageDesignMessageCallback.h
 *
 *  Created on: 02/02/2011
 *      Author: Polonel
 */

#ifndef IMAGEDESIGNCHANGEMESSAGECALLBACK_H_
#define IMAGEDESIGNCHANGEMESSAGECALLBACK_H_

#include "ObjectControllerMessageCallback.h"
#include "ImageDesignMessage.h"
#include "server/zone/objects/player/sessions/ImageDesignSession.h"
#include "server/zone/objects/player/sessions/ImageDesignData.h"

class ImageDesignChangeMessageCallback : public MessageCallback {
	uint64 designerID;
	uint64 targetID;
	uint64 tentID;
	uint8 type;

	ObjectControllerMessageCallback* objectControllerMain;

	ImageDesignData imageDesignData;


public:
	ImageDesignChangeMessageCallback(ObjectControllerMessageCallback* objectControllerCallback) :
		MessageCallback(objectControllerCallback->getClient(), objectControllerCallback->getServer()),
		designerID(0), targetID(0), tentID(0), type(0), objectControllerMain(objectControllerCallback) {

	}

	void parse(Message* message) {
		message->shiftOffset(4); // ?Not sure this should be here.

		designerID = message->parseLong();

		//System::out << "designerID " << designerID << "\n";
		targetID = message->parseLong();

		//System::out << "targetID " << targetID << "\n";
		tentID = message->parseLong();
		type = message->parseByte();

		//System::out << "unknown byte shit " << type << "\n";

		imageDesignData.parse(message);
	}

	void run() {
		ManagedReference<CreatureObject*> player = static_cast<CreatureObject*>(client->getPlayer().get().get());

		if (player == NULL)
			return;

		ManagedReference<Facade*> facade = player->getActiveSession(SessionFacadeType::IMAGEDESIGN);
		ManagedReference<ImageDesignSession*> session = dynamic_cast<ImageDesignSession*>(facade.get());

		if (session == NULL)
			return;

		session->updateImageDesign(player, designerID, targetID, tentID, type, imageDesignData);
	}


};


#endif /* IMAGEDESIGNCHANGEMESSAGECALLBACK_H_ */
