/*
 * GuildObjectImplementation.cpp
 *
 *  Created on: Oct 27, 2010
 *      Author: crush
 */

#include "server/zone/objects/guild/GuildObject.h"
#include "server/zone/ZoneServer.h"
#include "server/ServerCore.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/scene/SceneObject.h"
#include "server/zone/objects/guild/GuildMemberInfo.h"
#include "server/zone/objects/guild/GuildUpdateEvent.h"
#include "server/chat/ChatManager.h"
#include "server/zone/packets/chat/ChatRoomMessage.h"
#include "server/chat/room/ChatRoom.h"

#define EVENT_RANDOM_MAX_DELTA 3600 * 2 /*2 hours*/

void GuildObjectImplementation::initializeTransientMembers() {
	SceneObjectImplementation::initializeTransientMembers();

	transferPending = false;
}

void GuildObjectImplementation::rescheduleUpdateEvent(uint32 seconds) {
	Locker locker(_this.getReferenceUnsafeStaticCast());

	if (guildUpdateEvent == nullptr) {
		guildUpdateEvent = new GuildUpdateEvent(_this.getReferenceUnsafeStaticCast(), ServerCore::getZoneServer());
	} else if (guildUpdateEvent->isScheduled()) {
		guildUpdateEvent->cancel();
	}

	seconds += System::random(EVENT_RANDOM_MAX_DELTA);

	guildUpdateEvent->schedule(seconds * 1000);

	AtomicTime next;
	Core::getTaskManager()->getNextExecutionTime(guildUpdateEvent, next);

	nextUpdateTime = next.getTimeObject();
}

void GuildObjectImplementation::sendBaselinesTo(SceneObject* player) {
}

void GuildObjectImplementation::broadcastMessage(BaseMessage* msg) {
}

void GuildObjectImplementation::broadcastMessage(CreatureObject* player, BaseMessage* msg, bool sendSelf) {
}

void GuildObjectImplementation::addMember(uint64 playerID) {
	Locker locker(_this.getReferenceUnsafeStaticCast());

	GuildMemberInfo gmi(playerID);
	guildMembers.put(playerID, gmi);
}

void GuildObjectImplementation::removeMember(uint64 playerID) {
	guildMembers.drop(playerID);

	if (candidates.contains(playerID)) {
		removeCandidate(playerID);
	}

	if (votes.contains(playerID)) {
		setVote(playerID, 0);
		votes.drop(playerID);
	}

	if (playerID == guildLeaderID) {
		guildLeaderID = 0;
	}
}

bool GuildObjectImplementation::hasMember(uint64 playerID) {
	Locker locker(_this.getReferenceUnsafeStaticCast());

	return guildMembers.contains(playerID);
}

uint64 GuildObjectImplementation::getMember(int index) {
	return guildMembers.get(index).getPlayerID();
}

GuildMemberInfo* GuildObjectImplementation::getMember(uint64 playerID) {
	return &guildMembers.get(playerID);
}

void GuildObjectImplementation::setGuildMemberTitle(uint64 playerID, const String& title) {
	GuildMemberInfo* gmi = getMember(playerID);
	if (gmi == nullptr)
		return;

	gmi->setGuildTitle(title);
}

String GuildObjectImplementation::getGuildMemberTitle(uint64 playerID) {
	Locker locker(_this.getReferenceUnsafeStaticCast());

	GuildMemberInfo* gmi = getMember(playerID);
	if (gmi == nullptr)
		return "";

	return gmi->getGuildTitle();
}

bool GuildObjectImplementation::isInWaringGuild(CreatureObject* creature) {
	ManagedReference<GuildObject*> attackerGuild = creature->getGuildObject().get();

	if (attackerGuild != nullptr) {

		try {
			if (isAtWarWith(attackerGuild->getObjectID())) {
				return true;
			}
		} catch (...) {
			error("unreported exception caught in bool GuildObjectImplementation::isInWaringGuild(CreatureObject* creature)");
		}

	}

	return false;
}

bool GuildObjectImplementation::isAtWarWith(unsigned long long guildoid) {
	waringGuildsMutex.rlock();

	bool res = waringGuilds.get(guildoid) == WAR_MUTUAL;

	waringGuildsMutex.runlock();

	return res;
}

bool GuildObjectImplementation::hasMailPermission(uint64 playerID) {
	Locker locker(_this.getReferenceUnsafeStaticCast());

	if (!hasMember(playerID))
		return false;

	GuildMemberInfo* gmi = getMember(playerID);

	return gmi->hasPermission(GuildObject::PERMISSION_MAIL);
}

bool GuildObjectImplementation::hasSponsorPermission(uint64 playerID) {
	Locker locker(_this.getReferenceUnsafeStaticCast());

	if (!hasMember(playerID))
		return false;

	GuildMemberInfo* gmi = getMember(playerID);

	return gmi->hasPermission(GuildObject::PERMISSION_SPONSOR);
}

bool GuildObjectImplementation::hasAcceptPermission(uint64 playerID) {
	Locker locker(_this.getReferenceUnsafeStaticCast());

	if (!hasMember(playerID))
		return false;

	GuildMemberInfo* gmi = getMember(playerID);

	return gmi->hasPermission(GuildObject::PERMISSION_ACCEPT);
}

