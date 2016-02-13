#include <SDL2/SDL.h>
#include <algorithm>

#include "UserInterface.hpp"
#include "Game.hpp"
#include "globals.hpp"
#include "Command.hpp"
#include "Unit.hpp"
#include "Logging.hpp"

UserInterface::UserInterface(Game& g, TeamID teamID):
	quit(false),
	drawSelectionBox(false),
	game(g),
	teamID(teamID),
	viewCenter(Coordinate(5000, 5000)),
	viewMagnification(1.0),
	cameraVy(0),
	cameraVx(0),
	viewCenterMaxSpeed(1000)
{
	
}

void UserInterface::handleInputEvent(const SDL_Event& event){
	if (event.type == SDL_QUIT) {
		this->quit = true;
	}
	else if (event.type == SDL_MOUSEBUTTONDOWN) {
		if (event.button.button == SDL_BUTTON_RIGHT) {
			Coordinate targetCoord = objectiveCoordinateFromScreen(Coordinate(event.button.x, event.button.y));
			for ( auto &i : game.inhabitedGrid.unitsInCircle(targetCoord, (Distance)10) ) {
				if (!game.teamsAreFriendly(this->teamID, this->game.getUnit(i).teamID) ) {
					debugLog("issuing attack command");
					Command atkcmd(CMD_ATTACKMOVE);
					atkcmd.targetCoord = targetCoord;
					atkcmd.commanded = this->unitsInSelectionBox;
					game.handleCommand(atkcmd);
					return;
				}
			}

			Command movecmd(CMD_GOTOCOORD);
			movecmd.targetCoord = objectiveCoordinateFromScreen(Coordinate(event.button.x, event.button.y));
			movecmd.queueSetting = QUEUE_OVERWRITE;
			movecmd.commanded = this->unitsInSelectionBox;
			game.handleCommand(movecmd);
		}
		else if (event.button.button == SDL_BUTTON_LEFT) {
			this->selectionBoxCorner1 = this->objectiveCoordinateFromScreen(Coordinate(event.button.x, event.button.y));
			this->selectionBoxCorner2 = this->objectiveCoordinateFromScreen(Coordinate(event.button.x, event.button.y));
			this->drawSelectionBox = true;

			this->unitsInSelectionBox.clear();
			auto unfilteredUnits = this->game.inhabitedGrid.unitsInRectangle(this->selectionBoxCorner1, this->selectionBoxCorner2);
			auto lambda = [this](UnitID u) {
				return this->game.teamsAreFriendly(this->teamID, this->game.getUnit(u).teamID);
			};
			std::copy_if(unfilteredUnits.begin(), unfilteredUnits.end(), std::back_inserter(this->unitsInSelectionBox), lambda);
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
		if (SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(1)) { // if left mouse button is being held down
			this->selectionBoxCorner2 = this->objectiveCoordinateFromScreen(Coordinate(event.button.x, event.button.y));

			auto unfilteredUnits = this->game.inhabitedGrid.unitsInRectangle(this->selectionBoxCorner1, this->selectionBoxCorner2);
			auto lambda = [this](UnitID u) {
				return this->game.teamsAreFriendly(this->teamID, this->game.getUnit(u).teamID);
			};

			this->unitsInSelectionBox.clear();
			std::copy_if(unfilteredUnits.begin(), unfilteredUnits.end(), std::back_inserter(this->unitsInSelectionBox), lambda);
		}
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
	drawRect.w = 30;
	drawRect.h = 15;
	for (UnitID &i : unitsInSelectionBox){
		Unit& u = this->game.getUnit(i);
		Coordinate drawCenter = this->screenCoordinateFromObjective(u.xy);
		drawRect.x = drawCenter.first-15;
		drawRect.y = drawCenter.second;
		SDL_RenderDrawRect(renderer, &drawRect);

		if (u.dimension.air){
			SDL_RenderDrawLine(renderer, drawCenter.first, drawCenter.second+7, drawCenter.first, drawCenter.second-AIRBORNE_RENDER_HEIGHT);
		}
	}
}

void UserInterface::renderHUD( SDL_Renderer* renderer ) {
	if (this->drawSelectionBox){
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
		//debugLog(this->unitsInSelectionBox.size());
	}
}

void UserInterface::renderAll( SDL_Renderer* renderer ){
	SDL_SetRenderDrawColor(renderer, 128, 128, 128, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);

	this->renderSelection( renderer );

	for (auto &u : this->game.unitsByID){
		u.second.draw( renderer, this );
	}

	this->renderHUD( renderer );

	SDL_RenderPresent( renderer );
}

void UserInterface::tick() {
	this->viewCenter.first += this->cameraVx;
	this->viewCenter.second += this->cameraVy;
}

Coordinate UserInterface::objectiveCoordinateFromScreen(const Coordinate c){
	return Coordinate(	PIXEL_WIDTH/this->viewMagnification*c.first + this->viewCenter.first,
						PIXEL_HEIGHT/this->viewMagnification*c.second + this->viewCenter.second );
}

Coordinate UserInterface::screenCoordinateFromObjective(const Coordinate c){
	return Coordinate(	(c.first-this->viewCenter.first)*this->viewMagnification/PIXEL_WIDTH,
						(c.second-this->viewCenter.second)*this->viewMagnification/PIXEL_HEIGHT );
}
