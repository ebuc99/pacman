#include "labyrinth.h"
#include "string.h"

Labyrinth::Labyrinth(Screen *screen):
	cnt_pill_animation(0),
	punktestand(0),
	bonus_stage(200),
	cnt_hunting_mode(-1),
	cnt_sleep(-1),
	font(NULL),
	smallFont(NULL),
	smallScore(NULL),
	initText(NULL),
	level(1){
	this->screen = screen;
	s0 = new Rail(207, 338, 412, 338);
	s1 = new Rail(207, 37, 207, 380);
	s2 = new Rail(138, 92, 480, 92);
	s3 = new Rail(412, 37, 412, 380);
	s4 = new Rail(138, 37, 290, 37);
	s5 = new Rail(330, 37, 480, 37);
	s6 = new Rail(138, 37, 138, 133);
	s7 = new Rail(480, 37, 480, 133);
	s8 = new Rail(138, 133, 207, 133);
	s9 = new Rail(412, 133, 480, 133);
	s10 = new Rail(290, 37, 290, 92);
	s11 = new Rail(330, 37, 330, 92);
	s12 = new Rail(249, 92, 249, 133);
	s13 = new Rail(371, 92, 371, 133);
	s14 = new Rail(249, 133, 290, 133);
	s15 = new Rail(330, 133, 371, 133);
	s16 = new Rail(290, 133, 290, 173);
	s17 = new Rail(330, 133, 330, 173);
	s18 = new Rail(249, 173, 371, 173);
	s19 = new Rail(249, 173, 249, 298);
	s20 = new Rail(371, 173, 371, 298);
	s21 = new Rail(249, 257, 371, 257);
	s22 = new Rail(100, 215, 249, 215);
	s23 = new Rail(371, 215, 515, 215);
	s24 = new Rail(138, 298, 290, 298);
	s25 = new Rail(330, 298, 480, 298);
	s26 = new Rail(290, 298, 290, 338);
	s27 = new Rail(330, 298, 330, 338);
	s28 = new Rail(138, 298, 138, 338);
	s29 = new Rail(480, 298, 480, 338);
	s30 = new Rail(138, 338, 166, 338);
	s31 = new Rail(452, 338, 480, 338);
	s32 = new Rail(166, 338, 166, 380);
	s33 = new Rail(452, 338, 452, 380);
	s34 = new Rail(138, 380, 207, 380);
	s35 = new Rail(412, 380, 480, 380);
	s36 = new Rail(138, 380, 138, 420);
	s37 = new Rail(480, 380, 480, 420);
	s38 = new Rail(138, 420, 480, 420);
	s39 = new Rail(249, 338, 249, 380);
	s40 = new Rail(371, 338, 371, 380);
	s41 = new Rail(249, 380, 290, 380);
	s42 = new Rail(330, 380, 371, 380);
	s43 = new Rail(290, 380, 290, 420);
	s44 = new Rail(330, 380, 330, 420);

	// ghost castle
	s45 = new Rail(310, 174, 310, 222);
	s46 = new Rail(280, 222, 309, 222);
	s47 = new Rail(311, 222, 340, 222);
	s48 = new Rail(280, 200, 280, 222);
	s49 = new Rail(340, 200, 340, 222);

	Rail *array_rails_temp[this->NUMBER_RAILS] = {s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11, s12, s13, s14, s15, s16, s17, s18, s19, s20, s21, s22, s23, s24, s25, s26, s27, s28, s29, s30, s31, s32, s33, s34, s35, s36, s37, s38, s39, s40, s41, s42, s43, s44, s45, s46, s47, s48, s49};
	memcpy(array_rails, array_rails_temp, sizeof(array_rails_temp));
	Rail *array_rails_pills_temp[this->NUMBER_RAILS_PILLS] = {s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11, s12, s13, s14, s15, s24, s25, s26, s27, s28, s29, s30, s31, s32, s33, s34, s35, s36, s37, s38, s39, s40, s41, s42, s43, s44};
	memcpy(array_rails_pills, array_rails_pills_temp, sizeof(array_rails_pills_temp));

	pille = this->LoadSurface("/usr/local/share/pacman/gfx/pille.png");	
	ar_superpille[0] = this->LoadSurface("/usr/local/share/pacman/gfx/superpille_1.png", 0);
	ar_superpille[1] = this->LoadSurface("/usr/local/share/pacman/gfx/superpille_2.png", 0);
	ar_superpille[2] = this->LoadSurface("/usr/local/share/pacman/gfx/superpille_3.png", 0);
	ar_superpille[3] = this->LoadSurface("/usr/local/share/pacman/gfx/superpille_3.png", 0);
	ar_superpille[4] = this->LoadSurface("/usr/local/share/pacman/gfx/superpille_2.png", 0);
	superpille = ar_superpille[cnt_pill_animation];

	textweiss = {255, 255, 255, 0};
	textgelb = {255, 247, 11, 0};
}   

