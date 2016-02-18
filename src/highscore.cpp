#include "highscore.h"

HighscoreEntry::HighscoreEntry(char *playerName, int score, int level) {
	this->playerName = new std::string(playerName);
	this->score      = score;
	this->level      = level;
}

HighscoreEntry::~HighscoreEntry() {
	if (playerName) {
		delete playerName;
		playerName = NULL;
	}
}


HighscoreList *HighscoreList::instance = NULL;

HighscoreList *HighscoreList::getInstance() {
	if (instance == NULL)
		instance = new HighscoreList(Constants::HIGHSCORE_LIST_MAX_SIZE);
	return instance;
}

void HighscoreList::cleanUpInstance() {
	if (instance) {
		delete instance;
		instance = NULL;
	}
}

HighscoreList::HighscoreList(uint8_t maxSize) {
	this->maxSize = maxSize;
	entries = new std::vector<HighscoreEntry*>();
	idxLastInsertedEntry = -1;
}

HighscoreList::~HighscoreList() {
	if (entries) {
		for (std::vector<HighscoreEntry*>::iterator it = entries->begin(); it != entries->end(); ++it) {
			delete *it;
		}
		delete entries;
		entries = NULL;
	}
}

int HighscoreList::insertEntry(HighscoreEntry *entry) {
	int i = 0;
	for (std::vector<HighscoreEntry*>::iterator it = entries->begin(); it != entries->end(); ++it) {
		if (entry->getScore()>(*it)->getScore() || (entry->getScore()==(*it)->getScore() && entry->getLevel()>(*it)->getLevel())) {
			entries->insert(it, entry);
			if ((int) entries->size() > maxSize)
				entries->pop_back();
			idxLastInsertedEntry = i;
			return idxLastInsertedEntry;
		}
		++i;
	}
	if ((int) entries->size() < maxSize) {
		entries->push_back(entry);
		idxLastInsertedEntry = entries->size()-1;
	} else {
		idxLastInsertedEntry = -1;
	}
	return idxLastInsertedEntry;
}

void HighscoreList::print() {
	std::cout << "Highscore list:" << std::endl
	          << "Pos.| Name | Score | Level" << std::endl;
	int i = 0;
	for (std::vector<HighscoreEntry*>::iterator it = entries->begin(); it != entries->end(); ++it) {
		std::cout << (i+1) << " | " << (*it)->getPlayerName() << " | " << (*it)->getScore() << " | " << (*it)->getLevel();
		if (i == idxLastInsertedEntry)
			std::cout << " (new)";
		std::cout << std::endl;
		++i;
	}
}
