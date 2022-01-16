/*
 * PendingTasksMap.h
 *
 *  Created on: 04/05/2010
 *      Author: victor
 */

#ifndef PENDINGTASKSMAP_H_
#define PENDINGTASKSMAP_H_

#include "engine/engine.h"
#include <boost/lockfree/queue.hpp>

namespace server {
namespace zone {
namespace objects {
namespace scene {
class SceneObject;

namespace variables {

template <class TaskOwner>
class OrderedTaskExecutioner;
}
}
} // namespace objects
} // namespace zone
} // namespace server

class PendingTasksMap : public Object {
protected:
	mutable Mutex mutex;

	VectorMap<String, Reference<Task*>> taskMap;

	typedef boost::lockfree::queue<Task*, boost::lockfree::fixed_sized<false>> TaskQueue;
	TaskQueue pendingTasks{};
	AtomicLong pendingTasksSize{};

public:
	PendingTasksMap();

	int put(const String& name, Task* task);

	void drop(const String& name);

	bool contains(const String& name);

	Reference<Task*> get(const String& name) const;

	template <class Owner>
	void putOrdered(Task* task, Owner* owner) {
		task->acquire();

		const auto values = pendingTasksSize.increment();

		const bool result = pendingTasks.push(task);
		E3_ASSERT(result);

		if (values == 1) {
			auto newTask = new server::zone::objects::scene::variables::OrderedTaskExecutioner<Owner>(owner);
			newTask->setCustomTaskQueue(task->getCustomTaskQueue());
			newTask->execute();
		}
	}

	Pair<Reference<Task*>, std::size_t> popNextOrderedTask();
};

#endif /* PENDINGTASKSMAP_H_ */
