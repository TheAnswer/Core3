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

#ifndef OPTIONBITMASK_H_
#define OPTIONBITMASK_H_

class OptionBitmask {
public:
	enum {
		NONE       = 0x00000000,
		ACTIVATED  = 0x00000001,
		VENDOR     = 0x00000002, //Vendor flag in creatures
		INSURED    = 0x00000004,
		CONVERSE   = 0x00000008, //Converse flag in creatures
		UNKNOWN3   = 0x00000010,
		YELLOW     = 0x00000020, //Yellow items in tangibles
		UNKNOWN5   = 0x00000040,
		UNKNOWN6   = 0x00000080,
		HIDEHAM    = 0x00000100, //Shows no information about condition or HAM
		UNKNOWN8   = 0x00000200,
		UNKNOWN9   = 0x00000400,
		QUESTGIVER = 0x00000800, //Info Icon 1 in creatures
		VEHICLE    = 0x00001000, //Vehicles will only appear with this bit enabled
		HASSERIAL  = 0x00002000, // Has a serial number
		UNKNOWN13  = 0x00004000,
		QUESTDONE  = 0x00008000, //Info Icon 2 in creatures
		UNKNOWN14  = 0x00010000,
		UNKNOWN15  = 0x00020000,
		UNKNOWN16  = 0x00040000,
		UNKNOWN17  = 0x00080000,
		UNKNOWN18  = 0x00100000,
		UNKNOWN19  = 0x00200000,
		UNKNOWN20  = 0x00400000,
		UNKNOWN21  = 0x00800000,
		UNKNOWN22  = 0x01000000,
		UNKNOWN23  = 0x02000000,
		UNKNOWN24  = 0x04000000,
		UNKNOWN25  = 0x08000000,
		UNKNOWN26  = 0x10000000,
		UNKNOWN27  = 0x20000000,
		UNKNOWN28  = 0x40000000,
		UNKNOWN29  = 0x80000000
	};
};

#endif /*OptionBitmask_H_*/
