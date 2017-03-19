/*
 * CityRemoveMilitiaSessionImplementation.cpp
 *
 *  Created on: Feb 13, 2012
 *      Author: xyborn
 */

#include "server/zone/objects/player/sessions/CityRemoveMilitiaSession.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/objects/region/CityRegion.h"
#include "server/zone/objects/player/sui/SuiWindowType.h"
#include "server/zone/objects/player/sui/messagebox/SuiMessageBox.h"
#include "server/zone/objects/player/sui/callbacks/CityRemoveMilitiaMemberSuiCallback.h"

int CityRemoveMilitiaSessionImplementation::initializeSession() {
	ManagedReference<CreatureObject*> creature = creatureObject.get();
	ManagedReference<CityRegion*> city = cityRegion.get();

	if (creature == NULL || city == NULL)
		return cancelSession();

	PlayerObject* ghost = creature->getPlayerObject();

	if (ghost == NULL)
		return cancelSession();

	if (!city->isMayor(creature->getObjectID()) && !ghost->isAdmin())
		return cancelSession();

	ManagedReference<SceneObject*> militiaMember = creature->getZoneServer()->getObject(militiaID);

	if (militiaMember == NULL)
		return cancelSession();

	StringBuffer text;
	text << "@city/city:remove_militia_prefix " << militiaMember->getDisplayedName() << " @city/city:remove_militia_suffix";

	ManagedReference<SuiMessageBox*> sui = new SuiMessageBox(creature, SuiWindowType::CITY_REMOVE_MILITIA);
	sui->setPromptTitle("@city/city:remove_militia_confirm"); //Confirm Removal
	sui->setPromptText(text.toString());
	sui->setCallback(new CityRemoveMilitiaMemberSuiCallback(creature->getZoneServer(), city));
	sui->setUsingObject(terminalObject.get());
	sui->setForceCloseDistance(16.f);

	ghost->addSuiBox(sui);
	creature->sendMessage(sui->generateMessage());

	return 1;
}
