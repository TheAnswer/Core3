/*
 * GuildWarEnemyNameSuiCallback.h
 *
 *  Created on: Jun 7, 2012
 *      Author: chris.rush
 */

#ifndef GUILDWARENEMYNAMESUICALLBACK_H_
#define GUILDWARENEMYNAMESUICALLBACK_H_

#include "server/zone/managers/guild/GuildManager.h"
#include "server/zone/objects/player/sui/SuiCallback.h"

class GuildWarEnemyNameSuiCallback : public SuiCallback {
	ManagedWeakReference<GuildObject*> guildObject;

public:
	GuildWarEnemyNameSuiCallback(ZoneServer* server, GuildObject* guild)
		: SuiCallback(server) {

		guildObject = guild;
	}

	void run(CreatureObject* player, SuiBox* suiBox, uint32 eventIndex, Vector<UnicodeString>* args) {
		bool cancelPressed = (eventIndex == 1);

		if (!suiBox->isInputBox() || player == NULL || args->size() <= 0 || cancelPressed)
			return;

		String value = args->get(0).toString();

		server->getGuildManager()->declareWarByName(player, guildObject.get(), value);
	}
};

#endif /* GUILDWARENEMYNAMESUICALLBACK_H_ */
