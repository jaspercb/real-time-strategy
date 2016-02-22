#include <SDL2/SDL.h>
#include <algorithm>

#include "UserInterface.hpp"
#include "Game.hpp"
#include "globals.hpp"
#include "Command.hpp"
#include "Unit.hpp"
#include "Logging.hpp"
#include "Animation.hpp"
#include "FontManager.hpp"

UserInterface::UserInterface(Game& g, TeamID teamID):
	quit(false),
	drawSelectionBox(false),
	game(g),
	teamID(teamID),
	viewCenter(Coordinate(5000, 5000)),
	viewMagnification(1.0),
	cameraVy(0),
	cameraVx(0),
	viewCenterMaxSpeed(1000),
	keyDown( SDL_GetKeyboardState(NULL) ),
	uiWireframe(gResourceManager->get("ui-mockup"))
{
	
}

int UserInterface::handleInputEvents(){
	SDL_PumpEvents();
	
	SDL_Event event;

	while(SDL_PollEvent(&event)) {
		this->handleInputEvent(event);
	}

	return this->quit;
}

void UserInterface::handleInputEvent(const SDL_Event& event) {
	if (event.type == SDL_QUIT) {
		this->quit = true;
	}
	else if (event.type == SDL_MOUSEBUTTONDOWN) {
		Coordinate clickedCoord = objectiveCoordinateFromScreen( Coordinate(event.button.x, event.button.y) );

		if (event.button.button == SDL_BUTTON_RIGHT) {
			for ( auto &id : game.inhabitedGrid.unitsInCircle(clickedCoord, (Distance)1000) ) { // hackish way of handling targeting selection
				if (!game.teamsAreFriendly(this->teamID, this->game.getUnit(id).teamID) ) {
					this->issueAttackCmd(id);
					return;
				}
			}
			// if we didn't click on an enemy, we move to that point
			this->issueGotoCoordCmd(clickedCoord);

		}
		
		else if (event.button.button == SDL_BUTTON_LEFT) {
			if (this->keyDown[SDL_SCANCODE_A] && this->selectedUnits.size() ) {
				this->issueAttackMoveCmd(clickedCoord);
				return;
			}

			this->selectionBoxCorner1 = this->objectiveCoordinateFromScreen(Coordinate(event.button.x, event.button.y));
			this->selectionBoxCorner2 = this->objectiveCoordinateFromScreen(Coordinate(event.button.x, event.button.y));
			this->drawSelectionBox = true;

			this->selectedUnits.clear();
			auto unfilteredUnits = this->game.inhabitedGrid.unitsInRectangle(this->selectionBoxCorner1, this->selectionBoxCorner2);
			auto lambda = [this](UnitID u) {
				return this->game.teamsAreFriendly(this->teamID, this->game.getUnit(u).teamID);
			};
			std::copy_if(unfilteredUnits.begin(), unfilteredUnits.end(), std::back_inserter(this->selectedUnits), lambda);
		}
	}
	else if (event.type == SDL_MOUSEBUTTONUP) {
		if (event.button.button == SDL_BUTTON_LEFT) {
			this->drawSelectionBox = false;
		}
	}
	else if (event.type == SDL_MOUSEMOTION) {
		if (SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(2)) { // if middle mouse button is being held down
			Coordinate newViewCenter = this->screenCoordinateFromObjective(this->viewCenter);
			newViewCenter.first -= event.motion.xrel;
			newViewCenter.second -= event.motion.yrel;
			this->viewCenter = this->objectiveCoordinateFromScreen(newViewCenter);
		}
		if (this->drawSelectionBox && SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(1)) { // if left mouse button is being held down
			this->selectionBoxCorner2 = this->objectiveCoordinateFromScreen(Coordinate(event.button.x, event.button.y));

			auto unfilteredUnits = this->game.inhabitedGrid.unitsInRectangle(this->selectionBoxCorner1, this->selectionBoxCorner2);
			auto lambda = [this](UnitID u) {
				return this->game.teamsAreFriendly(this->teamID, this->game.getUnit(u).teamID);
			};

			this->selectedUnits.clear();
			std::copy_if(unfilteredUnits.begin(), unfilteredUnits.end(), std::back_inserter(this->selectedUnits), lambda);
		}
	}

	else if (event.type == SDL_MOUSEWHEEL) {
		this->zoom(event.wheel.y);
	}

	else if (event.type == SDL_KEYDOWN) {
		if (!event.key.repeat) {
			switch(event.key.keysym.sym) {
				case SDLK_UP: {
					this->cameraVy -= this->viewCenterMaxSpeed;
					break;
				}
				case SDLK_DOWN: {
					this->cameraVy += this->viewCenterMaxSpeed;
					break;
				}
				case SDLK_LEFT: {
					this->cameraVx -= this->viewCenterMaxSpeed;
					break;
				}
				case SDLK_RIGHT: {
					this->cameraVx += this->viewCenterMaxSpeed;
					break;
				}
			}
		}
	}

	else if (event.type == SDL_KEYUP) {
		switch(event.key.keysym.sym) {
			case SDLK_UP: {
				this->cameraVy = 0;
				break;
		}
			case SDLK_DOWN: {
				this->cameraVy = 0;
				break;
			}
			case SDLK_LEFT: {
				this->cameraVx = 0;
				break;
			}
			case SDLK_RIGHT: {
				this->cameraVx = 0;
				break;
			}
		}
	}
}

