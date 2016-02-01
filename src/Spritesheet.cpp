#include <cassert>
#include <string>

#include "Logging.hpp"
#include "Spritesheet.hpp"

SDL_Texture* loadTexture( SDL_Renderer* renderer, std::string path ) {
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface( renderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	return newTexture;
}

Spritesheet::Spritesheet(SDL_Texture* src, int w, int h, int sX, int sY, int offX, int offY, int gX, int gY):
	sheet(src),
	spriteW(w),
	spriteH(h),
	spritesX(sX),
	spritesY(sY),
	offsetX(offX),
	offsetY(offY),
	gapX(gX),
	gapY(gY),
	clip(SDL_Rect()),
	tclip(SDL_Rect())
	{
		clip.w = spriteW;
		clip.h = spriteH;
		tclip.w = spriteW;
		tclip.h = spriteH;
	}

Spritesheet::Spritesheet(SDL_Renderer* renderer, const char* src, int w, int h, int sX, int sY, int offX, int offY, int gX, int gY):
	sheet(loadTexture(renderer, (std::string)src)),
	spriteW(w),
	spriteH(h),
	spritesX(sX),
	spritesY(sY),
	offsetX(offX),
	offsetY(offY),
	gapX(gX),
	gapY(gY),
	clip(SDL_Rect()),
	tclip(SDL_Rect())
	{
		clip.w = spriteW;
		clip.h = spriteH;
		tclip.w = spriteW;
		tclip.h = spriteH;
	}

void Spritesheet::render(SDL_Renderer* renderer, int spriteX, int spriteY, int renderX, int renderY){
	assert(spriteX>=0);
	assert(spriteY>=0);

	tclip.x = renderX;
	tclip.y = renderY;

	clip.y = offsetY + spriteY*(spriteH+gapY);

	if (spriteX>=2*spritesX){
		debugLog(spriteX);
		return;
	}
	if (spriteX<spritesX){
		clip.x = offsetX + spriteX*(spriteW+gapX);
		SDL_RenderCopyEx( renderer, sheet, &clip, &tclip, 0 /*angle*/, NULL /*center*/, SDL_FLIP_NONE /*flip parameter*/ );
	} else {
		clip.x = offsetX + (spritesX-(spriteX%spritesX))*(spriteW+gapX);
		SDL_RenderCopyEx( renderer, sheet, &clip, &tclip, 0 /*angle*/, NULL /*center*/, SDL_FLIP_HORIZONTAL /*flip parameter*/ );
	}
}
