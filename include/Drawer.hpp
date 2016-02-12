#pragma once

#include <fstream>
#include "Spritesheet.hpp"

class Unit;
class UserInterface;

class Drawer{
	// Class that attaches to a Unit and manages all the data that is only relevant to drawing
	public:
		Drawer(Spritesheet* sp);
		Drawer(std::ifstream&, TeamColor teamColor);
		~Drawer();
		void draw(SDL_Renderer* renderer, Unit& unit, UserInterface* ui/*, Coordinate cameraposition */);

		int idleCycleStart;
		int idleCycleLength;

		int walkCycleStart;
		int walkCycleLength;

		int attackCycleStart;
		int attackCycleLength;

		int deathCycleStart;
		int deathCycleLength;
		bool deathCycleVertical;

		int numFacingDirections;

	private:
		Spritesheet *spritesheet, *shadowsheet;
};

void draw_HP_bar(SDL_Renderer* renderer, Unit& unit, const int renderX, const int renderY);
