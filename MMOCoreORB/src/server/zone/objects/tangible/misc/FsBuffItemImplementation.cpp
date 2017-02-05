#include "server/zone/objects/tangible/misc/FsBuffItem.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/packets/scene/AttributeListMessage.h"
#include "server/zone/packets/object/ObjectMenuResponse.h"
#include "server/zone/objects/creature/BuffAttribute.h"
#include "server/zone/Zone.h"
#include "server/zone/ZoneServer.h"

void FsBuffItemImplementation::fillObjectMenuResponse(ObjectMenuResponse* menuResponse, CreatureObject* player) {
	TangibleObjectImplementation::fillObjectMenuResponse(menuResponse, player);

}

int FsBuffItemImplementation::handleObjectMenuSelect(CreatureObject* player, byte selectedID) {

	return 1;
}

void FsBuffItemImplementation::fillAttributeList(AttributeListMessage* alm, CreatureObject* object) {
	alm->insertAttribute("reuse_time", getTimeString(reuseTime));
	alm->insertAttribute("examine_dot_attribute", BuffAttribute::getName(buffAttribute));
	alm->insertAttribute("potency", buffValue);
	alm->insertAttribute("duration", getTimeString(buffDuration));
}

void FsBuffItemImplementation::applyBuff(CreatureObject* player) {
	uint32 buffCRC = getBuffCRC();

	if (player->hasBuff(buffCRC)) {
		player->sendSystemMessage("@quest/force_sensitive/utils:has_buff");
		return;
	}

	Reference<Buff*> buff = new Buff(player, buffCRC, buffDuration, BuffType::MEDICAL);

	buff->setAttributeModifier(buffAttribute, buffValue);

	player->addBuff(buff);

	player->sendSystemMessage("@quest/force_sensitive/utils:buff_applied");
}

uint32 FsBuffItemImplementation::getBuffCRC() {
	switch (buffAttribute) {
	case 0: return BuffCRC::MEDICAL_ENHANCE_HEALTH;
	case 1: return BuffCRC::MEDICAL_ENHANCE_STRENGTH;
	case 2: return BuffCRC::MEDICAL_ENHANCE_CONSTITUTION;
	case 3: return BuffCRC::MEDICAL_ENHANCE_ACTION;
	case 4: return BuffCRC::MEDICAL_ENHANCE_QUICKNESS;
	case 5: return BuffCRC::MEDICAL_ENHANCE_STAMINA;
	case 6: return BuffCRC::MEDICAL_ENHANCE_POISON;
	case 7: return BuffCRC::MEDICAL_ENHANCE_DISEASE;
	default: return BuffCRC::MEDICAL_ENHANCE_HEALTH;
	}
}

String FsBuffItemImplementation::getTimeString(uint32 timestamp) {
	String abbrvs[4] = {"seconds", "minutes", "hours", "days"};

	int intervals[4] = {1, 60, 3600, 86400};
	int values[4] = {0, 0, 0, 0};

	StringBuffer str;

	for (int i = 3; i > -1; --i) {
		values[i] = floor((float)(timestamp / intervals[i]));
		timestamp -= values[i] * intervals[i];

		if (values[i] > 0) {
			if (str.length() > 0)
				str << ", ";

			str << ((i == 0) ? "and " : "") << values[i] << " " << abbrvs[i];
		}
	}

	return str.toString();
}
