/*
 * Servlet.h
 *
 *  Created on: May 31, 2011
 *      Author: kyle
 */

#ifndef SERVLET_H_
#define SERVLET_H_

#include "../HttpRequest.h"
#include "../HttpResponse.h"
#include "engine/engine.h"
#include "server/zone/ZoneServer.h"

class Servlet : protected Logger {
private:
	String context;
protected:
	ManagedReference<ZoneServer*> server;

public:
	Servlet(String context);
	virtual ~Servlet();

	virtual void handleGet(HttpRequest* request, HttpResponse* response) = 0;

	virtual void handlePost(HttpRequest* request, HttpResponse* response) = 0;

	void handleRequest(struct mg_connection *conn, HttpRequest* request, HttpResponse* response);

	void setZoneServer(ZoneServer* serv) {
		server = serv;
	}

	String getContext() {
		return context;
	}

private:
	void outputResponse(struct mg_connection *conn, HttpResponse* response);

};

#endif /* SERVLET_H_ */
