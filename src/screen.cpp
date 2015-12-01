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
	rect_num(0)
{
	// initialize SDL
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        printf("SDL initialization failed: %s\n", SDL_GetError());
        sdlInitErrorOccured = true;
    }
	if(!sdlInitErrorOccured && TTF_Init() == -1) {
		printf("TTF initialization failed: %s\n", TTF_GetError());
        sdlInitErrorOccured = true;
	}
	if (!sdlInitErrorOccured) {
		window = SDL_CreateWindow("Pacman",
								  SDL_WINDOWPOS_UNDEFINED,
                                  SDL_WINDOWPOS_UNDEFINED,
                           		  Constants::WINDOW_WIDTH,
                           		  Constants::WINDOW_HEIGHT,
                           		  fullscreen ? SDL_WINDOW_FULLSCREEN : 0);
		screen_surface = SDL_GetWindowSurface(window);
		if(screen_surface == 0) {
			printf("Setting video mode failed: %s\n",SDL_GetError());
			sdlInitErrorOccured = true;
		}
	}
	atexit(Screen::cleanUpInstance);
}

Screen::~Screen() {
	TTF_Quit();
	SDL_Quit();
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
	if (x + w > screen_surface->w)
		w = screen_surface->w - x;
	if (y + h > screen_surface->h)
		h = screen_surface->h - y;
	if (w <= 0 || h <= 0)
		return;
	rects[rect_num].x = (short int) x;
	rects[rect_num].y = (short int) y;
	rects[rect_num].w = (short int) w;
	rects[rect_num].h = (short int) h;
	rect_num++;
}

void Screen::addTotalUpdateRect() {
	rect_num = 0;  // all other update rects will be included in this one
	AddUpdateRects(0, 0, screen_surface->w, screen_surface->h);
}

void Screen::Refresh() {
	SDL_UpdateWindowSurfaceRects(window, rects, rect_num);
	rect_num = 0;
}

void Screen::draw_dynamic_content(SDL_Surface *surface, int x, int y) {
	SDL_Rect dest;
	dest.x = (short int)x;
	dest.y = (short int)y;
	dest.w = (short int)surface->w;
	dest.h = (short int)surface->h;
	SDL_BlitSurface(surface, NULL, this->screen_surface, &dest);
	this->AddUpdateRects(dest.x, dest.y, surface->w + 10, surface->h);
}

void Screen::draw(SDL_Surface* graphic, int offset_x, int offset_y) {
    if (0 == offset_x && 0 == offset_y) {
        SDL_BlitSurface(graphic, NULL, screen_surface, NULL);
    } else {
        SDL_Rect position;
        position.x = (short int)offset_x;
        position.y = (short int)offset_y;
		position.w = (short int)graphic->w;
		position.h = (short int)graphic->h;
        SDL_BlitSurface(graphic, NULL, screen_surface, &position);
    }
}

void Screen::setFullscreen(bool fs) {
    if (fs == fullscreen)
        return;  // the desired mode already has been activated, so do nothing
	SDL_SetWindowSize(window, Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT);
	SDL_SetWindowPosition(window, 0, 0);
    if(fs)
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
	else
		SDL_SetWindowFullscreen(window, 0);
	SDL_SetWindowSize(window, Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT);
	SDL_SetWindowPosition(window, 0, 0);
	SDL_Surface* newScreen = SDL_GetWindowSurface(window);
	if(newScreen) {
		screen_surface = newScreen;
		AddUpdateRects(0, 0, screen_surface->w, screen_surface->h);
		fullscreen = fs;
	} else {
		if (fs) {
			printf("Switching to fullscreen mode failed: %s\n",SDL_GetError());
		} else {
			printf("Switching from fullscreen mode failed: %s\n",SDL_GetError());
		}
	}
}

