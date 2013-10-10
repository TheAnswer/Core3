/*
 * DestructibleBuildingDataComponent.h
 *
 *  Created on: Oct 22, 2012
 *      Author: pete
 */

#ifndef DESTRUCTIBLEBUILDINGDATACOMPONENT_H_
#define DESTRUCTIBLEBUILDINGDATACOMPONENT_H_
#include "engine/engine.h"
//#include "server/zone/objects/scene/components/DataObjectComponent.h"
#include "server/zone/objects/building/components/BuildingDataComponent.h"
#include "system/util/Vector.h"
#include "system/util/VectorMap.h"
#include "system/util/HashTable.h"

class DestructibleBuildingDataComponent : public BuildingDataComponent, public Logger {

private:
	HashTable<int, String> dnaProfiles;

	String lastDNAString;

	int  intCurrentState; // serialized
	bool terminalDamaged; //serialized
	bool inRepair; // serialized
	int  sampleMatches; // serialized
	int  switchesTurnedOn; // serialized
	Vector<bool> powerSwitchesTester;
	Vector<uint64> turretSlots;
	Vector<uint64> minefieldSlots;
	Vector<uint64> scannerSlots;

	Time lastVulnerableTime; // serialized
	Time nextVulnerableTime; // serialized
	Time vulnerabilityEndTime; //se rialized
	Time placementTime; // serialized
	Time lastResetTime; // serialized
	Time repairTime; // serialized
	Time rebootFinishTime; // serialized
	int uplinkBand; // secret code used to jam the uplink
	bool activeDefenses;
	bool exposed;

public:
	const static int INVULNERABLE = 0;
	const static int VULNERABLE = 1;
	const static int BAND = 2;
	const static int JAMMED = 3;
	const static int SLICED = 4;
	const static int DNA = 5;
	const static int OVERLOADED = 6;
	const static int ALIGNED = 7;
	const static int SHUTDOWNSEQUENCE = 8;

	const static int POWERSWITCHCOUNT = 8;

	DestructibleBuildingDataComponent(){
		this->setLoggingName("DESTOBJ");
		intCurrentState = INVULNERABLE;
		repairTime = 0;
		sampleMatches = 0;
		lastDNAString = "";

		// all switches on
		switchesTurnedOn = 8;
		//powerSwitches[8] = {true};

		for(int i=0;i<POWERSWITCHCOUNT;i++){
			powerSwitchesTester.add(i,true);

		}
		powerSwitchesTester.get(0) = false;

		activeDefenses = true;
		exposed = false;
		terminalDamaged = false;
		rebootFinishTime = Time(0);
		dnaProfiles.removeAll();

		uplinkBand = 0;
		inRepair = false;
	}

	virtual ~DestructibleBuildingDataComponent(){

	}

	bool toBinaryStream(ObjectOutputStream* stream);

	bool parseFromBinaryStream(ObjectInputStream* stream);

	bool isVulnerable(){
		return (intCurrentState >= VULNERABLE);
	}

	bool isDestructibleBuildingData(){
		return true;
	}

	int getState(){
		return intCurrentState;
	}

	Time getLastVulnerableTime(){
		return lastVulnerableTime;
	}

	Time getNextVulnerableTime(){
		return nextVulnerableTime;
	}

	Time getVulnerabilityEndTime(){
		return vulnerabilityEndTime;
	}

	Time getPlacmenetTime(){
		return placementTime;
	}

	Time getLastResetTime(){
		return lastResetTime;
	}

	Time getSliceRepairTime(){
		return repairTime;
	}


	int getUplinkBand(){
		return uplinkBand;
	}

	int getSampleMatches(){
		return sampleMatches;
	}

	void incrementSampleMatches(){
		sampleMatches++;
	}

	int isTerminalBeingRepaired(){
		return inRepair;
	}

	bool isTerminalDamanged(){
		return terminalDamaged;
	}
	void setState(int state);

	void setLastVulnerableTime(Time time){
		lastVulnerableTime = time;
	}

	void setNextVulnerableTime(Time time){
		nextVulnerableTime = time;
	}

	void setVulnerabilityEndTime(Time time){
		vulnerabilityEndTime = time;
	}

	void setPlacementTime(Time time){
		placementTime = time;
	}

	void setLastResetTime(Time time){
		lastResetTime = time;
	}

	void setUplinkBand(int band){
		uplinkBand = band;
	}

	void setSliceRepairTime(Time time){
		repairTime = time;
	}

	void setTerminalBeingRepaired(bool val){
		inRepair = val;
	}

	void setTerminalDamaged(bool val){
		terminalDamaged = val;
	}

