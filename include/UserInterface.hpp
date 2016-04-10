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
	UserInterface(Game* g, TeamID t);
	int handleInputEvents(); // returns 1 if should quit
	void handleInputEvent(const SDL_Event& event);
	void updateSelectedUnits();
	void renderSelection( SDL_Renderer* renderer );
	void renderHUD( SDL_Renderer* renderer );
	void renderAll( SDL_Renderer* renderer );

	void renderTextbox( SDL_Renderer* renderer, std::vector<std::string> lines, Coordinate screenpos );

	void tick();

	void zoom(int dy);

	Coordinate objectiveCoordinateFromScreen(const Coordinate c);
	Coordinate screenCoordinateFromObjective(const Coordinate c);

	void issueGotoCoordCmd(Coordinate targetCoord);

	void issueAttackCmd(UnitID targetID);

	void issueAttackMoveCmd(Coordinate targetCoord);

	void issueHaltCmd();

	void issueBuildCmd(UnitTemplateID s);

	void playAnimation(std::string animation, Coordinate pos, int tickTime);

	Game* game;

	TeamID teamID;

	float viewMagnification;

	Coordinate viewCenter;
private:
	bool quit;
	
	bool drawSelectionBox;
	Coordinate selectionBoxCorner1;
	Coordinate selectionBoxCorner2;
	std::set<UnitID> selectedUnits;
	std::set<UnitID> previousSelectedUnits;
	int cameraVx, cameraVy;
	int viewCenterMaxSpeed;

	std::shared_ptr<Spritesheet> uiWireframe;
	std::shared_ptr<Spritesheet> uiControlGroupTab;
	std::set<std::unique_ptr<Animation> > animations;

	int frame;

	std::array<std::set<UnitID>, 10> controlGroups;

	const Uint8 *keyDown;

	bool shiftHeld() {
		return this->keyDown[SDL_SCANCODE_LSHIFT] || this->keyDown[SDL_SCANCODE_RSHIFT];
	}

	bool ctrlHeld() {
		return this->keyDown[SDL_SCANCODE_LCTRL] || this->keyDown[SDL_SCANCODE_RCTRL];
	}

	void setControlGroup(int ctrlGroupIndex);
	void switchToControlGroup(int ctrlGroupIndex);
};
