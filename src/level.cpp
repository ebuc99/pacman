#include "level.h"
#include <iostream>

Level *Level::instance = NULL;

Level *Level::getInstance() {
	if(!instance)
		instance = new Level();
	return instance;
}

void Level::cleanUpInstance() {
	if(instance) {
		delete instance;
		instance = NULL;
	}
}

Level::Level() {
}

Level::~Level() {
}
