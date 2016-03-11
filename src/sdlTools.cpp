#include "sdlTools.hpp"
#include "Logging.hpp"
#include "Spritesheet.hpp"
#include <cmath>

float colorThetas[NUM_TEAMCOLORS] = {
					0, //null
					0, //pink
					0.55, //red
					5, //blue
					2.5, //green
					5.6, //purple
					1, //orange
					1.3, //brown
					1.52, //yellow
					4.2, //teal
					4.0, //pale
					5.3, //iris
					2, //olive
				};

void SetRenderDrawColor(SDL_Renderer* renderer, SDL_Color color) {
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

void renderRectBorder(SDL_Renderer* renderer, Coordinate a, Coordinate b, SDL_Color color) {
	SDL_Rect drawRect;
	
	drawRect.x = a.x;
	drawRect.y = a.y;
	drawRect.w = b.x-a.x;
	drawRect.h = b.y-a.y;

	SetRenderDrawColor(renderer, color);

	SDL_RenderDrawRect(renderer, &drawRect);
}

void renderLine(SDL_Renderer* renderer, Coordinate a, Coordinate b, SDL_Color color) {
	SDL_Rect drawRect;
	
	SetRenderDrawColor(renderer, color);

	SDL_RenderDrawLine(renderer, a.x, a.y, b.x, b.y);
}



void ellipsePlotPoints (SDL_Renderer* renderer, int xc, int yc, int x, int y) {
	SDL_RenderDrawPoint(renderer, xc + x, yc + y);
	SDL_RenderDrawPoint(renderer, xc - x, yc + y);
	SDL_RenderDrawPoint(renderer, xc + x, yc - y);
	SDL_RenderDrawPoint(renderer, xc - x, yc - y);
}

void renderEllipse(SDL_Renderer* renderer, Coordinate center, int a, int b, SDL_Color color) {
	SetRenderDrawColor(renderer, color);

	int xc = center.x;
	int yc = center.y;
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

SDL_Color Uint32toSDL_Color(Uint32 int_color) {
	//Change from an "int color" to an SDL_Color
	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		SDL_Color color = {
			(Uint8)((int_color & 0x00ff0000)/0x10000),
			(Uint8)((int_color & 0x0000ff00)/0x100),
			(Uint8)((int_color & 0x000000ff)),
			(Uint8)((int_color & 0xff000000)/0x1000000)};
	#else
		SDL_Color color = {
			(Uint8)((int_color & 0x000000ff)),
			(Uint8)((int_color & 0x0000ff00)/0x100),
			(Uint8)((int_color & 0x00ff0000)/0x10000),
			(Uint8)((int_color & 0xff000000)/0x1000000)};
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
