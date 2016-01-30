#pragma once

#include <SDL2/SDL.h>
#include "Unit.hpp"

class Spritesheet{
	// Class for the actual image files that form spritesheets, and abstracting away all the pixel alignment messiness.
	public:
		Spritesheet(SDL_Surface* src, int w, int h, int offX = 0, int offY = 0, int gX = 0, int gY = 0);

		Spritesheet(const char* src, int w, int h, int offX = 0, int offY = 0, int gX = 0, int gY = 0);

		void render(SDL_Surface* screen, int spriteX, int spriteY, int renderX, int renderY);

	private:
		SDL_Surface* sheet;
		SDL_Rect clip, tclip;
		int spriteW, spriteH; // the height and width of an individual sprite
		int offsetX, offsetY; // the gap between the top-left pixel of the image and the top-left pixel of the first sprite
		int gapX, gapY; // the gap between successive sprites
};
