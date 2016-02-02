#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Unit.hpp"

enum TeamColor{
	/*	These values are set to match with the colors in resources/graphics/team-colors.png
	*/
	COLOR_PINK=0,
	COLOR_RED=1,
	COLOR_BLUE=2,
	COLOR_TEAL=3,
	COLOR_PURPLE=4,
	COLOR_ORANGE=5,
	COLOR_BROWN=6,
	COLOR_WHITE=7,
	COLOR_YELLOW=8,
	COLOR_GREEN=9,
	COLOR_TAN=10,
	COLOR_PALE=11,
	COLOR_IRIS=12,
	COLOR_OLIVE=13,
	COLOR_METAL=14,
	COLOR_CYAN=15

};

SDL_Texture* loadTexture( SDL_Renderer* renderer, std::string path );

class Spritesheet{
	// Class for the actual image files that form spritesheets, and abstracting away all the pixel alignment messiness.
	public:
		Spritesheet(SDL_Texture* src, int w, int h, int sX, int sY, int offX = 0, int offY = 0, int gX = 0, int gY = 0);

		Spritesheet(SDL_Renderer* renderer, const char* src, int w, int h, int sX, int sY, int offX = 0, int offY = 0, int gX = 0, int gY = 0);

		void render(SDL_Renderer* renderer, int spriteX, int spriteY, int renderX, int renderY);
	private:
		SDL_Texture* sheet;
		SDL_Rect clip, tclip;
		int spriteW, spriteH; // the height and width of an individual sprite
		int offsetX, offsetY; // the gap between the top-left pixel of the image and the top-left pixel of the first sprite
		int gapX, gapY; // the gap between successive sprites
		int spritesX, spritesY; // the number of sprites wide and high
};
