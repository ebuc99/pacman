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

Level::Level():
	level(1)
{
}

Level::~Level() {
}

int Level::getLevelNumber() {
	return level;
}

void Level::setLevel(int const level) {
	this->level = level;
}

void Level::nextLevel() {
	++level;
}

int Level::getPillLimit() {
	switch(level) {
		case 1:
			return -1;
		case 2:
		case 3:
			return 5;
		case 4:
		case 5:
			return 10;
		default:
			return 15;
	}
}
