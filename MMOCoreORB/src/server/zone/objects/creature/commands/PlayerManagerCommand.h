/*
 * PlayerManagerCommand.h
 *
 *  Created on: 2/28/2012
 *      Author: Kyle
 */

#ifndef PLAYERMANAGERCOMMAND_H_
#define PLAYERMANAGERCOMMAND_H_

#include "engine/engine.h"
#include "server/zone/managers/player/PlayerManager.h"
#include "templates/appearance/MeshData.h"
#include "pathfinding/RecastNavMeshBuilder.h"

class PlayerManagerCommand {
public:
	static int executeCommand(CreatureObject* creature, uint64 target, const UnicodeString& arguments) {
		if (!creature->isPlayerCreature())
			return 1;

		ManagedReference<CreatureObject*> player = cast<CreatureObject*>(creature);
		ManagedReference<PlayerObject*> ghost = player->getPlayerObject();

		if (ghost == NULL || ghost->getAdminLevel() < 15) {
			return 1;
		}

		ManagedReference<PlayerManager*> playerManager = player->getZoneServer()->getPlayerManager();

		if (playerManager == NULL) {
			creature->sendSystemMessage("playerManager not found");
			return 0;
		}

		//Parse the weather command.
		StringTokenizer tokenizer(arguments.toString());
		if (!tokenizer.hasMoreTokens()) {
			sendSyntax(player);
			return 1;
		}

		String command;
		tokenizer.getStringToken(command);
		command = command.toLowerCase();
		
		if (command == "dumpmesh") {
			if (!tokenizer.hasMoreTokens()) {
				sendSyntax(player);
				return 1;
			}
			
			float range = tokenizer.getFloatToken();
			SortedVector<ManagedReference<QuadTreeEntry*> > closeObjects;
			player->getZone()->getInRangeSolidObjects(player->getWorldPositionX(), player->getWorldPositionY(), range, &closeObjects, true);
			Matrix4 identity;
			
			Vector<Reference<MeshData*> > meshData;
			
			for (int i=0; i<closeObjects.size(); i++) {
				SceneObject *sceno = closeObjects.get(i).castTo<SceneObject*>();
				if(sceno)
					meshData.addAll(sceno->getTransformedMeshData(&identity));
			}
			
			// Get Terrain
			//info("Building region terrain", true);
			Vector3 position = player->getWorldPosition();
			position = Vector3(position.getX(), position.getZ(), position.getY());
			meshData.add(RecastNavMeshBuilder::getTerrainMesh(position, range*2, player->getZone()->getPlanetManager()->getTerrainManager()));
			
			// Flatten Transforms
			Reference<MeshData*> flattened = RecastNavMeshBuilder::flattenMeshData(meshData);
			meshData.removeAll();
			
			// Dump OBJ (optional - but very useful for debugging)
			Vector<Reference<MeshData*> > test;
			test.add(flattened);
			RecastNavMeshBuilder::dumpOBJ("test.obj", test);
			
			
			return 0;
		}

		if (command == "listjedi") {
			player->sendSystemMessage("Please wait. This may take a while.");

			EXECUTE_TASK_2(playerManager, player, {
				playerManager_p->sendAdminJediList(player_p);
			});

			return 0;

		} else if (command == "listfrs") {
			player->sendSystemMessage("Please wait. This may take a while.");

			EXECUTE_TASK_2(playerManager, player, {
					playerManager_p->sendAdminFRSList(player_p);
			});

			return 0;

		} else if (command == "listadmins") {
			player->sendSystemMessage("Please wait. This may take a while.");

			EXECUTE_TASK_2(playerManager, player, {
					playerManager_p->sendAdminList(player_p);
			});

			return 0;

		} else if (command == "setxpmodifier") {
			if (!tokenizer.hasMoreTokens()) {
				sendSyntax(player);
				return 1;
			}

			float multiplier = tokenizer.getFloatToken();

			playerManager->setExperienceMultiplier(multiplier);

			StringBuffer message;
			message << "Experience now increased by " << multiplier << "x";

			player->sendSystemMessage(message.toString());

		} else {
			sendSyntax(player);
			return 1;
		}

		return 0;

	}

	static void sendSyntax(CreatureObject* player) {
		if (player != NULL) {
			player->sendSystemMessage("Syntax: /server playermanager [setxpmodifier] [value]");
			player->sendSystemMessage("Syntax: /server playermanager [listjedi]");
			player->sendSystemMessage("Syntax: /server playermanager [list_frsjedi]");
			player->sendSystemMessage("Syntax: /server playermanager [listadmins]");
		}
	}
};

#endif /* PLAYERMANAGERCOMMAND_H_ */
