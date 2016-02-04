#include <SDL2/SDL.h>

#include "Drawer.hpp"
#include "Spritesheet.hpp"
#include "Logging.hpp"
#include "UnitTemplate.hpp"
#include "globals.hpp"

SDL_Texture* hpBarEmpty = NULL;
SDL_Texture* hpBarFull = NULL;

Drawer::Drawer(Spritesheet* sp):
	spritesheet(sp)
{
}
Drawer::Drawer(std::ifstream& is):
	walkCycleStart(0),
	attackCycleStart(0),
	walkCycleLength(0),
	attackCycleLength(0),
	spritesheet(NULL),
	shadowsheet(NULL)
{
	int sw=0, sh=0, ox=0, oy=0, gx=0, gy=0, sx=0, sy=0, shh=0, shw=0, shx=0, shy=0;
	std::string spritefilename, shadowfilename;
	bool hasShadowsheet=false, hasSpritesheet=false;
	std::string s;

	while (is>>s){
		if (s=="#"){
			getline(is, s); // this is a comment. we ignore comments.
		}
		else if (s=="spritesheet"){
			is>>spritefilename;
			hasSpritesheet=true;
		}

		else if (s=="shadowsheet"){
			is>>shadowfilename;
			hasShadowsheet=true;
		}
		
		else if (s=="numFacingDirections"){
			is>>numFacingDirections;
		}

		else if (s=="walkCycleStart"){
			is>>walkCycleStart;
		}

		else if (s=="walkCycleLength"){
			is>>walkCycleLength;
		}

		else if (s=="attackCycleLength"){
			is>>attackCycleLength;
		}

		else if (s=="attackCycleStart"){
			is>>attackCycleStart;
		}

		else if (s=="deathCycleLength"){
			is>>deathCycleLength;
		}

		else if (s=="deathCycleStart"){
			is>>deathCycleStart;
		}

		else if (s=="spritesize"){
			is>>sw;
			is>>sh;
		}

		else if (s=="shadowsize"){
			is>>shw;
			is>>shh;
		}

		else if (s=="spritesheetsize"){
			is>>sx;
			is>>sy;
		}

		else if (s=="shadowsheetsize"){
			is>>shx;
			is>>shy;
		}

		else if (s=="offset"){
			is>>ox;
			is>>oy;
		}

		else if (s=="gap"){
			is>>gx;
			is>>gy;
		}

		else if (s=="}"){
			if (hasSpritesheet) {
				spritesheet = new Spritesheet(gRenderer, spritefilename.c_str(), sw, sh, sx, sy, ox, oy, gx, gy, false);
			}
			if (hasShadowsheet)
				shadowsheet = new Spritesheet(gRenderer, shadowfilename.c_str(), shw, shh, shx, shy, ox, oy, gx, gy, true);
			return;
		}
		else{
			debugLog("Error: Drawer constructor passed ifstream with unrecognized label:"+s);
		}
	}
}

void Drawer::draw(SDL_Renderer* renderer, Unit& unit /*, Coordinate cameraposition */){
	// Draws the unit to the given surface.
	//spritesheet->render(renderer, 0, 0 , unit.xy.first, unit.xy.second);
	switch (unit.animationState) {
		case ANIMSTATE_WALKING:
			if (NULL != shadowsheet)
				shadowsheet->render(renderer,
					( (unit.drawFacingAngle+90+360)*2*numFacingDirections/360) % (2*numFacingDirections),
					walkCycleStart + std::abs(unit.drawAnimationStep)%walkCycleLength,
					unit.xy.first,
					unit.xy.second);
			if (NULL != spritesheet)
				spritesheet->render(renderer,
					( (unit.drawFacingAngle+90+360)*2*numFacingDirections/360) % (2*numFacingDirections),
					walkCycleStart + std::abs(unit.drawAnimationStep)%walkCycleLength,
					unit.xy.first,
					unit.xy.second);
			break;

		case ANIMSTATE_ATTACKING:
			if (NULL != shadowsheet)
				shadowsheet->render(renderer,
					( (unit.drawFacingAngle+90+360)*2*numFacingDirections/360) % (2*numFacingDirections),
					attackCycleStart + std::abs(unit.drawAnimationStep)%attackCycleLength,
					unit.xy.first,
					unit.xy.second);		
			if (NULL != spritesheet)
				spritesheet->render(renderer,
					( (unit.drawFacingAngle+90+360)*2*numFacingDirections/360) % (2*numFacingDirections),
					attackCycleStart + std::abs(unit.drawAnimationStep)%attackCycleLength,
					unit.xy.first,
					unit.xy.second);
			break;

		case ANIMSTATE_DYING:
			spritesheet->render(renderer,
				unit.drawAnimationStep < deathCycleLength ? unit.drawAnimationStep : deathCycleLength-1,
				deathCycleStart,
				unit.xy.first,
				unit.xy.second);
			break;
	}
	if (unit.animationState!=ANIMSTATE_DYING){
		draw_HP_bar(renderer, unit, unit.xy.first, unit.xy.second);
	}
}

void draw_HP_bar(SDL_Renderer* renderer, Unit& unit, const int renderX, const int renderY){
	// Draws an HP bar centered at (renderX, renderY)
	const int granularity = 25;

	int HP = unit.hp;
	int maxHP = unit.getUnitTemplate().maxHP();

	if (hpBarEmpty==NULL)
		hpBarEmpty=loadTexture(renderer, "../resources/graphics/ui/tpbrempt.png");
	if (hpBarFull==NULL)
		hpBarFull=loadTexture(renderer, "../resources/graphics/ui/tpbrfull.png");

	SDL_Rect fullclip, emptyclip, tclip;

	int barlength = 5+3*maxHP/granularity;

	fullclip.w = 3 + 3*HP/granularity;
	fullclip.h = 10;
	fullclip.x = 0;
	fullclip.y = 0;
	
	emptyclip.w = barlength-fullclip.w;
	emptyclip.h = 10;
	emptyclip.x = 107-emptyclip.w;
	emptyclip.y = 0;

	tclip.w = 3 + 3*HP/granularity;
	tclip.h = 10;
	tclip.x=renderX-(barlength)/2;
	tclip.y=renderY-20;
	
	SDL_RenderCopy(renderer, hpBarFull, &fullclip, &tclip);
	tclip.x+=fullclip.w;
	tclip.w=emptyclip.w;
	SDL_RenderCopy(renderer, hpBarEmpty, &emptyclip, &tclip);
}
