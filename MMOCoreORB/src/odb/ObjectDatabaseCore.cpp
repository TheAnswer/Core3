/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#include "server/zone/managers/object/ObjectManager.h"

#include "ObjectDatabaseCore.h"
#include <fstream>
#include <thread>

#define BACK_ITER_T

AtomicInteger ObjectDatabaseCore::dispatchedTasks;
ParsedObjectsHashTable ObjectDatabaseCore::parsedObjects;
AtomicInteger ObjectDatabaseCore::pushedObjects;
AtomicInteger ObjectDatabaseCore::backPushedObjects;

ObjectDatabaseCore::ObjectDatabaseCore(Vector<String> arguments, const char* engine) : Core("log/odb3.log", engine, LogLevel::LOG),
	Logger("ObjectDatabaseCore"), arguments(std::move(arguments)) {

	setLogLevel(LogLevel::LOG);
}

void ObjectDatabaseCore::initialize() {
	info("starting up ObjectDatabase..");

	Core::initializeProperties("ODB3");

	Core::MANAGED_REFERENCE_LOAD = false;

	objectManager = new ObjectManager(false); //initialize databases but not the templates

	//DistributedObjectBroker* orb = DistributedObjectBroker::initialize("", 44230);
	//orb->setCustomObjectManager(new ObjectManager(false));
}

bool ObjectDatabaseCore::getJSONString(uint64 oid, ObjectDatabase* database, std::ostream& returnData) {
	static bool reportError = Core::getIntProperty("ODB3.reportParsingErrors", 1);

	try {
		ObjectInputStream objectData(1024);

		if (database->getData(oid, &objectData)) {
			return false;
		}

		ObjectDatabaseCore::dispatchedTasks.increment();

		String className;

		if (!Serializable::getVariable<String>(STRING_HASHCODE("_className"), &className, &objectData)) {
			return false;
		}

		UniqueReference<DistributedObjectPOD*> pod(Core::getObjectBroker()->createObjectPOD(className));

		if (pod == nullptr) {
			if (reportError)
				Logger::console.error("could not create pod object for class name " + className);

			return false;
		}

		try {
			pod->readObject(&objectData);

			nlohmann::json j;
			pod->writeJSON(j);

			nlohmann::json thisObject;
			thisObject[String::valueOf(oid).toCharArray()] = j;

			returnData << thisObject.dump();
		} catch (const Exception& e) {
			if (reportError)
				Logger::console.error("parsing data for object 0x:" + String::hexvalueOf(oid) + " " + e.getMessage());

			return false;
		} catch (const std::exception& e) {
			if (reportError)
				Logger::console.error("parsing data for object 0x:" + String::hexvalueOf(oid) + " " + e.what());

			return false;
		} catch (...) {
			if (reportError)
				Logger::console.error("parsing data for object 0x:" + String::hexvalueOf(oid));


			return false;
		}

		return true;
	} catch (...) {
		if (reportError)
			Logger::console.error("parsing data for object 0x:" + String::hexvalueOf(oid));
	}

	return false;
}

void dispatchWriterTask(std::stringstream* data, const String& fileName, int writerThread) {
	 Core::getTaskManager()->executeTask([data, fileName]() {
		UniqueReference<std::stringstream*> guard(data);
		std::ofstream jsonFile(fileName.toCharArray(), std::fstream::out | std::fstream::app);

		jsonFile << data->str();

		jsonFile.close();
	 }, "WriteJSONTask", ("Writer" + String::valueOf(writerThread)).toCharArray());
}

void ObjectDatabaseCore::dispatchTask(const Vector<uint64>& currentObjects, ObjectDatabase* database, const String& fileName, int maxWriterThreads, int dispatcher) {
	static AtomicInteger taskCount;
	int writerThread = taskCount.increment() % maxWriterThreads;

	Core::getTaskManager()->executeTask([fileName, dispatcher, currentObjects, database, writerThread]() {
			const String file = fileName + String::valueOf(writerThread);
			std::stringstream* buffer(new std::stringstream());
			int count = 0;

			for (const auto& oid : currentObjects) {
				if (dispatcher == 2) {
					backPushedObjects.decrement();
				} else {
					pushedObjects.decrement();
				}

				try {
					if (getJSONString(oid, database, *buffer)) {
						*buffer << "\n";

						++count;
					}
				} catch (...) {
				}
			}

			if (count) {
				dispatchWriterTask(buffer, file, writerThread);
			} else {
				delete buffer;
			}
	}, "DumpJSONTask", "ODBReaderThreads");
}

