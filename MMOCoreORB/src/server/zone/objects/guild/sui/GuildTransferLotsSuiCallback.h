/*
 * GuildTransferLotsSuiCallback.h
 *
 *  Created on: Oct 9, 2012
 *      Author: root
 */

#ifndef GUILDTRANSFERLOTSSUICALLBACK_H_
#define GUILDTRANSFERLOTSSUICALLBACK_H_
#include "server/zone/managers/guild/GuildManager.h"
#include "server/zone/objects/player/sui/SuiCallback.h"

class GuildTransferLotsSuiCallback : public SuiCallback {
public:
	GuildTransferLotsSuiCallback(ZoneServer* server)
		: SuiCallback(server) {
	}

	void run(CreatureObject* player, SuiBox* suiBox, uint32 eventIndex, Vector<UnicodeString>* args) {
		bool cancelPressed = (eventIndex == 1);

		if (!suiBox->isMessageBox())
			return;

		if (cancelPressed)
			return;

		ManagedReference<GuildManager*> guildManager = server->getGuildManager();

		ManagedReference<SceneObject*> usingObject = suiBox->getUsingObject().get();
		if (usingObject == nullptr)
			return;

		Terminal* terminal = usingObject.castTo<Terminal*>();
		if (terminal == nullptr || !terminal->isGuildTerminal())
			return;

		GuildTerminal* guildTerminal = cast<GuildTerminal*>( terminal);
		if (guildTerminal == nullptr)
			return;

		ManagedReference<BuildingObject*> buildingObject = guildTerminal->getParentRecursively(SceneObjectType::BUILDING).castTo<BuildingObject*>();
		if (buildingObject == nullptr)
			return;

		ManagedReference<CreatureObject*> owner = buildingObject->getOwnerCreatureObject();
		if (owner == nullptr || !owner->isPlayerCreature())
			return;

		ManagedReference<PlayerObject*> ownerGhost = owner->getPlayerObject().get();
		ManagedReference<GuildObject*> guildObject = owner->getGuildObject().get();
		if (ownerGhost == nullptr || guildObject == nullptr)
			return;

		if (player->getPlayerObject()->getLotsRemaining() < 5) {
			player->sendSystemMessage("@guild:no_lots"); // You don't have enough lots free to take over the PA hall. You need to have 5 free lots.
			return;
		}

		if (guildObject->getGuildLeaderID() == player->getObjectID() && !ownerGhost->isOnline() && ownerGhost->getDaysSinceLastLogout() >= 28) {
			guildManager->transferGuildHall(player, usingObject);
		}

	}

};

#endif /* GUILDTRANSFERLOTSSUICALLBACK_H_ */
