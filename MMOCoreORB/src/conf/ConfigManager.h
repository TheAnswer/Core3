/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef CONFIGMANAGER_H_
#define CONFIGMANAGER_H_

// #define DEBUG_CONFIGMANAGER

#include "engine/engine.h"

namespace conf {
	using namespace sys::thread;

	class ConfigDataItem {
		bool asBool;
		String asString;
		lua_Number asNumber;
		Vector <ConfigDataItem *>* asVector;
		int usageCounter = 0;

	public:
		ConfigDataItem(lua_Number value);
		ConfigDataItem(int value);
		ConfigDataItem(bool value);
		ConfigDataItem(float value);
		ConfigDataItem(const String& value);
		ConfigDataItem(Vector <ConfigDataItem *>* value);

		inline bool getBool() {
			usageCounter++;
			return asBool;
		}

		inline float getFloat() {
			usageCounter++;
			return (float)asNumber;
		}

		inline int getInt() {
			usageCounter++;
			return (int)asNumber;
		}

		inline const String& getString() {
			usageCounter++;
			return asString;
		}

		inline Vector<String>& getStringVector() {
			Vector<String>* elements = new Vector<String>();

			if (asVector == nullptr)
				return *elements;

			for (int i = 0;i < asVector->size(); i++) {
				ConfigDataItem *curItem = asVector->get(i);

				if (curItem == nullptr)
					continue;

				elements->add(curItem->getString());
			}

			return *elements;
		}

		inline String toString() {
			usageCounter++;

			if (asVector == nullptr)
				return String(asString);

			Vector<String> elements = getStringVector();

			StringBuffer buf;

            buf << asString << " = {";

			for (int i = 0; i < elements.size(); ++i) {
				buf << (i == 0 ? " " : ", ")
					<< elements.get(i);
			}

			buf << " }";

			return buf.toString();
		}

		inline int getUsageCounter() {
			return usageCounter;
		}

		inline int resetUsageCounter() {
			int prevCount = usageCounter;
			usageCounter = 0;
			return prevCount;
		}

#ifdef DEBUG_CONFIGMANAGER
	private:
		String debugTag;

	public:
		~ConfigDataItem();
		inline void setDebugTag(const String& tag) {
			debugTag = String(tag);
		}
#endif // DEBUG_CONFIGMANAGER
	};

	class ConfigManager : public Singleton<ConfigManager>, public Lua {
		Timer configStartTime;
		VectorMap<String, ConfigDataItem *> configData;

		// Cached values
		bool cache_PvpMode;
		bool cache_ProgressMonitors;
		bool cache_UnloadContainers;

	public:
		ConfigManager();

		~ConfigManager() {
		}

		bool loadConfigData();
		bool parseConfigData(const String& prefix, bool isGlobal = false, int maxDepth = 5);
		void cacheHotItems();
		void dumpConfig(bool includeSecure = false);
		bool testConfig(ConfigManager* configManager);

		uint64 getConfigDataAgeMs() {
			return configStartTime.elapsedMs();
		}

		// General config functions
		ConfigDataItem* findItem(const String& name);
		int getInt(const String& name, int defaultValue);
		bool getBool(const String& name, bool defaultValue);
		float getFloat(const String& name, float defaultValue);
		const String& getString(const String& name, const String& defaultValue);
		Vector<String>& getStringVector(const String& name);
		SortedVector<String>& getSortedStringVector(const String& name);

		bool updateItem(const String& name, ConfigDataItem* newItem);
		bool setNumber(const String& name, lua_Number newValue);
		bool setInt(const String& name, int newValue);
		bool setBool(const String& name, bool newValue);
		bool setFloat(const String& name, float newValue);
		bool setString(const String& name, const String& newValue);
		bool setStringFromFile(const String& name, const String& fileName);

		// Legacy getters
		inline bool getMakeLogin() {
			return getBool("Core3.MakeLogin", true);
		}

		inline bool getMakeZone() {
			return getBool("Core3.MakeZone", true);;
		}

