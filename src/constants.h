#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <SDL/SDL.h>

class Constants {
	public:
		static const uint16_t INTELLIGENCE_BLINKY = 90;    // intelligence for each ghost
		static const uint16_t INTELLIGENCE_PINKY  = 60;
		static const uint16_t INTELLIGENCE_INKY   = 30;
		static const uint16_t INTELLIGENCE_CLYDE  = 0;
		static const uint16_t INIT_UP_DOWN        = 0;     // initial up/down cycles before the ghost will be allowed to leave the castle
		static const uint16_t INIT_UP_DOWN_INKY   = 5;
		static const uint16_t INIT_UP_DOWN_CLYDE  = 11;
		static const int MIN_FRAME_DURATION       = 30;    // duration of a loop in milliseconds (i.e. minimum time between frames)
		static const uint16_t START_OFFSET        = 4500;  // waiting time when the level starts, in ms
		static const uint16_t START_OFFSET_2      = 1500;  // waiting time after respawn, in ms
		static const int RED                      = 2;     // color red for init text
		static const int INITIAL_LIVES            = 3;     // number of times the player must die to get the "game over"
	private:
		Constants();
		~Constants();
};

#endif
