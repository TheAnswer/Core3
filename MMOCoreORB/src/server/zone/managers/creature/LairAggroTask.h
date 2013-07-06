/*
 * LairAggroTask.h
 *
 *  Created on: Jul 6, 2013
 *      Author: swgemu
 */

#ifndef LAIRAGGROTASK_H_
#define LAIRAGGROTASK_H_

#include "engine/engine.h"
#include "LairObserver.h"
#include "server/zone/objects/tangible/TangibleObject.h"
class LairAggroTask : public Task {

	ManagedWeakReference<TangibleObject*> lair;
	ManagedWeakReference<LairObserver*> observer;
	ManagedWeakReference<TangibleObject*> attacker;

public:

	LairAggroTask(TangibleObject* obj, ManagedWeakReference<TangibleObject*> attacker, LairObserver* observer) {
		 lair = obj;
		 this->attacker = attacker;
		 this->observer = observer;
	}

	void run() {
		ManagedReference<TangibleObject*> strongRef = lair.get();

		if (strongRef == NULL)
			return;


		ManagedReference<LairObserver*> strongObserver = observer.get();
		ManagedReference<TangibleObject*> strongAttackerRef = attacker.get();

		if (strongObserver == NULL)
			return;

		Locker locker(strongRef);

		strongObserver->doAggro(strongRef, strongAttackerRef);
	}
};


#endif /* LAIRAGGROTASK_H_ */