		inline bool getMakePing() {
			return getBool("Core3.MakePing", true);
		}

		inline bool getMakeStatus() {
			return getBool("Core3.MakeStatus", true);
		}

		inline bool getMakeWeb() {
			return getBool("Core3.MakeWeb", true);
		}

		inline bool getDumpObjFiles() {
			return getBool("Core3.DumpObjFiles", true);
		}

		inline bool shouldUnloadContainers() {
			// Use cached value as this is called often
			return cache_UnloadContainers;
		}

		inline bool shouldUseMetrics() {
			return getBool("Core3.UseMetrics", false);
		}

		inline bool getPvpMode() {
			// Use cached value as this is a hot item called in:
			//   CreatureObjectImplementation::isAttackableBy
			//   CreatureObjectImplementation::isAggressiveTo
			return cache_PvpMode;
		}

		inline bool setPvpMode(bool val) {
			if (!setBool("Core3.PvpMode", val))
				return false;

			// Updated cached value
			cache_PvpMode = getBool("Core3.PvpMode", val);

			return true;
		}

		inline const String& getORBNamingDirectoryAddress() {
			return getString("Core3.ORB", "");
		}

		inline uint16 getORBNamingDirectoryPort() {
			return getInt("Core3.ORBPort", 44419);
		}

		inline const String& getDBHost() {
			return getString("Core3.DBHost", "127.0.0.1");
		}

		inline bool isProgressMonitorActivated() {
			// Use cached value as this a hot item called in lots of loops
			return cache_ProgressMonitors;
		}

		inline const uint16& getDBPort() {
			const uint16& ret { (uint16)getInt("Core3.DBPort", 3306) };
			return ret;
		}

		inline const String& getDBName() {
			return getString("Core3.DBName", "swgemu");
		}

		inline const String& getDBUser() {
			return getString("Core3.DBUser", "root");
		}

		inline const String& getDBPass() {
			return getString("Core3.DBPass", "Gemeni1");
		}

		inline const String& getDBSecret() {
			return getString("Core3.DBSecret", "swgemusecret");
		}

		inline const String& getMantisHost() {
			return getString("Core3.MantisHost", "127.0.0.1");
		}

		inline const uint16& getMantisPort() {
			const uint16& ret { (uint16)getInt("Core3.MantisPort", 3306) };
			return ret;
		}

		inline const Vector<String>& getTreFiles() {
			return getStringVector("Core3.TreFiles");
		}

		inline const String& getMantisName() {
			return getString("Core3.MantisName", "swgemu");
		}

		inline const String& getMantisUser() {
			return getString("Core3.MantisUser", "root");
		}

		inline const String& getMantisPass() {
			return getString("Core3.MantisPass", "Gemeni1");
		}

		inline const String& getMantisPrefix() {
			return getString("Core3.MantisPrfx", "");
		}

		inline const String& getMessageOfTheDay() {
			return getString("Core3.MOTD", "Welcome to SWGEmu!");
		}

		inline const String& getRevision() {
			return getString("Core3.Revision", "");
		}

		inline const String& getMetricsHost() {
			return getString("Core3.MetricsHost", "127.0.0.1");
		}

		inline const String& getMetricsPrefix() {
			return getString("Core3.MetricsPrefix", "");
		}

		inline int getMetricsPort() {
			return getInt("Core3.MetricsPort", 8125);
		}

		inline const String& getTrePath() {
			return getString("Core3.TrePath", "tre");
		}

		inline uint16 getLoginPort() {
			return getInt("Core3.LoginPort", 44453);
		}

		inline uint16 getStatusPort() {
			return getInt("Core3.StatusPort", 44455);
		}

		inline uint16 getPingPort() {
			return getInt("Core3.PingPort", 44462);
		}

		inline const String& getWebPorts() {
			return getString("Core3.WebPorts", "44460");
		}

		inline const String& getWebAccessLog() {
			return getString("Core3.WebAccessLog", "log/web_access.log");
		}

