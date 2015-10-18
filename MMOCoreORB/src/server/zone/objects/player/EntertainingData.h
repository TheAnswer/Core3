/*
 * EntertainingData.h
 *
 *  Created on: 20/09/2010
 *      Author: victor
 */

#ifndef ENTERTAININGDATA_H_
#define ENTERTAININGDATA_H_

#include "engine/engine.h"

class EntertainingData : public Serializable {
	int duration;
	int strength;
	int timeStarted;
public:
	EntertainingData() {
		duration = 0;
		strength = 0;
		timeStarted = time(0);
		addSerializableVariables();
	}

	EntertainingData(const EntertainingData& d) : Object(), Serializable() {
		duration = d.duration;
		strength = d.strength;
		timeStarted = d.timeStarted;

		addSerializableVariables();
	}

	EntertainingData& operator=(const EntertainingData& d) {
		if (this == &d)
			return *this;

		duration = d.duration;
		strength = d.strength;
		timeStarted = d.timeStarted;

		return *this;
	}

	inline void addSerializableVariables() {
		addSerializableVariable("duration", &duration);
		addSerializableVariable("strength", &strength);
	}

	inline int getDuration() {
		return duration;
	}

	inline int getStrength() {
		return strength;
	}
	inline int getTimeStarted() {
		return timeStarted;
	}
	inline void setStrength(int str) {
		strength = str;
	}

	inline void incrementStrength(int incr) {
		strength += incr;
	}

	inline void setDuration(int dur) {
		duration = dur;
	}

	inline void incrementDuration(int incr) {
		duration += incr;
	}
};

#endif /* ENTERTAININGDATA_H_ */
