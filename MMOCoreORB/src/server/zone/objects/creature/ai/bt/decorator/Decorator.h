#ifndef DECORATOR_H_
#define DECORATOR_H_

#include "server/zone/objects/creature/ai/bt/Behavior.h"

namespace server {
namespace zone {
namespace objects {
namespace creature {
namespace ai {
namespace bt {
namespace decorator {

class Decorator : public Behavior {
protected:
	Reference<Behavior*> child;

public:
	Decorator(const String& className, const uint32 id, const LuaObject& args)
		: Behavior(className, id, args) {}

	Decorator(const Decorator& b)
		: Behavior(b) {
		child = b.child;
	}

	Decorator& operator=(const Decorator& b) {
		if (this == &b)
			return *this;

		Behavior::operator=(b);
		child = b.child;

		return *this;
	}

	virtual ~Decorator() {}
	
	bool isDecorator() const {
		return true;
	}

	bool hasChild(Behavior* c) const {
		return child == c;
	}

	Behavior* getChild(uint32 cID) const {
		if (child->getID() == cID) return child;

		return NULL;
	}

	Vector<const Behavior*> getRecursiveChildList() const {
		Vector<const Behavior*> retVal;
		retVal.add(this);

		retVal.addAll(child->getRecursiveChildList());

		return retVal;
	}

	virtual void setChild(Reference<Behavior*> newChild) {
		assert(child != this);

		child = newChild;
	}

	String print() const {
		StringBuffer stream;
		stream << Behavior::print() << "[";
		if (child != NULL)
			stream << child->print();
		stream << "]";
		return stream.toString();
	}

	virtual bool checkConditions(AiAgent* agent) const {
		return child != NULL && Behavior::checkConditions(agent);
	}

	Behavior::Status doAction(Reference<AiAgent*> agent) const {
		std::cout << "0x" << std::hex << id << " " << print().toCharArray() << std::endl;

		if (!this->checkConditions(agent))
			return INVALID; // TODO: should this be FAILURE?
		
		if (!agent->isRunningBehavior(id))
			this->start(agent);
		else
			agent->popRunningChain();
			
		Behavior::Status result = this->execute(agent);
		std::cout << "0x" << std::hex << id << " " << result << std::endl;
		
		if (result == RUNNING)
			agent->addRunningID(id);
		else
			this->end(agent);
		
		return result;
    }
};

}
}
}
}
}
}
}

#endif /* DECORATOR_H_ */
