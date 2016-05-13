#include <algorithm>
#include <sstream>

#include "globals.hpp"

#include "UserInterface.hpp"
#include "Game.hpp"
#include "Command.hpp"
#include "Unit.hpp"
#include "UnitTemplate.hpp"
#include "WeaponTemplate.hpp"
#include "Team.hpp"
#include "Logging.hpp"
#include "Animation.hpp"
#include "FontManager.hpp"
#include "sdlTools.hpp"
#include "InhabitedGrid.hpp"
#include "Spritesheet.hpp"

#define max(a, b) ((a>b) ? a : b)
#define min(a, b) ((a<b) ? a : b)

UserInterface::UserInterface(Game* g, TeamID teamID):
	game(g),
	teamID(teamID),
	viewMagnification(1.0),
	viewCenter(Coordinate(0, 0)),
	quit(false),
	drawSelectionBox(false),
	cameraVx(0),
	cameraVy(0),
	viewCenterMaxSpeed(2500),
	uiWireframe(gResourceManager->get("ui-mockup")),
	uiControlGroupTab(gResourceManager->get("ui-ctrlgroup-bar")),
	frame(0),
	keyDown( SDL_GetKeyboardState(NULL) )
{
	
}

int UserInterface::handleInputEvents(){
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
			for ( auto &id : game->inhabitedGrid.unitsInCircle(clickedCoord, (Distance)1000) ) { // hackish way of handling targeting selection
				if (!game->teamsAreFriendly(this->teamID, game->getUnit(id)->teamID)
				&& this->selectedUnits.size()
				&& game->inhabitedGrid.unitIsVisibleToTeam(game->getUnit(id), this->teamID)) {
					this->issueAttackCmd(id);
					return;
				}
			}
			// if we didn't click on an enemy, we move to that point
			if (this->selectedUnits.size())
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
			newViewCenter.x -= event.motion.xrel;
			newViewCenter.y -= event.motion.yrel;
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
				
				#define CtrlGroupLogic(x) if (this->ctrlHeld()) this->setControlGroup(x); else this->switchToControlGroup(x); break;

				case SDLK_1:
					CtrlGroupLogic(0);
				case SDLK_2:
					CtrlGroupLogic(1);
				case SDLK_3:
					CtrlGroupLogic(2);
				case SDLK_4:
					CtrlGroupLogic(3);
				case SDLK_5:
					CtrlGroupLogic(4);
				case SDLK_6:
					CtrlGroupLogic(5);
				case SDLK_7:
					CtrlGroupLogic(6);
				case SDLK_8:
					CtrlGroupLogic(7);
				case SDLK_9:
					CtrlGroupLogic(8);
				case SDLK_0:
					CtrlGroupLogic(9);

				#undef CtrlGroupLogic

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

				case SDLK_s: {
					this->issueHaltCmd();
					break;
				}

				case SDLK_q: {
					this->issueBuildCmd("../conf/units/bomber");
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
	if (!this->shiftHeld()) {
		this->previousSelectedUnits = this->selectedUnits;
		this->selectedUnits.clear();
	}

	int diff = std::abs(this->selectionBoxCorner1.x-this->selectionBoxCorner2.x) + std::abs(this->selectionBoxCorner1.y-this->selectionBoxCorner2.y);
	if (diff < 10) {
		auto center = this->objectiveCoordinateFromScreen(this->selectionBoxCorner1);
		Distance closestDistance = MAX_DISTANCE;
		UnitID closestUnitID = 0;
		for (auto &i : game->inhabitedGrid.unitsInCircle(center, 2000) ) {
			const Unit* unit = game->getUnit(i);
			if (unit->teamID == this->teamID
				&& pythagoreanDistanceLessThan(unit->xy, center, min(closestDistance, (Distance)unit->getUnitTemplate()->radius()+500 ))
				&& unit->getUnitTemplate()->isSelectable() ) {
				closestUnitID = i;
			}
		}

		if (this->previousSelectedUnits.size() == 1 && *this->previousSelectedUnits.begin()==closestUnitID) {
			// double-clicking a unit selects all visible units with the same type
			UnitID prevUnitID = *this->previousSelectedUnits.begin();
			UnitTemplateID prevUnitTemplateID = game->getUnit(prevUnitID)->getUnitTemplate()->name;
			TeamID prevTeamID = game->getUnit(prevUnitID)->teamID;
			this->previousSelectedUnits.clear();
			this->selectedUnits.clear();
			for (const auto &i : game->unitsByID) {
				if (coordInRect(this->screenCoordinateFromObjective(i.second->xy), Coordinate(0, 0), Coordinate(SCREEN_WIDTH, SCREEN_HEIGHT))
				&& i.second->teamID == prevTeamID
				&& i.second->teamID == prevTeamID )
					this->selectedUnits.insert(i.first);
			}
		}

		else if (closestUnitID){
			this->selectedUnits.insert(closestUnitID);
		}
	}
	else{
		auto lambda = [this](Unit*u) {
			return u->getUnitTemplate()->isSelectable() && coordInRect(this->screenCoordinateFromObjective(u->xy) , this->selectionBoxCorner1, this->selectionBoxCorner2) && game->teamsAreFriendly(this->teamID, u->teamID);
		};

		for (auto& unitID_unit : game->unitsByID) {
			if (lambda(unitID_unit.second))
				this->selectedUnits.insert(unitID_unit.first);
		}
	}
}

void UserInterface::renderSelection( SDL_Renderer* renderer ) {
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);

	SDL_Color selectionColor = {0, 255, 0, SDL_ALPHA_OPAQUE};
	SDL_Color waypointColor = {255, 255, 255, (Uint8) (10*std::abs( (this->frame % 18) - 9) + 64) };

	// Draw paths
	for (UnitID i : this->selectedUnits) {
		Unit* u = game->getUnit(i);
		std::vector<Coordinate> path = u->getStateWaypoints();
		int numWaypoints = path.size() + 1;
		if ( numWaypoints >= 2 ) {
			renderLine(renderer, this->screenCoordinateFromObjective(u->xy), this->screenCoordinateFromObjective(path[0]), waypointColor);
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
	for (UnitID i : this->selectedUnits) {
		Unit* u = game->getUnit(i);
		UnitTemplate* uTemplate = u->getUnitTemplate();
		
		Coordinate drawCenter = this->screenCoordinateFromObjective(u->xy);
		
		int rw = uTemplate->radius()/PIXEL_WIDTH * this->viewMagnification;
		int rh = uTemplate->radius()/PIXEL_HEIGHT * this->viewMagnification;

		renderEllipse(renderer, drawCenter, rw, rh, selectionColor);

		if (u->dimension.air) {
			Coordinate airborneCenter = drawCenter;
			airborneCenter.y -= AIRBORNE_RENDER_HEIGHT * this->viewMagnification;
			renderLine(renderer, drawCenter, airborneCenter, selectionColor);
		}
	}
}

void UserInterface::renderHUD( SDL_Renderer* renderer ) {
	int cx = 0;
	for (auto &i : this->controlGroups) {
		cx += 55;
		if (i.size()) {
			this->uiControlGroupTab->render(gRenderer, 0, 0, 290+cx, 720, 1.0);
			gFontManager->renderLine(std::to_string(i.size()), {280+cx, 700}, SDL_Colors::WHITE);
		}
	}

	if (this->drawSelectionBox) {
		renderRectBorder(renderer, selectionBoxCorner1, selectionBoxCorner2, SDL_Color{0, 255, 0, SDL_ALPHA_OPAQUE});
	}

	this->uiWireframe->render(gRenderer, 0, 0, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 1.0);

	game->terrain.renderMinimap(renderer, this);

	selectedUnits.removeInvalidUnits();

	if (selectedUnits.size() == 1) {
		const Unit* unit = game->getUnit(*selectedUnits.begin());
		const UnitTemplate* uTemplate = unit->getUnitTemplate();

		const int renderX = 370;
		const int renderY = 810;
		const Uint8 colormod = 255*unit->hp/uTemplate->maxHP();

		uTemplate->renderIcon( renderX, renderY, colormod );

		// ES
		std::stringstream es_txt;
		es_txt << unit->es << "/" << uTemplate->maxES();
		gFontManager->renderLine( es_txt.str(), Coordinate(renderX-24, renderY+24), SDL_Colors::LIGHTBLUE);

		// HP
		std::stringstream hp_txt;
		hp_txt << unit->hp << "/" << uTemplate->maxHP();
		gFontManager->renderLine( hp_txt.str(), Coordinate(renderX-24, renderY+48), SDL_Colors::GREEN);

		// BUILDING
		if (unit->builder) {
			const Builder* builder = unit->builder;
			for (int index=0; index<builder->building.size(); index++) {
				auto &buildQueue = builder->building[index];
				int queuePosition = 0;
				for (auto &buildData : buildQueue) {
					const Coordinate renderpos(500+50*queuePosition, 810+48*index);
					const UnitTemplate* buildingTemplate = game->getUnitTemplate(unit->teamID, buildData.unitTemplateID);
					float fractionComplete = 1-float(buildData.ticksUntilDone)/buildData.totalTicks;
					buildingTemplate->renderIcon(renderpos.x, renderpos.y, 255*fractionComplete);
					renderRectFilled(gRenderer, renderpos+Coordinate(-24,24), renderpos+Coordinate(24,23), SDL_Colors::LIGHTGREY);
					renderRectFilled(gRenderer, renderpos+Coordinate(-24,24), renderpos+Coordinate(-24+48*fractionComplete,23), SDL_Colors::GREEN);

					queuePosition++;
				}
			}
		}
	} else {
		int unitIndex = 0;

		auto iconbox = gResourceManager->get("ui-iconbox");
		const int iconsPerRow = 20;
		for (const auto& unitID : selectedUnits) {
			const Unit* unit = game->getUnit(unitID);
			const UnitTemplate* uTemplate = unit->getUnitTemplate();

			const int renderX = 326 + (unitIndex%iconsPerRow)*49;
			const int renderY = 768+(unitIndex/iconsPerRow)*49;
			const Uint8 colormod = 255*unit->hp/uTemplate->maxHP();

			iconbox->render(gRenderer, 0, 0, renderX, renderY, 1);
			uTemplate->renderIcon( renderX, renderY, colormod );
			unitIndex++;
			if (unitIndex==60) {
				break;
			}
		}
	}

	// Draw minimap

	Coordinate corner1 = minimapCoordinateFromScreen({0, 0});
	corner1.x = max(corner1.x, 10);
	corner1.y = max(corner1.y, 610);
	corner1.x = min(corner1.x, 290);
	corner1.y = min(corner1.y, 890);

	Coordinate corner2 = minimapCoordinateFromScreen({SCREEN_WIDTH, SCREEN_HEIGHT});
	corner2.x = max(corner2.x, 10);
	corner2.y = max(corner2.y, 610);
	corner2.x = min(corner2.x, 290);
	corner2.y = min(corner2.y, 890);

	Coordinate corner3 = minimapCoordinateFromScreen({SCREEN_WIDTH, 750});
	corner3.x = max(corner3.x, 10);
	corner3.y = max(corner3.y, 610);
	corner3.x = min(corner3.x, 290);
	corner3.y = min(corner3.y, 890);

	renderRectBorder(renderer, corner1, corner2, SDL_Colors::WHITE);
	renderRectBorder(renderer, corner1, corner3, SDL_Colors::RED);
}

void UserInterface::renderAll( SDL_Renderer* renderer ) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);

	game->terrain.render(renderer, this);

	this->renderSelection( renderer );

	Coordinate screenCorner1 = Coordinate(-500, -500);
	Coordinate screenCorner2 = Coordinate(SCREEN_WIDTH+500, SCREEN_HEIGHT+500);

	std::vector<Unit*> visibleUnitsInDrawOrder;
	for (auto &i : game->unitsByID)
		if (game->inhabitedGrid.getCoordVisibility(i.second->xy, this->teamID))
			visibleUnitsInDrawOrder.push_back(i.second);

	auto sortLambda = [this](Unit* a, Unit* b) {
		return (this->screenCoordinateFromObjective(a->xy).y) < (this->screenCoordinateFromObjective(b->xy).y);
	};

	std::sort(visibleUnitsInDrawOrder.begin(), visibleUnitsInDrawOrder.end(), sortLambda);
	
	// draw non-air units
	for (auto &u : visibleUnitsInDrawOrder) {
		if (coordInRect(this->screenCoordinateFromObjective(u->xy), screenCorner1, screenCorner2)
		&& u->dimension.overlaps(GROUND_ONLY) ) {
			u->draw( renderer, this );
			drawHPbar(gRenderer, u, gUserInterface);
		}
	}

	// draw misc. animations n stuff
	for (auto &animation : this->animations){
		animation->draw(renderer, this);
	}

	// draw air units
	for (auto &u : visibleUnitsInDrawOrder) {
		if (coordInRect(this->screenCoordinateFromObjective(u->xy), screenCorner1, screenCorner2)
		&& !u->dimension.overlaps(GROUND_ONLY) ) {
			u->draw( renderer, this );
			drawHPbar(gRenderer, u, gUserInterface);
		}
	}

	this->renderHUD( renderer );

	//this->renderTextbox(gRenderer, std::vector<std::string>(1,"I continue to test, test, test!"),  {800, 100});

	SDL_RenderPresent( renderer );
}

