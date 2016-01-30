#include <cassert>
#include "Spritesheet.hpp"

Spritesheet::Spritesheet(SDL_Surface* src, int w, int h, int offX, int offY, int gX, int gY):
	sheet(src),
	spriteW(w),
	spriteH(h),
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

Spritesheet::Spritesheet(const char* src, int w, int h, int offX, int offY, int gX, int gY):
	sheet(SDL_LoadBMP(src)),
	spriteW(w),
	spriteH(h),
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

void Spritesheet::render(SDL_Surface* screen, int spriteX, int spriteY, int renderX, int renderY){
	assert(spriteX>=0);
	assert(spriteY>=0);

	clip.x = offsetX + spriteX*(spriteW+gapX);
	clip.y = offsetY + spriteY*(spriteH+gapY);

	tclip.x = renderX;
	tclip.y = renderY;

	SDL_BlitSurface(sheet, &clip, screen, &tclip);
}