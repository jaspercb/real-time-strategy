#pragma once

#include <vector>

#include "typedefs.hpp"

// Forward declarations
union SDL_Event;
class Game;

class UserInterface {
public:
	UserInterface(Game& g, TeamID t);
	void handleInputEvent(const SDL_Event& event);
	void renderSelection( SDL_Renderer* renderer );
	void renderHUD( SDL_Renderer* renderer );
	void renderAll( SDL_Renderer* renderer );

	void tick();

	void zoom(int dy);

	Coordinate objectiveCoordinateFromScreen(const Coordinate c);
	Coordinate screenCoordinateFromObjective(const Coordinate c);

	Game& game;

	bool quit;

	TeamID teamID;
	
	float viewMagnification;

private:
	bool drawSelectionBox;
	Coordinate selectionBoxCorner1;
	Coordinate selectionBoxCorner2;
	Coordinate viewCenter;
	std::vector<UnitID> unitsInSelectionBox;
	int cameraVx, cameraVy;
	int viewCenterMaxSpeed;
};