void UserInterface::renderTextbox( SDL_Renderer* renderer, std::vector<std::string> lines, Coordinate screenpos ) {
	// Renders an opaque text box. 'screenpos' gives the screen coordinates of the bottom-right corner
	long int maxwidth = 0;
	for (const auto &line : lines) {
		maxwidth = max(maxwidth, (long int)line.size());
	}
	Coordinate topLeft = screenpos-Coordinate(maxwidth*8, lines.size()*20);
	renderRectBorder(gRenderer, screenpos, topLeft, SDL_Colors::WHITE);
	gFontManager->renderVector( lines, topLeft+Coordinate(10,0));
}

/* 
 * Advances all animations by one frame.
*/
void UserInterface::tick() {
	this->frame++;

	this->viewCenter.x += this->cameraVx;
	this->viewCenter.y += this->cameraVy;

	for (auto &animation : this->animations){
		if (UpdateStatus::Remove == animation->tick())
			this->animations.erase(animation);
	}
}

/* 
 * Zooms in the current viewing pane. positive argument => zoom in, negative => zoom out.
*/
void UserInterface::zoom(int dy) {
	if (dy > 0)
		this->viewMagnification *= 1.1;
	else if (dy < 0) {
		this->viewMagnification /= 1.1;
	}
}

Coordinate UserInterface::objectiveCoordinateFromScreen(const Coordinate c) {
	// returns an isometric coordinate
	Distance x = PIXEL_WIDTH/viewMagnification*(c.x - SCREEN_WIDTH/2);
	Distance y = PIXEL_WIDTH/viewMagnification*(c.y - SCREEN_HEIGHT/2);
	return Coordinate( (2*y + x)/2  + viewCenter.x,
	                   (2*y - x)/2  + viewCenter.y );
}