Labyrinth::~Labyrinth(){
	SDL_FreeSurface(pille);
	SDL_FreeSurface(punkte);
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

void Labyrinth::init_pillen() {
	int m = 0;
	this->cnt_pills = this->NUMBER_PILLS;
	int i_ar_pille_x[26] = {148,162,176,190,204,217,231,245,259,273,287,300,314,327,340,354,368,381,395,409,422,436,449,462,476,490};
	int i_ar_pille_y[29] = {47,61,75,89,102,116,130,143,157,170,184,197,211,225,239,252,266,280,294,308,321,335,348,362,376,390,404,417,430};
		for(int i = 0; i < 26; i++) {
			for(int j = 0; j < 29; j++) {
				for(int k = 0; k < NUMBER_RAILS_PILLS; k++) {
					if(((array_rails_pills[k]->x1 <= (i_ar_pille_x[i] - 10)) && (array_rails_pills[k]->x2 >= (i_ar_pille_x[i] - 10)) && (array_rails_pills[k]->y1 == (i_ar_pille_y[j] - 10)) && (array_rails_pills[k]->y2 == (i_ar_pille_y[j] - 10))) || ((array_rails_pills[k]->x1 == (i_ar_pille_x[i] - 10)) && (array_rails_pills[k]->x2 == (i_ar_pille_x[i] - 10)) && (array_rails_pills[k]->y1 <= (i_ar_pille_y[j] - 10)) && (array_rails_pills[k]->y2 >= (i_ar_pille_y[j] - 10)))) {
						pillen[m].x = i_ar_pille_x[i];
						pillen[m].y = i_ar_pille_y[j];
						pillen[m].sichtbar = 1;
						if(((i_ar_pille_x[i] == 148) && (i_ar_pille_y[j] == 75)) || ((i_ar_pille_x[i] == 490) && (i_ar_pille_y[j] == 75)) || ((i_ar_pille_x[i] == 148) && (i_ar_pille_y[j] == 348)) || ((i_ar_pille_x[i] == 490) && (i_ar_pille_y[j] == 348))) {
							pillen[m].superpille = 1;
						}
						else
							pillen[m].superpille = 0;
						m++;
						break;
					}
				}
			}
		}
}

void Labyrinth::draw_pillen() {
	SDL_Rect dest;
	for(int i = 0; i < NUMBER_PILLS; i++) {
		if(pillen[i].sichtbar && !pillen[i].superpille) {
			dest.x = (short int)pillen[i].x;
			dest.y = (short int)pillen[i].y;
			SDL_BlitSurface(pille, NULL, screen->getSurface(), &dest);
		}
		if(pillen[i].sichtbar && pillen[i].superpille) {
			dest.x = (short int)(pillen[i].x - 4);
			dest.y = (short int)(pillen[i].y - 4);
			SDL_BlitSurface(superpille, NULL, screen->getSurface(), &dest);
			screen->AddUpdateRects(dest.x , dest.y, superpille->w, superpille->h);
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

void Labyrinth::compute_score(/*SDL_Surface *punkte, SDL_Color *textgelb*/) {
	char char_punktestand[8] = "0";
	sprintf(char_punktestand, "%d", this->punktestand);
	this->punkte = TTF_RenderText_Solid(font, char_punktestand, textgelb);
	screen->draw_dynamic_content(punkte, 530, 60);
}

void Labyrinth::startHuntingMode() {
	if (cnt_hunting_mode < 0) {
		this->bonus_stage = 200;
		this->cnt_hunting_mode = 7000;
	} else {
		// hunting mode was still active - prolong the it's duration
		this->cnt_hunting_mode += 7000;
	}
}

void Labyrinth::stopHuntingMode() {
	this->cnt_hunting_mode = -1;
	this->bonus_stage = 200;
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

void Labyrinth::setFonts(TTF_Font* font, TTF_Font* smallFont) {
	this->font      = font;
	this->smallFont = smallFont;
}

// decrease pills
void Labyrinth::decreasePills() {
	--cnt_pills;
}
// get exisiting pills
int Labyrinth::getExisitingPills() const {
	return cnt_pills;
}

void Labyrinth::setInitText(char *text) {
	char c_restartText[10] = "Get Ready";
	initText = TTF_RenderText_Solid(font, /*c_restartText*/text, this->textweiss);
}

void Labyrinth::drawInitText() {
	if(initText)
		screen->draw_dynamic_content(initText, 254, 258);
}

void Labyrinth::hideInitText() {
	if (initText) {
		screen->AddUpdateRects(254, 258, initText->w, initText->h);
		SDL_FreeSurface(initText);
		initText = NULL;
	}
}

void Labyrinth::initNewLevel() {
	this->stopHuntingMode();
	this->compute_score();
	screen->AddUpdateRects(0, 0, 500, 512);
	screen->Refresh();
	this->init_pillen();
	this->draw_pillen();
	this->hideFruit();
	this->startFruitRandomizer(true);
	++level;
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

void Labyrinth::startFruitRandomizer(int new_level) {
	if(new_level) 
		cnt_displayed_fruits = 0;
	if(cnt_displayed_fruits >= 2)
		return;
	fruit_display_time = 0;
	next_fruit = ((rand() % 4) + 5) * 10;
	next_fruit = getExisitingPills() - next_fruit;
}

void Labyrinth::setFruit(uint32_t time) {
	if(cnt_displayed_fruits >= 2)
		return;
	if(!fruit_display_time) {
		if(getExisitingPills() <= next_fruit) {
			fruit = LoadSurface("/usr/local/share/pacman/gfx/cherry.png",255);
			screen->draw_dynamic_content(fruit, 310, 257);
			fruit_display_time = time + 10000;
		}
	}
	else {
		if(time > fruit_display_time) {
			this->hideFruit();
		} else {
			screen->draw(fruit, 310, 257);
		}
	}	
}

void Labyrinth::hideFruit() {
	if(cnt_displayed_fruits > 3 || !fruit_display_time)
		return;
	//hideSurface(fruit, 310, 257);
	if(fruit) {
		screen->AddUpdateRects(310, 257, fruit->w, fruit->h);
		SDL_FreeSurface(fruit);
		fruit = NULL;
	}
	fruit_display_time = 0;
	++cnt_displayed_fruits;
	startFruitRandomizer(false);

}

int Labyrinth::fruitIsDisplayed() {
	return fruit_display_time;
}