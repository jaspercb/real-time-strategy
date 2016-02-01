#pragma once

class SDL_Renderer;

#include <fstream>

class Spritesheet; // forward declarations
class Unit;

class Drawer{
	// Class that attaches to a Unit and manages all the data that is only relevant to drawing
	public:
		Drawer(Spritesheet* sp);
		Drawer(std::ifstream&, SDL_Renderer*);
		void draw(SDL_Renderer* renderer, Unit& unit/*, Coordinate cameraposition */);

		int walkCycleStart;
		int walkCycleLength;
		int attackCycleLength;
		int attackCycleStart;
		int numFacingDirections;

	private:
		Spritesheet* spritesheet;
};
