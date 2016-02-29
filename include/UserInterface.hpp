#pragma once

#include <vector>
#include <memory>
#include <set>

#include "typedefs.hpp"
#include "Animation.hpp"

// Forward declarations
union SDL_Event;
class Game;

class UserInterface {
public:
	UserInterface(Game& g, TeamID t);
	int handleInputEvents(); // returns 1 if should quit
	void handleInputEvent(const SDL_Event& event);
	void renderSelection( SDL_Renderer* renderer );
	void renderHUD( SDL_Renderer* renderer );
	void renderAll( SDL_Renderer* renderer );

	void tick();

	void zoom(int dy);

	Coordinate objectiveCoordinateFromScreen(const Coordinate c);
	Coordinate screenCoordinateFromObjective(const Coordinate c);

	void issueGotoCoordCmd(Coordinate targetCoord);

	void issueAttackCmd(UnitID targetID);

	void issueAttackMoveCmd(Coordinate targetCoord);

	Game& game;

	TeamID teamID;
	
	float viewMagnification;

	std::set<std::unique_ptr<Animation> > animations;

private:
	bool quit;
	
	bool drawSelectionBox;
	Coordinate selectionBoxCorner1;
	Coordinate selectionBoxCorner2;
	Coordinate viewCenter;
	std::vector<UnitID> selectedUnits;
	int cameraVx, cameraVy;
	int viewCenterMaxSpeed;

	std::shared_ptr<Spritesheet> uiWireframe;

	long frame;

	const Uint8 *keyDown;

	bool shiftHeld() {
		return this->keyDown[SDL_SCANCODE_LSHIFT] || this->keyDown[SDL_SCANCODE_RSHIFT];
	}
};

