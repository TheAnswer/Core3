/*
Copyright (C) 2007 <SWGEmu>
 
This File is part of Core3.
 
This program is free software; you can redistribute 
it and/or modify it under the terms of the GNU Lesser 
General Public License as published by the Free Software
Foundation; either version 2 of the License, 
or (at your option) any later version.
 
This program is distributed in the hope that it will be useful, 
but WITHOUT ANY WARRANTY; without even the implied warranty of 
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
See the GNU Lesser General Public License for
more details.
 
You should have received a copy of the GNU Lesser General 
Public License along with this program; if not, write to
the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 
Linking Engine3 statically or dynamically with other modules 
is making a combined work based on Engine3. 
Thus, the terms and conditions of the GNU Lesser General Public License 
cover the whole combination.
 
In addition, as a special exception, the copyright holders of Engine3 
give you permission to combine Engine3 program with free software 
programs or libraries that are released under the GNU LGPL and with 
code included in the standard release of Core3 under the GNU LGPL 
license (or modified versions of such code, with unchanged license). 
You may copy and distribute such a system following the terms of the 
GNU LGPL for Engine3 and the licenses of the other code concerned, 
provided that you include the source code of that other code when 
and as the GNU LGPL requires distribution of source code.
 
Note that people who make modified versions of Engine3 are not obligated 
to grant this special exception for their modified versions; 
it is their choice whether to do so. The GNU Lesser General Public License 
gives permission to release a modified version without this exception; 
this exception also makes it possible to release a modified version 
which carries forward this exception.
*/

#ifndef SHIPOBJECTMESSAGE4_H_
#define SHIPOBJECTMESSAGE4_H_

#include "../../packets/BaseLineMessage.h"
#include "server/zone/objects/ship/ShipObject.h"


class ShipObjectMessage4 : public BaseLineMessage {
public:
	ShipObjectMessage4(ShipObject* ship)
			: BaseLineMessage(ship->getObjectID(), 0x53484950, 4, 0x06) {

		insertFloat(0.80000001f); //mass
		insertFloat(512.f); //mass max

		insertFloat(100.f); // capacitor ?
		insertFloat(0); //float

		insertInt(0); //const Archive::AutoDeltaPackedMap<int,float,Archive::DefaultObjectType>::`vftable'
		insertInt(0);

		setSize();
	}
		
};

#endif /*SHIPOBJECTMESSAGE4_H_*/
