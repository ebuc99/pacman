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

HighscoreList::HighscoreList(uint8_t maxSize):
	sfTitle(NULL),
	sfPosTitle(NULL),
	sfNameTitle(NULL),
	sfScoreTitle(NULL),
	sfLevelTitle(NULL),
	sfBackItem(NULL)
{
	this->maxSize = maxSize;
	entries = new std::vector<HighscoreEntry*>();
	idxLastInsertedEntry = -1;
	sfPositions   = new SDL_Surface*[maxSize];
	sfPlayerNames = new SDL_Surface*[maxSize];
	sfScores      = new SDL_Surface*[maxSize];
	sfLevels      = new SDL_Surface*[maxSize];
	for (int i = 0; i < maxSize; ++i) {
		sfPositions[i]   = NULL;
		sfPlayerNames[i] = NULL;
		sfScores[i]      = NULL;
		sfLevels[i]      = NULL;
	}
}

HighscoreList::~HighscoreList() {
	if (entries) {
		int i = 0;
		for (std::vector<HighscoreEntry*>::iterator it = entries->begin(); it != entries->end(); ++it) {
			delete *it;
			if (sfPositions[i])
				SDL_FreeSurface(sfPositions[i]);
			if (sfPlayerNames[i])
				SDL_FreeSurface(sfPlayerNames[i]);
			if (sfScores[i])
				SDL_FreeSurface(sfScores[i]);
			if (sfLevels[i])
				SDL_FreeSurface(sfLevels[i]);
			++i;
		}
		delete entries;
		entries = NULL;
		delete [] sfPositions;
		delete [] sfPlayerNames;
		delete [] sfScores;
		delete [] sfLevels;
	}
	if (sfTitle)
		SDL_FreeSurface(sfTitle);
	if (sfPosTitle)
		SDL_FreeSurface(sfPosTitle);
	if (sfNameTitle)
		SDL_FreeSurface(sfNameTitle);
	if (sfScoreTitle)
		SDL_FreeSurface(sfScoreTitle);
	if (sfLevelTitle)
		SDL_FreeSurface(sfLevelTitle);
	if (sfBackItem)
		SDL_FreeSurface(sfBackItem);
}

