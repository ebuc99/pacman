#include "labyrinth.h"
#include <string.h>

Labyrinth::Labyrinth(Screen *screen):
	cnt_pill_animation(0),
	punktestand(0),
	bonus_stage(200),
	cnt_hunting_mode(-1),
	cnt_sleep(-1),
	font(NULL),
	smallFont(NULL),
	smallScore(NULL),
	score(NULL),
	initText(NULL),
	infoFruit(NULL),
	fruit(NULL),
	pillSurface(NULL),
	bgSurface(NULL),
	level(1){
	this->screen = screen;
	// horizontal rails, row by row, from left to right
	s0  = new Rail(138,  37, 207,  37);
	s1  = new Rail(207,  37, 290,  37);

	s2  = new Rail(330,  37, 412,  37);
	s3  = new Rail(412,  37, 480,  37);

	s4  = new Rail(138,  92, 207,  92);
	s5  = new Rail(207,  92, 249,  92);
	s6  = new Rail(249,  92, 290,  92);
	s7  = new Rail(290,  92, 330,  92);
	s8  = new Rail(330,  92, 371,  92);
	s9  = new Rail(371,  92, 412,  92);
	s10 = new Rail(412,  92, 480,  92);

	s11 = new Rail(138, 133, 207, 133);

	s12 = new Rail(249, 133, 290, 133);

	s13 = new Rail(330, 133, 371, 133);

	s14 = new Rail(412, 133, 480, 133);

	s15 = new Rail(249, 174, 290, 174);
	s16 = new Rail(290, 174, 310, 174);
	s17 = new Rail(310, 174, 330, 174);
	s18 = new Rail(330, 174, 371, 174);

	s19 = new Rail(100, 215, 207, 215);  // tunnel left
	s20 = new Rail(207, 215, 249, 215);

	s21 = new Rail(371, 215, 412, 215);
	s22 = new Rail(412, 215, 515, 215);  // tunnel right

	s23 = new Rail(249, 257, 371, 257);

	s24 = new Rail(138, 298, 207, 298);
	s25 = new Rail(207, 298, 249, 298);
	s26 = new Rail(249, 298, 290, 298);

	s27 = new Rail(330, 298, 371, 298);
	s28 = new Rail(371, 298, 412, 298);
	s29 = new Rail(412, 298, 480, 298);

	s30 = new Rail(138, 339, 166, 339);

	s31 = new Rail(207, 339, 249, 339);
	s32 = new Rail(249, 339, 290, 339);
	s33 = new Rail(290, 339, 330, 339);
	s34 = new Rail(330, 339, 371, 339);
	s35 = new Rail(371, 339, 412, 339);

	s36 = new Rail(452, 339, 480, 339);

	s37 = new Rail(138, 380, 166, 380);
	s38 = new Rail(166, 380, 207, 380);

	s39 = new Rail(249, 380, 290, 380);

	s40 = new Rail(330, 380, 371, 380);

	s41 = new Rail(412, 380, 452, 380);
	s42 = new Rail(452, 380, 480, 380);

	s43 = new Rail(138, 421, 290, 421);
	s44 = new Rail(290, 421, 330, 421);
	s45 = new Rail(330, 421, 480, 421);

	// vertical rails, column by column, from top to bottom
	s46 = new Rail(138,  37, 138,  92);
	s47 = new Rail(138,  92, 138, 133);

	s48 = new Rail(138, 298, 138, 339);

	s49 = new Rail(138, 380, 138, 421);

	s50 = new Rail(166, 339, 166, 380);

	s51 = new Rail(207,  37, 207,  92);
	s52 = new Rail(207,  92, 207, 133);
	s53 = new Rail(207, 133, 207, 215);
	s54 = new Rail(207, 215, 207, 298);
	s55 = new Rail(207, 298, 207, 339);
	s56 = new Rail(207, 339, 207, 380);

	s57 = new Rail(249,  92, 249, 133);

	s58 = new Rail(249, 174, 249, 215);
	s59 = new Rail(249, 215, 249, 257);
	s60 = new Rail(249, 257, 249, 298);

	s61 = new Rail(249, 339, 249, 380);

	s62 = new Rail(290,  37, 290,  92);

	s63 = new Rail(290, 133, 290, 174);

	s64 = new Rail(290, 298, 290, 339);

	s65 = new Rail(290, 380, 290, 421);

	s66 = new Rail(330,  37, 330,  92);

	s67 = new Rail(330, 133, 330, 174);

	s68 = new Rail(330, 298, 330, 339);

	s69 = new Rail(330, 380, 330, 421);

	s70 = new Rail(371,  92, 371, 133);

	s71 = new Rail(371, 174, 371, 215);
	s72 = new Rail(371, 215, 371, 257);
	s73 = new Rail(371, 257, 371, 298);

	s74 = new Rail(371, 339, 371, 380);

	s75 = new Rail(412,  37, 412,  92);
	s76 = new Rail(412,  92, 412, 133);
	s77 = new Rail(412, 133, 412, 215);
	s78 = new Rail(412, 215, 412, 298);
	s79 = new Rail(412, 298, 412, 339);
	s80 = new Rail(412, 339, 412, 380);

	s81 = new Rail(452, 339, 452, 380);

	s82 = new Rail(480,  37, 480,  92);
	s83 = new Rail(480,  92, 480, 133);

	s84 = new Rail(480, 298, 480, 339);

	s85 = new Rail(480, 380, 480, 421);

	// ghost castle
	s86 = new Rail(280, 222, 309, 222);  // horizontal, the left one - 1 pixel too short, so it is a one-way outwards
	s87 = new Rail(311, 222, 340, 222);  // horizontal, the right one - 1 pixel too short, so it is a one-way outwards
	s88 = new Rail(280, 200, 280, 222);  // vertical, the left one
	s89 = new Rail(310, 175, 310, 222);  // vertical, the middle (long) one - 1 pixel too short, so it usually is a one-way outwards
	s90 = new Rail(340, 200, 340, 222);  // vertical, the right one

	Rail *array_rails_temp[this->NUMBER_RAILS] = {s0,  s1,  s2,  s3,  s4,  s5,  s6,  s7,  s8,  s9,
	                                             s10, s11, s12, s13, s14, s15, s16, s17, s18, s19,
	                                             s20, s21, s22, s23, s24, s25, s26, s27, s28, s29,
	                                             s30, s31, s32, s33, s34, s35, s36, s37, s38, s39,
	                                             s40, s41, s42, s43, s44, s45, s46, s47, s48, s49,
	                                             s50, s51, s52, s53, s54, s55, s56, s57, s58, s59,
	                                             s60, s61, s62, s63, s64, s65, s66, s67, s68, s69,
	                                             s70, s71, s72, s73, s74, s75, s76, s77, s78, s79,
	                                             s80, s81, s82, s83, s84, s85, s86, s87, s88, s89,
	                                             s90};
	memcpy(array_rails, array_rails_temp, sizeof(array_rails_temp));
	Rail *array_rails_pills_temp[this->NUMBER_RAILS_PILLS] = {s0,  s1,  s2,  s3,  s4,  s5,  s6,  s7,  s8,  s9,
	                                                         s10, s11, s12, s13, s14,
	                                                                             s24, s25, s26, s27, s28, s29, 
	                                                         s30, s31, s32, s33, s34, s35, s36, s37, s38, s39,
	                                                         s40, s41, s42, s43, s44, s45, s46, s47, s48, s49,
	                                                         s50, s51, s52, s53, s54, s55, s56, s57,
	                                                              s61, s62,      s64, s65, s66,      s68, s69,
	                                                         s70,                s74, s75, s76, s77, s78, s79, 
	                                                         s80, s81, s82, s83, s84, s85};
	memcpy(array_rails_pills, array_rails_pills_temp, sizeof(array_rails_pills_temp));

	char filePath[256];
	getFilePath(filePath, "gfx/pille.png");
	pille = this->LoadSurface(filePath, 0);
	getFilePath(filePath, "gfx/superpille_1.png");
	ar_superpille[0] = this->LoadSurface(filePath, 0);
	getFilePath(filePath, "gfx/superpille_2.png");
	ar_superpille[1] = this->LoadSurface(filePath, 0);
	getFilePath(filePath, "gfx/superpille_3.png");
	ar_superpille[2] = this->LoadSurface(filePath, 0);
	getFilePath(filePath, "gfx/superpille_3.png");
	ar_superpille[3] = this->LoadSurface(filePath, 0);
	getFilePath(filePath, "gfx/superpille_2.png");
	ar_superpille[4] = this->LoadSurface(filePath, 0);
	superpille = ar_superpille[cnt_pill_animation];

	textweiss = {255, 255, 255, 0};
	textgelb = {255, 247, 11, 0};
	textrot = {255, 0, 0, 0};
	sounds = new Sounds();
}   

