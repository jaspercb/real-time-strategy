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
	void render( SDL_Renderer* renderer );

	Game& game;

	bool quit;

private:
	bool drawSelectionBox;
	Coordinate selectionBoxCorner1;
	Coordinate selectionBoxCorner2;
	std::vector<UnitID> unitsInSelectionBox;
};

