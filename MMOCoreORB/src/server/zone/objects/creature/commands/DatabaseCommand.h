/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef DATABASECOMMAND_H_
#define DATABASECOMMAND_H_

#include "engine/engine.h"

#include "QueueCommand.h"

class DatabaseCommand : public QueueCommand {
public:

	DatabaseCommand(const String& name, ZoneProcessServer* server)
		: QueueCommand(name, server) {

	}

	int doQueueCommand(CreatureObject* creature, const uint64& target, const UnicodeString& arguments) const {
		if (!checkStateMask(creature))
			return INVALIDSTATE;

		if (!checkInvalidLocomotions(creature))
			return INVALIDLOCOMOTION;

		UnicodeTokenizer tokenizer(arguments);
		tokenizer.setDelimeter(" ");

		String arg0, arg1;
		uint64 objectID;

		if (!tokenizer.hasMoreTokens())
			return INVALIDPARAMETERS;

		tokenizer.getStringToken(arg0);

		try {
			if (!tokenizer.hasMoreTokens())
				return INVALIDPARAMETERS;

			objectID = tokenizer.getLongToken();

		} catch (const Exception& err) {
			creature->sendSystemMessage("Error parsing objectID: " +  err.getMessage());
			return INVALIDPARAMETERS;
		}

		String strResource;

		if (!(arg0 == "cityregions" || arg0 == "factionstructures" || arg0 == "playerstructures" || arg0 == "sceneobjects" || arg0 == "clientobjects" || arg0 == "resourcespawns" ||
				arg0 == "characters" || arg0 == "deleted_characters") ){
			creature->sendSystemMessage("Command format is database  < playerstructures | cityregions  | sceneobjects | clientobjects >  <objectid>");

			return INVALIDPARAMETERS;
		}

		try {
			if (arg0 == "characters" || arg0 == "deleted_characters" ) {
				doSQLQuery(creature, arg0, objectID);
			} else {
				doObjectDBQuery(creature, arg0, objectID);
			}
		} catch (const  Exception& err) {
			creature->sendSystemMessage("Error in database lookup: " + err.getMessage());
			error() << err.getMessage();
		}

		return SUCCESS;
	}


private:
	void doObjectDBQuery(CreatureObject* creature, String db, uint64 objectID) const {
		StringBuffer msg;

		debug() << "doing object query for " << db << " with object " << objectID;

		try {
			ObjectDatabaseManager* dManager = ObjectDatabaseManager::instance();
			uint16 id = ObjectDatabaseManager::instance()->getDatabaseID(db);

			if (id == 0) {
				creature->sendSystemMessage("invalid db");
				return;
			}

			ObjectDatabase* thisDatabase = cast<ObjectDatabase*>(ObjectDatabaseManager::instance()->getDatabase(id));

			if(thisDatabase == nullptr || !thisDatabase->isObjectDatabase()) {
				creature->sendSystemMessage("Error retrieving " + db + " database.");
				return;
			}

			ObjectInputStream objectData(2000);

			if (!(thisDatabase->getData(objectID,&objectData))) {
				uint32 serverObjectCRC;
				String className;

				if (Serializable::getVariable<String>(STRING_HASHCODE("_className"), &className, &objectData)) {
					msg << endl << "OID: " + String::valueOf(objectID) << endl;
					msg << "Database: " << db << endl;
					msg << "ClassName: " << className << endl;

					creature->sendSystemMessage(msg.toString());
				} else {
					msg << "ERROR desrializing from db" << endl;
				}

			} else {
				creature->sendSystemMessage("Object " + String::valueOf(objectID) + " was not found in " + db + " database.");
			}
		} catch (const DatabaseException& err) {
			msg << endl << err.getMessage();
		} catch (const Exception& err){
			msg << endl << err.getMessage();
		}

		creature->sendSystemMessage(msg.toString());
	}

	void doSQLQuery(CreatureObject* creature, String db, uint64 objectID) const {
		StringBuffer selectStatement;
		StringBuffer msg;

		try {
			selectStatement << "SELECT * FROM " << db << " WHERE character_oid = " << objectID;
			UniqueReference<ResultSet*> queryResults(ServerDatabase::instance()->executeQuery(selectStatement));

			if (queryResults == nullptr || queryResults.get()->getRowsAffected() == 0){
				msg << endl << "No results for " << selectStatement.toString();
			} else if ( queryResults->getRowsAffected() > 1) {
				msg << endl << "Duplicate character id.";
			} else {
				while (queryResults->next()) {
					msg << endl << "Found in the database";

					uint64 newOID = queryResults->getUnsignedLong(0);

					msg << "oid " << String::valueOf(newOID) << endl;
					msg << "account id: " << String::valueOf(queryResults->getUnsignedInt(1)) << endl;
					msg << "galaxy id " << String::valueOf(queryResults->getUnsignedInt(2)) << endl;

					if (db == "characters")
						msg << "Name: " << queryResults->getString(3) << " " << queryResults->getString(4) << endl;

					if (db == "deleted_characters")
						msg << "db_deleted: " <<  String::valueOf(queryResults->getInt(9)) << endl;
				}
			}
		} catch (const DatabaseException& err) {
			msg << endl << err.getMessage();
		} catch (const Exception& err) {
			msg << endl << err.getMessage();
		}

		creature->sendSystemMessage(msg.toString());
	}
};

#endif //DATABASECOMMAND_H_
