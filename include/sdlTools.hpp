#include <SDL2/SDL.h>

#include "typedefs.hpp"

void renderRectBorder(SDL_Renderer* renderer, Coordinate a, Coordinate b, SDL_Color color);
void renderLine(SDL_Renderer* renderer, Coordinate a, Coordinate b, SDL_Color color);
void renderEllipse(SDL_Renderer* renderer, Coordinate center, int rx, int ry SDL_Color color);
