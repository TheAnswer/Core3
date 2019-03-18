/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef STRINGIDCHATPARAMETER_H_
#define STRINGIDCHATPARAMETER_H_

#include "server/chat/ChatParameter.h"
#include "server/zone/objects/scene/variables/StringIdParameter.h"
#include "server/zone/objects/scene/variables/StringId.h"

#include "engine/util/json_utils.h"

namespace server {
namespace zone {
namespace objects {
namespace scene {
namespace variables {
	class StringIdParameter;
}
}
}
}
}

using namespace server::zone::objects::scene::variables;

namespace server {
namespace chat {

class StringIdChatParameter : public ChatParameter, public StringId {
protected:
	StringIdParameter TT;
	StringIdParameter TU;
	StringIdParameter TO;

	uint32 DI;
	float DF;

	byte unknownByte;

	/**
	* Inserts stf string identifier and variables into a packet
	* @pre { packet is a valid Message }
	* @post { data is inserted into packet }
	* @param packet Message that data is to be inserted into
	*/
	void addToPacketStream(Message* packet);

public:
	StringIdChatParameter();
	StringIdChatParameter(const StringId& id);
	StringIdChatParameter(const char * cstr);
	StringIdChatParameter(const String& fullPath);
	StringIdChatParameter(const String& fil, const String& stringId);
	StringIdChatParameter(const StringIdChatParameter& custom);

	bool toBinaryStream(ObjectOutputStream* stream);
	bool parseFromBinaryStream(ObjectInputStream* stream);

	friend void to_json(nlohmann::json& j, const StringIdChatParameter& p);

	virtual String toString() const {
		return StringId::getFullPath();
	}

	StringIdChatParameter& operator=(const StringIdChatParameter& id) {
		if (this == &id)
			return *this;

		TT = id.TT;
		TU = id.TU;
		TO = id.TO;

		DI = id.DI;
		DF = id.DF;

		unknownByte = id.unknownByte;

		ChatParameter::operator=(id);
		StringId::operator=(id);

		return *this;
	}

	void parse(Message* message);

	/**
	* Sets the TT variable
	* @pre { obj is one of the following:
	* 		String, UnicodeString, pointer to SceneObject that is in the client's memory,
	* 		objectid of SceneObject that is in the client's memory, StringId, pointer to StringId
	*  	   }
	* @post { TT variable is set }
	* @param obj Data that the TT variable is to be set as
	*/
	template<class T>
	inline void setTT(const T& obj) {
		TT.set(obj);
	}

	/**
	* Sets the TT variable of this stf string to another stf string
	* @pre { }
	* @post { TT variable is set }
	* @param file stf file name that contains the stf string
	* @param stringid stringid of stf string
	*/
	inline void setTT(const String& file, const String& stringid) {
		TT.set(file, stringid);
	}

	/**
	* Sets the TU variable of a stf string
	* @pre { obj is one of the following:
	* 		String, UnicodeString, pointer to SceneObject that is in the client's memory,
	* 		objectid of SceneObject that is in the client's memory, StringId, pointer to StringId
	*  	   }
	* @post { TU variable is set }
	* @param obj Data that the TU variable is to be set as
	*/
	template<class T>
	inline void setTU(const T& obj) {
		TU.set(obj);
	}

	/**
	* Sets the TU variable of this stf string to another stf string
	* @pre { }
	* @post { TU variable is set }
	* @param file stf file name that contains the stf string
	* @param stringid stringid of stf string
	*/
	inline void setTU(const String& file, const String& stringid) {
		TU.set(file, stringid);
	}

	/**
	* Sets the TO variable of a stf string
	* @pre { obj is one of the following:
	* 		String, UnicodeString, pointer to SceneObject that is in the client's memory,
	* 		objectid of SceneObject that is in the client's memory, StringId, pointer to StringId
	*  	   }
	* @post { TO variable is set }
	* @param obj Data that the TO variable is to be set as
	*/
	template<class T>
	inline void setTO(const T& obj) {
		TO.set(obj);
	}

	/**
	* Sets the TO variable of this stf string to another stf string
	* @pre { }
	* @post { TO variable is set }
	* @param file stf file name that contains the stf string
	* @param stringid stringid of stf string
	*/
	inline void setTO(const String& file, const String& stringid) {
		TO.set(file, stringid);
	}

	/**
	* Sets the DI variable
	* @pre { }
	* @post { DI variable is set }
	* @param i value for DI
	*/
	inline void setDI(uint32 i) {
		DI = i;
	}

	/**
	* Sets the DF variable
	* @pre { }
	* @post { DF variable is set }
	* @param f value for DF
	*/
	inline void setDF(float f) {
		DF = f;
	}

	inline void setUnknownByte(uint16 val) {
		unknownByte = val;
	}

	StringIdParameter* getTT() {
		return &TT;
	}

	StringIdParameter* getTU() {
		return &TU;
	}

	StringIdParameter* getTO() {
		return &TO;
	}
};

}
}

using namespace server::chat;

#endif /* STRINGIDCHATPARAMETER_H_ */
