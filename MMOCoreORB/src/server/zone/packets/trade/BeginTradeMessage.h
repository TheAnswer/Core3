/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef BEGINTRADEMESSAGE_H_
#define BEGINTRADEMESSAGE_H_

class BeginTradeMessage : public BaseMessage {
public:
	BeginTradeMessage(uint64 playerID) : BaseMessage(0x0E) {
		insertShort(0x02);
		insertInt(0x325932D8);
		insertLong(playerID);
	}
};

#endif /*BEGINTRADEMESSAGE_H_*/
