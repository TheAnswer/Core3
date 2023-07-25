#include "ChecksSpace.h"
#include "server/zone/objects/ship/ai/ShipAiAgent.h"
#include "server/zone/objects/ship/ShipComponentFlag.h"

// full template specializations need to go in cpp so they don't get
// defined multiple times.

using namespace server::zone::objects::ship::ai::btspace::leafspace;

template<> bool CheckDestination::check(ShipAiAgent* agent) const {
	return agent->setDestination() > 0;
}

template<> bool CheckHasPatrol::check(ShipAiAgent* agent) const {
	return agent->getPatrolPointSize() > 0;
}

template<> bool CheckMovementState::check(ShipAiAgent* agent) const {
	return agent->getMovementState() == checkVar;
}

template<> bool CheckProspectInRange::check(ShipAiAgent* agent) const {
	ManagedReference<ShipObject*> targetShip = nullptr;

	if (agent->peekBlackboard("targetShipProspect"))
		targetShip = agent->readBlackboard("targetShipProspect").get<ManagedReference<ShipObject*>>();

	if (targetShip == nullptr)
		return false;

	Locker clock(targetShip, agent);

	float aggroMod = 0.25f;

	if (agent->peekBlackboard("aggroMod"))
		aggroMod = agent->readBlackboard("aggroMod").get<float>();

	float radius = ShipAiAgent::DEFAULTAGGRORADIUS; // TODO: base on template/ship size

	radius = Math::min(ShipAiAgent::DEFAULTAGGRORADIUS, radius * aggroMod);

#ifdef DEBUG_SHIP_AI
	if (agent->peekBlackboard("aiDebug") && agent->readBlackboard("aiDebug") == true)
		agent->info(true) << agent->getDisplayedName() << " - CheckProspectInRange -- AggroRadius = " << radius;
#endif // DEBUG_SHIP_AI

	return agent->isInRange(targetShip, radius);
}

template<> bool CheckAggroDelayPast::check(ShipAiAgent* agent) const {
	return agent->isAggroDelayPast();
}

template<> bool CheckHasFollow::check(ShipAiAgent* agent) const {
	return agent->getFollowShipObject() != nullptr;
}

template<> bool CheckRetreat::check(ShipAiAgent* agent) const {
	if (agent->isRetreating())
		return false;

	SpacePatrolPoint* homeLocation = agent->getHomeLocation();

	if (homeLocation == nullptr)
		return false;

	ShipObject* target = agent->getFollowShipObject().get();

	if (target != nullptr)
		return !homeLocation->isInRange(target, checkVar);

	return !homeLocation->isInRange(agent, checkVar);
}

template<> bool CheckProspectAggression::check(ShipAiAgent* agent) const {
	ManagedReference<ShipObject*> targetShip = nullptr;

	if (agent->peekBlackboard("targetShipProspect"))
		targetShip = agent->readBlackboard("targetShipProspect").get<ManagedReference<ShipObject*> >();

	if (targetShip == nullptr)
		return false;

	Locker clock(targetShip, agent);

	return agent->isAggressiveTo(targetShip);
}

template<> bool CheckRefireRate::check(ShipAiAgent* agent) const {
	if (!agent->peekBlackboard("refireInterval"))
		return true;

	uint64 timeNow = System::getMiliTime();
	uint64 lastFire = agent->readBlackboard("refireInterval").get<uint64>() + (uint64)checkVar;
	int timeDiff = (int)timeNow - (int)lastFire;

	//agent->info(true) << agent->getDisplayedName() <<  " RefireInterval check time diff: " << timeDiff << " Last Fire: " << lastFire << " Time now: " << timeNow;

	return timeDiff > 0;
}

template<> bool CheckEvadeDelayPast::check(ShipAiAgent* agent) const {
	return agent->isEvadeDelayPast();
}

template<> bool CheckTargetIsValid::check(ShipAiAgent* agent) const {
	ManagedReference<ShipObject*> targetShip = nullptr;

	if (agent->peekBlackboard("targetShipProspect"))
		targetShip = agent->readBlackboard("targetShipProspect").get<ManagedReference<ShipObject*> >();

	if (targetShip == nullptr)
		return false;

	Locker clock(targetShip, agent);

	// agent->info(true) << agent->getDisplayedName() << " CheckTargetIsValid - Target: " << targetShip->getDisplayedName() << " Chassis Health: " << targetShip->getChassisCurrentHealth();

	return targetShip->getChassisCurrentHealth() > 0.f && targetShip->isAttackableBy(agent);
}

template<> bool CheckEnginesDisabled::check(ShipAiAgent* agent) const {
	uint32 engineBitmask = agent->getComponentOptionsMap()->get(Components::ENGINE);

	return engineBitmask & ShipComponentFlag::DISABLED;
}

template<> bool CheckEvadeChance::check(ShipAiAgent* agent) const {
	return System::random(100) <= checkVar;
}