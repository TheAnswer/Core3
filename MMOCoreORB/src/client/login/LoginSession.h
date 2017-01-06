/*
 * LoginSession.h
 *
 *  Created on: Sep 6, 2009
 *      Author: theanswer
 */

#ifndef LOGINSESSION_H_
#define LOGINSESSION_H_

#include "system/lang.h"
#include "engine/log/Logger.h"

class LoginClient;

class LoginSession : public Mutex, public Runnable, public Logger, public Object {
	Condition sessionFinalized;

	uint32 accountID;
	uint32 sessionID;

	Vector<uint64> characterObjectIds;
	int selectedCharacter;

	int instance;

	class LoginClientThread* loginThread;

	Reference<LoginClient*> login;

public:
	LoginSession(int instance);

	~LoginSession();

	void run();

	void addCharacter(uint64 objectID) {
		characterObjectIds.add(objectID);
	}

	void setSelectedCharacter(int id) {
		lock();

		selectedCharacter = id;

		sessionFinalized.signal(this);

		unlock();
	}

	void setAccountID(uint32 id) {
		accountID = id;
	}

	void setSessionID(uint32 id) {
		sessionID = id;
	}

	uint32 getAccountID() {
		return accountID;
	}

	uint32 getSessionID() {
		return sessionID;
	}

	int getSelectedCharacter() {
		return selectedCharacter;
	}

	uint64 getCharacterObjectID(uint32 id) {
		return characterObjectIds.get(id);
	}
};


#endif /* LOGINSESSION_H_ */
