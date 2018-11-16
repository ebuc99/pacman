#include "screen.h"

TTF_Font *Screen::smallFont     = NULL;
TTF_Font *Screen::font          = NULL;
TTF_Font *Screen::largeFont     = NULL;
TTF_Font *Screen::veryLargeFont = NULL;
TTF_Font *Screen::hugeFont      = NULL;

Screen *Screen::instance = NULL;

Screen *Screen::getInstance() {
	if (!instance) {
		instance = new Screen();
	}
	return instance;
}

void Screen::cleanUpInstance() {
	if (smallFont) {
		TTF_CloseFont(smallFont);
		smallFont = NULL;
	}
	if (font) {
		TTF_CloseFont(font);
		font = NULL;
	}
	if (largeFont) {
		TTF_CloseFont(largeFont);
		largeFont = NULL;
	}
	if (veryLargeFont) {
		TTF_CloseFont(veryLargeFont);
		veryLargeFont = NULL;
	}
	if (hugeFont) {
		TTF_CloseFont(hugeFont);
		hugeFont = NULL;
	}
	if (instance) {
		delete instance;
		instance = NULL;
	}
}

Screen::Screen():
	sdlInitErrorOccured(false),
	fullscreen(CommandLineOptions::exists("f","fullscreen")),
	rect_num(0),
	scalingFactor(1)
{
	// initialize SDL
	if(SDL_InitSubSystem(SDL_INIT_VIDEO) != 0) {
		std::cout << "SDL video initialization failed: " << SDL_GetError() << std::endl;
        sdlInitErrorOccured = true;
    }
	if(!sdlInitErrorOccured && TTF_Init() == -1) {
		std::cout << "TTF initialization failed: " << TTF_GetError() << std::endl;
        sdlInitErrorOccured = true;
	}
	if (!sdlInitErrorOccured) {
		window = SDL_CreateWindow("Pacman",
								  SDL_WINDOWPOS_UNDEFINED,
                                  SDL_WINDOWPOS_UNDEFINED,
                           		  Constants::WINDOW_WIDTH,
                           		  Constants::WINDOW_HEIGHT,
                           		  fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
		screen_surface = SDL_GetWindowSurface(window);
		computeClipRect();
		if(screen_surface == 0) {
			std::cout << "Setting video mode failed: " << SDL_GetError() << std::endl;
			sdlInitErrorOccured = true;
		}
	}
	atexit(Screen::cleanUpInstance);
}

Screen::~Screen() {
	TTF_Quit();
	SDL_QuitSubSystem(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER);
}

void Screen::AddUpdateRects(int x, int y, int w, int h) {
	if (rect_num >= Constants::MAX_UPDATE_RECTS)
		return;  // prevent index out of bounds problems
	if (x < 0) {
		w += x;
		x = 0;
	}
	if (y < 0) {
		h += y;
		y = 0;
	}
	if (x + w > clipRect.w)
		w = clipRect.w - x;
	if (y + h > clipRect.h)
		h = clipRect.h - y;
	if (w <= 0 || h <= 0)
		return;
	rects[rect_num].x = (short int) x*scalingFactor + clipRect.x;
	rects[rect_num].y = (short int) y*scalingFactor + clipRect.y;
	rects[rect_num].w = (short int) w * scalingFactor;
	rects[rect_num].h = (short int) h * scalingFactor;
	rect_num++;
}

void Screen::addTotalUpdateRect() {
	rects[0].x = 0;
	rects[0].y = 0;
	rects[0].w = screen_surface->w;  // no scalingFactor as screen_surface already is the total screen surface
	rects[0].h = screen_surface->h;
	rect_num = 1;  // all other update rects will be included in this one
}

void Screen::addUpdateClipRect() {
	AddUpdateRects(0, 0, Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT);
}

void Screen::Refresh() {
	SDL_UpdateWindowSurfaceRects(window, rects, rect_num);
	rect_num = 0;
}

void Screen::draw_dynamic_content(SDL_Surface *surface, int x, int y) {
	SDL_Rect dest;
	dest.x = (short int) x*scalingFactor + clipRect.x;
	dest.y = (short int) y*scalingFactor + clipRect.y;
	dest.w = (short int) surface->w * scalingFactor;
	dest.h = (short int) surface->h * scalingFactor;
	if (scalingFactor > 1) {
		SDL_BlitScaled(surface, NULL, screen_surface, &dest);
	} else {
		SDL_BlitSurface(surface, NULL, screen_surface, &dest);
	}
	AddUpdateRects(x, y, surface->w + 10, surface->h);
}

void Screen::draw(SDL_Surface* graphic, int offset_x, int offset_y) {
    if (0 == offset_x && 0 == offset_y && 0 == clipRect.x && 0 == clipRect.y && scalingFactor == 1) {
        SDL_BlitSurface(graphic, NULL, screen_surface, NULL);
    } else {
        SDL_Rect position;
        position.x = (short int) offset_x*scalingFactor + clipRect.x;
        position.y = (short int) offset_y*scalingFactor + clipRect.y;
		position.w = (short int) graphic->w * scalingFactor;
		position.h = (short int) graphic->h * scalingFactor;
		if (scalingFactor > 1) {
			SDL_BlitScaled(graphic, NULL, screen_surface, &position);
		} else {
			SDL_BlitSurface(graphic, NULL, screen_surface, &position);
		}
    }
}

void Screen::setFullscreen(bool fs) {
	if (fs == fullscreen) {
		return;  // the desired mode already has been activated, so do nothing
	}
	SDL_SetWindowSize(window, Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT);
	SDL_SetWindowPosition(window, 0, 0);
	if (fs) {
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	} else {
		SDL_SetWindowFullscreen(window, 0);
	}
	SDL_SetWindowSize(window, Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT);
	SDL_SetWindowPosition(window, 0, 0);
	SDL_Surface* newScreen = SDL_GetWindowSurface(window);
	if(newScreen) {
		screen_surface = newScreen;
		computeClipRect();
		clearOutsideClipRect();
		addTotalUpdateRect();
		fullscreen = fs;
	} else {
		if (fs) {
			std::cout << "Switching to fullscreen mode failed: " << SDL_GetError() << std::endl;
		} else {
			std::cout << "Switching from fullscreen mode failed: " << SDL_GetError() << std::endl;
		}
	}
}

SDL_Surface *Screen::loadImage(const char *filename, int transparentColor) {
	char filePath[256];
	getFilePath(filePath, filename);
	SDL_Surface *surface, *temp;
	temp = IMG_Load(filePath);
	if (!temp) {
		std::cout << "Unable to load image: " << IMG_GetError() << std::endl;
		exit(EXIT_FAILURE);
	}
	if (transparentColor != -1)
		SDL_SetColorKey(temp, SDL_TRUE | SDL_RLEACCEL, (Uint32)SDL_MapRGB(temp->format, (uint8_t)transparentColor, (uint8_t)transparentColor, (uint8_t)transparentColor));
	surface = SDL_ConvertSurface(temp,  Screen::getInstance()->getSurface()->format, 0);
	if (surface == NULL) {
		std::cout << "Unable to convert image to display format: " << SDL_GetError() << std::endl;
		exit(EXIT_FAILURE);
	}
	SDL_FreeSurface(temp);
	return surface;
}

TTF_Font *Screen::loadFont(const char *filename, int ptSize) {
	char filePath[256];
	getFilePath(filePath, filename);
	TTF_Font *font = TTF_OpenFont(filePath, ptSize);
	if (!font) {
		std::cout << "Unable to open TTF font: " << TTF_GetError() << std::endl;
		exit(EXIT_FAILURE);
	}
	return font;
}

SDL_Surface *Screen::getTextSurface(TTF_Font *font, const char *text, SDL_Color color) {
	SDL_Surface *temp = TTF_RenderText_Solid(font, text, color);
	if (!temp) {
		std::cout << "Unable to render text \"" << text << "\": " << TTF_GetError() << std::endl;
		exit(EXIT_FAILURE);
	}
	SDL_Surface *surface = SDL_ConvertSurface(temp,  Screen::getInstance()->getSurface()->format, 0);
	if (surface == NULL) {
		std::cout << "Unable to convert text surface to display format: " << SDL_GetError() << std::endl;
		exit(EXIT_FAILURE);
	}
	SDL_FreeSurface(temp);
	return surface;
}

void Screen::clear() {
	SDL_Rect rect = {0, 0, screen_surface->w * scalingFactor, screen_surface->h * scalingFactor};
	SDL_FillRect(screen_surface, &rect, SDL_MapRGB(screen_surface->format, 0, 0, 0));
}

void Screen::clearOutsideClipRect() {
	SDL_Rect rect;
	if (clipRect.x > 0) {
		rect.x = 0;
		rect.y = 0;
		rect.w = clipRect.x;
		rect.h = screen_surface->h;
		SDL_FillRect(screen_surface, &rect, SDL_MapRGB(screen_surface->format, 0, 0, 0));
	}
	if (clipRect.x + clipRect.w*scalingFactor < screen_surface->w) {
		rect.x = clipRect.x + clipRect.w*scalingFactor;
		rect.y = 0;
		rect.w = screen_surface->w - rect.x;
		rect.h = screen_surface->h;
		SDL_FillRect(screen_surface, &rect, SDL_MapRGB(screen_surface->format, 0, 0, 0));
	}
	if (clipRect.y > 0) {
		rect.x = clipRect.x;
		rect.y = 0;
		rect.w = clipRect.w*scalingFactor;
		rect.h = clipRect.y;
		SDL_FillRect(screen_surface, &rect, SDL_MapRGB(screen_surface->format, 0, 0, 0));
	}
	if (clipRect.y + clipRect.h*scalingFactor < screen_surface->h) {
		rect.x = clipRect.x;
		rect.y = clipRect.y + clipRect.h*scalingFactor;
		rect.w = clipRect.w*scalingFactor;
		rect.h = screen_surface->h - rect.y;
		SDL_FillRect(screen_surface, &rect, SDL_MapRGB(screen_surface->format, 0, 0, 0));
	}
}

void Screen::fillRect(SDL_Rect *rect, Uint8 r, Uint8 g, Uint8 b) {
	if (0 == clipRect.x && 0 == clipRect.y && scalingFactor == 1) {
		SDL_FillRect(screen_surface, rect, SDL_MapRGB(screen_surface->format, r, g, b));
	} else {
		SDL_Rect rect_moved;
		rect_moved.x = rect->x * scalingFactor + clipRect.x;
		rect_moved.y = rect->y * scalingFactor + clipRect.y;
		rect_moved.w = rect->w * scalingFactor;
		rect_moved.h = rect->h * scalingFactor;
		SDL_FillRect(screen_surface, &rect_moved, SDL_MapRGB(screen_surface->format, r, g, b));
	}
}

TTF_Font *Screen::getSmallFont() {
	if (!smallFont)
		smallFont = loadFont("fonts/Cheapmot.TTF", 12);
	return smallFont;
}
TTF_Font *Screen::getFont() {
	if (!font)
		font = loadFont("fonts/Cheapmot.TTF", 20);
	return font;
}
TTF_Font *Screen::getLargeFont() {
	if (!largeFont)
		largeFont = loadFont("fonts/Cheapmot.TTF", 24);
	return largeFont;
}
TTF_Font *Screen::getVeryLargeFont() {
	if (!veryLargeFont)
		veryLargeFont = loadFont("fonts/Cheapmot.TTF", 48);
	return veryLargeFont;
}
TTF_Font *Screen::getHugeFont() {
	if (!hugeFont)
		hugeFont = loadFont("fonts/Cheapmot.TTF", 96);
	return hugeFont;
}

void Screen::computeClipRect() {
	bool scaling_allowed = !CommandLineOptions::exists("","noscaling");
	bool centering_allowed = !CommandLineOptions::exists("","nocentering");
	if (screen_surface->w == Constants::WINDOW_WIDTH || !centering_allowed) {
		clipRect.x = 0;
	} else {
		clipRect.x = (screen_surface->w - Constants::WINDOW_WIDTH) >> 1;
		if (clipRect.x < 0)
			clipRect.x = 0;
	}
	clipRect.w = Constants::WINDOW_WIDTH;
	if (screen_surface->h == Constants::WINDOW_HEIGHT || !centering_allowed) {
		clipRect.y = 0;
	} else {
		clipRect.y = (screen_surface->h - Constants::WINDOW_HEIGHT) >> 1;
		if (clipRect.y < 0)
			clipRect.y = 0;
	}
	clipRect.h = Constants::WINDOW_HEIGHT;
	if (scaling_allowed) {
		int scalingX = screen_surface->w / clipRect.w;
		int scalingY = screen_surface->h / clipRect.h;
		scalingFactor = scalingX < scalingY ? scalingX : scalingY;
		if (scalingFactor < 1) {
			scalingFactor = 1;
		}
		if (scalingFactor >= 2 && centering_allowed) {
			clipRect.x = (screen_surface->w - clipRect.w * scalingFactor) >> 1;
			clipRect.y = (screen_surface->h - clipRect.h * scalingFactor) >> 1;
		}
	}
}

int Screen::xToClipRect(int x) {
	return (x - Screen::getInstance()->getOffsetX()) / Screen::getInstance()->getScalingFactor();
}

int Screen::yToClipRect(int y) {
	return (y - Screen::getInstance()->getOffsetY()) / Screen::getInstance()->getScalingFactor();
}
