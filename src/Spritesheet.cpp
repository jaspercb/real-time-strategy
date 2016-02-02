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

SDL_Texture* loadSpritesheet( SDL_Renderer* renderer, std::string path, TeamColor color){
	// Loads a spritesheet. Makes palette swaps with respect to TeamColor.
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = loadSurface(path);
	
	color = COLOR_PINK;
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

SDL_Texture* loadShadowsheet( SDL_Renderer* renderer, std::string path ){
	// Loads a spritesheet. Makes palette swaps with respect to TeamColor.
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = loadSurface(path);
	
	SDL_Color color;
	color.r = 0;
	color.g = 0;
	color.b = 0;
	color.a = 128;
	
	SDL_SetPaletteColors(loadedSurface->format->palette, &color, 1, 1); // the first 1 is hardcoded and assumes the shadow color palette will bind 1 to the shadow color.

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
	clip(),
	tclip()
	{
		this->clip.w = w;
		this->clip.h = h;
		this->tclip.w = w;
		this->tclip.h = h;
	}

Spritesheet::Spritesheet(SDL_Renderer *renderer, const char* src, int w, int h, int sX, int sY, int offX, int offY, int gX, int gY, bool shadow):
	spriteW(w),
	spriteH(h),
	spritesX(sX),
	spritesY(sY),
	offsetX(offX),
	offsetY(offY),
	gapX(gX),
	gapY(gY),
	clip(),
	tclip()
	{
		if (shadow){
			this->sheet = loadShadowsheet(renderer, (std::string)src);
		}
		else{
			this->sheet = loadSpritesheet(renderer, (std::string)src);
		}
		this->clip.w = w;
		this->clip.h = h;
		this->tclip.w = w;
		this->tclip.h = h;
	}

void Spritesheet::render(SDL_Renderer *renderer, int spriteX, int spriteY, int renderX, int renderY){
	//draws the spritesheet CENTERED at (renderX, renderY)
	assert(spriteX>=0);
	assert(spriteY>=0);

	this->tclip.x = renderX-this->spriteW/2;
	this->tclip.y = renderY-spriteH/2;

	this->clip.y = offsetY + spriteY*(spriteH+gapY);

	if (spriteX>=2*spritesX){
		debugLog(spriteX);
		return;
	}
	if (spriteX<spritesX){
		this->clip.x = offsetX + spriteX*(spriteW+gapX);
		SDL_RenderCopyEx( renderer, sheet, &clip, &tclip, 0 /*angle*/, NULL /*center*/, SDL_FLIP_NONE /*flip parameter*/ );
	} else {
		this->clip.x = offsetX + (spritesX-(spriteX%spritesX))*(spriteW+gapX);
		SDL_RenderCopyEx( renderer, sheet, &this->clip, &this->tclip, 0 /*angle*/, NULL /*center*/, SDL_FLIP_HORIZONTAL /*flip parameter*/ );
	}
}
