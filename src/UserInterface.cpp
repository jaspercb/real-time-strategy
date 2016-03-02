#include <algorithm>
#include <sstream>

#include "UserInterface.hpp"
#include "Game.hpp"
#include "globals.hpp"
#include "Command.hpp"
#include "Unit.hpp"
#include "Logging.hpp"
#include "Animation.hpp"
#include "FontManager.hpp"
#include "sdlTools.hpp"
#include "InhabitedGrid.hpp"

UserInterface::UserInterface(Game& g, TeamID teamID):
	quit(false),
	drawSelectionBox(false),
	game(g),
	teamID(teamID),
	viewCenter(Coordinate(5000, 5000)),
	viewMagnification(1.0),
	cameraVy(0),
	cameraVx(0),
	viewCenterMaxSpeed(2500),
	keyDown( SDL_GetKeyboardState(NULL) ),
	uiWireframe(gResourceManager->get("ui-mockup")),
	frame(0)
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

			this->selectionBoxCorner1 = Coordinate(event.button.x, event.button.y);
			this->selectionBoxCorner2 = Coordinate(event.button.x, event.button.y);
			this->drawSelectionBox = true;

			this->updateSelectedUnits();
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
			this->selectionBoxCorner2 = Coordinate(event.button.x, event.button.y);
			this->updateSelectedUnits();
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
					this->cameraVx -= this->viewCenterMaxSpeed;
					break;
				}
				case SDLK_DOWN: {
					this->cameraVy += this->viewCenterMaxSpeed;
					this->cameraVx += this->viewCenterMaxSpeed;
					break;
				}
				case SDLK_LEFT: {
					this->cameraVx -= this->viewCenterMaxSpeed;
					this->cameraVy += this->viewCenterMaxSpeed;
					break;
				}
				case SDLK_RIGHT: {
					this->cameraVx += this->viewCenterMaxSpeed;
					this->cameraVy -= this->viewCenterMaxSpeed;
					break;
				}
			}
		}
	}

	else if (event.type == SDL_KEYUP) {
		switch(event.key.keysym.sym) {
			case SDLK_UP: {
				this->cameraVy += this->viewCenterMaxSpeed;
				this->cameraVx += this->viewCenterMaxSpeed;
				break;
			}
			case SDLK_DOWN: {
				this->cameraVy -= this->viewCenterMaxSpeed;
				this->cameraVx -= this->viewCenterMaxSpeed;
				break;
			}
			case SDLK_LEFT: {
				this->cameraVx += this->viewCenterMaxSpeed;
				this->cameraVy -= this->viewCenterMaxSpeed;
				break;
			}
			case SDLK_RIGHT: {
				this->cameraVx -= this->viewCenterMaxSpeed;
				this->cameraVy += this->viewCenterMaxSpeed;
				break;
			}
		}
	}
}

void UserInterface::updateSelectedUnits() {
	this->selectedUnits.clear();
	
	auto lambda = [this](Unit& u) {
		return coordInRect(this->screenCoordinateFromObjective(u.xy) , this->selectionBoxCorner1, this->selectionBoxCorner2) && this->game.teamsAreFriendly(this->teamID, u.teamID);
	};

	for (auto& unitID_unit : this->game.unitsByID) {
		if (lambda(unitID_unit.second))
			this->selectedUnits.push_back(unitID_unit.first);
	}

}

void UserInterface::renderSelection( SDL_Renderer* renderer ) {
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
	
	SDL_Rect drawRect;
	SDL_Color selectionColor = {0, 255, 0, SDL_ALPHA_OPAQUE};
	SDL_Color waypointColor = {255, 255, 255, 10*std::abs( (this->frame % 18) - 9) + 64 };

	// Draw paths
	for (UnitID &i : this->selectedUnits) {
		Unit& u = this->game.getUnit(i);
		std::vector<Coordinate> path = u.getStateWaypoints();
		int numWaypoints = path.size() + 1;
		if ( numWaypoints >= 2 ) {
			renderLine(renderer, this->screenCoordinateFromObjective(u.xy), this->screenCoordinateFromObjective(path[0]), waypointColor);
			for (int i=0; i<numWaypoints-1; i++) {
				int size = 800;
				renderEllipse(renderer,
							this->screenCoordinateFromObjective(path[i]),
							size/PIXEL_WIDTH * this->viewMagnification,
							size/PIXEL_HEIGHT * this->viewMagnification,
							waypointColor);
			}
		}
		if ( numWaypoints >= 3 ) {
			for (int i=1; i<numWaypoints-1; i++)
				renderLine(renderer, this->screenCoordinateFromObjective(path[i]), this->screenCoordinateFromObjective(path[i-1]), waypointColor);
		}
	}

	// Draw a little individual green selection box for each selected unit
	for (UnitID &i : this->selectedUnits) {
		Unit& u = this->game.getUnit(i);
		UnitTemplate& uTemplate = u.getUnitTemplate();
		
		Coordinate drawCenter = this->screenCoordinateFromObjective(u.xy);
		
		int rw = uTemplate.radius()/PIXEL_WIDTH * this->viewMagnification;
		int rh = uTemplate.radius()/PIXEL_HEIGHT * this->viewMagnification;

		renderEllipse(renderer, drawCenter, rw, rh, selectionColor);

		if (u.dimension.air) {
			Coordinate airborneCenter = drawCenter;
			airborneCenter.second -= AIRBORNE_RENDER_HEIGHT * this->viewMagnification;
			renderLine(renderer, drawCenter, airborneCenter, selectionColor);
		}
	}
}

