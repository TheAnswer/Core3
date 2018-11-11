/**
 * @author      : theanswer (theanswer@Victors-MacBook-Pro.local)
 * @file        : RESTServer
 * @created     : Sunday Nov 11, 2018 11:31:08 CET
 */

#include "RESTServer.h"

using namespace server::web3;

const Logger server::web3::RESTServer::logger("RESTServer");

RESTServer::RESTServer(uint16 port) : port(port) {

}

RESTServer::~RESTServer() {
}

#ifdef COMPILE_CORE3_REST
#include <cpprest/http_listener.h>
#include <cpprest/json.h>

#include <map>
#include <set>
#include <string>
#include <memory>

#include "engine/engine.h"

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

using namespace std;

void handle_get(http_request request) {
	const auto& uri = request.relative_uri();

	auto fragments = uri::split_path(uri.to_string());

	nlohmann::json responses = nlohmann::json::array();

	for (const auto& frag : fragments) {
		try {
			uint64 oid = UnsignedLong::valueOf(frag.c_str());

			auto obj = Core::lookupObject(oid).castTo<ManagedObject*>();

			if (obj != nullptr) {
				ReadLocker lock(obj);

				nlohmann::json jsonData;
				obj->writeJSON(jsonData);

				nlohmann::json entry;
				entry[frag] = jsonData;

				responses.push_back(entry);
			}
		} catch (const Exception& e) {
			RESTServer::logger.error(e.getMessage());
		}
	}

	if (responses.empty()) {
		request.reply(status_codes::NotFound);
	} else {
		request.reply(status_codes::OK, json::value(responses.dump(2), false));
	}
}

UniqueReference<http_listener*> restListener;

void RESTServer::run() {
	if (restListener != nullptr) {
		restListener->close().wait();
	}

	restListener = new http_listener(("http://localhost:" + String::valueOf(port) + "/object").toCharArray());

	restListener->support(methods::GET, handle_get);
	//listener.support(methods::POST, handle_post);
	//listener.support(methods::PUT, handle_put);
	//listener.support(methods::DEL, handle_del);

	try {
		restListener->open()
			.then([this] {
			logger.info("listening to port " + String::valueOf(port), true);
		}).wait();
	} catch (exception const & e) {
		logger.error(e.what());
	}
}

void RESTServer::stop() {
	doRun.set(false);

	if (restListener != nullptr) {
		logger.info("shutting down...", true);

		restListener->close().wait();
	}
}

#else

void RESTServer::run() {
	logger.error("disabled, server compiled without rest support");
}

void RESTServer::stop() {
	doRun.set(false);
}

#endif
