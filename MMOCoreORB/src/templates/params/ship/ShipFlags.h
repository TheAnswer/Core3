/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.
*/

#ifndef SHIPFLAG_H_
#define SHIPFLAG_H_

class ShipFlag {
public:
	enum {
		NONE 				= 0x00,

		TEST 				= 0x000001,
		LASTAIMASK 			= 0x000002, // keep this updated so we can loop through the masks
	};
};

#endif /* SHIPFLAG_H_ */
