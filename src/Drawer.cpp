#include <SDL2/SDL.h>

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

Drawer::Drawer(std::ifstream& is, TeamColor teamColor):
	idleCycleStart(0),
	idleCycleLength(1),
	walkCycleStart(0),
	walkCycleLength(0),
	attackCycleStart(0),
	attackCycleLength(0),
	deathCycleStart(0),
	deathCycleLength(0),
	deathCycleVertical(false),
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
			debugLog("Error: Drawer constructor ifstream contained unknown token:"+s);
		}
	}
}

Drawer::~Drawer() {
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

void Drawer::draw(SDL_Renderer* renderer, Unit& unit, UserInterface* ui, int alphaMulti /*, Coordinate cameraposition */) {
	// Draws the unit to the given surface.
	//spritesheet->render(renderer, 0, 0 , unit.xy.x, unit.xy.y);
	
	SDL_SetTextureAlphaMod( this->spritesheet->sheet, alphaMulti);

	int dy = unit.dimension.air ? -AIRBORNE_RENDER_HEIGHT : 0;
	dy += (unit.animationState == ANIMSTATE_DYING && unit.dimension.air) ? 0.03*(unit.drawAnimationStep+2)*(unit.drawAnimationStep+2) : 0;
	dy *= ui->viewMagnification;
	
	Coordinate pos = ui->screenCoordinateFromObjective(unit.xy);

	switch (unit.animationState) {
		case ANIMSTATE_IDLE: {
			this->drawIdle(renderer, unit.drawFacingAngle, ui, pos, positiveRemainder(unit.drawAnimationStep, this->idleCycleLength), dy);
			break;
		}
		case ANIMSTATE_WALKING: {
			this->drawWalking(renderer, unit.drawFacingAngle, ui, pos, positiveRemainder(unit.drawAnimationStep, this->walkCycleLength), dy);
			break;
		}
		case ANIMSTATE_ATTACKING:
			if (unit.drawAnimationStep < 0)
				this->drawIdle(renderer, unit.drawFacingAngle, ui, pos, positiveRemainder(unit.drawAnimationStep, this->idleCycleLength), dy);
			else
				this->drawAttacking(renderer, unit.drawFacingAngle, ui, pos, std::max(0, positiveRemainder(unit.drawAnimationStep, this->attackCycleLength) ), dy);
			break;

		case ANIMSTATE_DYING:
			this->drawDying(renderer, unit.drawFacingAngle, ui, pos, std::min(this->deathCycleLength -1, std::abs(unit.drawAnimationStep)), dy);
			break;
	}

	SDL_SetTextureAlphaMod( this->spritesheet->sheet, alphaMulti);
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

void drawHPbar(SDL_Renderer* renderer, int HP, int maxHP, const Coordinate renderLocation, const float magnification, int dy /* = 0 */) {
	// Draws an HP bar centered at (renderX, renderY+dy)
	const int granularity = 50;

	SDL_Rect fullclip, emptyclip, tclip;

	int renderX = renderLocation.x;
	int renderY = renderLocation.y;

	int barlength = 5+6*maxHP/granularity;

	fullclip.w = 3 + 6*HP/granularity;
	fullclip.h = 10;
	fullclip.x = 0;
	fullclip.y = 0;

	emptyclip.w = barlength-fullclip.w;
	emptyclip.h = 10;
	emptyclip.x = 107-emptyclip.w;
	emptyclip.y = 0;

	tclip.w = magnification*(3 + 6*HP/granularity);
	tclip.h = magnification*10;
	tclip.x=renderX-(magnification*barlength)/2;
	tclip.y=renderY+ (dy - 25) * magnification;
	
	SDL_RenderCopy(renderer, gResourceManager->get("hpbar-full", COLOR_NULL)->sheet, &fullclip, &tclip);
	tclip.x += magnification*fullclip.w;
	tclip.w = magnification*emptyclip.w;
	SDL_RenderCopy(renderer, gResourceManager->get("hpbar-empty", COLOR_NULL)->sheet, &emptyclip, &tclip);
}

void drawHPbar(SDL_Renderer* renderer, const Unit& unit, UserInterface* ui) {
	if (unit.animationState != ANIMSTATE_DYING) {
		int dy = unit.dimension.air ? -AIRBORNE_RENDER_HEIGHT : 0;
		drawHPbar(renderer, unit.hp, unit.getUnitTemplate().maxHP(), ui->screenCoordinateFromObjective(unit.xy), ui->viewMagnification, dy);
	}
}
