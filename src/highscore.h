#ifndef HIGHSCORE_H
#define HIGHSCORE_H

#include "constants.h"
#include <string>
#include <vector>
#include <iostream>

class HighscoreEntry {
	public:
		HighscoreEntry(char *playerName, int score, int level);
		~HighscoreEntry();
		int getScore() { return score; }
		int getLevel() { return level; }
		const char *getPlayerName() { return playerName->c_str(); }
	private:
		std::string *playerName;
		int score;
		int level;
};

class HighscoreList {
	public:
		static HighscoreList *getInstance();
		static void cleanUpInstance();
		int insertEntry(HighscoreEntry *entry);  // returns index of the inserted entry (i.e. its position-1), or -1 if it was not inserted (e.g. because the score was not enough)
		void print();
	private:
		static HighscoreList *instance;
		HighscoreList(uint8_t maxSize);
		~HighscoreList();
		uint8_t maxSize;
		std::vector<HighscoreEntry*> *entries;
		int idxLastInsertedEntry;
};

#endif