void Screen::drawHorizontalLine(int x1, int x2, int y, Uint8 r, Uint8 g, Uint8 b) {
	if (SDL_MUSTLOCK(this->screen_surface))
		SDL_LockSurface(this->screen_surface);
	Uint8* p;
	for (int i = x1; i <= x2; ++i) {
		p = (Uint8*) this->screen_surface->pixels + (y * this->screen_surface->pitch) + (i * sizeof(Uint8) * 3);
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		p[0] = r; p[1] = g; p[2] = b;
#else
		p[0] = b; p[1] = g; p[2] = r;
#endif
	}
	if (SDL_MUSTLOCK(this->screen_surface))
		SDL_UnlockSurface(this->screen_surface);
}

void Screen::drawVerticalLine(int x, int y1, int y2, Uint8 r, Uint8 g, Uint8 b) {
	if (SDL_MUSTLOCK(this->screen_surface))
		SDL_LockSurface(this->screen_surface);
	Uint8* p;
	for (int i = y1; i <= y2; ++i) {
		p = (Uint8*) this->screen_surface->pixels + (i * this->screen_surface->pitch) + (x * sizeof(Uint8) * 3);
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		p[0] = r; p[1] = g; p[2] = b;
#else
		p[0] = b; p[1] = g; p[2] = r;
#endif
	}
	if (SDL_MUSTLOCK(this->screen_surface))
		SDL_UnlockSurface(this->screen_surface);
}

// deprecated
SDL_Surface *Screen::LoadSurface(const char *filename, int transparent_color) {
	// deprecated
	std::cerr << "Screen::LoadSurface is deprecated! Please use Screen::loadImage instead and provide a relative Path there." << std::endl;
	SDL_Surface *surface, *temp;
	temp = IMG_Load(filename);
	if(!temp) {
		printf("Unable to load image: %s\n", IMG_GetError());
		exit(-1);
	}
	if(transparent_color != -1)
		SDL_SetColorKey(temp, SDL_TRUE | SDL_RLEACCEL, (Uint32)SDL_MapRGB(temp->format, (uint8_t)transparent_color, (uint8_t)transparent_color, (uint8_t)transparent_color));
	surface = SDL_ConvertSurface(temp, temp->format, 0);
	if(surface == NULL) {
		printf("Unable to convert image to display format: %s\n", SDL_GetError());
                exit(EXIT_FAILURE);
        }
    SDL_FreeSurface(temp);
    return surface;
}

SDL_Surface *Screen::loadImage(const char *filename, int transparentColor) {
	char filePath[256];
	getFilePath(filePath, filename);
	SDL_Surface *surface, *temp;
	temp = IMG_Load(filePath);
	if (!temp) {
		printf("Unable to load image: %s\n", IMG_GetError());
		exit(EXIT_FAILURE);
	}
	if (transparentColor != -1)
		SDL_SetColorKey(temp, SDL_TRUE | SDL_RLEACCEL, (Uint32)SDL_MapRGB(temp->format, (uint8_t)transparentColor, (uint8_t)transparentColor, (uint8_t)transparentColor));
	surface = SDL_ConvertSurface(temp,  Screen::getInstance()->getSurface()->format, 0);
	if (surface == NULL) {
		printf("Unable to convert image to display format: %s\n", SDL_GetError());
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
		printf("Unable to open TTF font: %s\n", TTF_GetError());
		exit(EXIT_FAILURE);
	}
	return font;
}

SDL_Surface *Screen::getTextSurface(TTF_Font *font, const char *text, SDL_Color color) {
	SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);
	if (!surface) {
		printf("Unable to render text \"%s\": %s\n", text, TTF_GetError());
		exit(EXIT_FAILURE);
	}
	return surface;
}

void Screen::clear() {
	SDL_Rect rect = {0, 0, screen_surface->w, screen_surface->h};
	fillRect(&rect, 0, 0, 0);
}

void Screen::fillRect(SDL_Rect *rect, Uint8 r, Uint8 g, Uint8 b) {
	SDL_FillRect(screen_surface, rect, SDL_MapRGB(screen_surface->format, r, g, b));
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
