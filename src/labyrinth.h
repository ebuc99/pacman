#ifndef LABYRINTH_H
#define LABYRINTH_H

#include <SDL/SDL_ttf.h>
#include "screen.h"
#include "pille.h"
#include "rail.h"

class Labyrinth {
	private: 
		static const int NUMBER_RAILS = 50; // number of rails
		static const int NUMBER_RAILS_PILLS = 37; // number of pill-filled rails
		int cnt_pill_animation; 
		Rail *s0, *s1, *s2, *s3, *s4, *s5, *s6, *s7, *s8, *s9, *s10, *s11, 
				*s12, *s13, *s14, *s15, *s16, *s17, *s18, *s19, *s20, *s21, 
				*s22, *s23, *s24, *s25,	*s26, *s27, *s28, *s29, *s30, *s31,
				*s32, *s33, *s34, *s35, *s36, *s37, *s38, *s39, *s40, *s41,
				*s42, *s43, *s44, *s45, *s46, *s47, *s48, *s49;
		SDL_Surface *pille, *superpille, *ar_superpille[5];
		
		//very bad, because the same method is in figur (and pacman)
		// it must be in a basic class in future
		SDL_Surface *LoadSurface(const char *filename, int transparent_color = -1);
		int punktestand;
		int bonus_stage; //200, 400, 800, 1600
		TTF_Font *font, *smallFont;
		SDL_Surface *smallScore, *initText, *punkte, *fruit;
		int smallScore_x, smallScore_y;
		SDL_Color textweiss, textgelb, textrot;
		int cnt_pills;
		int level;
		int next_fruit;
		uint32_t fruit_display_time;
		int cnt_displayed_fruits;
		int fruit_displayed;
		int fruit_bonus;
	public:
		//constructor and destructor
		Labyrinth(Screen *screen);
		~Labyrinth();

		// At the left and right of the tunnel, there are two black rectangles,
		// below which pacman and the ghosts will disappear if they leave the
		// level to the left or right.
		void draw_blocks();

		/* initially, throw some pills across the level */
		void init_pillen();

		/* draw pills, but only if Blinky has moved */
		void draw_pillen();

		// superpill animation
		void pill_animation();

		void compute_score(/*SDL_Surface *punkte, SDL_Color *textgelb*/);

		int number_rails() const;

		Screen *screen;
		int sdl_init_error;

		static const int NUMBER_PILLS = 246; // number of pills
		Pille pillen[NUMBER_PILLS];
		
		Rail *array_rails[NUMBER_RAILS];
		Rail *array_rails_pills[NUMBER_RAILS_PILLS];

		int cnt_hunting_mode;

		// call these methods when hunting mode starts and ends
		void startHuntingMode();
		void stopHuntingMode();
		// increases the bonus stage to the next value: 200 -> 400 -> 800 -> 1600
		void increaseBonusStage();
		// adds a value to the player's score
		void addScore(int value, int show_x, int show_y);  // use this method if the score should be displayed
		void addScore(int value);                          // use this method if the score should not be displayed
		void drawSmallScore();
		void hideSmallScore();
		// adds the current bonus stage to the player's score
		void addBonusScore(int show_x, int show_y);
		// retrieve current score
		int getScore();

		// sleep for a short time - should be used after a ghost or a fruit has been eaten (display score, sleep shortly, remove score)
		void sleep(int frames);
		int cnt_sleep;

		// assigns fonts
		void setFonts(TTF_Font* font, TTF_Font* smallFont);

		// decrease pills
		void decreasePills();

		// get exisiting pills
		int getExisitingPills() const;

		// set init text, color is 1=yellow, 2=red, white otherwise.
		void setInitText(char *text, int color = 0);
		
		// level init text
		void drawInitText();

		// hide init text
		void hideInitText();

		// init new level
		void initNewLevel();

		// get level
		int getLevel() const;

		// set level
		void setLevel(int level);

		// start the fruit randomizer 
		// first fruit after 50, 60, 70 or 80
		// eaten pills
		void startFruitRandomizer(int new_level);

		// display fruit, if the number of
		// pills was eaten or don't display
		// if 10 s are over
		void setFruit();

		// dont show fruit
		void hideFruit();

		// fruit is displayed
		int fruitIsDisplayed();

		// set fruit bonus
		void setFruitBonus(int fruit_bonus);

		// get fruit bonus
		int getFruitBonus() const;

		// draw fruit in info area
		void drawInfoFruit();
};
#endif
