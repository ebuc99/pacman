#include <SDL/SDL.h>

class Screen {
	public:
		Screen();
		~Screen();
		
		/* register graphic parts for updating */
		void AddUpdateRects(int x, int y, int w, int h);
		void Refresh(int moving);
		void draw_static_content(SDL_Surface *surface, int x, int y, int moving, int force = 0);
		void draw_dynamic_content(SDL_Surface *surface, int x, int y, int moving);
			
		SDL_Surface *screen_sf;
		int sdl_init_error;
	private:
		SDL_Rect rects[200];
		int rect_num;
};