void UserInterface::renderHUD( SDL_Renderer* renderer ) {
	if (this->drawSelectionBox) {
		SDL_Rect selectionBox;
		renderRectBorder(renderer, selectionBoxCorner1, selectionBoxCorner2, SDL_Color{0, 255, 0, SDL_ALPHA_OPAQUE});
	}

	this->uiWireframe->render(gRenderer, 0, 0, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 1.0);

}

void UserInterface::renderAll( SDL_Renderer* renderer ) {
	SDL_SetRenderDrawColor(renderer, 128, 128, 128, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);

	this->game.terrain.render(renderer, this);

	for (auto &animation : this->animations){
		animation->draw(renderer, this);
	}

	this->renderSelection( renderer );

	Coordinate screenCorner1 = this->objectiveCoordinateFromScreen(Coordinate(-200, -200));
	Coordinate screenCorner2 = this->objectiveCoordinateFromScreen(Coordinate(SCREEN_WIDTH+200, SCREEN_HEIGHT+200));

	for (auto &u : this->game.unitsByID) {
		//if (unitInRectangle(u.second, screenCorner1, screenCorner2))
			u.second.draw( renderer, this);
	}

	this->renderHUD( renderer );
	this->game.terrain.renderMinimap(renderer, this);

	int numSelectedUnits = this->selectedUnits.size();
	if (numSelectedUnits == 1) {
		Unit& selectedUnit = this->game.getUnit(this->selectedUnits[0]);
		std::stringstream infostream;
		
		infostream<<"HP: "<<std::to_string(selectedUnit.hp)<<"/"<<std::to_string(selectedUnit.getUnitTemplate().maxHP())<<std::endl;
		
		if (selectedUnit.weapons_.size())
			infostream << "WEAPON: "<< std::to_string(selectedUnit.weapons_[0].ticksUntilCanFire) << "/" << std::to_string(selectedUnit.weapons_[0].weaponTemplate.reloadTime()) << std::endl;
		else
			infostream << "UNARMED" << std::endl;

		gFontManager->renderMultipleLines(infostream.str(), 320, 750, SDL_Colors::WHITE);


	}
	else if (numSelectedUnits > 1) {
		int text = this->selectedUnits.size();

		gFontManager->renderLine("UNITS SELECTED: "+std::to_string(text), 320, 750, SDL_Colors::WHITE);
	}

	SDL_RenderPresent( renderer );
}

void UserInterface::tick() {
	this->frame++;

	this->viewCenter.first += this->cameraVx;
	this->viewCenter.second += this->cameraVy;

	for (auto &animation : this->animations){
		if (STATUS_REMOVE == animation->tick())
			this->animations.erase(animation);
	}
}

void UserInterface::zoom(int dy) {
	//this->viewCenter.first += SCREEN_WIDTH * PIXEL_WIDTH/this->viewMagnification/2;
	//this->viewCenter.second += SCREEN_HEIGHT * PIXEL_HEIGHT/this->viewMagnification/2;
	
	if (dy > 0)
		this->viewMagnification *= 1.1;
	else if (dy < 0) {
		this->viewMagnification /= 1.1;
	}

	//this->viewCenter.first -= SCREEN_WIDTH * PIXEL_WIDTH/this->viewMagnification/2;
	//this->viewCenter.second -= SCREEN_HEIGHT * PIXEL_HEIGHT/this->viewMagnification/2;
}

Coordinate UserInterface::objectiveCoordinateFromScreen(const Coordinate c) {
	// returns an isometric coordinate

	//return Coordinate(	PIXEL_WIDTH/this->viewMagnification*c.first + this->viewCenter.first,
	//					PIXEL_HEIGHT/this->viewMagnification*c.second + this->viewCenter.second );
	Distance x = PIXEL_WIDTH/this->viewMagnification*(c.first - SCREEN_WIDTH/2);
	Distance y = PIXEL_WIDTH/this->viewMagnification*(c.second - SCREEN_HEIGHT/2);
	return Coordinate(	(2*y + x)/2  + this->viewCenter.first,
						(2*y - x)/2  + this->viewCenter.second);
}

Coordinate UserInterface::screenCoordinateFromObjective(const Coordinate c) {
	Distance x = (c.first-this->viewCenter.first)*this->viewMagnification/PIXEL_WIDTH;
	Distance y = (c.second-this->viewCenter.second)*this->viewMagnification/PIXEL_WIDTH;
	return Coordinate(	(x-y) + SCREEN_WIDTH/2,
						(x+y)/2 + SCREEN_HEIGHT/2);
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