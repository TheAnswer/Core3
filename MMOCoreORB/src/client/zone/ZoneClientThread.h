/*
 * ZoneClientThread.h
 *
 *  Created on: Sep 4, 2009
 *      Author: theanswer
 */

#ifndef ZONECLIENTTHREAD_H_
#define ZONECLIENTTHREAD_H_

#include "system/lang.h"

class ZoneClient;

class ZoneClientThread : public Thread {
	Reference<ZoneClient*> client;

public:
	ZoneClientThread(ZoneClient* zoneClient);

	~ZoneClientThread();

	void run();

	void stop();
};

#endif /* ZONECLIENTTHREAD_H_ */