Labyrinth::~Labyrinth(){
	SDL_FreeSurface(pille);
	SDL_FreeSurface(score);
	SDL_FreeSurface(infoFruit);
	SDL_FreeSurface(initText);
	delete sounds;
}

void Labyrinth::draw_blocks() {
  	SDL_Rect b1, b2;
  	b1.x = 100;
  	b1.y = 215;
  	b1.w = 30;
  	b1.h = 30;
  
 	b2.x = 515;
  	b2.y = 215;
  	b2.w = 30;
  	b2.h = 30;
  
  	SDL_FillRect(this->screen->getSurface(), &b1, SDL_MapRGB(this->screen->getSurface()->format, 0, 0, 0));
  	SDL_FillRect(this->screen->getSurface(), &b2, SDL_MapRGB(this->screen->getSurface()->format, 0, 0, 0));
}

void Labyrinth::init_pillen(SDL_Surface *background) {
	int m = 0;
	int s = 0;
	this->cnt_pills = this->NUMBER_PILLS;
	int i_ar_pille_x[26] = {148,162,176,190,204,217,231,245,259,273,287,300,314,327,340,354,368,381,395,409,422,436,449,462,476,490};
	int i_ar_pille_y[29] = {47,61,75,89,102,116,130,143,157,170,184,197,211,225,239,252,266,280,294,308,321,335,349,362,376,390,404,417,431};
	for(int i = 0; i < 26; i++) {
		for(int j = 0; j < 29; j++) {
			for(int k = 0; k < NUMBER_RAILS_PILLS; k++) {
				if (array_rails_pills[k]->x1 <= i_ar_pille_x[i]-10 && i_ar_pille_x[i]-10 <= array_rails_pills[k]->x2 && array_rails_pills[k]->y1 <= i_ar_pille_y[j]-10 && i_ar_pille_y[j]-10 <= array_rails_pills[k]->y2) {
					pillen[m].x = i_ar_pille_x[i];
					pillen[m].y = i_ar_pille_y[j];
					pillen[m].sichtbar = 1;
					if (((i_ar_pille_x[i]==148)||(i_ar_pille_x[i] == 490)) && ((i_ar_pille_y[j] == 75)||(i_ar_pille_y[j] == 349))) {
						pillen[m].superpille = 1;
						idxSuperpills[s] = m;
						s++;
					}
					else
						pillen[m].superpille = 0;
					m++;
					break;
				}
			}
		}
	}
	if (background)
		bgSurface = background;
	if (bgSurface) {
		if (!pillSurface) {
			pillSurface = SDL_CreateRGBSurface(0, bgSurface->w, bgSurface->h, bgSurface->format->BitsPerPixel,
			  bgSurface->format->Rmask, bgSurface->format->Gmask, bgSurface->format->Bmask, bgSurface->format->Amask);
		}
		SDL_BlitSurface(bgSurface, NULL, pillSurface, NULL);
		SDL_Rect dest;
		for (int i = 0; i < NUMBER_PILLS; i++) {
			if (!pillen[i].superpille) {
				dest.x = (short int) pillen[i].x;
				dest.y = (short int) pillen[i].y;
				SDL_BlitSurface(pille, NULL, pillSurface, &dest);
			}
		}
	}
}

