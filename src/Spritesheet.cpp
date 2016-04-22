#include <cassert>
#include <cmath>
#include <string>

#include "Logging.hpp"
#include "Spritesheet.hpp"
#include "ResourceData.hpp"

SDL_Surface* loadSurface( std::string path) {
	
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	
	if( loadedSurface == NULL )
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	
	//debugLog("loading: "+path+". Bits per pixel:");
	//debugLog(loadedSurface->format->BytesPerPixel);
	//debugLog(loadedSurface->format->format);

	return loadedSurface;
}

SDL_Texture* loadTexture( SDL_Renderer* renderer, std::string path) {
	// Loads a spritesheet. Makes no palette swaps.
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = loadSurface(path);

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

SDL_Texture* loadSpritesheet( SDL_Renderer* renderer, std::string path, TeamColor::Enum color) {
	// Loads a spritesheet. Makes palette swaps with respect to TeamColor.
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

SDL_Texture* loadShadowsheet( SDL_Renderer* renderer, std::string path ) {
	// Loads a spritesheet. Makes palette swaps with respect to TeamColor.
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = loadSurface(path);
	
	SDL_Color color;
	color.r = 0;
	color.g = 0;
	color.b = 0;
	color.a = 128;

	for (int i=1; i<255; i++)
		SDL_SetPaletteColors(loadedSurface->format->palette, &color, i, 1);

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

Spritesheet::Spritesheet(SDL_Renderer *renderer, const ResourceData& resourceData, TeamColor::Enum teamColor):
	spriteW(resourceData.spriteW),
	spriteH(resourceData.spriteH),
	spritesX(resourceData.spritesX),
	spritesY(resourceData.spritesY),
	offsetX(resourceData.offsetX),
	offsetY(resourceData.offsetY),
	gapX(resourceData.gapX),
	gapY(resourceData.gapY)
	{
		if (resourceData.isShadow) {
			this->sheet = loadShadowsheet(renderer, resourceData.filepath);
		}
		else if (resourceData.shouldApplyTeamColor) {
			this->sheet = loadSpritesheet(renderer, resourceData.filepath, teamColor);
		}
		else{
			this->sheet = loadTexture(renderer, resourceData.filepath);
		}
		this->clip.w = resourceData.spriteW;
		this->clip.h = resourceData.spriteH;
		this->tclip.w = resourceData.spriteW;
		this->tclip.h = resourceData.spriteH;
	}

Spritesheet::~Spritesheet() {
	SDL_DestroyTexture(this->sheet);
}


void Spritesheet::render(SDL_Renderer *renderer, int spriteX, int spriteY, int renderX, int renderY, float magnification) {
	//draws the spritesheet CENTERED at (renderX, renderY)
	assert(spriteX>=0);
	assert(spriteY>=0);

	this->tclip.x = renderX - magnification*this->spriteW/2 -1;
	this->tclip.y = renderY - magnification*this->spriteH/2 -1;

	this->tclip.w = std::ceil(magnification*(this->clip.w))+2;
	this->tclip.h = std::ceil(magnification*(this->clip.h))+2;

	this->clip.y = offsetY + spriteY*(spriteH+gapY);

	if (spriteX>=2*spritesX) {
		debugLog(spriteX);
		return;
	}
	if (spriteX<spritesX) {
		this->clip.x = offsetX + spriteX*(spriteW+gapX);
		SDL_RenderCopy( renderer, sheet, &clip, &tclip );
	} else {
		this->clip.x = offsetX + (spritesX-1-(spriteX%spritesX))*(spriteW+gapX);
		SDL_RenderCopyEx( renderer, sheet, &this->clip, &this->tclip, 0 /*angle*/, NULL /*center*/, SDL_FLIP_HORIZONTAL /*flip parameter*/ );
	}
}

void Spritesheet::setColorMod(Uint8 r, Uint8 g, Uint8 b) {
	SDL_SetTextureColorMod(this->sheet, r, g, b);
}

void Spritesheet::resetColorMod() {
	this->setColorMod(255, 255, 255);
}

void Spritesheet::setAlphaMod(Uint8 a) {
	SDL_SetTextureAlphaMod(this->sheet, a);
}

void Spritesheet::resetAlphaMod() {
	this->setAlphaMod(255);
}
