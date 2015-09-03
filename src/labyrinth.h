#ifndef LABYRINTH_H
#define LABYRINTH_H

#include <SDL/SDL_ttf.h>
#include "screen.h"
#include "pille.h"
#include "rail.h"
#include "sounds.h"
#include "platform.h"

class Labyrinth {
	private:
		static Labyrinth *instance;

		//constructor and destructor
		Labyrinth();
		~Labyrinth();

		static const int NUMBER_RAILS = 91; // number of rails
		static const int NUMBER_RAILS_PILLS = 69; // number of pill-filled rails
		int cnt_pill_animation;
		Rail *s0,  *s1,  *s2,  *s3,  *s4,  *s5,  *s6,  *s7,  *s8,  *s9,
		    *s10, *s11, *s12, *s13, *s14, *s15, *s16, *s17, *s18, *s19,
		    *s20, *s21, *s22, *s23, *s24, *s25,	*s26, *s27, *s28, *s29,
		    *s30, *s31, *s32, *s33, *s34, *s35, *s36, *s37, *s38, *s39,
		    *s40, *s41, *s42, *s43, *s44, *s45, *s46, *s47, *s48, *s49,
		    *s50, *s51, *s52, *s53, *s54, *s55, *s56, *s57, *s58, *s59,
		    *s60, *s61, *s62, *s63, *s64, *s65, *s66, *s67, *s68, *s69,
		    *s70, *s71, *s72, *s73, *s74, *s75, *s76, *s77, *s78, *s79,
		    *s80, *s81, *s82, *s83, *s84, *s85, *s86, *s87, *s88, *s89,
		    *s90;
		SDL_Surface *pille, *superpille, *ar_superpille[5];
		int punktestand;
		int bonus_stage; //200, 400, 800, 1600
		TTF_Font *font, *smallFont;
		SDL_Surface *smallScore, *initText, *score, *infoFruit, *fruit, *pillSurface, *bgSurface;
		int smallScore_x, smallScore_y;
		SDL_Color textweiss, textgelb, textrot;
		int cnt_pills;
		int level;
		int next_fruit;
		int fruit_display_time;
		int cnt_displayed_fruits;
		int fruit_displayed;
		int fruit_bonus;
		int idxSuperpills[4];
	public:
		static Labyrinth* getInstance();
		static void cleanUpInstance();

		// At the left and right of the tunnel, there are two black rectangles,
		// below which pacman and the ghosts will disappear if they leave the
		// level to the left or right.
		void draw_blocks();

		/* initially, throw some pills across the level */
		void init_pillen(SDL_Surface *background, bool firstInit);

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

		// hide the pill having the specified index
		void hidePill(int idxPill);

		// decrease pills
		void decreasePills();

		// get exisiting pills
		int getExisitingPills() const;

		// set init text, color is 1=yellow, 2=red, white otherwise.
		void setInitText(const char *text, int color = 0);

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

		// Check whether enough pills have been eaten to display the next fruit.
		// If the fruit is displayed, decrement it's display time, and hide it
		// if 10 s are over.
		void checkFruit(int ms);

		// dont show fruit
		void hideFruit();

		// fruit is displayed
		int fruitIsDisplayed();

		// set fruit bonus
		void setFruitBonus(int fruit_bonus);

		// get fruit bonus
		int getFruitBonus() const;

		// draws the fruit, if it is displayed
		void drawFruit();

		// draw fruit in info area
		void drawInfoFruit();

		// play sound extra man
		void playSoundExtraMan();

		// eating the fruit sound
		void playSoundFruit();

		// the sound of dying
		void playSoundDying();

		// pacman eats ghost
		void playEatGhost();

		// get all the fancy sounds
		Sounds* getSounds();

		// Return indexes of the Rails from the specified point (x,y), -1 if there is no such rail.
		void getRailsForPoint(int x, int y, int *left, int *right, int *up, int *down);

		void resetScore();

		// Return the current surface of the superpill
		SDL_Surface *get_superpill_sf();
};
#endif
