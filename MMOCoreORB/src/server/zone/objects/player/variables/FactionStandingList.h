/*
 * FactionStandingList.h
 *
 *  Created on: Mar 16, 2011
 *      Author: crush
 */

#ifndef FACTIONSTANDINGLIST_H_
#define FACTIONSTANDINGLIST_H_

#include "engine/engine.h"

#include "engine/util/json_utils.h"

class FactionStandingList : public Serializable {
	SerializableString factionRank;
	int rebelPoints;
	int imperialPoints;
	SynchronizedVectorMap<String, float> factions;
	//int huttPoints; //Disabled

public:
	FactionStandingList() {
		factions.setAllowOverwriteInsertPlan();
		factions.setNullValue(0.f);

		rebelPoints = 0;
		imperialPoints = 0;

		addSerializableVariables();
	}

	FactionStandingList(const FactionStandingList& f) : Object(), Serializable() {
		factions.setAllowOverwriteInsertPlan();
		factions.setNullValue(0.f);

		factionRank = f.factionRank;
		rebelPoints = f.rebelPoints;
		imperialPoints = f.imperialPoints;
		factions = f.factions;

		addSerializableVariables();
	}

	FactionStandingList& operator=(const FactionStandingList& f) {
		if (this == &f)
			return *this;

		factionRank = f.factionRank;
		rebelPoints = f.rebelPoints;
		imperialPoints = f.imperialPoints;
		factions = f.factions;

		return *this;
	}

	void addSerializableVariables() {
		addSerializableVariable("factionRank", &factionRank);
		addSerializableVariable("rebelPoints", &rebelPoints);
		addSerializableVariable("imperialPoints", &imperialPoints);
		addSerializableVariable("factions", &factions);
	}

	friend void to_json(nlohmann::json& j, const FactionStandingList& l) {
		j["factionRank"] = l.factionRank;
		j["rebelPoints"] = l.rebelPoints;
		j["imperialPoints"] = l.imperialPoints;
		j["factions"] = l.factions.getMapUnsafe();
	}

	float get(const String& faction) const {
		return getFactionStanding(faction);
	}

	int size() const {
		return factions.size();
	}

	void put(const String& faction, float amount) {
		if (faction == "imperial")
			setImperialPoints(amount);
		else if (faction == "rebel")
			setRebelPoints(amount);
		else
			factions.put(faction, amount);
	}

	float getFactionStanding(const String& faction) const {
		if (faction == "imperial")
			return getImperialPoints();
		else if (faction == "rebel")
			return getRebelPoints();
		else
			return factions.get(faction);
	}

	bool contains(const String& faction) const {
		if (faction == "imperial" || faction == "rebel")
			return true;
		else
			return factions.contains(faction);
	}

	bool isPvpFaction(const String& faction) const {
		return faction == "imperial" || faction == "rebel";
	}

	const String& getFactionRank() const {
		return factionRank;
	}

	void setRebelPoints(int amount) {
		rebelPoints = amount;
	}

	void setImperialPoints(int amount) {
		imperialPoints = amount;
	}

	void increaseRebelPoints(int amount) {
		rebelPoints += amount;
	}

	void decreaseRebelPoints(int amount) {
		rebelPoints -= amount;
	}

	void increaseImperialPoints(int amount) {
		imperialPoints += amount;
	}

	void decreaseImperialPoints(int amount) {
		imperialPoints -= amount;
	}

	void setFactionRank(const String& rank) {
		factionRank = rank;
	}

	int getImperialPoints() const {
		return imperialPoints;
	}

	int getRebelPoints() const {
		return rebelPoints;
	}

	int getHuttPoints() const {
		return 0;
	}

	void insertToMessage(BaseMessage* message) const {
		message->insertAscii(factionRank);
		message->insertInt(rebelPoints);
		message->insertInt(imperialPoints);
		message->insertInt(0); //Hutt Points;

		int listSize = factions.size();

		message->insertInt(listSize);

		for (int i = 0; i < listSize; ++i) {
			auto key = factions.getKey(i);

			message->insertAscii(key);
		}

		message->insertInt(listSize);

		for (int i = 0; i < listSize; ++i)
			message->insertFloat(factions.get(i));
	}
};

#endif /* FACTIONSTANDINGLIST_H_ */
