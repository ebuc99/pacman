#ifndef FIGUR_H
#define FIGUR_H
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

class Figur {
	public:
		// constructor and destructor
		Figur(int init_x, int init_y, float init_v);
		~Figur();
		
		// position of the figure
		int x, y; 
		
		// last x and y values (important when checking whether pills have been eaten)
		int last_x, last_y; 
		
		// four directions of movement
		void move_left(float ms, float max_step = 999);
		void move_up(float ms, float max_step = 999);
		void move_right(float ms, float max_step = 999);
		void move_down(float ms, float max_step = 999);
		
		// wrapper for the directional movement functions
		void move(float ms, int direction);
		
		// sets the speed of the figure
		void set_speed(float v);
		
		// returns the speed
		float get_speed() const;
		
		// has the figure moved? (needed for performance tuning)
		int was_moving();
		
		// is the figure parking?
		void parking();
		
		// returns the direction
		int get_richtung() const;
		
		// sets the direction of movement
		void set_richtung(int p_richtung);
		
	protected:
		float cur_y;
		float cur_x;
		int wechsel_x;
		int wechsel_y;
		int wechsel_rate;
		float dx, dy;
		int richtung;
		int intelligence;
		float least(float a, float b);
		int initial_x;
		int initial_y;
		float initial_v;
		SDL_Surface *LoadSurface(const char *filename, int transparent_color);
};
#endif
