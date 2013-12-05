/*
 * GroupObjectImplementation.cpp
 *
 *  Created on: 29/12/2009
 *      Author: victor
 */

#include "server/zone/objects/group/GroupObject.h"
#include "server/zone/packets/group/GroupObjectMessage3.h"
#include "server/zone/packets/group/GroupObjectMessage6.h"
#include "server/zone/packets/group/GroupObjectDeltaMessage6.h"
#include "server/zone/ZoneClientSession.h"
#include "server/chat/room/ChatRoom.h"
#include "server/chat/ChatManager.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/ZoneProcessServer.h"
#include "server/zone/ZoneServer.h"
#include "server/zone/objects/player/events/SquadLeaderBonusTask.h"
#include "server/zone/managers/objectcontroller/ObjectController.h"
#include "server/zone/objects/group/RemovePetsFromGroupTask.h"

void GroupObjectImplementation::sendBaselinesTo(SceneObject* player) {
	ZoneClientSession* client = player->getClient();
	if (client == NULL)
		return;

	BaseMessage* grup3 = new GroupObjectMessage3(_this.get());
	client->sendMessage(grup3);

	BaseMessage* grup6 = new GroupObjectMessage6(_this.get());
	client->sendMessage(grup6);

	if (player->isPlayerCreature() && chatRoom != NULL)
		chatRoom->sendTo(cast<CreatureObject*>( player));
}

void GroupObjectImplementation::startChatRoom() {
	CreatureObject* leader = cast<CreatureObject*>(groupMembers.get(0).get());
	ChatManager* chatManager = server->getZoneServer()->getChatManager();

	chatRoom = chatManager->createGroupRoom(getObjectID(), leader);
}

void GroupObjectImplementation::destroyChatRoom() {
	if (chatRoom == NULL)
		return;

	ManagedReference<ChatRoom*> room = chatRoom->getParent();
	ManagedReference<ChatRoom*> parent = room->getParent();

	ChatManager* chatManager = server->getZoneServer()->getChatManager();

	chatManager->destroyRoom(chatRoom);
	chatManager->destroyRoom(room);

	chatRoom = NULL;
}

void GroupObjectImplementation::broadcastMessage(BaseMessage* msg) {
	for (int i = 0; i < groupMembers.size(); i++) {
		SceneObject* member = groupMembers.get(i);

		if (member->isPlayerCreature())
			member->sendMessage(msg->clone());
	}

	delete msg;
}

void GroupObjectImplementation::broadcastMessage(CreatureObject* player, BaseMessage* msg, bool sendSelf) {
	for (int i = 0; i < groupMembers.size(); i++) {
		SceneObject* member = groupMembers.get(i);

		if(!sendSelf && member == player)
			continue;

		if (member->isPlayerCreature())
			member->sendMessage(msg->clone());
	}

	delete msg;
}

void GroupObjectImplementation::addMember(SceneObject* newMember) {
	Locker locker(_this.get());

	GroupObjectDeltaMessage6* grp = new GroupObjectDeltaMessage6(_this.get());
	grp->startUpdate(1);
	groupMembers.add(newMember, grp);
	grp->close();

	broadcastMessage(grp);

	if (newMember->isPlayerCreature())
		sendTo(newMember, true);

	if (hasSquadLeader() && newMember->isPlayerCreature()) {
		ManagedReference<CreatureObject*> playerCreature = cast<CreatureObject*>( newMember);
		addGroupModifiers(playerCreature);
	}

	calcGroupLevel();
}

void GroupObjectImplementation::removeMember(SceneObject* member) {
	ManagedReference<SceneObject*> obj = member;

	for (int i = 0; i < groupMembers.size(); i++) {
		SceneObject* scno = groupMembers.get(i);

		if (scno == member) {
			GroupObjectDeltaMessage6* grp = new GroupObjectDeltaMessage6(_this.get());
			grp->startUpdate(1);
			groupMembers.remove(i, grp);
			grp->close();

			broadcastMessage(grp);
		}
	}

	if (member->isPlayerCreature()) {
		// Remove member's pets
		CreatureObject* mem = cast<CreatureObject*>(member);
		RemovePetsFromGroupTask* task = new RemovePetsFromGroupTask(mem, _this.get());
		task->execute();

		if (hasSquadLeader()) {
			ManagedReference<CreatureObject*> playerCreature = cast<CreatureObject*>( member);
			removeGroupModifiers(playerCreature);
		}
	}

	calcGroupLevel();
}

