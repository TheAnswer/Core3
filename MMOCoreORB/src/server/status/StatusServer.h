/*
 				Copyright <SWGEmu>
		See file COPYING for copying conditions. */

#ifndef STATUSSERVER_H_
#define STATUSSERVER_H_

#include "engine/engine.h"

#include "server/zone/ZoneServer.h"

#include "conf/ConfigManager.h"

class StatusHandler;

class StatusServer: public StreamServiceThread {
	ZoneServer* zoneServer;
	StatusHandler* statusHandler;

	ConfigManager* configManager;

	unsigned int statusInterval;

	Time timestamp;
	bool lastStatus;

public:
	StatusServer(ConfigManager* conf, ZoneServer * server);

	~StatusServer();

	void init();

	void run();

	void shutdown();

	ServiceClient* createConnection(Socket* sock, SocketAddress& addr);

	Packet* getStatusXMLPacket();

	bool testZone();
};

#endif /* STATUSSERVER_H_ */
