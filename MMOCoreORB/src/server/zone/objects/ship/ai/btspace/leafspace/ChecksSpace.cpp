#include "ChecksSpace.h"
#include "server/zone/objects/ship/ai/ShipAiAgent.h"

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
	ManagedReference<SceneObject*> tar = nullptr;

	if (agent->peekBlackboard("targetProspect"))
		tar = agent->readBlackboard("targetProspect").get<ManagedReference<SceneObject*> >();

	if (checkVar > 0.f) {
		return tar != nullptr && agent->isInRange(tar, checkVar);
	} else if (tar != nullptr && agent->peekBlackboard("aggroMod")) {
		float aggroMod = agent->readBlackboard("aggroMod").get<float>();
		float radius = ShipAiAgent::DEFAULTAGGRORADIUS; // TODO: base on template/ship size

		if (radius == 0)
			radius = ShipAiAgent::DEFAULTAGGRORADIUS;

		radius = Math::min(512.f, radius * aggroMod);

		return agent->isInRange(tar, radius);
	}

	return false;
}

template<> bool CheckAggroDelayPast::check(ShipAiAgent* agent) const {
	Time* delay = agent->getAggroDelay();

	return delay != nullptr && delay->isPast();
}

template<> bool CheckHasFollow::check(ShipAiAgent* agent) const {
	return agent->getFollowObject() != nullptr;
}

template<> bool CheckRetreat::check(ShipAiAgent* agent) const {
	if (agent->isRetreating())
		return false;

	SpacePatrolPoint* homeLocation = agent->getHomeLocation();

	if (homeLocation == nullptr)
		return false;

	SceneObject* target = agent->getFollowObject().get();

	if (target != nullptr)
		return !homeLocation->isInRange(target, checkVar);

	return !homeLocation->isInRange(agent, checkVar);
}