#pragma once

#include <SDL2/SDL.h>

class Spritesheet; // forward declarations
class Unit;

class Drawer{
	// Class that attaches to a Unit and manages all the data that is only relevant to drawing
	public:
		Drawer(Spritesheet* sp);
		void draw(SDL_Surface* screen, Unit& unit/*, Coordinate cameraposition */);

	private:
		Spritesheet* spritesheet;
};
