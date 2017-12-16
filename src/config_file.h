#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class ConfigFile {
	public:
		static ConfigFile* getInstance();
		static void cleanUpInstance();
		char* readConfig();
	private:
		ConfigFile();
		~ConfigFile();
		static ConfigFile* instance;
		ifstream *configfile;
};
