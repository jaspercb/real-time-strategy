#include <SDL2/SDL.h>

#include "sdlTools.hpp"
#include "Drawer.hpp"
#include "Spritesheet.hpp"
#include "Logging.hpp"
#include "UnitTemplate.hpp"
#include "globals.hpp"
#include "Unit.hpp"
#include "UserInterface.hpp"
#include "ResourceManager.hpp"

int AIRBORNE_RENDER_HEIGHT = 40;

const int PIXEL_WIDTH = 100;
const int PIXEL_HEIGHT = 200;

int positiveRemainder(int a, int b) {
	// returns the unique integer 0<=r<b such that mb+r = a for some integer n
	return ((a%b)+b)%b;
}

Drawer::Drawer() {

}

Drawer::Drawer(std::shared_ptr<Spritesheet> sp):
	spritesheet(sp)
	{
	}

Drawer::Drawer(std::ifstream& is, TeamColor::Enum teamColor):
	idleCycleStart(0),
	idleCycleLength(1),
	walkCycleStart(0),
	walkCycleLength(0),
	attackCycleStart(0),
	attackCycleLength(0),
	deathCycleStart(0),
	deathCycleLength(0),
	deathCycleVertical(false),
	wireframeX(0),
	wireframeY(0),
	spritesheet(NULL),
	shadowsheet(NULL)
{
	std::string spritefilename, shadowfilename;
	bool hasShadowsheet=false, hasSpritesheet=false;
	std::string s;

	while (is>>s) {
		if (s=="#") {
			getline(is, s); // this is a comment. we ignore comments.
		}
		else if (s=="spritesheet") {
			is>>spritefilename;
			hasSpritesheet=true;
		}

		else if (s=="shadowsheet") {
			is>>shadowfilename;
			hasShadowsheet=true;
		}
		
		else if (s=="wireframe") {
			is>>wireframeX;
			is>>wireframeY;
		}

		else if (s=="numFacingDirections") {
			is>>numFacingDirections;
		}

		else if (s=="idleCycleStart") {
			is>>idleCycleStart;
		}

		else if (s=="idleCycleLength") {
			is>>idleCycleLength;
		}

		else if (s=="walkCycleStart") {
			is>>walkCycleStart;
		}

		else if (s=="walkCycleLength") {
			is>>walkCycleLength;
		}

		else if (s=="attackCycleLength") {
			is>>attackCycleLength;
		}

		else if (s=="attackCycleStart") {
			is>>attackCycleStart;
		}

		else if (s=="deathCycleLength") {
			is>>deathCycleLength;
		}

		else if (s=="deathCycleStart") {
			is>>deathCycleStart;
		}

		else if (s=="deathCycleVertical") {
			is>>deathCycleVertical;
		}

		else if (s=="}") {
			if (hasSpritesheet) {
				spritesheet = gResourceManager->get(spritefilename, teamColor);
			}
			if (hasShadowsheet)
				shadowsheet = gResourceManager->get(shadowfilename, teamColor);
			return;
		}
		else{
			Logging::error("Error: Drawer constructor ifstream contained unknown token:"+s);
		}
	}
}

Drawer::~Drawer() {
}

void Drawer::draw(SDL_Renderer* renderer, Unit* unit, UserInterface* ui, int alphaMulti /*, Coordinate cameraposition */) {
	// Draws the unit to the given surface.
	//spritesheet->render(renderer, 0, 0 , unit->xy.x, unit->xy.y);
	
	this->spritesheet->setAlphaMod(alphaMulti);

	int dy = unit->dimension.air ? -AIRBORNE_RENDER_HEIGHT : 0;
	dy += (unit->animationState == AnimationState::Dying && unit->dimension.air) ? 0.03*(unit->drawAnimationStep+2)*(unit->drawAnimationStep+2) : 0;
	dy *= ui->viewMagnification;
	
	Coordinate pos = ui->screenCoordinateFromObjective(unit->xy);

	switch (unit->animationState) {
		case AnimationState::Idle: {
			this->drawIdle(renderer, unit->drawFacingAngle, ui, pos, positiveRemainder(unit->drawAnimationStep, this->idleCycleLength), dy);
			break;
		}
		case AnimationState::Walking: {
			this->drawWalking(renderer, unit->drawFacingAngle, ui, pos, positiveRemainder(unit->drawAnimationStep, this->walkCycleLength), dy);
			break;
		}
		case AnimationState::Attacking:
			if (unit->drawAnimationStep < 0)
				this->drawIdle(renderer, unit->drawFacingAngle, ui, pos, positiveRemainder(unit->drawAnimationStep, this->idleCycleLength), dy);
			else
				this->drawAttacking(renderer, unit->drawFacingAngle, ui, pos, std::max(0, positiveRemainder(unit->drawAnimationStep, this->attackCycleLength) ), dy);
			break;

		case AnimationState::Dying:
			this->drawDying(renderer, unit->drawFacingAngle, ui, pos, std::min(this->deathCycleLength -1, std::abs(unit->drawAnimationStep)), dy);
			break;
	}

	this->spritesheet->resetAlphaMod();
}

void Drawer::drawIdle(SDL_Renderer* renderer, int drawFacingAngle, UserInterface* ui, Coordinate drawPos, int frame, int dy) {
	if (NULL != shadowsheet)
		shadowsheet->render(renderer,
			spriteXFromAngle(drawFacingAngle),
			this->idleCycleStart + frame,
			drawPos.x,
			drawPos.y + dy,
			ui->viewMagnification);
	if (NULL != spritesheet)
		spritesheet->render(renderer,
			spriteXFromAngle(drawFacingAngle),
			idleCycleStart + frame,
			drawPos.x,
			drawPos.y + dy,
			ui->viewMagnification);
}

