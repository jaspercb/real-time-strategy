#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "enums.hpp"

class ResourceData;

SDL_Surface* loadSurface( std::string path );
SDL_Texture* loadTexture( SDL_Renderer* renderer, std::string path);
SDL_Texture* loadSpritesheet( SDL_Renderer* renderer, std::string path, TeamColor::Enum color);
SDL_Texture* loadShadowsheet( SDL_Renderer* renderer, std::string path );


void teamColorSpritesheet(SDL_Surface *surface, TeamColor::Enum color);

class Spritesheet {
	// Class for the actual image files that form spritesheets, and abstracting away all the pixel alignment messiness.
	public:
		Spritesheet(SDL_Texture* src, int w, int h, int sX, int sY, int offX = 0, int offY = 0, int gX = 0, int gY = 0);

		Spritesheet(SDL_Renderer* renderer, const ResourceData&, TeamColor::Enum teamColor=TeamColor::Enum::Orange);

		~Spritesheet();

		void render(SDL_Renderer* renderer, int spriteX, int spriteY, int renderX, int renderY, float magnification);

		void setColorMod(Uint8 r, Uint8 g, Uint8 b);
		void resetColorMod();
		void setAlphaMod(Uint8 a);
		void resetAlphaMod();

		SDL_Texture* sheet;

		const int spriteW, spriteH; // the height and width of an individual sprite
		const int spritesX, spritesY; // the number of sprites wide and high
		const int offsetX, offsetY; // the gap between the top-left pixel of the image and the top-left pixel of the first sprite
		const int gapX, gapY; // the gap between successive sprites

	private:
		SDL_Rect clip, tclip;
};
