/*
 * DestructibleBuildingDataComponent.h
 *
 *  Created on: Oct 22, 2012
 *      Author: pete
 */

#ifndef DESTRUCTIBLEBUILDINGDATACOMPONENT_H_
#define DESTRUCTIBLEBUILDINGDATACOMPONENT_H_
#include "engine/engine.h"
#include "server/zone/objects/building/components/BuildingDataComponent.h"
#include "system/util/Vector.h"
#include "system/util/VectorMap.h"
#include "system/util/HashTable.h"

class DestructibleBuildingDataComponent : public BuildingDataComponent, public Logger {

private:
	Vector<String> dnaStrand;
	Vector<int> dnaLocks;
	String currentDnaChain;

	Vector<int> powerSwitchRules;
	Vector<bool> powerSwitchStates;

	int  intCurrentState; // serialized
	bool terminalDamaged; //serialized
	bool inRepair; // serialized

	Vector<uint64> turretSlots;
	Vector<uint64> minefieldSlots;
	Vector<uint64> scannerSlots;

	Time lastVulnerableTime; // serialized
	Time nextVulnerableTime; // serialized
	Time vulnerabilityEndTime; //se rialized
	Time placementTime; // serialized
	Time lastResetTime; // serialized
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
	const static int SHUTDOWNSEQUENCE = 7;
	const static int REBOOTSEQUENCE = 8;

	DestructibleBuildingDataComponent() {
		this->setLoggingName("DESTOBJ");
		intCurrentState = INVULNERABLE;

		currentDnaChain = "";

		activeDefenses = true;
		exposed = false;
		terminalDamaged = false;

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

	int getUplinkBand(){
		return uplinkBand;
	}

	int isTerminalBeingRepaired(){
		return inRepair;
	}

	bool isTerminalDamaged(){
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

	void setTerminalBeingRepaired(bool val){
		inRepair = val;
	}

	void setTerminalDamaged(bool val){
		terminalDamaged = val;
	}

	void setActiveTurret(int indx, uint64 turretOID){
		turretSlots.get(indx) = turretOID;
	}

	void setActiveMinefield(int indx, uint64 minefieldOID){
		minefieldSlots.get(indx) == minefieldOID;
	}

	void initializeTransientMembers();

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

	void clearDnaStrand() {
		dnaStrand.removeAll();
	}

	void setDnaStrand(Vector<String> strand) {
		dnaStrand = strand;
	}

	Vector<String> getDnaStrand() {
		return dnaStrand;
	}

	void clearDnaLocks() {
		dnaLocks.removeAll();
	}

	void setDnaLocks(Vector<int> locks) {
		dnaLocks = locks;
	}

	Vector<int> getDnaLocks() {
		return dnaLocks;
	}

	String getCurrentDnaChain() {
		return currentDnaChain;
	}

	void setCurrentDnaChain(String chain) {
		currentDnaChain = chain;
	}

	Vector<int> getPowerSwitchRules() {
		return powerSwitchRules;
	}

	void setPowerSwitchRules(Vector<int> rules) {
		powerSwitchRules = rules;
	}

	Vector<bool> getPowerSwitchStates() {
		return powerSwitchStates;
	}

	void setPowerSwitchStates(Vector<bool> states) {
		powerSwitchStates = states;
	}

	bool getPowerPosition(int indx) {
		return powerSwitchStates.get(indx);
	}

private:

	int writeObjectMembers(ObjectOutputStream* stream);
	bool readObjectMember(ObjectInputStream* stream, const String& name);
};




#endif /* DESTRUCTIBLEBUILDINGDATACOMPONENT_H_ */