bool GuildObjectImplementation::hasKickPermission(uint64 playerID) {
	Locker locker(_this.getReferenceUnsafeStaticCast());

	if (!hasMember(playerID))
		return false;

	GuildMemberInfo* gmi = getMember(playerID);

	return gmi->hasPermission(GuildObject::PERMISSION_KICK);
}

bool GuildObjectImplementation::hasDisbandPermission(uint64 playerID) {
	Locker locker(_this.getReferenceUnsafeStaticCast());

	if (!hasMember(playerID))
		return false;

	GuildMemberInfo* gmi = getMember(playerID);

	return gmi->hasPermission(GuildObject::PERMISSION_DISBAND);
}

bool GuildObjectImplementation::hasNamePermission(uint64 playerID) {
	Locker locker(_this.getReferenceUnsafeStaticCast());

	if (!hasMember(playerID))
		return false;

	GuildMemberInfo* gmi = getMember(playerID);

	return gmi->hasPermission(GuildObject::PERMISSION_NAME);
}

bool GuildObjectImplementation::hasTitlePermission(uint64 playerID) {
	Locker locker(_this.getReferenceUnsafeStaticCast());

	if (!hasMember(playerID))
		return false;

	GuildMemberInfo* gmi = getMember(playerID);

	return gmi->hasPermission(GuildObject::PERMISSION_TITLE);
}

bool GuildObjectImplementation::hasWarPermission(uint64 playerID) {
	Locker locker(_this.getReferenceUnsafeStaticCast());

	if (!hasMember(playerID))
		return false;

	GuildMemberInfo* gmi = getMember(playerID);

	return gmi->hasPermission(GuildObject::PERMISSION_WAR);
}

void GuildObjectImplementation::toggleMemberPermission(uint64 playerID, uint8 permission) {
	GuildMemberInfo* gmi = getMember(playerID);
	if (gmi == nullptr)
		return;

	gmi->togglePermission(permission);
}

void GuildObjectImplementation::removeCandidate(uint64 candidateoid) {
	candidates.drop(candidateoid);

	// Remove votes for the candidate
	for (int i = votes.size() - 1; i >= 0; --i) {
		if (votes.get(i) == candidateoid) {
			votes.drop(votes.elementAt(i).getKey());
		}
	}
}

uint64 GuildObjectImplementation::getMemberWithHighestPermission() {
	uint64 highestMember = 0;
	uint8 highestPerm = 0;

	for (int i = 0; i < guildMembers.size(); i++) {
		GuildMemberInfo* gmi = &guildMembers.get(i);

		if (gmi == nullptr)
			continue;

		uint8 perm = gmi->getPermissions();

		if (perm > highestPerm) {
			highestPerm = perm;
			highestMember = gmi->getPlayerID();
		}
	}

	return highestMember;
}

void GuildObjectImplementation::notifyGuildMemberStatus(CreatureObject* member, bool online) {
	if (member == nullptr)
		return;

	ManagedReference<ChatRoom*> guildChat = getChatRoom();

	if (guildChat == nullptr)
		return;

	auto zoneServer = ServerCore::getZoneServer();

	if (zoneServer == nullptr)
		return;

	ChatManager* chatManager = zoneServer->getChatManager();

	if (chatManager == nullptr)
		return;

	String playerName = member->getFirstName();
	StringBuffer bufferMessage;
	Time now;

	bufferMessage << "\r\\#99FF00" << now.getFormattedTime("%m-%d-%Y %H:%M:%S") << " <" << guildName << ">";

	if (isGuildLeader(member))
		bufferMessage << " Guild Leader ";
	else
		bufferMessage << " Guild Member ";

	bufferMessage << playerName;

	GuildMemberInfo* gmi = getMember(member->getObjectID());

	if (gmi != nullptr) {
		gmi->setOnlineStatus(online);
	}

	if (online) {
		bufferMessage << " is online.";
	} else {
		bufferMessage << " has gone offline.";
	}

	UnicodeString message(bufferMessage.toString());
	UnicodeString formattedMsg(chatManager->formatMessage(message));

	BaseMessage* msg = new ChatRoomMessage("", zoneServer->getGalaxyName(), formattedMsg, guildChat->getRoomID());
	guildChat->broadcastMessage(msg);
}

int GuildObjectImplementation::writeRecursiveJSON(JSONSerializationType& j, int maxDepth, Vector<uint64>* oidPath) {
	if (maxDepth <= 0)
		return 0;

	int count = SceneObjectImplementation::writeRecursiveJSON(j, maxDepth, oidPath);

	if (oidPath == nullptr)
		oidPath = new Vector<uint64>();

	oidPath->add(getObjectID());

	auto server = ServerCore::getZoneServer();

	if (server == nullptr) {
		return count;
	}

	auto guildLeader = server->getObject(guildLeaderID);

	if (guildLeader != nullptr) {
		count += guildLeader->writeRecursiveJSON(j, maxDepth - 1, oidPath);
	}

	for (int i = 0; i < getTotalMembers(); i++) {
		uint64 memberID = getMember(i);

		auto member = server->getObject(memberID);

		if (member != nullptr) {
			count += member->writeRecursiveJSON(j, maxDepth - 1, oidPath);
		}
	}

	if (oidPath->size() == 0) {
		delete oidPath;
	}

	return count;
}
