/*
Copyright (C) 2007 <SWGEmu>

This File is part of Core3.

This program is free software; you can redistribute
it and/or modify it under the terms of the GNU Lesser
General Public License as published by the Free Software
Foundation; either version 2 of the License,
or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU Lesser General Public License for
more details.

You should have received a copy of the GNU Lesser General
Public License along with this program; if not, write to
the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

Linking Engine3 statically or dynamically with other modules
is making a combined work based on Engine3.
Thus, the terms and conditions of the GNU Lesser General Public License
cover the whole combination.

In addition, as a special exception, the copyright holders of Engine3
give you permission to combine Engine3 program with free software
programs or libraries that are released under the GNU LGPL and with
code included in the standard release of Core3 under the GNU LGPL
license (or modified versions of such code, with unchanged license).
You may copy and distribute such a system following the terms of the
GNU LGPL for Engine3 and the licenses of the other code concerned,
provided that you include the source code of that other code when
and as the GNU LGPL requires distribution of source code.

Note that people who make modified versions of Engine3 are not obligated
to grant this special exception for their modified versions;
it is their choice whether to do so. The GNU Lesser General Public License
gives permission to release a modified version without this exception;
this exception also makes it possible to release a modified version
which carries forward this exception.
*/

#ifndef GUILDREMOVECOMMAND_H_
#define GUILDREMOVECOMMAND_H_

#include "server/zone/objects/scene/SceneObject.h"
#include "server/zone/managers/guild/GuildManager.h"

class GuildremoveCommand : public QueueCommand {
public:

	GuildremoveCommand(const String& name, ZoneProcessServer* server)
		: QueueCommand(name, server) {

	}

	int doQueueCommand(CreatureObject* creature, const uint64& target, const UnicodeString& arguments) const {

		if (!checkStateMask(creature))
			return INVALIDSTATE;

		if (!checkInvalidLocomotions(creature))
			return INVALIDLOCOMOTION;

		if (!creature->isPlayerCreature())
			return INVALIDPARAMETERS;

		CreatureObject* player = cast<CreatureObject*>(creature);

		if (!player->isInGuild()) {
			player->sendSystemMessage("@base_player:guildremove_not_in_guild"); //You are not in a guild.
			return GENERALERROR;
		}

		if(server == NULL)
			return GENERALERROR;

		ManagedReference<ZoneServer* > zserv = server->getZoneServer();
		if( zserv == NULL )
			return GENERALERROR;

		ManagedReference<CreatureObject*> playerToKick = NULL;
		ManagedReference<SceneObject* > targetedObject = zserv->getObject(target);
		ManagedReference<GuildObject*> guild = player->getGuildObject();
		ManagedReference<GuildManager*> guildManager = zserv->getGuildManager();
		ManagedReference<PlayerManager*> playerManager = zserv->getPlayerManager();

		if( guild == NULL || guildManager == NULL || playerManager == NULL)
			return GENERALERROR;

		String lowerNamedTarget = arguments.toString().toLowerCase();

		if(!lowerNamedTarget.isEmpty()) {
			playerToKick = playerManager->getPlayer(lowerNamedTarget);
		} else if(targetedObject != NULL && targetedObject->isPlayerCreature()) {
			playerToKick = cast<CreatureObject*>( targetedObject.get());
		}

		if(playerToKick == NULL || !playerToKick->isInGuild() || guild->getMember(playerToKick->getObjectID()) == NULL) {
			player->sendSystemMessage("You can only remove members of your guild.");
			return GENERALERROR;
		}

		if(playerToKick->getObjectID() != creature->getObjectID()) {
			if(!guild->hasKickPermission(creature->getObjectID())) {
				player->sendSystemMessage("@guild:generic_fail_no_permission"); //You do not have permission to perform that operation.
				return GENERALERROR;
			}
			guildManager->sendGuildKickPromptTo(player, playerToKick);
		}
		else
		{
			// TODO: Allow leader to leave guild once guild elections are enabled
			if(guild->getGuildLeaderID() == player->getObjectID())
				player->sendSystemMessage("Guild leader cannot leave the guild");
			else
				guildManager->leaveGuild(player, guild);
		}

		return SUCCESS;
	}

};

#endif //GUILDREMOVECOMMAND_H_
