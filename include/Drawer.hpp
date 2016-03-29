#pragma once

#include <fstream>
#include <memory>

#include "Spritesheet.hpp"
#include "typedefs.hpp"

class Unit;
class UserInterface;

class Drawer {
	// Class that attaches to a Unit and manages all the data that is only relevant to drawing
	public:
		Drawer();
		Drawer(std::shared_ptr<Spritesheet> sp);
		Drawer(std::ifstream&, TeamColor teamColor);
		~Drawer();
		void draw(SDL_Renderer* renderer, Unit& unit, UserInterface* ui, int colorMulti = 255/*, Coordinate cameraposition */);
		
		void drawIdle(SDL_Renderer* renderer, int drawFacingAngle, UserInterface* ui, Coordinate drawPos, int frame, int dy);
		void drawWalking(SDL_Renderer* renderer, int drawFacingAngle, UserInterface* ui, Coordinate drawPos, int frame, int dy);
		void drawAttacking(SDL_Renderer* renderer, int drawFacingAngle, UserInterface* ui, Coordinate drawPos, int frame, int dy);
		void drawDying(SDL_Renderer* renderer, int drawFacingAngle, UserInterface* ui, Coordinate drawPos, int frame, int dy);

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
		std::shared_ptr<Spritesheet> spritesheet, shadowsheet;

		int spriteXFromAngle(int drawFacingAngle);
};

void drawHPbar(SDL_Renderer* renderer, int HP, int maxHP, const Coordinate renderLocation, const float magnification, int dy=0);
void drawHPbar(SDL_Renderer* renderer, const Unit& unit, UserInterface* ui);
