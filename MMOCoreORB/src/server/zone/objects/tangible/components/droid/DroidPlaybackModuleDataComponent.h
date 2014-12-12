/*
 * Copyright (C) 2014 <SWGEmu>
 * This File is part of Core3.
 * This program is free software; you can redistribute
 * it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software
 * Foundation; either version 2 of the License,
 * or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this program; if not, write to
 * the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 *
 * Linking Engine3 statically or dynamically with other modules
 * is making a combined work based on Engine3.
 * Thus, the terms and conditions of the GNU Lesser General Public License
 * cover the whole combination.
 *
 * In addition, as a special exception, the copyright holders of Engine3
 * give you permission to combine Engine3 program with free software
 * programs or libraries that are released under the GNU LGPL and with
 * code included in the standard release of Core3 under the GNU LGPL
 * license (or modified versions of such code, with unchanged license).
 * You may copy and distribute such a system following the terms of the
 * GNU LGPL for Engine3 and the licenses of the other code concerned,
 * provided that you include the source code of that other code when
 * and as the GNU LGPL requires distribution of source code.
 *
 * Note that people who make modified versions of Engine3 are not obligated
 * to grant this special exception for their modified versions;
 * it is their choice whether to do so. The GNU Lesser General Public License
 * gives permission to release a modified version without this exception;
 * this exception also makes it possible to release a modified version
 * which carries forward this exception.
 */

#ifndef DROIDPLAYBACKMODULEDATACOMPONENT_H_
#define DROIDPLAYBACKMODULEDATACOMPONENT_H_

#include "BaseDroidModuleComponent.h"
#include "engine/core/ManagedReference.h"
#include "system/util/Vector.h"
#include "system/util/VectorMap.h"
#include "server/zone/objects/tangible/components/droid/DroidPlaybackObserver.h"

namespace server {
namespace zone {
namespace objects {
namespace tangible {
namespace components {
namespace droid {


class DroidPlaybackModuleDataComponent : public BaseDroidModuleComponent {

protected:
	bool active;
	bool recording;
	int recordingTrack;
	String recordingSong;
	int recordingInstrument;
	int totalTracks;
	int selectedIndex;
	Vector<String> tracks; // Recorded Tracks
	Vector<int> instruments; // instrument for a given track
	ManagedReference<DroidPlaybackObserver*> observer;
	// states of recording
	static const uint8 STATE_WAITING_TO_RECORD = 1;
	static const uint8 STATE_RECORDING_TRACK = 2;
	Vector<uint64> listeners;

public:
	DroidPlaybackModuleDataComponent();
	~DroidPlaybackModuleDataComponent();
	String getModuleName();
	void initializeTransientMembers();
	void fillAttributeList(AttributeListMessage* msg, CreatureObject* droid);
	void fillObjectMenuResponse(SceneObject* droidObject, ObjectMenuResponse* menuResponse, CreatureObject* player);
	int handleObjectMenuSelect(CreatureObject* player, byte selectedID, PetControlDevice* controller);
	int getBatteryDrain();
	void deactivate();
	String toString();
	void onCall();
	void onStore();
	bool isActive();
	void addListener(uint64 id);
	virtual bool isStackable() { return true; }
	virtual void addToStack(BaseDroidModuleComponent* other);
	virtual void copy(BaseDroidModuleComponent* other);
	void deleteTrack(CreatureObject* player, int slotIndex);
	void setTrack( CreatureObject* player, String song, int instrument);
	bool toBinaryStream(ObjectOutputStream* stream);
	bool parseFromBinaryStream(ObjectInputStream* stream);
	bool isPlayingMusic();
	String getCurrentTrack();
	int getCurrentInstrument();
	void sessionTimeout(CreatureObject* player, int state);
	void songChanged(CreatureObject* player);
	void songStopped(CreatureObject* player);
	void startRecordingSession(CreatureObject* entertainer, int index);
	void playSong(CreatureObject* player, int index);
	bool trackEmpty(int index);
	void stopTimer();
	void doFlourish(int number);
private:
	void stopRecording(CreatureObject* player, bool success);
	int writeObjectMembers(ObjectOutputStream* stream);
	bool readObjectMember(ObjectInputStream* stream, const String& name);
};


} // droid
} // components
} // tangible
} // objects
} // zone
} // server
using namespace server::zone::objects::tangible::components::droid;
#endif /* DROIDPLAYBACKMODULEDATACOMPONENT_H_ */
