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

	float radius = ShipAiAgent::DEFAULTAGGRORADIUS + agent->getTemplateRadius() + targetShip->getTemplateRadius();

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
	return agent->getFollowShipObject().get() != nullptr;
}

template<> bool CheckRetreat::check(ShipAiAgent* agent) const {
	if (agent->isRetreating())
		return false;

	SpacePatrolPoint* homeLocation = agent->getHomeLocation();

	if (homeLocation == nullptr)
		return false;

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

template<> bool CheckStopEvading::check(ShipAiAgent* agent) const {
	int pointSize = agent->getPatrolPointSize();

	// No evade points set or agents engines are disabled
	if (pointSize <= 0 || (agent->getCurrentSpeed() == 0.f))
		return true;

	ManagedReference<ShipObject*> targetShip = agent->getTargetShipObject();

	if (targetShip != nullptr && targetShip->isShipAiAgent()) {
		Locker clocker(targetShip, agent);

		ShipAiAgent* targetAgent = targetShip->asShipAiAgent();

		// Target Agent is already evading, lets not do it at the same time;
		if (targetAgent != nullptr && targetAgent->getMovementState() == ShipAiAgent::EVADING)
			return true;
	}

	SpacePatrolPoint evadePoint = agent->getFinalPosition();

	return evadePoint.isEvadePoint() && agent->isInRangePosition(evadePoint.getWorldPosition(), agent->getMaxDistance());
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
	auto componentOptMap = agent->getComponentOptionsMap();

	if (componentOptMap == nullptr)
		return false;

	uint32 flags = componentOptMap->get(Components::ENGINE);

	return (flags & ShipComponentFlag::DISABLED) || (flags & ShipComponentFlag::DEMOLISHED);
}

template<> bool CheckEvadeChance::check(ShipAiAgent* agent) const {
	// Agent engines are disabled, no evading
	if (agent->getCurrentSpeed() == 0.f)
		return false;

	// Don't immediately evade again
	if (!agent->isEvadeDelayPast())
		return false;

	ManagedReference<ShipObject*> targetShip = agent->getTargetShipObject();

	if (targetShip == nullptr)
		return false;

	Locker clocker(targetShip, agent);

	if (targetShip->isShipAiAgent()) {
		ShipAiAgent* targetAgent = targetShip->asShipAiAgent();

		// Target Agent is already evading, lets not do it at the same time;
		if (targetAgent != nullptr && targetAgent->getMovementState() == ShipAiAgent::EVADING)
			return false;
	}

	// Random chance we do not evade
	if (System::random(100) < 50)
		return false; 

	float maxDistance = agent->getMaxDistance();
	float maxSquared = maxDistance * maxDistance;

	float minDistance = targetShip->getBoundingRadius() + agent->getBoundingRadius();
	float minSquared = minDistance * minDistance;

	SpacePatrolPoint nextPoint = agent->getNextPosition();
	float sqrDist = nextPoint.getWorldPosition().squaredDistanceTo(targetShip->getPosition());

	// agent->info(true) << agent->getDisplayedName() << " next patrol point: " << nextPoint.getWorldPosition().toString() << " Sq Distance to Target = " << sqrDist;
	// agent->info(true) << " Min Distance: " << minDistance << " Min Distance Sq: " << minSquared << " Max Distance: " << maxDistance << " Max Distance Sq: " << maxSquared;

	// Evade if the too far or too close to the target ship
	return sqrDist > maxSquared || sqrDist < minSquared;
}
