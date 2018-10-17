/*
 * CloseObjectsVector.h
 *
 *  Created on: 11/06/2012
 *      Author: victor
 */

#ifndef ZONECLOSEOBJECTSVECTOR_H_
#define ZONECLOSEOBJECTSVECTOR_H_

#include "system/util/SortedVector.h"
#include "system/lang/ref/Reference.h"
#include "system/thread/ReadWriteLock.h"

#include "engine/core/ManagedReference.h"
namespace server {
 namespace zone {
class QuadTreeEntry;

class CloseObjectsVector : public Object {
	mutable ReadWriteLock mutex;
	SortedVector<Reference<server::zone::QuadTreeEntry*> > objects;

	VectorMap<uint32, SortedVector<server::zone::QuadTreeEntry*> > messageReceivers;

	AtomicInteger count;

#ifdef CXX11_COMPILER
	static_assert(sizeof(server::zone::QuadTreeEntry*) == sizeof(Reference<server::zone::QuadTreeEntry*>), "Reference<> size is not the size of a pointer");
#endif

protected:
	void dropReceiver(server::zone::QuadTreeEntry* entry);
	void putReceiver(server::zone::QuadTreeEntry* entry, uint32 receiverTypes);
public:
	enum { PLAYERTYPE=1, CREOTYPE=2, COLLIDABLETYPE=4, STRUCTURETYPE=8, MAXTYPES=STRUCTURETYPE };

	CloseObjectsVector();

	Reference<server::zone::QuadTreeEntry*> remove(int index);

	bool contains(const Reference<server::zone::QuadTreeEntry*>& o) const;

	void removeAll(int newSize = 10, int newIncrement = 5);

	bool drop(const Reference<server::zone::QuadTreeEntry*>& o);

	void safeCopyTo(Vector<server::zone::QuadTreeEntry*>& vec) const;

	void safeCopyReceiversTo(Vector<server::zone::QuadTreeEntry*>& vec, uint32 receiverType) const;
	void safeCopyReceiversTo(Vector<ManagedReference<server::zone::QuadTreeEntry*> >& vec, uint32 receiverType) const;
	void safeAppendReceiversTo(Vector<server::zone::QuadTreeEntry*>& vec, uint32 receiverType) const;
	void safeAppendReceiversTo(Vector<ManagedReference<server::zone::QuadTreeEntry*> >& vec, uint32 receiverType) const;

	void safeCopyTo(Vector<ManagedReference<server::zone::QuadTreeEntry*> >& vec) const;

	SortedVector<ManagedReference<server::zone::QuadTreeEntry*> > getSafeCopy() const;

	const Reference<server::zone::QuadTreeEntry*>& get(int idx) const;

	int put(const Reference<server::zone::QuadTreeEntry*>& o);
	int put(Reference<server::zone::QuadTreeEntry*>&& o);

	int size() const NO_THREAD_SAFETY_ANALYSIS {
		return count;
	}

	void setNoDuplicateInsertPlan() {
		objects.setNoDuplicateInsertPlan();
	}
};

 }
}

using namespace server::zone;

#endif /* ZONECLOSEOBJECTSVECTOR_H_ */
