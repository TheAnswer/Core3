/*
 * GuildMemberInfo.h
 *
 *  Created on: Oct 27, 2010
 *      Author: crush
 */

#ifndef GUILDMEMBERINFO_H_
#define GUILDMEMBERINFO_H_

#include "engine/engine.h"
#include "engine/util/json_utils.h"
#include <system/lang/Time.h>

namespace server {
namespace zone {
namespace objects {
namespace guild {

	class GuildObject;

	class GuildMemberInfo : public Object {
		uint64 playerID;
		String guildTitle;
		uint8 permissions;
		uint64 declaredAllegiance; // TODO: remove after wipe
		bool onlineStatus;
		Time lastLogin;

	public:
		GuildMemberInfo() {
			playerID = 0;
			permissions = 0;
			declaredAllegiance = 0;
			onlineStatus = false;

			//addSerializableVariables();
		}

		GuildMemberInfo(uint64 playerid);

		GuildMemberInfo(const GuildMemberInfo& gmi) : Object() {
			playerID = gmi.playerID;
			guildTitle = gmi.guildTitle;
			permissions = gmi.permissions;
			declaredAllegiance = gmi.declaredAllegiance;
			onlineStatus = gmi.onlineStatus;
			lastLogin = gmi.lastLogin;

			//addSerializableVariables();
		}

		int compareTo(const GuildMemberInfo& gmi) const;

		GuildMemberInfo& operator=(const GuildMemberInfo& gmi);

		friend void to_json(nlohmann::json& j, const GuildMemberInfo& m) {
			j["playerID"] = m.playerID;
			j["guildTitle"] = m.guildTitle;
			j["permissions"] = m.permissions;
			j["declaredAllegiance"] = m.declaredAllegiance;
		}

		bool toBinaryStream(ObjectOutputStream* stream) {
			stream->writeLong(playerID);
			guildTitle.toBinaryStream(stream);
			stream->writeByte(permissions);
			stream->writeLong(declaredAllegiance);

			return true;
		}

		bool parseFromBinaryStream(ObjectInputStream* stream) {
			playerID = stream->readLong();
			guildTitle.parseFromBinaryStream(stream);
			permissions = stream->readByte();
			declaredAllegiance = stream->readLong();

			return true;
		}

		/*inline void addSerializableVariables() {
			addSerializableVariable("playerID", &playerID);
			addSerializableVariable("guildTitle", &guildTitle);
			addSerializableVariable("permissions", &permissions);
			addSerializableVariable("declaredAllegiance", &declaredAllegiance);
		}*/

		inline uint64 getPlayerID() {
			return playerID;
		}

		inline void setPlayerID(uint64 playerid) {
			playerID = playerid;
		}

		inline String& getGuildTitle() {
			return guildTitle;
		}

		inline void setGuildTitle(const String& title) {
			guildTitle = title;
		}

		inline uint8 getPermissions() {
			return permissions;
		}

		inline void setPermissions(uint8 permission) {
			permissions = permission;
		}

		inline void addPermission(uint8 permission) {
			permissions |= permission;
		}

		inline void removePermission(uint8 permission) {
			permissions &= ~permission;
		}

		inline void togglePermission(uint8 permission) {
			permissions ^= permission;
		}

		inline bool hasPermission(uint8 permission) {
			return (permissions & permission);
		}

		inline void setOnlineStatus(bool status) {
			onlineStatus = status;

			if (status) {
				Time now;
				lastLogin = now;
			}
		}

		inline bool isOnline() {
			return onlineStatus;
		}

		inline Time& getLastLoginTime() {
			return lastLogin;
		}


	};
}
}
}
}
using namespace server::zone::objects::guild;

#endif /* GUILDMEMBERINFO_H_ */
