/*
 * HolocronManager.h
 *
 *  Created on: Nov 22, 2010
 *      Author: crush
 */

#ifndef HOLOCRONMANAGER_H_
#define HOLOCRONMANAGER_H_

#include <algorithm>

#include "BugCategory.h"
#include "engine/log/Logger.h"
#include "engine/util/Singleton.h"
#include "system/lang/Object.h"
#include "system/lang/String.h"
#include "system/lang/UnicodeString.h"
#include "system/platform.h"
#include "system/util/SortedVector.h"

namespace server {
namespace zone {
	class ZoneClientSession;
	class ZoneProcessServer;
}
}

using namespace server::zone;

namespace server {
	namespace zone {
		namespace managers {
			namespace holocron {

				class HolocronManager : public Singleton<HolocronManager>, public Logger, public Object {
					ZoneProcessServer* processor;
					SortedVector<BugCategory> categories;

				public:
					HolocronManager(ZoneProcessServer* pserv) {
						processor = pserv;

						setLoggingName("HolocronManager");
						setGlobalLogging(false);
						setLogging(false);

						loadBugCategories();
					}

					void loadBugCategories();
					void sendRequestCategoriesResponseTo(ZoneClientSession* client);

					void submitTicket(ZoneClientSession* client, const UnicodeString& ticketBody);

					uint32 getReporterId(ZoneClientSession* client);
					uint32 createReporterId(ZoneClientSession* client);

					String getTokenValue(const String& token, const UnicodeString& report);
					uint32 getReproducibilityFromString(const String& str);
					uint32 getSeverityFromString(const String& str);
				};

			}
		}
	}
}

using namespace server::zone::managers::holocron;

#endif /* HOLOCRONMANAGER_H_ */
