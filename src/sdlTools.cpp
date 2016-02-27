#include "sdlTools.hpp"
#include <cmath>

void SetRenderDrawColor(SDL_Renderer* renderer, SDL_Color color) {
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

void renderRectBorder(SDL_Renderer* renderer, Coordinate a, Coordinate b, SDL_Color color) {
	SDL_Rect drawRect;
	
	drawRect.x = a.first;
	drawRect.y = a.second;
	drawRect.w = b.first-a.first;
	drawRect.h = b.second-a.second;

	SetRenderDrawColor(renderer, color);

	SDL_RenderDrawRect(renderer, &drawRect);
}

void renderLine(SDL_Renderer* renderer, Coordinate a, Coordinate b, SDL_Color color) {
	SDL_Rect drawRect;
	
	SetRenderDrawColor(renderer, color);

	SDL_RenderDrawLine(renderer, a.first, a.second, b.first, b.second);
}



void ellipsePlotPoints (SDL_Renderer* renderer, int xc, int yc, int x, int y) {
	SDL_RenderDrawPoint(renderer, xc + x, yc + y);
	SDL_RenderDrawPoint(renderer, xc - x, yc + y);
	SDL_RenderDrawPoint(renderer, xc + x, yc - y);
	SDL_RenderDrawPoint(renderer, xc - x, yc - y);
}

void renderEllipse(SDL_Renderer* renderer, Coordinate center, int a, int b, SDL_Color color) {
	SetRenderDrawColor(renderer, color);

	int xc = center.first;
	int yc = center.second;
	int a2 = a * a;
	int b2 = b * b;
	int twoa2 = 2 * a2;
	int twob2 = 2 * b2;
	int p;
	int x = 0;
	int y = b;
	int px = 0;
	int py = twoa2 * y;

	/* Plot the initial point in each quadrant. */
	ellipsePlotPoints(renderer, xc, yc, x, y);

	/* Region 1 */
	p = std::round (b2 - (a2 * b) + (0.25 * a2));
	while (px < py) {
		x++;
		px += twob2;
		if (p < 0)
		p += b2 + px;
		else {
		y--;
		py -= twoa2;
		p += b2 + px - py;
		}
		ellipsePlotPoints(renderer, xc, yc, x, y);
	}

	/* Region 2 */
	p = std::round (b2 * (x+0.5) * (x+0.5) + a2 * (y-1) * (y-1) - a2 * b2);
	
	while (y > 0) {
		y--;
		py -= twoa2;
		if (p > 0)
		p += a2 - py;
		else {
		x++;
		px += twob2;
		p += a2 - py + px;
		}
		ellipsePlotPoints(renderer, xc, yc, x, y);
	}
}

void renderCircle(SDL_Renderer* renderer, Coordinate center, int r, SDL_Color color) {
	renderEllipse(renderer, center, r, r, color);
}