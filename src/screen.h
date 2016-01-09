#ifndef SCREEN_H
#define SCREEN_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include "cmdline_options.h"
#include "constants.h"
#include "platform.h"

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

		SDL_Window* getWindow() { return window; };

		// return the screen surface pointer
		SDL_Surface* getSurface() {
		    return screen_surface;
		}

		// return the screen texture
		SDL_Texture* getTexture() {
			return txScreenSurface;
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
		void clear();

		static SDL_Surface *loadImage(const char *filename, int transparentColor = -1);
		static TTF_Font *loadFont(const char *filename, int ptSize);
		static SDL_Surface *getTextSurface(TTF_Font *font, const char *text, SDL_Color color);
		//SDL_Renderer *getRenderer();

		// draw a filled rectangle
		void fillRect(SDL_Rect *rect, Uint8 r, Uint8 g, Uint8 b);

		// deprecated functions, will be removed in the future
		SDL_Surface *LoadSurface(const char *filename, int transparent_color = -1);

		static TTF_Font *getSmallFont();
		static TTF_Font *getFont();
		static TTF_Font *getLargeFont();
		static TTF_Font *getVeryLargeFont();
		static TTF_Font *getHugeFont();

	private:
		static Screen *instance;
        // constructor and destructor
		Screen();
		~Screen();
		SDL_Window *window;
		SDL_Rect rects[Constants::MAX_UPDATE_RECTS];
		int rect_num;
		SDL_Surface *screen_surface;
		SDL_Texture *txScreenSurface;
		bool sdlInitErrorOccured;
		bool fullscreen;
		static TTF_Font *smallFont, *font, *largeFont, *veryLargeFont, *hugeFont;
};
#endif
