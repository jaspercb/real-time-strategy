#include <SDL2/SDL.h>

#include "UserInterface.hpp"
#include "Game.hpp"
#include "globals.hpp"
#include "Command.hpp"
#include "Unit.hpp"
#include "Logging.hpp"

UserInterface::UserInterface(Game& g):
	quit(false),
	drawSelectionBox(false),
	game(g)
{
	
}

void UserInterface::handleInputEvent(const SDL_Event& event){
	if (event.type == SDL_QUIT) {
		this->quit = true;
	}
	else if (event.type == SDL_MOUSEBUTTONDOWN) {
		if (event.button.button == SDL_BUTTON_RIGHT) {
			Command movecmd(CMD_GOTOCOORD);
			movecmd.targetCoord = objectiveCoordinateFromScreen(Coordinate(event.button.x, event.button.y));
			movecmd.queueSetting = QUEUE_OVERWRITE;
			for (auto &i : unitsInSelectionBox) {
				this->game.getUnit(i).handleCommand(movecmd);
			}
		}
		else if (event.button.button == SDL_BUTTON_LEFT) {
			this->selectionBoxCorner1 = this->objectiveCoordinateFromScreen(Coordinate(event.button.x, event.button.y));
			this->selectionBoxCorner2 = this->objectiveCoordinateFromScreen(Coordinate(event.button.x, event.button.y));
			this->drawSelectionBox = true;

			this->unitsInSelectionBox.clear();
			this->unitsInSelectionBox = this->game.inhabitedGrid.unitsInRectangle(this->selectionBoxCorner1, this->selectionBoxCorner2);
		}
	}
	else if (event.type == SDL_MOUSEBUTTONUP) {
		if (event.button.button == SDL_BUTTON_LEFT) {
			this->drawSelectionBox = false;
		}
	}
	else if (event.type == SDL_MOUSEMOTION) {
		if (SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(1)) { // if left mouse button is being held down
			this->selectionBoxCorner2 = this->objectiveCoordinateFromScreen(Coordinate(event.button.x, event.button.y));
			std::cout<< this->selectionBoxCorner1.first <<","<< this->selectionBoxCorner1.second <<std::endl;
			std::cout<< this->selectionBoxCorner2.first <<","<< this->selectionBoxCorner2.second <<std::endl;
			std::cout<<std::endl;
			this->unitsInSelectionBox.clear();
			this->unitsInSelectionBox = this->game.inhabitedGrid.unitsInRectangle(this->selectionBoxCorner1, this->selectionBoxCorner2);
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


Coordinate UserInterface::objectiveCoordinateFromScreen(const Coordinate c){
	return Coordinate(c.first*PIXEL_WIDTH, c.second*PIXEL_HEIGHT);
}

Coordinate UserInterface::screenCoordinateFromObjective(const Coordinate c){
	return Coordinate(c.first/PIXEL_WIDTH, c.second/PIXEL_HEIGHT);
}