		inline const String& getWebErrorLog() {
			return getString("Core3.WebErrorLog", "log/web_error.log");
		}

		inline int getWebSessionTimeout() {
			return getInt("Core3.WebAccessLog", 600);
		}

		inline const String& getLoginRequiredVersion() {
			return getString("Core3.LoginRequiredVersion", "20050408-18:00");
		}

		inline int getLoginProcessingThreads() {
			return getInt("Core3.LoginProcessingThreads", 1);
		}

		inline int getLoginAllowedConnections() {
			return getInt("Core3.LoginAllowedConnections", 30);
		}

		inline int getStatusAllowedConnections() {
			return getInt("Core3.StatusAllowedConnections", 100);
		}

		inline int getPingAllowedConnections() {
			return getInt("Core3.PingAllowedConnections", 3000);
		}

		inline int getStatusInterval() {
			return getInt("Core3.StatusInterval", 60);
		}

		inline int getAutoReg() {
			return getBool("Core3.AutoReg", true);
		}

		inline int getZoneProcessingThreads() {
			return getInt("Core3.ZoneProcessingThreads", 10);
		}

		inline int getZoneAllowedConnections() {
			return getInt("Core3.ZoneAllowedConnections", 300);
		}

		inline int getZoneGalaxyID() {
			return getInt("Core3.ZoneGalaxyID", 2);
		}

		inline int getZoneServerPort() {
			return getInt("Core3.ZoneServerPort", 0);
		}

		const SortedVector<String>& getEnabledZones() {
			return getSortedStringVector("Core3.ZonesEnabled");
		}

		inline int getPurgeDeletedCharacters() {
			return getInt("Core3.PurgeDeletedCharacters", 10); // In minutes
		}

		inline int getMaxNavMeshJobs() {
			return getInt("Core3.MaxNavMeshJobs", 6);
		}

		inline int getMaxAuctionSearchJobs() {
			return getInt("Core3.MaxAuctionSearchJobs", 1);
		}

		inline const String& getLogFile() {
			return getString("Core3.LogFile", "log/core3.log");
		}

		inline int getLogFileLevel() {
			return getInt("Core3.LogFileLevel", Logger::INFO);
		}

		inline void setProgressMonitors(bool val) {
			setBool("Core3.ProgressMonitors", val);

			// Updated cached value
			cache_ProgressMonitors = getBool("Core3.ProgressMonitors", val);
		}

		inline const String& getTermsOfService() {
			return getString("Core3.TermsOfService", "");
		}

		inline int getTermsOfServiceVersion() {
			return getInt("Core3.TermsOfServiceVersion", 0);
		}

		inline bool getJsonLogOutput() {
			return getBool("Core3.LogJSON", false);
		}

		inline bool getSyncLogOutput() {
			return getBool("Core3.LogSync", false);
		}

		inline bool getLuaLogJSON() {
			return getBool("Core3.LuaLogJSON", false);
		}

		inline bool getPathfinderLogJSON() {
			return getBool("Core3.PathfinderLogJSON", false);
		}

		inline int getCleanupMailCount() {
			return getInt("Core3.CleanupMailCount", 25000);
		}

		inline int getRESTPort() {
			return getInt("Core3.RESTServerPort", 0);
		}

		inline const String& getInactiveAccountTitle() {
			return getString("Core3.InactiveAccountTitle", "Account Disabled");
		}

		inline const String& getInactiveAccountText() {
			return getString("Core3.InactiveAccountText", "The server administrators have disabled your account.");
		}

		inline bool getCharacterBuilderEnabled() {
			return getBool("Core3.CharacterBuilderEnabled", false);
		}

		inline int getPlayerLogLevel() {
			return getInt("Core3.PlayerLogLevel", Logger::INFO);
		}

		inline int getMaxLogLines() {
			return getInt("Core3.MaxLogLines", 1000000);
		}
	};
}

using namespace conf;

#endif // #ifndef CONFIGMANAGER_H_