void UserInterface::renderSelection( SDL_Renderer* renderer ) {
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
	
	SDL_Rect drawRect;
	for (UnitID &i : selectedUnits) {
		Unit& u = this->game.getUnit(i);
		UnitTemplate& uTemplate = u.getUnitTemplate();
		drawRect.w = 2*uTemplate.radius()/PIXEL_WIDTH * this->viewMagnification;
		drawRect.h = 2*uTemplate.radius()/PIXEL_HEIGHT * this->viewMagnification;

		Coordinate drawCenter = this->screenCoordinateFromObjective(u.xy);
		drawRect.x = drawCenter.first-drawRect.w/2;
		drawRect.y = drawCenter.second-drawRect.h/2;
		SDL_RenderDrawRect(renderer, &drawRect);

		if (u.dimension.air) {
			SDL_RenderDrawLine(renderer, drawCenter.first, drawCenter.second, drawCenter.first, drawCenter.second-AIRBORNE_RENDER_HEIGHT);
		}
	}
}

void UserInterface::renderHUD( SDL_Renderer* renderer ) {
	if (this->drawSelectionBox) {
		SDL_Rect selectionBox;
		Coordinate screenCorner1 = this->screenCoordinateFromObjective(this->selectionBoxCorner1);
		Coordinate screenCorner2 = this->screenCoordinateFromObjective(this->selectionBoxCorner2);
		selectionBox.x = std::min(screenCorner1.first, screenCorner2.first);
		selectionBox.y = std::min(screenCorner1.second, screenCorner2.second);
		selectionBox.w = std::abs(screenCorner2.first - screenCorner1.first);
		selectionBox.h = std::abs(screenCorner2.second - screenCorner1.second);

		SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
		if (SDL_RenderDrawRect(renderer, &selectionBox))
			debugLog(SDL_GetError());
		//debugLog(this->selectedUnits.size());
	}

	this->uiWireframe->render(gRenderer, 0, 0, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 1.0);

}