	void setSampleMatches(int val){
		sampleMatches = val;
	}

	void setActiveTurret(int indx, uint64 turretOID){
		turretSlots.get(indx) = turretOID;
	}

	void setActiveMinefield(int indx, uint64 minefieldOID){
		minefieldSlots.get(indx) == minefieldOID;
	}

	void modifySampleAt(int indx, String val){
		if(indx < dnaProfiles.size()) {
			dnaProfiles.remove(indx);
			dnaProfiles.put(indx,val);
		}
	}
	void initializeTransientMembers();

	void addDNAProfile(int indx, String val){
		dnaProfiles.put(indx, val);

	}
	void setSystemDNAString(String val){
		lastDNAString = val;
	}
	String getSystemDNAString(){
		return lastDNAString;
	}
	String getDNAProfile(int id){
		return dnaProfiles.get(id);
	}

	bool getPowerPosition(int indx){
		if(indx<8)
		{
			return powerSwitchesTester.get(indx);
			//return powerSwitches[indx];

		}
		return false;
	}
	void clearDNAProfiles(){
		dnaProfiles.removeAll();
	}

	bool isDNAInitialized(){
		return (dnaProfiles.size() > 0);
	}

	void turnSwitchOff(int indx){
		if(indx < 8 && powerSwitchesTester.get(indx))	{
			powerSwitchesTester.get(indx) = false;
			switchesTurnedOn--;
		}

	}

	void turnSwitchOn(int indx){
		if(indx < 8 && !powerSwitchesTester.get(indx))	{
			powerSwitchesTester.get(indx) = true;
			switchesTurnedOn++;
		}
	}

	int getOnSwitchCount(){
		return switchesTurnedOn;
	}

	void turnAllSwitchesOn(){
		for(int i = 0;i < POWERSWITCHCOUNT;i++){
			//powerSwitches.put(i,true);
			powerSwitchesTester.get(i) = true;

		}
		switchesTurnedOn = POWERSWITCHCOUNT;
	}


	int getTotalTurretCount(){
		return turretSlots.size();
	}

	int getTotalMinefieldCount(){
		return minefieldSlots.size();
	}

	int getTotalScannerCount(){
		return scannerSlots.size();
	}
	bool isTurretSlotOccupied(int indx){
		return (turretSlots.get(indx) > 0);
	}

	bool isMinefieldSlotOccupied(int indx){
		return (minefieldSlots.get(indx) > 0);
	}

	uint64 getTurretID(int indx){
		return turretSlots.elementAt(indx);
	}

	uint64 getMinefieldOID(int indx){
		return minefieldSlots.elementAt(indx);
	}

	bool hasTurret(uint64 turretID){
		return turretSlots.contains(turretID);
	}

	bool hasMinefield(uint64 minefieldOID){
		return minefieldSlots.contains(minefieldOID);
	}

	int getIndexOfTurret(uint64 turretID){

		for(int i =0; i< turretSlots.size();i++){
			if(turretSlots.elementAt(i) == turretID)
				return i;
		}
		return -1;
	}

	int getIndexOfMinefield(uint64 minefieldOID){
		for(int i=0; i < minefieldSlots.size(); i++){
			if(minefieldSlots.elementAt(i) == minefieldOID){
				return i;
			}
		}
		return -1;
	}

	void setTurretID(int indx, uint64 turretOID){
		turretSlots.elementAt(indx) = turretOID;
	}

	void addScanner(int indx, uint64 scannerOID){
		scannerSlots.add(indx,scannerOID);
	}
	void addTurret(int indx, uint64 turretOID){
		turretSlots.add(indx, turretOID);
	}

	void setScannerID(int indx, uint64 scannerOID){
		scannerSlots.elementAt(indx) = scannerOID;
	}
	void setMinefieldID(int indx, uint64 minefieldOID){
		minefieldSlots.elementAt(indx) = minefieldOID;
	}

	void addMinefield(int indx, uint64 minefieldOID){
		minefieldSlots.add(indx, minefieldOID);
	}

	bool isGCWBaseData(){
		return true;
	}

	bool hasDefense(){
		return activeDefenses;
	}

	void setDefense(bool value){
		activeDefenses = value;
	}

	void setRebootFinishTime(Time val){
		rebootFinishTime = val;
	}

	Time getRebootFinishTime(){
		return rebootFinishTime;
	}

private:

	int writeObjectMembers(ObjectOutputStream* stream);
	bool readObjectMember(ObjectInputStream* stream, const String& name);
};




#endif /* DESTRUCTIBLEBUILDINGDATACOMPONENT_H_ */
