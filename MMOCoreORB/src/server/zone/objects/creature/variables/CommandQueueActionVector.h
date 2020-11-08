/*
 * CommandQueueActionVector.h
 *
 *  Created on: May 30, 2011
 *      Author: theanswer
 */

#ifndef COMMANDQUEUEACTIONVECTOR_H_
#define COMMANDQUEUEACTIONVECTOR_H_

#include "engine/engine.h"
#include "CommandQueueAction.h"

template <class O>
class CommandReference : public Reference<O> {
public:
	CommandReference() : Reference<O>() {

	}

	CommandReference(const CommandReference& ref) : Reference<O>(ref) {

	}

	CommandReference(O obj) : Reference<O>(obj) {

	}

	CommandReference& operator=(const CommandReference& val) {
		if (this == &val) {
			return *this;
		}

		Reference<O>::operator=(val);

		return *this;
	}

	int compareTo(const CommandReference& val) const {
		return Reference<O>::get()->compareTo(val.get());
	}

};

class CommandQueueActionVector : public SortedVector<CommandReference<CommandQueueAction*> > {
public:
	CommandQueueActionVector() {

	}

	CommandQueueActionVector(const CommandQueueActionVector& v) : SortedVector<CommandReference<CommandQueueAction*> >(v) {

	}

	CommandQueueActionVector& operator=(const CommandQueueActionVector& v) {
		if (this == &v)
			return *this;

		SortedVector<CommandReference<CommandQueueAction*> >::operator=(v);

		return *this;
	}

	Object* clone() {
		return ObjectCloner<CommandQueueActionVector>::clone(this);
	}

	Object* clone(void* object) {
		return TransactionalObjectCloner<CommandQueueActionVector>::clone(this);
	}
};


#endif /* COMMANDQUEUEACTIONVECTOR_H_ */