void ObjectDatabaseCore::startBackIteratorTask(ObjectDatabase* database, const String& fileName, int writerThreads) {
#ifdef BACK_ITER_T
	berkley::CursorConfig config;
	config.setReadUncommitted(true);

	auto taskManager = Core::getTaskManager();

	//back iterator
	taskManager->executeTask([database, writerThreads, fileName, config]() mutable {
		ObjectDatabaseIterator iterator(database, config);
		uint64 oid = 0;

		Vector<uint64> currentObjects;

		constexpr static const int objectsPerTask = 15;

		if (!iterator.getLastKey(oid)) {
			return; //no last key
		}

		ObjectInputStream data;

		if (!iterator.getSearchKey(oid, &data)) {
			Logger::console.error("false search key");

			return;
		}

		if (parsedObjects.put(oid, 2)) {
			return;
		}

		currentObjects.add(oid);

		while (iterator.getPrevKey(oid)) {
			if (parsedObjects.put(oid, 2)) {
				break;
			}

			currentObjects.add(oid);

			if (currentObjects.size() >= objectsPerTask) {
				backPushedObjects.add(currentObjects.size());

				dispatchTask(currentObjects, database, fileName, writerThreads, 2);

				currentObjects.removeRange(0, currentObjects.size());
			}
		}

		if (currentObjects.size()) {
			backPushedObjects.add(currentObjects.size());

			dispatchTask(currentObjects, database, fileName, writerThreads, 2);
		}
	}, "BackIteratorTask", "BackIteratorThread");
#endif
}

void ObjectDatabaseCore::dumpDatabaseToJSON(const String& databaseName) {
	auto databaseManager  = ObjectDatabaseManager::instance();
	auto database = databaseManager->loadObjectDatabase(databaseName, false);

	if (!database) {
		error("invalid database " + databaseName);

		showHelp();

		return;
	}

	int readerThreads = getIntArgument(2, 4);
	int writerThreads = readerThreads / 2 + 1;

	auto taskManager = Core::getTaskManager();

	taskManager->initializeCustomQueue("ODBReaderThreads", readerThreads);
	taskManager->initializeCustomQueue("BackIteratorThread", 1);

	for (int i = 0; i < writerThreads; ++i) {
		Core::getTaskManager()->initializeCustomQueue("Writer" + String::valueOf(i), 1);
	}

	berkley::CursorConfig config;
	config.setReadUncommitted(true);

	const String fileName = getArgument(3, database->getDatabaseFileName() + ".json");
	startBackIteratorTask(database, fileName, writerThreads);

	//forward iterator
	ObjectDatabaseIterator iterator(database, config);
	uint64 oid = 0;

	Vector<uint64> currentObjects;
	constexpr static const int objectsPerTask = 15;

	while (iterator.getNextKey(oid)) {
		if (this->parsedObjects.put(oid, 1)) {
			break;
		}

		currentObjects.add(oid);

		if (currentObjects.size() >= objectsPerTask) {
			pushedObjects.add(currentObjects.size());

			dispatchTask(currentObjects,  database, fileName, writerThreads, 1);

			currentObjects.removeRange(0, currentObjects.size());
		}
	}

	if (currentObjects.size()) {
		pushedObjects.add(currentObjects.size());

		dispatchTask(currentObjects, database, fileName, writerThreads, 1);
	}

	info("finished main iterator " + String::valueOf(parsedObjects.size()), true);
}

ObjectDatabase* ObjectDatabaseCore::getDatabase(uint64_t objectID) {
	auto databaseManager = ObjectDatabaseManager::instance();
	uint16 tableID = (uint16)(objectID >> 48);

	LocalDatabase* db = databaseManager->getDatabase(tableID);

	if (db == NULL || !db->isObjectDatabase())
		return NULL;

	ObjectDatabase* database = static_cast<ObjectDatabase*>( db);

	return database;
}

void ObjectDatabaseCore::dumpObjectToJSON(uint64_t objectID) {
	ObjectDatabase* database = getDatabase(objectID);

	if (!database) {
		error("cant map object id to database");

		showHelp();

		return;
	}

	std::stringstream json;
	if (getJSONString(objectID, database, json)) {
		std::cout << json.str() << std::endl;
	}
}

void ObjectDatabaseCore::showHelp() {
	info("Arguments: \n"
		"\todb3 dumpdb <database> <threads> \n"
		"\todb3 dumpobj <objectid>\n"
		, true);
}

void ObjectDatabaseCore::run() {
	info("initialized");

	const String& operation = getArgument(0, "none");

	if (operation == "dumpdb") {
		dumpDatabaseToJSON(getArgument(1));
	} else if (operation == "dumpobj") {
		dumpObjectToJSON(getLongArgument(1));
	} else {
		showHelp();
	}
}

int main(int argc, char* argv[]) {
	try {
		Vector<String> arguments;
		for (int i = 1; i < argc; ++i) {
			arguments.add(argv[i]);
		}

		ObjectDatabaseCore core(std::move(arguments), "odb3");

		core.start();
	} catch (Exception& e) {
		System::out << e.getMessage() << "\n";
		e.printStackTrace();
	}

	return 0;
}