bool GroupObjectImplementation::hasMember(SceneObject* member) {
	for (int i = 0; i < groupMembers.size(); i++) {
		SceneObject* play = groupMembers.get(i);

		if (play == member)
			return true;
	}

	return false;
}

bool GroupObjectImplementation::hasMember(uint64 member) {
	for (int i = 0; i < groupMembers.size(); i++) {
		SceneObject* play = groupMembers.get(i);

		if (play->getObjectID() == member)
			return true;
	}

	return false;
}

void GroupObjectImplementation::makeLeader(SceneObject* player) {
	if (groupMembers.size() < 2 || !player->isPlayerCreature())
		return;

	//SceneObject* obj = groupMembers.get();

	ManagedReference<SceneObject*> temp = cast<SceneObject*>( groupMembers.get(0).get());

	for (int i = 0; i < groupMembers.size(); ++i) {
		if (groupMembers.get(i) == player) {
			GroupObjectDeltaMessage6* grp = new GroupObjectDeltaMessage6(_this.get());
			grp->startUpdate(1);

			if (hasSquadLeader())
				removeGroupModifiers();

			groupMembers.set(0, player, grp, 2);
			groupMembers.set(i, temp.get(), grp, 0);

			grp->close();

			broadcastMessage(grp);

			if (hasSquadLeader())
				addGroupModifiers();

			return;
		}
	}
}

void GroupObjectImplementation::disband() {
	// this locked
	ManagedReference<ChatRoom* > chat = chatRoom;

	for (int i = 0; i < groupMembers.size(); i++) {
		CreatureObject* crea = cast<CreatureObject*>(groupMembers.get(i).get());
		try {
			Locker clocker(crea, _this.get());

			if (crea->isPlayerCreature()) {
				CreatureObject* play = cast<CreatureObject*>( crea);

				if (chat != NULL) {
					chat->removePlayer(play, false);
					chat->sendDestroyTo(play);

					ChatRoom* room = chat->getParent();
					room->sendDestroyTo(play);
				}

			}

			crea->updateGroup(NULL);
			//play->updateGroupId(0);

			//sendClosestWaypointDestroyTo(play);

			//removeSquadLeaderBonuses(play);

			if (crea->isPlayerCreature())
				sendDestroyTo(crea);

		} catch (Exception& e) {
			System::out << "Exception in GroupObject::disband(Player* player)\n";
		}
	}

	destroyChatRoom();

	if (hasSquadLeader())
		removeGroupModifiers();

	groupMembers.removeAll();

	//The mission waypoints should not be destroyed. They belong to the players.
	//missionWaypoints.removeAll();
}

bool GroupObjectImplementation::hasSquadLeader() {
	if (getLeader() == NULL)
		return false;

	if (getLeader()->isPlayerCreature()) {

		ManagedReference<CreatureObject*> leader = cast<CreatureObject*>( getLeader());

		if (leader->hasSkill("outdoors_squadleader_novice"))
			return true;
	}

	return false;
}

void GroupObjectImplementation::addGroupModifiers() {
	ManagedReference<GroupObject*> thisGroup = _this.get();

	Locker glocker(thisGroup);

	for (int i = 0; i < groupMembers.size(); i++) {
		CreatureObject* crea = cast<CreatureObject*>(groupMembers.get(i).get());

		if (crea == NULL)
			continue;

		if (!crea->isPlayerCreature())
			continue;

		ManagedReference<CreatureObject*> player = cast<CreatureObject*>( crea);

		Locker clocker(player, thisGroup);

		addGroupModifiers(player);
	}

	if (getLeader() != NULL) {
		if (getLeader()->isPlayerCreature()) {
			CreatureObject* leader = cast<CreatureObject*>( getLeader());

			Reference<SquadLeaderBonusTask*> bonusTask = new SquadLeaderBonusTask(leader);

			bonusTask->schedule(300 * 1000);
		}
	}
}

void GroupObjectImplementation::removeGroupModifiers() {
	for (int i = 0; i < groupMembers.size(); i++) {
		CreatureObject* crea = cast<CreatureObject*> (groupMembers.get(i).get() );

		if (crea == NULL)
			continue;

		if (!crea->isPlayerCreature())
			continue;

		ManagedReference<CreatureObject*> player = cast<CreatureObject*>( crea);
		removeGroupModifiers(player);
	}
}

