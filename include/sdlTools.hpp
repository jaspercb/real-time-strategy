#pragma once

#include <SDL2/SDL.h>

#include "typedefs.hpp"

namespace SDL_Colors {
	static const SDL_Color BLACK = {0, 0, 0, SDL_ALPHA_OPAQUE};
	static const SDL_Color LIGHTGREY = {63, 63, 63, SDL_ALPHA_OPAQUE};
	static const SDL_Color GREY = {127, 127, 127, SDL_ALPHA_OPAQUE};
	static const SDL_Color DARKGREY = {191, 191, 191, SDL_ALPHA_OPAQUE};
	static const SDL_Color WHITE = {255, 255, 255, SDL_ALPHA_OPAQUE};

	static const SDL_Color RED = {255, 0, 0, SDL_ALPHA_OPAQUE};
	static const SDL_Color GREEN = {255, 0, 0, SDL_ALPHA_OPAQUE};
	static const SDL_Color BLUE = {255, 0, 0, SDL_ALPHA_OPAQUE};
	
	static const SDL_Color YELLOW = {255, 204, 0, SDL_ALPHA_OPAQUE};
	static const SDL_Color ORANGE = {255, 104, 0, SDL_ALPHA_OPAQUE};
	static const SDL_Color PURPLE = {255, 0, 0, SDL_ALPHA_OPAQUE};
};

void renderRectBorder(SDL_Renderer* renderer, Coordinate a, Coordinate b, SDL_Color color);
void renderLine(SDL_Renderer* renderer, Coordinate a, Coordinate b, SDL_Color color);
void renderEllipse(SDL_Renderer* renderer, Coordinate center, int a, int b, SDL_Color color);
void renderCircle(SDL_Renderer* renderer, Coordinate center, int r, SDL_Color color);
