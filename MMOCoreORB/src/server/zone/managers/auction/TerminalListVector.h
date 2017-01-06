/*
 * TerminalListVector.h
 *
 *  Created on: Jul 4, 2012
 *      Author: kyle
 */

#ifndef TERMINALLISTVECTOR_H_
#define TERMINALLISTVECTOR_H_

class TerminalItemList : public SortedVector<ManagedReference<AuctionItem*> >, public ReadWriteLock {
protected:
	bool searchable;

public:
	TerminalItemList() {
		searchable = false;
	}

	TerminalItemList(const TerminalItemList& list) : SortedVector<ManagedReference<AuctionItem*> >(list), ReadWriteLock() {
		searchable = list.searchable;
	}

	TerminalItemList& operator=(const TerminalItemList& list) {
		if (this == &list)
			return *this;

		searchable = list.searchable;

		return *this;
	}

	inline void setSearchable(bool value) {
		searchable = value;
	}

	inline bool isSearchable() {
		return searchable == true;
	}

	int put(const ManagedReference<AuctionItem*>& o) {
		Locker locker(this);

		return SortedVector<ManagedReference<AuctionItem*> >::put(o);
	}

	bool drop(const ManagedReference<AuctionItem*>& o) {
		Locker locker(this);

		return SortedVector<ManagedReference<AuctionItem*> >::drop(o);
	}

};

class TerminalRegionList : public VectorMap<uint64, Reference<TerminalItemList*> >, public ReadWriteLock {

};

class TerminalPlanetList : public VectorMap<String, Reference<TerminalRegionList*> >, public ReadWriteLock {

};

class TerminalGalaxyList : public VectorMap<String, Reference<TerminalPlanetList*> >, public ReadWriteLock {

};

class TerminalListVector : public SortedVector<Reference<TerminalItemList*> > {

};

#endif /* TERMINALLISTVECTOR_H_ */
