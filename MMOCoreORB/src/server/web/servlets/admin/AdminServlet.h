/*
 * AdminServlet.h
 *
 *  Created on: Jun 1, 2011
 *      Author: swgemu
 */

#ifndef ADMINSERVLET_H_
#define ADMINSERVLET_H_

#include "../Servlet.h"
#include "server/web/servlets/admin/../../servlets/Servlet.h"
#include "system/lang/String.h"

namespace server {
namespace web {
class HttpRequest;
class HttpResponse;
}  // namespace web
}  // namespace server

class AdminServlet : public Servlet {
public:
	AdminServlet(const String& context);
	virtual ~AdminServlet();

	void handleGet(HttpRequest* request, HttpResponse* response);
	void handlePost(HttpRequest* request, HttpResponse* response);
};

#endif /* ADMINSERVLET_H_ */
