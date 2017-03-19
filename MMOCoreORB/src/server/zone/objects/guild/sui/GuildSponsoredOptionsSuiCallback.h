/*
 * GuildSponsoredOptionsSuiCallback.h
 *
 *  Created on: Nov 3, 2010
 *      Author: crush
 */

#ifndef GUILDSPONSOREDOPTIONSSUICALLBACK_H_
#define GUILDSPONSOREDOPTIONSSUICALLBACK_H_

class GuildSponsoredOptionsSuiCallback : public SuiCallback {
public:
	GuildSponsoredOptionsSuiCallback(ZoneServer* server)
		: SuiCallback(server) {
	}

	void run(CreatureObject* player, SuiBox* suiBox, uint32 eventIndex, Vector<UnicodeString>* args) {
		bool cancelPressed = (eventIndex == 1);

		if (!suiBox->isListBox() || cancelPressed)
			return;

		if (args->size() < 1)
			return;

		int index = Integer::valueOf(args->get(0).toString());

		ManagedReference<GuildManager*> guildManager = server->getGuildManager();

		if (guildManager == NULL)
			return;

		ManagedReference<SceneObject*> obj = suiBox->getUsingObject().get();

		if (obj == NULL || !obj->isTerminal())
			return;

		Terminal* terminal = cast<Terminal*>( obj.get());

		if (!terminal->isGuildTerminal())
			return;

		SuiListBox* listBox = cast<SuiListBox*>( suiBox);

		uint64 playerID = listBox->getMenuObjectID(index);

		ManagedReference<GuildObject*> guild = player->getGuildObject().get();

		if (guild == NULL || !guild->hasAcceptPermission(player->getObjectID()) || !guild->hasSponsoredPlayer(playerID)) {
			player->sendSystemMessage("@guild:generic_fail_no_permission"); // You do not have permission to perform that operation.
			return;
		}

		//Whether they accept, or decline, we are removing them from the sponsored list.
		guild->removeSponsoredPlayer(playerID);
		guildManager->removeSponsoredPlayer(playerID);

		if (index == 0) { //If they accepted, then ...
			guildManager->acceptSponsoredPlayer(player, playerID);
		} else {
			guildManager->declineSponsoredPlayer(player, playerID);
		}
	}
};

#endif /* GUILDSPONSOREDOPTIONSSUICALLBACK_H_ */