void Drawer::drawWalking(SDL_Renderer* renderer, int drawFacingAngle, UserInterface* ui, Coordinate drawPos, int frame, int dy) {
	if (NULL != shadowsheet)
		shadowsheet->render(renderer,
			spriteXFromAngle(drawFacingAngle),
			this->walkCycleStart + frame,
			drawPos.x,
			drawPos.y +  dy,
			ui->viewMagnification);
	if (NULL != spritesheet)
		spritesheet->render(renderer,
			spriteXFromAngle(drawFacingAngle),
			this->walkCycleStart + frame,
			drawPos.x,
			drawPos.y + dy,
			ui->viewMagnification);
}

void Drawer::drawAttacking(SDL_Renderer* renderer, int drawFacingAngle, UserInterface* ui, Coordinate drawPos, int frame, int dy) {
	if (NULL != shadowsheet)
		shadowsheet->render(renderer,
			spriteXFromAngle(drawFacingAngle),
			this->attackCycleStart + frame,
			drawPos.x,
			drawPos.y + dy,
			ui->viewMagnification);
	if (NULL != spritesheet)
		spritesheet->render(renderer,
			spriteXFromAngle(drawFacingAngle),
			this->attackCycleStart + frame,
			drawPos.x,
			drawPos.y + dy,
			ui->viewMagnification);
}

void Drawer::drawDying(SDL_Renderer* renderer, int drawFacingAngle, UserInterface* ui, Coordinate drawPos, int frame, int dy) {
	if (this->deathCycleVertical) {
		if (NULL != spritesheet)
			spritesheet->render(renderer,
				spriteXFromAngle(drawFacingAngle),
				this->deathCycleStart + frame,
				drawPos.x,
				drawPos.y + dy,
				ui->viewMagnification);
	} else {
		if (NULL != spritesheet)
			spritesheet->render(renderer,
				frame,
				this->deathCycleStart,
				drawPos.x,
				drawPos.y + dy,
				ui->viewMagnification);
	}
}

void Drawer::drawWireframe(SDL_Renderer* renderer, Coordinate drawPos) {
	if (NULL != spritesheet)
		spritesheet->render(renderer,
			wireframeX,
			wireframeY,
			drawPos.x,
			drawPos.y,
			1.0);
}

int Drawer::spriteXFromAngle(int drawFacingAngle) {
	// Why is this so complicated? Because spritesheets only give 180 degrees of imagery.
	// Because of the above, instead of taking the angle and modding by (2*numFacingDirections), we want to take the
	// angle, mod by (2*numFacingDirections - 2), then add an offset to avoid double-counting the 180 degree image
	if (this->numFacingDirections==1) {
		return 0;
	} else{
		int x = (int)std::round(((drawFacingAngle+135+360)*(2*numFacingDirections-2)/360.0)) % (2*numFacingDirections - 2);
		if (x>=numFacingDirections)
			x++;
		return x;
	}
}

void drawHPbar(SDL_Renderer* renderer, int HP, int maxHP, Coordinate renderLocation, const float magnification, int dy /* = 0 */) {
	// Draws an HP bar centered at (renderX, renderY+dy)
	const int hpPerSection = 50;
	const int sectionWidth = 8;

	renderLocation = renderLocation + Coordinate(0, dy*magnification);

	//void renderRectFilled(SDL_Renderer* renderer, Coordinate a, Coordinate b, SDL_Color color);
	//void renderRectBorder(SDL_Renderer* renderer, Coordinate a, Coordinate b, SDL_Color color);
	//void renderLine(SDL_Renderer* renderer, Coordinate a, Coordinate b, SDL_Color color);

	int barwidth = magnification*sectionWidth*maxHP/hpPerSection;
	int barheight = magnification*10;

	int filledwidth = magnification*sectionWidth*HP/hpPerSection;

	// red background
	Coordinate bottomLeftCorner = renderLocation - Coordinate(barwidth/2, barheight/2);
	Coordinate topRightCorner = bottomLeftCorner + Coordinate(barwidth, barheight);
	renderRectFilled(renderer, bottomLeftCorner, topRightCorner, SDL_Colors::RED);

	// green filled section
	topRightCorner = bottomLeftCorner + Coordinate(filledwidth, barheight);
	renderRectFilled(renderer, bottomLeftCorner, topRightCorner, SDL_Colors::GREEN);

	// black border
	topRightCorner = bottomLeftCorner + Coordinate(barwidth, barheight);
	renderRectBorder(renderer, bottomLeftCorner, topRightCorner, SDL_Colors::BLACK);

	// lines
	for (int i=0; i<maxHP/hpPerSection; i++) {
		renderLine(renderer, bottomLeftCorner+Coordinate(magnification*sectionWidth*i, 1), bottomLeftCorner+Coordinate(magnification*sectionWidth*i, barheight-1), SDL_Colors::BLACK);
	}
}

void drawHPbar(SDL_Renderer* renderer, const Unit* unit, UserInterface* ui) {
	if (unit->animationState != AnimationState::Dying) {
		int dy = unit->dimension.air ? -AIRBORNE_RENDER_HEIGHT : 0;
		drawHPbar(renderer, unit->hp, unit->getUnitTemplate()->maxHP(), ui->screenCoordinateFromObjective(unit->xy), ui->viewMagnification, dy);
	}
}