Coordinate UserInterface::screenCoordinateFromObjective(const Coordinate c) {
	Distance x = (c.x-viewCenter.x)*viewMagnification/PIXEL_WIDTH;
	Distance y = (c.y-viewCenter.y)*viewMagnification/PIXEL_WIDTH;
	return Coordinate( (x-y) + SCREEN_WIDTH/2,
	                   (x+y)/2 + SCREEN_HEIGHT/2);
}

Coordinate UserInterface::minimapCoordinateFromScreen(const Coordinate c) {
	return minimapCoordinateFromObjective(objectiveCoordinateFromScreen(c));
}

Coordinate UserInterface::minimapCoordinateFromObjective(const Coordinate c) {
	return Coordinate( 150 + (c.x - c.y)/(16*PIXEL_WIDTH),
	                   600 + (c.x + c.y)/(16*PIXEL_WIDTH) );
}

void UserInterface::issueGotoCoordCmd(Coordinate targetCoord) {
	Command cmd(CommandType::Enum::Gotocoord);
	cmd.commanded = this->selectedUnits;
	cmd.queueSetting = this->shiftHeld() ? QueueSetting::Back : QueueSetting::Overwrite;
	cmd.targetCoord = targetCoord;
	
	this->playAnimation("gotocoord-animation", targetCoord, 3);

	game->handleCommand(cmd);
}

