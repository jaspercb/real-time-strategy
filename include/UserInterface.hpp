#pragma once

#include <vector>

#include "typedefs.hpp"

// Forward declarations
union SDL_Event;
class Game;

class UserInterface{
public:
	UserInterface(Game& g);
	void handleInputEvent(const SDL_Event& event);
	void renderSelection( SDL_Renderer* renderer );
	void renderHUD( SDL_Renderer* renderer );

	Coordinate objectiveCoordinateFromScreen(const Coordinate c);
	Coordinate screenCoordinateFromObjective(const Coordinate c);

	Game& game;

	bool quit;

private:
	bool drawSelectionBox;
	Coordinate selectionBoxCorner1;
	Coordinate selectionBoxCorner2;
	std::vector<UnitID> unitsInSelectionBox;
};

