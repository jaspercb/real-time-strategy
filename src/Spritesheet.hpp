#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Unit.hpp"

SDL_Texture* loadTexture( SDL_Renderer* renderer, std::string path );

class Spritesheet{
	// Class for the actual image files that form spritesheets, and abstracting away all the pixel alignment messiness.
	public:
		Spritesheet(SDL_Texture* src, int w, int h, int sX, int sY, int offX = 0, int offY = 0, int gX = 0, int gY = 0);

		Spritesheet(SDL_Renderer* renderer, const char* src, int w, int h, int sX, int sY, int offX = 0, int offY = 0, int gX = 0, int gY = 0);

		void render(SDL_Renderer* renderer, int spriteX, int spriteY, int renderX, int renderY);

		int walkCycleStart, attackCycleStart;
	private:
		SDL_Texture* sheet;
		SDL_Rect clip, tclip;
		int spriteW, spriteH; // the height and width of an individual sprite
		int offsetX, offsetY; // the gap between the top-left pixel of the image and the top-left pixel of the first sprite
		int gapX, gapY; // the gap between successive sprites
		int spritesX, spritesY; // the number of sprites wide and high
};