void UserInterface::issueAttackCmd(UnitID targetID) {
	Command cmd(CommandType::Enum::Attack);
	cmd.commanded = this->selectedUnits;
	cmd.queueSetting = this->shiftHeld() ? QueueSetting::Back : QueueSetting::Overwrite;
	cmd.targetID = targetID;
	
	this->playAnimation("attackmove-animation", game->getUnit(targetID)->xy, 3);

	game->handleCommand(cmd);
}

void UserInterface::issueAttackMoveCmd(Coordinate targetCoord) {
	Command cmd(CommandType::Enum::Attackmove);
	cmd.commanded = this->selectedUnits;
	cmd.queueSetting = this->shiftHeld() ? QueueSetting::Back : QueueSetting::Overwrite;
	cmd.targetCoord = targetCoord;
	
	this->playAnimation("attackmove-animation", targetCoord, 3);

	game->handleCommand(cmd);
}

void UserInterface::issueHaltCmd() {
	Command cmd(CommandType::Enum::Halt);
	cmd.commanded = this->selectedUnits;
	game->handleCommand(cmd);
}

void UserInterface::issueBuildCmd(UnitTemplateID id) {
	Command cmd(CommandType::Enum::Build);
	cmd.commanded = this->selectedUnits;
	cmd.unitTemplateID = id;
	game->handleCommand(cmd);
}

void UserInterface::playAnimation(std::string animationName, Coordinate pos, int tickTime) {
	this->animations.insert(Animation::create(animationName, pos, tickTime));
}

void UserInterface::setControlGroup(int ctrlGroupIndex) {
	this->controlGroups[ctrlGroupIndex] = this->selectedUnits;
}

void UserInterface::switchToControlGroup(int ctrlGroupIndex) {
	if (this->shiftHeld())
		this->selectedUnits.insert(this->controlGroups[ctrlGroupIndex]);
	else
		this->selectedUnits = this->controlGroups[ctrlGroupIndex];
}
