/*
 * ZoneClientThread.cpp
 *
 *  Created on: Sep 4, 2009
 *      Author: theanswer
 */

#include "ZoneClientThread.h"

#include <stddef.h>

#include "ZoneClient.h"
#include "engine/service/proto/BaseClient.h"

ZoneClientThread::ZoneClientThread(ZoneClient* zoneClient) : Thread() {
	client = zoneClient;

	setDetached();
}

ZoneClientThread::~ZoneClientThread() {
	client = NULL;

#ifdef WITH_STM
	TransactionalMemoryManager::closeThread();
#endif
}

void ZoneClientThread::run() {
	client->getClient()->recieveMessages();
}

void ZoneClientThread::stop() {
	client->getClient()->stop();
}
