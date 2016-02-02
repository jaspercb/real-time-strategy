#include <cassert>
#include <string>

#include "Logging.hpp"
#include "Spritesheet.hpp"

SDL_Surface* loadSurface( std::string path) {
	
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	
	if( loadedSurface == NULL )
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	
	//debugLog("loading: "+path+". Bits per pixel:");
	//debugLog(loadedSurface->format->BytesPerPixel);
	//debugLog(loadedSurface->format->format);

	return loadedSurface;
}

SDL_Texture* loadTexture( SDL_Renderer* renderer, std::string path, TeamColor color) {
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = loadSurface(path);
	
	teamColorSpritesheet(loadedSurface, color);

	//Create texture from surface pixels
	newTexture = SDL_CreateTextureFromSurface( renderer, loadedSurface );
	if( newTexture == NULL )
	{
		printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
	}
	//Get rid of old loaded surface
	SDL_FreeSurface( loadedSurface );

	return newTexture;
}

Uint32 GetPixel8(const SDL_Surface *surface, const int x, const int y){
	Uint8 * pixel = (Uint8*)surface->pixels;
	pixel += (y * surface->pitch) + x;
	return *pixel;
}

void PutPixel8_nolock(SDL_Surface *surface, int x, int y, Uint8 color)
{
	Uint8 * pixel = (Uint8*)surface->pixels;
	pixel += (y * surface->pitch) + x;
	*pixel = color;
}

void teamColorSpritesheet(SDL_Surface *surface, TeamColor color){
	// Changes the color palette, replacing the magenta color key with the specified color. See /resources/graphics/team-colors.png

	SDL_Surface* colormapping = loadSurface("../resources/graphics/team-colors.png");

	SDL_Color colors[8];

	for (int i=0; i<8; i++){
		colors[i]=colormapping->format->palette->colors[GetPixel8(colormapping, i, (int)color)];
	}

	SDL_SetPaletteColors(surface->format->palette, colors, 8, 8);

	SDL_FreeSurface(colormapping);

	if( SDL_MUSTLOCK(surface) )
		SDL_UnlockSurface(surface);
}


Spritesheet::Spritesheet(SDL_Texture *src, int w, int h, int sX, int sY, int offX, int offY, int gX, int gY):
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

Spritesheet::Spritesheet(SDL_Renderer *renderer, const char* src, int w, int h, int sX, int sY, int offX, int offY, int gX, int gY):
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

void Spritesheet::render(SDL_Renderer *renderer, int spriteX, int spriteY, int renderX, int renderY){
	//draws the spritesheet CENTERED at (renderX, renderY)
	assert(spriteX>=0);
	assert(spriteY>=0);

	tclip.x = renderX-spriteW/2;
	tclip.y = renderY-spriteH/2;

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
