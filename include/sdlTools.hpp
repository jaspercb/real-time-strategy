#pragma once

#include <SDL2/SDL.h>

#include "typedefs.hpp"
#include "enums.hpp"

extern float colorThetas[TeamColor::Enum::num];

namespace SDL_Colors {
	static const SDL_Color BLACK = {0, 0, 0, SDL_ALPHA_OPAQUE};
	static const SDL_Color LIGHTGREY = {63, 63, 63, SDL_ALPHA_OPAQUE};
	static const SDL_Color GREY = {127, 127, 127, SDL_ALPHA_OPAQUE};
	static const SDL_Color DARKGREY = {191, 191, 191, SDL_ALPHA_OPAQUE};
	static const SDL_Color WHITE = {255, 255, 255, SDL_ALPHA_OPAQUE};

	static const SDL_Color RED = {255, 0, 0, SDL_ALPHA_OPAQUE};
	static const SDL_Color GREEN = {0, 255, 0, SDL_ALPHA_OPAQUE};
	static const SDL_Color BLUE = {0, 0, 255, SDL_ALPHA_OPAQUE};
	
	static const SDL_Color YELLOW = {255, 204, 0, SDL_ALPHA_OPAQUE};
	static const SDL_Color ORANGE = {255, 104, 0, SDL_ALPHA_OPAQUE};
	static const SDL_Color PURPLE = {255, 0, 0, SDL_ALPHA_OPAQUE};
};

void renderRectBorder(SDL_Renderer* renderer, Coordinate a, Coordinate b, SDL_Color color);
void renderLine(SDL_Renderer* renderer, Coordinate a, Coordinate b, SDL_Color color);
void renderEllipse(SDL_Renderer* renderer, Coordinate center, int a, int b, SDL_Color color);
void renderCircle(SDL_Renderer* renderer, Coordinate center, int r, SDL_Color color);

SDL_Color Uint32toSDL_Color(Uint32 int_color); //Change from an "int color" to an SDL_Col

Uint32 SDL_ColortoUint32(SDL_Color color);

Uint8 GetPixel8(const SDL_Surface *surface, const int x, const int y);

void PutPixel8_nolock(SDL_Surface *surface, int x, int y, Uint8 color);

Uint8 GetPixel32(const SDL_Surface *surface, const int x, const int y);

void PutPixel32_nolock(SDL_Surface *surface, int x, int y, Uint32 color);

Matrix3 makeHueRotationMatrix(float radians);

SDL_Color _rotateColor(SDL_Color in, const Matrix3& matrix);

Uint32 _rotateColor(Uint32 int_color, Matrix3 matrix);

SDL_Color rotateColor(SDL_Color color, float radians);

Uint32 rotateColor(Uint32 int_color, float radians);

bool shouldColorKey(SDL_Color color);

bool shouldColorKey(Uint32 int_color);

void rotateColorOfSurface(SDL_Surface* surface, float radians);

void teamColorSpritesheet(SDL_Surface *surface, TeamColor::Enum color);
