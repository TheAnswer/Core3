/*
 * GroupList.h
 *
 *  Created on: 29/12/2009
 *      Author: victor
 */

#ifndef GROUPLIST_H_
#define GROUPLIST_H_

#include "../scene/variables/DeltaVector.h"
#include "GroupMember.h"

class GroupList : public DeltaVector<GroupMember> {
public:
	virtual ~GroupList() {

	}
};


#endif /* GROUPLIST_H_ */
