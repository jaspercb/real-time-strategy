#pragma once

#include <fstream>

class Spritesheet; // forward declarations
class Unit;

class Drawer{
	// Class that attaches to a Unit and manages all the data that is only relevant to drawing
	public:
		Drawer(Spritesheet* sp);
		Drawer(std::ifstream&);
		void draw(SDL_Renderer* renderer, Unit& unit/*, Coordinate cameraposition */);

		int walkCycleStart;
		int walkCycleLength;

		int attackCycleStart;
		int attackCycleLength;

		int deathCycleStart;
		int deathCycleLength;
		int numFacingDirections;

	private:
		Spritesheet* spritesheet, *shadowsheet;
};

void draw_HP_bar(SDL_Renderer* renderer, Unit& unit, const int renderX, const int renderY);