void Labyrinth::draw_pillen() {
	SDL_BlitSurface(pillSurface, NULL, screen->getSurface(), NULL);
	SDL_Rect dest;
	for (int i = 0; i < 4; i++) {
		if (pillen[idxSuperpills[i]].sichtbar) {
			dest.x = (short int) (pillen[idxSuperpills[i]].x - 4);
			dest.y = (short int) (pillen[idxSuperpills[i]].y - 4);
			SDL_BlitSurface(superpille, NULL, screen->getSurface(), &dest);
			screen->AddUpdateRects(dest.x, dest.y, superpille->w, superpille->h);
		}
	}
}

int Labyrinth::number_rails() const {
	return this->NUMBER_RAILS;
}

void Labyrinth::pill_animation() {
	if(cnt_pill_animation == 4)
		cnt_pill_animation = 0;
	else
		cnt_pill_animation++;
	superpille = ar_superpille[cnt_pill_animation];
}

SDL_Surface *Labyrinth::LoadSurface(const char *filename, int transparent_color) {
	SDL_Surface *surface, *temp;
	temp = IMG_Load(filename);
	if(!temp) {
		printf("Unable to load image: %s\n", IMG_GetError());
		exit(-1);
	}
	if(transparent_color != -1)
		SDL_SetColorKey(temp, SDL_SRCCOLORKEY | SDL_RLEACCEL, (Uint32)SDL_MapRGB(temp->format, (uint8_t)transparent_color, (uint8_t)transparent_color, (uint8_t)transparent_color));
	surface = SDL_DisplayFormat(temp);
	if(surface == NULL) {
		printf("Unable to convert image to display format: %s\n", SDL_GetError());
                exit(EXIT_FAILURE);
        }
    SDL_FreeSurface(temp);
    return surface;	
}

