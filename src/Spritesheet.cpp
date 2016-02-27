#include <cassert>
#include <cmath>
#include <string>

#include "typedefs.hpp"
#include "Logging.hpp"
#include "Spritesheet.hpp"

float colorThetas[NUM_TEAMCOLORS] = {
					0, //null
					0, //pink
					0.55, //red
					5, //blue
					4.2, //teal
					5.6, //purple
					1, //orange
					1.3, //brown
					1.52, //yellow
					2.5, //green
					4.0, //pale
					5.3, //iris
					2, //olive
				};

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

SDL_Texture* loadSpritesheet( SDL_Renderer* renderer, std::string path, TeamColor color) {
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

SDL_Color Uint32toSDL_Color(Uint32 int_color) {
	//Change from an "int color" to an SDL_Color
	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		SDL_Color color={(int_color & 0x00ff0000)/0x10000,(int_color &
		0x0000ff00)/0x100,(int_color & 0x000000ff), (int_color & 0xff000000) / 0x1000000};
	#else
		SDL_Color color={(int_color & 0x000000ff),(int_color &
		0x0000ff00)/0x100,(int_color & 0x00ff0000)/0x10000, (int_color & 0xff000000) / 0x1000000};
	#endif
	return color;
}

Uint32 SDL_ColortoUint32(SDL_Color color) {
	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		Uint32 int_color = color.a*0x1000000 | color.r*0x10000 | color.g*0x100 | color.b;
	#else
		Uint32 int_color = color.a*0x1000000 | color.r | color.g*0x100 | color.b*0x10000;
	#endif
	return int_color;
}

Uint8 GetPixel8(const SDL_Surface *surface, const int x, const int y) {
	Uint8 * pixel = (Uint8*)surface->pixels;
	pixel += (y * surface->pitch) + x;
	return *pixel;
}

void PutPixel8_nolock(SDL_Surface *surface, int x, int y, Uint8 color) {
	Uint8 * pixel = (Uint8*)surface->pixels;
	pixel += (y * surface->pitch) + x;
	*pixel = color;
}

Uint8 GetPixel32(const SDL_Surface *surface, const int x, const int y) {
	Uint32 * pixel = (Uint32*)surface->pixels;
	pixel += (y * surface->pitch) + x;
	return *pixel;
}

void PutPixel32_nolock(SDL_Surface *surface, int x, int y, Uint32 color) {
	Uint32 * pixel = (Uint32*)surface->pixels;
	pixel += (y * surface->pitch) + x;
	*pixel = color;
}

Matrix3 makeHueRotationMatrix(float radians) {
	Matrix3 matrix;
	float cosA = cos(radians);
	float sinA = sin(radians);
	matrix[0][0] = cosA + (1.0 - cosA) / 3.0;
	matrix[0][1] = 1./3. * (1.0 - cosA) - sqrt(1./3.) * sinA;
	matrix[0][2] = 1./3. * (1.0 - cosA) + sqrt(1./3.) * sinA;
	matrix[1][0] = 1./3. * (1.0 - cosA) + sqrt(1./3.) * sinA;
	matrix[1][1] = cosA + 1./3.*(1.0 - cosA);
	matrix[1][2] = 1./3. * (1.0 - cosA) - sqrt(1./3.) * sinA;
	matrix[2][0] = 1./3. * (1.0 - cosA) - sqrt(1./3.) * sinA;
	matrix[2][1] = 1./3. * (1.0 - cosA) + sqrt(1./3.) * sinA;
	matrix[2][2] = cosA + 1./3. * (1.0 - cosA);

	return matrix;
}

SDL_Color _rotateColor(SDL_Color in, const Matrix3& matrix) {
	SDL_Color ret;

	ret.r = in.r * matrix[0][0] + in.g * matrix[0][1] + in.b * matrix[0][2];
    ret.g = in.r * matrix[1][0] + in.g * matrix[1][1] + in.b * matrix[1][2];
    ret.b = in.r * matrix[2][0] + in.g * matrix[2][1] + in.b * matrix[2][2];
	ret.a = in.a;

	return ret;
}

Uint32 _rotateColor(Uint32 int_color, Matrix3 matrix) {
	return SDL_ColortoUint32(_rotateColor( Uint32toSDL_Color(int_color), matrix ));
}

SDL_Color rotateColor(SDL_Color color, float radians) {
	return _rotateColor(color, makeHueRotationMatrix(radians) );
}

Uint32 rotateColor(Uint32 int_color, float radians) {
	return SDL_ColortoUint32(_rotateColor(Uint32toSDL_Color(int_color), makeHueRotationMatrix(radians) ) );
}

bool shouldColorKey(SDL_Color color) { // literally "is this more red than it is blue and green?"
	return color.r >= color.g && color.r >= color.b;
	// red is now off-limits as a color btw
}

bool shouldColorKey(Uint32 int_color) {
	return shouldColorKey(Uint32toSDL_Color(int_color));
}

void rotateColorOfSurface(SDL_Surface* surface, float radians) {
	Matrix3 matrix = makeHueRotationMatrix(radians);

	if (NULL == surface->format->palette) {
		int npixels = surface->w * surface->h;
		
		if (surface->format->BytesPerPixel == 4) {
			//debugLog("4 byte");
			Uint32* pixel = (Uint32*)surface->pixels;
			for (int i = 0; i < npixels; i++) {
				if (shouldColorKey(*pixel))
					*pixel = _rotateColor(*pixel, matrix);
					pixel++;
			}
		}
		//else if (surface->format->BytesPerPixel == 1) {
		//	debugLog("1 byte");
		//}
		else {
			debugLog("Weird bytes per pixel in call to rotateColorOfSurface:");
			debugLog(surface->format->BytesPerPixel);
			throw;
		}

	}
	
	else {
		SDL_Color* colors = new SDL_Color[surface->format->palette->ncolors];
		
		for (int i=0; i<surface->format->palette->ncolors; i++) {
			SDL_Color color = surface->format->palette->colors[i];
			if (shouldColorKey(color)) {
				colors[i] = rotateColor(surface->format->palette->colors[i], 1);
			} else{
				colors[i] = surface->format->palette->colors[i];
			}
		}
		SDL_SetPaletteColors(surface->format->palette, colors, 0, surface->format->palette->ncolors);
		delete colors;
	}
}

void teamColorSpritesheet(SDL_Surface *surface, TeamColor color) {
	// Changes the color palette, replacing the magenta color key with the specified color. See /resources/graphics/team-colors.png

	/*SDL_Surface* colormapping = loadSurface("../resources/graphics/team-colors.png");

	SDL_Color colors[8];

	for (int i=0; i<8; i++) {
		colors[i]=colormapping->format->palette->colors[GetPixel8(colormapping, i, (int)color)];
	}

	SDL_SetPaletteColors(surface->format->palette, colors, 8, 8);

	SDL_FreeSurface(colormapping);

	if( SDL_MUSTLOCK(surface) )
		SDL_UnlockSurface(surface);
	*/
	
	/*SDL_Color* colors = new SDL_Color[surface->format->palette->ncolors];
	for (int i=0; i<surface->format->palette->ncolors; i++) {
		colors[i] = rotateColor(surface->format->palette->colors[i], 0);
	}
	*/

	rotateColorOfSurface(surface, colorThetas[color]);

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

Spritesheet::Spritesheet(SDL_Renderer *renderer, ResourceData resourceData, TeamColor teamColor):
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

	this->tclip.x = renderX - magnification*this->spriteW/2;
	this->tclip.y = renderY - magnification*this->spriteH/2;

	this->tclip.w = magnification*this->clip.w;
	this->tclip.h = magnification*this->clip.h;

	this->clip.x = offsetX + spriteX*(spriteW+gapX);
	this->clip.y = offsetY + spriteY*(spriteH+gapY);

	if (spriteX>=2*spritesX) {
		debugLog(spriteX);
		return;
	}
	if (spriteX<spritesX) {
		this->clip.x = offsetX + spriteX*(spriteW+gapX);
		SDL_RenderCopyEx( renderer, sheet, &clip, &tclip, 0 /*angle*/, NULL /*center*/, SDL_FLIP_NONE /*flip parameter*/ );
	} else {
		this->clip.x = offsetX + (spritesX-1-(spriteX%spritesX))*(spriteW+gapX);
		SDL_RenderCopyEx( renderer, sheet, &this->clip, &this->tclip, 0 /*angle*/, NULL /*center*/, SDL_FLIP_HORIZONTAL /*flip parameter*/ );
	}
}
