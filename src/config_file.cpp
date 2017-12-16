#include "config_file.h"
#include <stdlib.h>
#include "platform.h"

ConfigFile *ConfigFile::instance = NULL;

ConfigFile::ConfigFile() {	
	char filePath[256];
	getGameDirPath(filePath, "pacman.conf");
	configfile = new ifstream();
	configfile->open(filePath);
	if (configfile->is_open())
		cout << "Config File geöffnet" << endl;
}

ConfigFile::~ConfigFile() {
	if (configfile->is_open()){
		cout << "Config File schliessen" << endl;
		configfile->close();
	}
}

ConfigFile* ConfigFile::getInstance() {
	if(instance == NULL)
		instance = new ConfigFile();
	return instance;
}

void ConfigFile::cleanUpInstance() {
	if(instance != NULL)
		delete instance;
}

char* ConfigFile::readConfig() {
	string line;
	if(configfile->is_open()){
		cout << "test" << endl;
		while(getline(*configfile,line))
			cout << line << endl;
	}

}