void Labyrinth::compute_score() {
	char char_punktestand[8] = "0";
	sprintf(char_punktestand, "%d", this->punktestand);
	if(this->score)
		SDL_FreeSurface(this->score);
	this->score = TTF_RenderText_Solid(font, char_punktestand, textgelb);
	screen->draw_dynamic_content(this->score, 530, 60);
}

void Labyrinth::startHuntingMode() {
	this->bonus_stage = 200;
	this->sounds->music_stop();
	this->sounds->superpill_start();
	if (cnt_hunting_mode < 0)
		this->cnt_hunting_mode = 7000;
	else // hunting mode was still active - prolong the it's duration
		this->cnt_hunting_mode += 7000;
}

void Labyrinth::stopHuntingMode() {
	this->cnt_hunting_mode = -1;
	this->bonus_stage = 200;
	this->sounds->music_stop();
}

void Labyrinth::increaseBonusStage() {
	if (this->bonus_stage < 1600)
		bonus_stage <<= 1;  // bit shifting is faster than bonus_stage *= 2;
}

void Labyrinth::sleep(int frames) {
	cnt_sleep = frames;
}

void Labyrinth::addScore(int value, int show_x, int show_y) {
	this->punktestand += value;
	// show the score at the specified position
	char ch[8] = "0";
	sprintf(ch, "%d", value);
	smallScore = TTF_RenderText_Solid(this->smallFont, ch, this->textweiss);
	if (smallScore == NULL) {
		printf("Unable to render text: %s\n", TTF_GetError());
		return;
	}
	smallScore_x = show_x - (smallScore->w >> 1);
	smallScore_y = show_y - (smallScore->h >> 1);
	drawSmallScore();
}

void Labyrinth::drawSmallScore() {
	if (smallScore)
		screen->draw_dynamic_content(smallScore, smallScore_x, smallScore_y);
}

void Labyrinth::hideSmallScore() {
	if (smallScore) {
		screen->AddUpdateRects(smallScore_x, smallScore_y, smallScore->w, smallScore->h);
		SDL_FreeSurface(smallScore);
		smallScore = NULL;
	}
}

void Labyrinth::addScore(int value) {
	this->punktestand += value;
}

void Labyrinth::addBonusScore(int show_x, int show_y) {
	this->addScore(this->bonus_stage, show_x, show_y);
}

int Labyrinth::getScore() {
	return this->punktestand;
}

void Labyrinth::setFonts(TTF_Font* font, TTF_Font* smallFont) {
	this->font      = font;
	this->smallFont = smallFont;
}

void Labyrinth::hidePill(int idxPill) {
	if (idxPill >= 0) {
		pillen[idxPill].sichtbar = 0;
		if (pillSurface && bgSurface) {
			SDL_Rect dest;
			dest.x = (short int) pillen[idxPill].x;
			dest.y = (short int) pillen[idxPill].y;
			dest.w = (short int) pille->w;
			dest.h = (short int) pille->h;
			SDL_BlitSurface(bgSurface, &dest, pillSurface, &dest);
		}
	}
}

// decrease pills
void Labyrinth::decreasePills() {
	--cnt_pills;
}
// get exisiting pills
int Labyrinth::getExisitingPills() const {
	return cnt_pills;
}

void Labyrinth::setInitText(char *text, int color) {
	char c_restartText[10] = "Get Ready";
	initText = TTF_RenderText_Solid(font, text, (color==1) ? this->textgelb : ((color==2)?this->textrot:this->textweiss));
}

void Labyrinth::drawInitText() {
	if(initText)
		screen->draw_dynamic_content(initText, 320-(initText->w >> 1), 268-(initText->h >> 1));
}

void Labyrinth::hideInitText() {
	if (initText) {
		screen->AddUpdateRects(320-(initText->w >> 1), 268-(initText->h >> 1), initText->w, initText->h);
		SDL_FreeSurface(initText);
		initText = NULL;
	}
}

void Labyrinth::initNewLevel() {
	this->stopHuntingMode();
	this->compute_score();
	screen->AddUpdateRects(0, 0, 500, 512);
	screen->Refresh();
	this->init_pillen(NULL);
	this->draw_pillen();
	this->hideFruit();
	this->startFruitRandomizer(true);
	setLevel(level + 1);
	char char_level[20];
	sprintf(char_level, "  Level %d", this->getLevel());
	this->setInitText(char_level);
	SDL_Delay(1000);
	screen->AddUpdateRects(0, 0, 500, 512);
	screen->Refresh();
}

int Labyrinth::getLevel() const {
	return level;
}

