#pragma once

#include <fstream>
#include <memory>

#include "typedefs.hpp"
#include "enums.hpp"

class Spritesheet;
class SDL_Renderer;
class Unit;
class UserInterface;

class Drawer {
	// Class that attaches to a Unit and manages all the data that is only relevant to drawing
	public:
		Drawer();
		Drawer(std::shared_ptr<Spritesheet> sp);
		Drawer(std::ifstream&, TeamColor::Enum teamColor);
		~Drawer();
		void draw(SDL_Renderer* renderer, Unit* unit, UserInterface* ui, int colorMulti = 255/*, Coordinate cameraposition */);
		
		void drawIdle(SDL_Renderer* renderer, int drawFacingAngle, UserInterface* ui, Coordinate drawPos, int frame, int dy);
		void drawWalking(SDL_Renderer* renderer, int drawFacingAngle, UserInterface* ui, Coordinate drawPos, int frame, int dy);
		void drawAttacking(SDL_Renderer* renderer, int drawFacingAngle, UserInterface* ui, Coordinate drawPos, int frame, int dy);
		void drawDying(SDL_Renderer* renderer, int drawFacingAngle, UserInterface* ui, Coordinate drawPos, int frame, int dy);

		void drawWireframe(SDL_Renderer* renderer, Coordinate drawPos);
		
		int idleCycleStart;
		int idleCycleLength;

		int walkCycleStart;
		int walkCycleLength;

		int attackCycleStart;
		int attackCycleLength;

		int deathCycleStart;
		int deathCycleLength;
		bool deathCycleVertical;

		int wireframeX;
		int wireframeY;

		int numFacingDirections;

	private:
		std::shared_ptr<Spritesheet> spritesheet, shadowsheet;

		int spriteXFromAngle(int drawFacingAngle);
};

void drawHPbar(SDL_Renderer* renderer, int HP, int maxHP, Coordinate renderLocation, const float magnification, int dy=0);
void drawHPbar(SDL_Renderer* renderer, const Unit* unit, UserInterface* ui);
