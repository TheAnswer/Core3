
#ifndef GUILDCHANGENAMERESPONSESUICALLBACK_H_
#define GUILDCHANGENAMERESPONSESUICALLBACK_H_

#include "server/zone/managers/guild/GuildManager.h"
#include "server/zone/objects/tangible/terminal/guild/GuildTerminal.h"
#include "server/zone/objects/player/sui/SuiCallback.h"
#include "server/zone/objects/player/PlayerObject.h"

class GuildChangeNameResponseSuiCallback : public SuiCallback {
	ManagedWeakReference<GuildObject*> guildObject;

public:
	GuildChangeNameResponseSuiCallback(ZoneServer* server, GuildObject* guild)
		: SuiCallback(server) {

		guildObject = guild;
	}

	void run(CreatureObject* player, SuiBox* suiBox, uint32 eventIndex, Vector<UnicodeString>* args) {
		bool cancelPressed = (eventIndex == 1);

		uint64 playerID = player->getObjectID();

		ManagedReference<GuildObject*> guild = guildObject.get();
		if (guild == NULL)
			return;

		if (!guild->hasNamePermission(playerID) && !player->getPlayerObject()->isPrivileged()) {
			player->sendSystemMessage("@guild:generic_fail_no_permission"); // You do not have permission to perform that operation.
			return;
		}

		if (!suiBox->isInputBox() || cancelPressed)
			return;

		if (args->size() < 1)
			return;

		String guildName = args->get(0).toString();

		ManagedReference<SceneObject*> obj = suiBox->getUsingObject();

		if (obj == NULL || !obj->isTerminal())
			return;

		Terminal* terminal = cast<Terminal*>( obj.get());

		if (!terminal->isGuildTerminal())
			return;

		GuildTerminal* guildTerminal = cast<GuildTerminal*>( terminal);

		ManagedReference<GuildManager*> guildManager = server->getGuildManager();

		//Check if this player is already creating a guild...
		if (guildManager->isCreatingGuild(playerID))
			return;

		if (guildManager->validateGuildName(player, guildName, guild)) {
			Locker glocker(guild, player);

			guild->setPendingNewName(guildName);
			guildManager->sendGuildChangeAbbrevTo(player, guild, guildTerminal);
			return;
		}

		//Resend the create name box.
		player->getPlayerObject()->addSuiBox(suiBox);
		player->sendMessage(suiBox->generateMessage());
	}
};

#endif /* GUILDCHANGENAMERESPONSESUICALLBACK_H_ */
