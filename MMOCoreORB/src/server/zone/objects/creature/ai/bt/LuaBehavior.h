/*
 * LuaBehavior.h
 *
 *  Created on: Sep 1, 2013
 *      Author: swgemu
 */

#ifndef LUABEHAVIOR_H_
#define LUABEHAVIOR_H_

#include "engine/engine.h"

namespace server {
namespace zone {
namespace objects {
namespace creature {
class AiAgent;
namespace ai {
namespace bt {

class LuaBehavior : public Object {
public:

	LuaBehavior(String name);
	virtual ~LuaBehavior();

	LuaBehavior(const LuaBehavior& b) : Object() {
		className = b.className;
	}

	LuaBehavior& operator=(const LuaBehavior& b) {
		if (this == &b)
			return *this;

		className = b.className;

		return *this;
	}

	bool initialize();

	bool checkConditions(AiAgent* agent);
	void start(AiAgent* agent);
	float end(AiAgent* agent);
	int doAction(AiAgent* agent);

	virtual uint16 getType();

	String print() {
		return this->className;
	}

protected:
	String className;
};

}
}
}
}
}
}

using namespace server::zone::objects::creature::ai::bt;

#endif /* LUABEHAVIOR_H_ */
