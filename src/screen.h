#ifndef SCREEN_H
#define SCREEN_H
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

class Screen {
	public:
	    // initialization
        // constructor and destructor
		Screen();
		~Screen();
        // return the initialization's error code (currently EXIT_FAILURE if an error occured)
        int occuredInitSdlError() {
            return sdl_init_error;
        }

        // methods for drawing
		// register graphic parts for updating
		void AddUpdateRects(int x, int y, int w, int h);
		// actually display the updated rectangles
		void Refresh();
		
		void draw_dynamic_content(SDL_Surface *surface, int x, int y);
			
		// return the screen surface pointer
		SDL_Surface* getSurface() {
		    return screen_surface;
		}
		// draw another graphic to the screen's surface
		void draw(SDL_Surface* graphic, int offset_x = 0, int offset_y = 0);

		// methods for fullscreen mode
		// return whether fullscreen mode is set
		bool isFullscreen() {
		    return fullscreen;
		}
		// switch to/from fullscreen
		void setFullscreen(bool fs);
		// toggle fullscreen mode
		void toggleFullscreen() {
		    setFullscreen(!fullscreen);
		}
	private:
		SDL_Rect rects[200];
		int rect_num;
		SDL_Surface *screen_surface;
		int sdl_init_error;
		bool fullscreen;
};
#endif