int HighscoreList::insertEntry(HighscoreEntry *entry) {
	int i = 0;
	for (std::vector<HighscoreEntry*>::iterator it = entries->begin(); it != entries->end(); ++it) {
		if (entry->getScore()>(*it)->getScore() || (entry->getScore()==(*it)->getScore() && entry->getLevel()>(*it)->getLevel())) {
			entries->insert(it, entry);
			if ((int) entries->size() > maxSize) {
				entries->pop_back();
				if (sfPlayerNames[maxSize-1]) {
					SDL_FreeSurface(sfPlayerNames[maxSize-1]);
					sfPlayerNames[maxSize-1] = NULL;
				}
				if (sfScores[maxSize-1]) {
					SDL_FreeSurface(sfScores[maxSize-1]);
					sfScores[maxSize-1] = NULL;
				}
				if (sfLevels[maxSize-1]) {
					SDL_FreeSurface(sfLevels[maxSize-1]);
					sfLevels[maxSize-1] = NULL;
				}
			}
			for (int j = maxSize-1; j > i; --j) {
				// position will not be moved
				sfPlayerNames[j] = sfPlayerNames[j-1];
				sfScores[j]      = sfScores[j-1];
				sfLevels[j]      = sfLevels[j-1];
			}
			sfPlayerNames[i] = NULL;
			sfScores[i]      = NULL;
			sfLevels[i]      = NULL;
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

void HighscoreList::draw() {
	if (!sfTitle)
		sfTitle = Screen::getTextSurface(Screen::getVeryLargeFont(), "Highscore List", Constants::WHITE_COLOR);
	if (!sfPosTitle)
		sfPosTitle = Screen::getTextSurface(Screen::getFont(), "Pos.", Constants::WHITE_COLOR);
	if (!sfNameTitle)
		sfNameTitle = Screen::getTextSurface(Screen::getFont(), "Name", Constants::WHITE_COLOR);
	if (!sfScoreTitle)
		sfScoreTitle = Screen::getTextSurface(Screen::getFont(), "Score", Constants::WHITE_COLOR);
	if (!sfLevelTitle)
		sfLevelTitle = Screen::getTextSurface(Screen::getFont(), "Lev.", Constants::WHITE_COLOR);
	if (!sfBackItem)
		sfBackItem = Screen::getTextSurface(Screen::getLargeFont(), "back to menu", Constants::WHITE_COLOR);
	char ch_array[8];
	int maxWidthPosition = sfPosTitle->w;
	int maxWidthName     = sfNameTitle->w;
	int maxWidthScore    = sfScoreTitle->w;
	int maxWidthLevel    = sfLevelTitle->w;
	int i = 0;
	for (std::vector<HighscoreEntry*>::iterator it = entries->begin(); it != entries->end(); ++it) {
		if (!sfPositions[i]) {
			sprintf(ch_array, "%d.", i+1);
			sfPositions[i] = Screen::getTextSurface(Screen::getFont(), ch_array, Constants::GRAY_COLOR);
		}
		if (!sfPlayerNames[i]) {
			sfPlayerNames[i] = Screen::getTextSurface(Screen::getFont(), (*it)->getPlayerName(), Constants::GRAY_COLOR);
		}
		if (!sfScores[i]) {
			sprintf(ch_array, "%d", (*it)->getScore());
			sfScores[i] = Screen::getTextSurface(Screen::getFont(), ch_array, Constants::GRAY_COLOR);
		}
		if (!sfLevels[i]) {
			sprintf(ch_array, "%d", (*it)->getLevel());
			sfLevels[i] = Screen::getTextSurface(Screen::getFont(), ch_array, Constants::GRAY_COLOR);
		}
		if (sfPositions[i]->w > maxWidthPosition)
			maxWidthPosition = sfPositions[i]->w;
		if (sfPlayerNames[i]->w > maxWidthName)
			maxWidthName = sfPlayerNames[i]->w;
		if (sfScores[i]->w > maxWidthScore)
			maxWidthScore = sfScores[i]->w;
		if (sfLevels[i]->w > maxWidthLevel)
			maxWidthLevel = sfLevels[i]->w;
		++i;
	}
	int totalWidth = maxWidthPosition+maxWidthName+maxWidthScore+maxWidthLevel + 3*Constants::HIGHSCORE_COLUMN_SPACING;
	if (totalWidth > Constants::WINDOW_WIDTH) {
		maxWidthName = maxWidthName - totalWidth + Constants::WINDOW_WIDTH;
		totalWidth   = Constants::WINDOW_WIDTH;
	}
	int x1 = (Constants::WINDOW_WIDTH - totalWidth) >> 1;
	int x2 = x1 + maxWidthPosition + Constants::HIGHSCORE_COLUMN_SPACING;
	int x3 = x2 + maxWidthName + Constants::HIGHSCORE_COLUMN_SPACING;
	int x4 = x3 + maxWidthScore + Constants::HIGHSCORE_COLUMN_SPACING;
	Screen::getInstance()->clear();
	Screen::getInstance()->draw(sfTitle, (Constants::WINDOW_WIDTH-sfTitle->w)>>1, 10);
	Screen::getInstance()->draw(sfPosTitle,   x1, 80);
	Screen::getInstance()->draw(sfNameTitle,  x2, 80);
	Screen::getInstance()->draw(sfScoreTitle, x3, 80);
	Screen::getInstance()->draw(sfLevelTitle, x4, 80);
	for (uint8_t i = 0; i < entries->size(); ++i) {
		int y = 120 + i*30;
		if (sfPositions[i])
			Screen::getInstance()->draw(sfPositions[i], x1 + maxWidthPosition - sfPositions[i]->w, y);
		if (sfPlayerNames[i])
			Screen::getInstance()->draw(sfPlayerNames[i], x2, y);
		if (sfScores[i])
			Screen::getInstance()->draw(sfScores[i], x3 + maxWidthScore - sfScores[i]->w, y);
		if (sfLevels[i])
			Screen::getInstance()->draw(sfLevels[i], x4 + maxWidthLevel - sfLevels[i]->w - 10, y);
	}
	Screen::getInstance()->draw(sfBackItem, (Constants::WINDOW_WIDTH-sfBackItem->w)>>1, 430);
	Screen::getInstance()->addTotalUpdateRect();
	Screen::getInstance()->Refresh();
}

bool HighscoreList::eventloop() {
	SDL_Event event;
	bool refreshWindow = false;
	while(SDL_PollEvent(&event)) {
		switch(event.type) {
		case SDL_KEYDOWN:
			if(event.key.keysym.sym == SDLK_RETURN) {
				return false;
			} else if(event.key.keysym.sym == SDLK_f) {
				Screen::getInstance()->toggleFullscreen();
				draw();
			} else if(event.key.keysym.sym == SDLK_s) {
				Sounds::getInstance()->toggleEnabled();
			} else if((event.key.keysym.sym == SDLK_q)||(event.key.keysym.sym == SDLK_ESCAPE))
				return false;
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT) {
				if ((Constants::WINDOW_WIDTH-sfBackItem->w)>>1 <= event.motion.x && event.motion.x <= (Constants::WINDOW_WIDTH+sfBackItem->w)>>1 && 430 <= event.motion.y && event.motion.y <= 430+sfBackItem->h) {
					return false;
				}
			}
			break;
		case SDL_QUIT:
			return false;
		}
		if (event.window.event == SDL_WINDOWEVENT_EXPOSED || event.window.event == SDL_WINDOWEVENT_FOCUS_GAINED) {
			refreshWindow = true;
		}
	}
	if (refreshWindow) {
		Screen::getInstance()->addTotalUpdateRect();
		Screen::getInstance()->Refresh();
	}
	return true;
}

void HighscoreList::show() {
	while (eventloop()) {
		draw();
		SDL_Delay(Constants::MIN_FRAME_DURATION);
	}
}