void UserInterface::renderAll( SDL_Renderer* renderer ) {
	SDL_SetRenderDrawColor(renderer, 128, 128, 128, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);

	for (auto &animation : this->animations){
		animation->draw(renderer, this);
	}

	this->renderSelection( renderer );

	for (auto &u : this->game.unitsByID) {
		u.second.draw( renderer, this );
	}

	this->renderHUD( renderer );


	int numSelectedUnits = this->selectedUnits.size();
	if (numSelectedUnits == 1) {
		Unit& selectedUnit = this->game.getUnit(this->selectedUnits[0]);
		gFontManager->renderText("hp: "+std::to_string(selectedUnit.hp)+"/"+std::to_string(selectedUnit.getUnitTemplate().maxHP()), 320, 750);
		gFontManager->renderText("weapon: " + std::to_string(selectedUnit.weapons_[0].ticksUntilCanFire) + "/" +std::to_string(selectedUnit.weapons_[0].weaponTemplate.reloadTime()), 320, 770);
		

	}
	else if (numSelectedUnits > 1) {
		int text = this->selectedUnits.size();

		gFontManager->renderText("UNITS SELECTED: "+std::to_string(text), 320, 750);		
	}

	SDL_RenderPresent( renderer );
}

void UserInterface::tick() {
	this->viewCenter.first += this->cameraVx;
	this->viewCenter.second += this->cameraVy;

	for (auto &animation : this->animations){
		if (STATUS_REMOVE == animation->tick())
			this->animations.erase(animation);
	}
}

void UserInterface::zoom(int dy) {
	this->viewCenter.first += SCREEN_WIDTH * PIXEL_WIDTH/this->viewMagnification/2;
	this->viewCenter.second += SCREEN_HEIGHT * PIXEL_HEIGHT/this->viewMagnification/2;
	
	if (dy > 0)
		this->viewMagnification *= 1.1;
	else if (dy < 0) {
		this->viewMagnification /= 1.1;
	}

	this->viewCenter.first -= SCREEN_WIDTH * PIXEL_WIDTH/this->viewMagnification/2;
	this->viewCenter.second -= SCREEN_HEIGHT * PIXEL_HEIGHT/this->viewMagnification/2;
}

Coordinate UserInterface::objectiveCoordinateFromScreen(const Coordinate c) {
	return Coordinate(	PIXEL_WIDTH/this->viewMagnification*c.first + this->viewCenter.first,
						PIXEL_HEIGHT/this->viewMagnification*c.second + this->viewCenter.second );
}

Coordinate UserInterface::screenCoordinateFromObjective(const Coordinate c) {
	return Coordinate(	(c.first-this->viewCenter.first)*this->viewMagnification/PIXEL_WIDTH,
						(c.second-this->viewCenter.second)*this->viewMagnification/PIXEL_HEIGHT );
}


void UserInterface::issueGotoCoordCmd(Coordinate targetCoord) {
	Command cmd(CMD_GOTOCOORD);
	cmd.commanded = this->selectedUnits;
	cmd.queueSetting = this->shiftHeld() ? QUEUE_BACK : QUEUE_OVERWRITE;
	cmd.targetCoord = targetCoord;
	
	this->animations.insert(newAnimation("gotocoord-animation", targetCoord, 1));

	game.handleCommand(cmd);
}

void UserInterface::issueAttackCmd(UnitID targetID) {
	Command cmd(CMD_ATTACK);
	cmd.commanded = this->selectedUnits;
	cmd.queueSetting = this->shiftHeld() ? QUEUE_BACK : QUEUE_OVERWRITE;
	cmd.targetID = targetID;
	
	this->animations.insert(newAnimation("attackmove-animation", this->game.getUnit(targetID).xy, 1));

	game.handleCommand(cmd);
}

void UserInterface::issueAttackMoveCmd(Coordinate targetCoord){
	Command cmd(CMD_ATTACKMOVE);
	cmd.commanded = this->selectedUnits;
	cmd.queueSetting = this->shiftHeld() ? QUEUE_BACK : QUEUE_OVERWRITE;
	cmd.targetCoord = targetCoord;
	
	this->animations.insert(newAnimation("attackmove-animation", targetCoord, 1));

	game.handleCommand(cmd);
}