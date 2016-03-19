#pragma once

#include <SDL2/SDL.h>

class ResourceManager;
class FontManager;
class Game;

extern SDL_Window* gWindow;
extern SDL_Renderer* gRenderer;
extern int AIRBORNE_RENDER_HEIGHT;

extern const int PIXEL_WIDTH;
extern const int PIXEL_HEIGHT;

extern const int SCREEN_HEIGHT;
extern const int SCREEN_WIDTH;

extern ResourceManager* gResourceManager;
extern FontManager* gFontManager;
extern Game* game;
