#include "sdlTools.hpp"

void renderRectBorder(SDL_Renderer* renderer, Coordinate a, Coordinate b, SDL_Color color) {
	SDL_Rect drawRect;
	
	drawRect.x = a.first;
	drawRect.y = a.second;
	drawRect.w = b.first-a.first;
	drawRect.h = b.second-a.second;

	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

	SDL_RenderDrawRect(renderer, &drawRect);
}

void renderLine(SDL_Renderer* renderer, Coordinate a, Coordinate b, SDL_Color color) {
	SDL_Rect drawRect;
	
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

	SDL_RenderDrawLine(renderer, a.first, a.second, b.first, b.second);
}

void renderEllipse(SDL_Renderer* renderer, Coordinate center, int rx, int ry SDL_Color color){
	
}