void GroupObjectImplementation::addGroupModifiers(CreatureObject* player) {
	if (player == NULL)
		return;

	if (getLeader() == NULL)
		return;

	if (!getLeader()->isPlayerCreature())
		return;

	ManagedReference<CreatureObject*> leader = cast<CreatureObject*>( getLeader());

	if (leader == player)
		return;

	Locker locker(player);

	int duration = 300;

	String action = "squadleader";

	if (player->hasBuff(action.hashCode()))
		player->removeBuff(action.hashCode());

	ManagedReference<Buff*> buff = new Buff(player, action.hashCode(), duration, BuffType::SKILL);
	buff->setSkillModifier("slope_move", leader->getSkillMod("group_slope_move"));
	buff->setSkillModifier("ranged_defense", leader->getSkillMod("group_ranged_defense"));
	buff->setSkillModifier("melee_defense", leader->getSkillMod("group_melee_defense"));
	buff->setSkillModifier("burst_run", leader->getSkillMod("group_burst_run"));

	player->addBuff(buff);
}

void GroupObjectImplementation::removeGroupModifiers(CreatureObject* player) {
	if (player == NULL)
		return;

	if (getLeader() == NULL)
		return;

	if (!getLeader()->isPlayerCreature())
		return;

	ManagedReference<CreatureObject*> leader = cast<CreatureObject*>( getLeader());

	if (leader == player)
		return;

	Locker locker(player);
	String action = "squadleader";

	if (player->hasBuff(action.hashCode()))
		player->removeBuff(action.hashCode());
}

float GroupObjectImplementation::getGroupHarvestModifier(CreatureObject* player) {

	String skillNovice = "outdoors_ranger_novice";
	String skillMaster = "outdoors_ranger_master";

	float modifier = 1.2f;

	for(int i = 0; i < groupMembers.size(); ++i) {
		ManagedReference<SceneObject*> scno = cast<SceneObject*>(groupMembers.get(i).get());

		if(scno->isPlayerCreature()) {
			CreatureObject* groupMember = cast<CreatureObject*>( scno.get());

			if(groupMember == player)
				continue;

			if(groupMember->hasSkill(skillNovice)) {

				if(groupMember->isInRange(player, 64.0f)) {

					if(groupMember->hasSkill(skillMaster)) {
						modifier = 1.4f;
						break;
					}
					modifier = 1.3f;
				}
			}
		}
	}
	return modifier;
}

void GroupObjectImplementation::calcGroupLevel() {
	groupLevel = 0;

	for (int i = 0; i < getGroupSize(); i++) {
		SceneObject* member = getGroupMember(i);

		if (member->isCreatureObject()) {
			CreatureObject* creature = cast<CreatureObject*>(member);

			int currentlevel = groupLevel - getGroupSize();
			int memberlevel = creature->getLevel();

			if (memberlevel > currentlevel)
				groupLevel = memberlevel + getGroupSize();
		}
	}

	GroupObjectDeltaMessage6* msg = new GroupObjectDeltaMessage6(_this.get());

	msg->updateLevel(this->groupLevel);
	msg->close();

	broadcastMessage(msg);
}

void GroupObjectImplementation::sendSystemMessage(StringIdChatParameter& param) {
	Locker lock(_this.get());

	for (int i = 0; i < groupMembers.size(); ++i) {
		GroupMember* member = &groupMembers.get(i);

		ManagedReference<SceneObject*> obj = member->get();

		if (obj == NULL || !obj->isPlayerCreature())
			continue;

		CreatureObject* creature = cast<CreatureObject*>(obj.get());
		creature->sendSystemMessage(param);
	}
}

void GroupObjectImplementation::sendSystemMessage(const String& fullPath) {
	Locker lock(_this.get());

	for (int i = 0; i < groupMembers.size(); ++i) {
		GroupMember* member = &groupMembers.get(i);

		ManagedReference<SceneObject*> obj = member->get();

		if (obj == NULL || !obj->isPlayerCreature())
			continue;

		CreatureObject* creature = cast<CreatureObject*>(obj.get());
		creature->sendSystemMessage(fullPath);
	}
}

bool GroupObjectImplementation::isOtherMemberPlayingMusic(CreatureObject* player) {
	for (int i = 0; i < getGroupSize(); ++i) {
		ManagedReference<CreatureObject*> groupMember = cast<CreatureObject*>(getGroupMember(i));

		if (groupMember == NULL || groupMember == player || !groupMember->isPlayerCreature())
			continue;

		if (groupMember->isPlayingMusic()) {
			return true;
		}
	}

	return false;
}
