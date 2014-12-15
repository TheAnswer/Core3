#ifndef DROID_MECHANICS_H
#define DROID_MECHANICS_H

#include "SharedLabratory.h"
#include "engine/engine.h"
#include "server/zone/managers/creature/DnaManager.h"
#include "server/zone/objects/creature/DroidObject.h"

namespace server {
namespace zone {
namespace managers {
namespace crafting {
namespace labratories {

class DroidMechanics {
public:
	DroidMechanics();
	virtual ~DroidMechanics();
	static float determineHam(float quality, int droidType){
		int maxHam = 1000;
		if(droidType == DroidObject::R_SERIES) {
			maxHam = 4500;
		}
		if(droidType == DroidObject::DZ70 || droidType == DroidObject::PROBOT) {
			maxHam = 3200;
		}
		if(droidType == DroidObject::LE_REPAIR) {
			maxHam = 6000;
		}
		return quality * (float)maxHam;
	}
	static float determineSpeed(int droidType, float ham) {
		if(droidType == DroidObject::R_SERIES)
			return 2.0 - (( 1 - ((4500.0 - ham)/1125.0))/2.0);
		if(droidType == DroidObject::PROBOT || droidType == DroidObject::DZ70)
			return 1.0 - (( 1 - ((3200.0)/800.0))/4.0);
		if (droidType == DroidObject::LE_REPAIR)
			return 2.0 - (( 1 - ((6000.0 - ham)/1500.0))/2.0);
		return 0;
	}
	static float determineHit(int droidType, float ham) {
		if(droidType == DroidObject::R_SERIES)
			return 0.33 + (0.08*(1.0 - ((4500.0-ham)/1125.0)));
		if(droidType == DroidObject::LE_REPAIR)
			return 0.33 + (0.08*(1.0 - ((6000.0-ham)/1500.0)));
		if(droidType == DroidObject::DZ70 || droidType == DroidObject::PROBOT)
			return 0.33 + (0.08*(1.0 - ((3200.0-ham)/800.0)));

		return 0;
	}
	static float determineMinDamage(int droidType, int rating) {
		if(droidType == DroidObject::R_SERIES)
			return ((rating/600.0) * 80) + 80;
		if(droidType == DroidObject::LE_REPAIR)
			return ((rating/600.0) * 45) + 45;
		if(droidType == DroidObject::PROBOT || droidType == DroidObject::DZ70)
			return ((rating/600.0) * 95) + 95;
		return 1;
	}
	static float determineMaxDamage(int droidType, int rating) {
		if(droidType == DroidObject::R_SERIES)
			return ((rating/600.0) * 85) + 85;
		if(droidType == DroidObject::LE_REPAIR)
			return ((rating/600.0) * 55) + 55;
		if(droidType == DroidObject::PROBOT || droidType == DroidObject::DZ70)
			return ((rating/600.0) * 100) + 100;
		return 1;
	}
	/** Used to determine harvest droid and trap droid skill mod*/
	static int determineDroidSkillBonus(float playerSkill, float droidSkill) {
		float bonus = (float)droidSkill/(3.55*((1 +playerSkill)/((float)100)));
		return ceil(bonus);
	}
};

}
}
}
}
}

using namespace server::zone::managers::crafting::labratories;
#endif /* DROID_MECHANICS_H */