void Labyrinth::setLevel(int level) {
	char fruit_file[256];
	this->level = level;
	switch(this->level) {
		case 1:
			setFruitBonus(100);
			getFilePath(fruit_file, "gfx/cherry.png");
			break;
		case 2:
			setFruitBonus(300);
			getFilePath(fruit_file, "gfx/strawberry.png");
			break;
		case 3:
		case 4:
			setFruitBonus(500);
			getFilePath(fruit_file, "gfx/orange.png");
			break;
		case 5:
		case 6:
			setFruitBonus(700);
			getFilePath(fruit_file, "gfx/apple.png");
			break;
		case 7:
		case 8:
			setFruitBonus(1000);
			getFilePath(fruit_file, "gfx/grapes.png");
			break;
		case 9:
		case 10:
			setFruitBonus(2000);
			getFilePath(fruit_file, "gfx/banana.png");
			break;
		case 11:
		case 12:
			setFruitBonus(3000);
			getFilePath(fruit_file, "gfx/pear.png");
			break;
		default:
			setFruitBonus(5000);
			getFilePath(fruit_file, "gfx/key.png");
	};
	infoFruit = LoadSurface(fruit_file, 255);
	fruit = NULL;
	drawInfoFruit();
	screen->AddUpdateRects(525, 430, infoFruit->w, infoFruit->h);
}

void Labyrinth::startFruitRandomizer(int new_level) {
	if(new_level) 
		cnt_displayed_fruits = 0;
	if(cnt_displayed_fruits >= 2)
		return;
	fruit_display_time = 0;
	next_fruit = ((rand() % 4) + 5) * 10;
	next_fruit = getExisitingPills() - next_fruit;
}

void Labyrinth::checkFruit(int ms) {
	if(fruit) {
		if (this->cnt_sleep <= 0) {
			fruit_display_time -= ms;
			if (fruit_display_time <= 0)
				this->hideFruit();
		}
	} else {
		if(getExisitingPills() <= next_fruit) {
			fruit = infoFruit;
			fruit_display_time = 10000;
			++cnt_displayed_fruits;
		}
	}
}

void Labyrinth::hideFruit() {
	if (fruit) {
		screen->AddUpdateRects(310, 257, fruit->w, fruit->h);
		fruit = NULL;
		fruit_display_time = 0;
		if (cnt_displayed_fruits < 2) {
			startFruitRandomizer(false);
		} else {
			next_fruit = -1;  // never again in this level
		}
	}
}

int Labyrinth::fruitIsDisplayed() {
	return (fruit != NULL);
}

// set fruit bonus
void Labyrinth::setFruitBonus(int fruit_bonus) {
	this->fruit_bonus = fruit_bonus;
}

// get fruit bonus
int Labyrinth::getFruitBonus() const {
	return fruit_bonus;
}

void Labyrinth::drawFruit() {
	if (fruit)
		screen->draw_dynamic_content(fruit, 310, 257);
}

void Labyrinth::drawInfoFruit() {
	if (infoFruit)
		screen->draw(infoFruit, 525, 430);
}

void Labyrinth::getRailsForPoint(int x, int y, int *left, int *right, int *up, int *down) {
	*left = *right = *up = *down = -1;
	for (int i = 0; i < NUMBER_RAILS; ++i) {
		if (array_rails[i]->y1 == y && array_rails[i]->y2 == y) {
			if (array_rails[i]->x1 <= x && x <= array_rails[i]->x2) {
				// overlapping horizontal rail - left or right or both
				if (array_rails[i]->x1 < x)
					*left = i;
				if (x < array_rails[i]->x2)
					*right = i;
			}
		} else if (array_rails[i]->x1 == x && array_rails[i]->x2 == x) {
			if (array_rails[i]->y1 <= y && y <= array_rails[i]->y2) {
				// overlapping vertical rail - up or down or both
				if (array_rails[i]->y1 < y)
					*up = i;
				if (y < array_rails[i]->y2)
					*down = i;
			}
		}
		if (*left>=0 && *right>=0 && *up>=0 && *down>=0)
			return;
	}
}

void Labyrinth::playSoundDying() {
	sounds->playSingleSound(Sounds::DYING);
}

void Labyrinth::playSoundExtraMan() {
	sounds->playSingleSound(Sounds::EXTRA_MAN);
}

void Labyrinth::playSoundFruit() {
	sounds->playSingleSound(Sounds::FRUIT);
}

void Labyrinth::playEatGhost() {
	sounds->playSingleSound(Sounds::EAT_GHOST);
}

Sounds* Labyrinth::getSounds() {
	return sounds;
}
