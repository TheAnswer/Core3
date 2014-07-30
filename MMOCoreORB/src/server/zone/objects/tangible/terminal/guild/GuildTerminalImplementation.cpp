/*
 * GuildTerminalImplementation.cpp
 *
 *  Created on: May 9, 2010
 *      Author: crush
 */


#include "server/zone/objects/tangible/terminal/guild/GuildTerminal.h"
#include "server/zone/ZoneServer.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/packets/object/ObjectMenuResponse.h"
#include "server/zone/objects/guild/GuildObject.h"
#include "server/zone/managers/guild/GuildManager.h"
#include "server/zone/managers/player/PlayerManager.h"
#include "server/zone/objects/building/BuildingObject.h"


void GuildTerminalImplementation::fillObjectMenuResponse(ObjectMenuResponse* menuResponse, CreatureObject* player) {
	if (guildObject == NULL) {
		ManagedReference<BuildingObject*> building = cast<BuildingObject*>( getParentRecursively(SceneObjectType::BUILDING).get().get());

		if (building != NULL && building->isOwnerOf(player)) {
			if (!player->isInGuild()) {
				menuResponse->addRadialMenuItem(185, 3, "@guild:menu_create"); //Create Guild
				return;
			}

			//If the owner is already in a guild, and this guild terminal doesn't have a guild assigned yet,
			//then use the owner's guild.
			guildObject = player->getGuildObject();
		} else {
			//They don't have permission to create a guild.
			return;
		}
	}

	uint64 playerID = player->getObjectID();

	//Only members have access.
	if (!guildObject->hasMember(playerID) && !player->getPlayerObject()->isPrivileged())
		return;

	//Guild exists -> display these functions.
	menuResponse->addRadialMenuItem(193, 3, "@guild:menu_guild_management"); //Guild Management
	menuResponse->addRadialMenuItemToRadialID(193, 186, 3, "@guild:menu_info"); //Guild Information
	menuResponse->addRadialMenuItemToRadialID(193, 189, 3, "@guild:menu_enemies"); //Guild Enemies
	if ( guildObject->getGuildLeaderID() == playerID )
	menuResponse->addRadialMenuItemToRadialID(193, 195, 3, "@guild:accept_hall"); // Accept

	if (guildObject->hasDisbandPermission(playerID))
		menuResponse->addRadialMenuItemToRadialID(193, 191, 3, "@guild:menu_disband"); //Disband Guild

	if (guildObject->hasNamePermission(playerID) || player->getPlayerObject()->isPrivileged())
		menuResponse->addRadialMenuItemToRadialID(193, 192, 3, "@guild:menu_namechange"); //Change Guild Name

	menuResponse->addRadialMenuItemToRadialID(193, 68, 3, "@guild:menu_enable_elections"); //Enable/Disable Elections

	menuResponse->addRadialMenuItem(194, 3, "@guild:menu_member_management"); //Member Management
	menuResponse->addRadialMenuItemToRadialID(194, 187, 3, "@guild:menu_members"); //Guild Members

	if (guildObject->getSponsoredPlayerCount() > 0)
		menuResponse->addRadialMenuItemToRadialID(194, 188, 3, "@guild:menu_sponsored"); //Sponsored for Membership

	menuResponse->addRadialMenuItemToRadialID(194, 190, 3, "@guild:menu_sponsor"); //Sponsor for Membership

	if (guildObject->getGuildLeaderID() == playerID || player->getPlayerObject()->isPrivileged())
		menuResponse->addRadialMenuItemToRadialID(194, 69, 3, "@guild:menu_leader_change"); //Transfer PA Leadership

	return;
}

int GuildTerminalImplementation::handleObjectMenuSelect(CreatureObject* player, byte selectedID) {
	Locker _lock(_this.get());

	ManagedReference<GuildManager*> guildManager = server->getZoneServer()->getGuildManager();

	if (guildManager == NULL)
		return TerminalImplementation::handleObjectMenuSelect(player, selectedID);

	uint64 playerID = player->getObjectID();

	switch (selectedID) {
	case 69:
		if (guildObject != NULL && (guildObject->getGuildLeaderID() == playerID || player->getPlayerObject()->isPrivileged())) {
			guildManager->sendGuildTransferTo(player, _this.get());
		}
		break;
	case 185:
		guildManager->sendGuildCreateNameTo(player, _this.get());
		break;
	case 189:
		if (guildObject != NULL)
			guildManager->sendGuildWarStatusTo(player, guildObject, _this.get());
		break;
	case 193:
	case 186:
		if (guildObject != NULL)
			guildManager->sendGuildInformationTo(player, guildObject, _this.get());
		break;
	case 191:
		if (guildObject != NULL)
			guildManager->sendGuildDisbandConfirmTo(player, guildObject, _this.get());
		break;
	case 194:
	case 187:
		if (guildObject != NULL)
			guildManager->sendGuildMemberListTo(player, guildObject, _this.get());
		break;
	case 188:
		if (guildObject != NULL)
			guildManager->sendGuildSponsoredListTo(player, guildObject, _this.get());
		break;
	case 190:
		if (guildObject != NULL)
			guildManager->sendGuildSponsorTo(player, guildObject, _this.get());
		break;
	case 192:
		if (guildObject != NULL)
			guildManager->sendGuildChangeNameTo(player, guildObject, _this.get());
		break;
	case 195:
		if (guildObject != NULL && (guildObject->getGuildLeaderID() == playerID) ) {
			guildManager->sendAcceptLotsTo(player, _this.get());
		}
		break;
	default:
		TerminalImplementation::handleObjectMenuSelect(player, selectedID);
	}
	return 0;
}


