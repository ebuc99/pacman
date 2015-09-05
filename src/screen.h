#ifndef SCREEN_H
#define SCREEN_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include "constants.h"

class Screen {
	public:
		static Screen *getInstance();
		static void cleanUpInstance();

	    // initialization
        // return whether an occured during the initialization of SDL
        bool hasSDLInitErrorOccured() {
            return sdlInitErrorOccured;
        }

        // methods for drawing
		// register graphic parts for updating
		void AddUpdateRects(int x, int y, int w, int h);  // should be renamed to addUpdateRect
		void addTotalUpdateRect();
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

		// draw a horizontal or vertical line using the specified color
		void drawHorizontalLine(int x1, int x2, int y, Uint8 r, Uint8 g, Uint8 b);
		void drawVerticalLine(int x, int y1, int y2, Uint8 r, Uint8 g, Uint8 b);
		SDL_Surface *LoadSurface(const char *filename, int transparent_color = -1);
		void clear();

		// draw a filled rectangle
		void fillRect(SDL_Rect *rect, Uint8 r, Uint8 g, Uint8 b);
	private:
		static Screen *instance;
        // constructor and destructor
		Screen();
		~Screen();

		SDL_Rect rects[Constants::MAX_UPDATE_RECTS];
		int rect_num;
		SDL_Surface *screen_surface;
		bool sdlInitErrorOccured;
		bool fullscreen;
};
#endif
