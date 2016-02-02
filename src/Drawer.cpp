#include <SDL2/SDL.h>

#include "Drawer.hpp"
#include "Spritesheet.hpp"
#include "Logging.hpp"

Drawer::Drawer(Spritesheet* sp):
	spritesheet(sp)
{
}

Drawer::Drawer(std::ifstream& is, SDL_Renderer* renderer):
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
				spritesheet = new Spritesheet(renderer, spritefilename.c_str(), sw, sh, sx, sy, ox, oy, gx, gy, false);
			}
			if (hasShadowsheet)
				shadowsheet = new Spritesheet(renderer, shadowfilename.c_str(), shw, shh, shx, shy, ox, oy, gx, gy, true);
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
	if (true) { // eventually, if walking
		if (NULL != shadowsheet)
			shadowsheet->render(renderer,
				( (unit.drawFacingAngle+90+360)*2*numFacingDirections/360) % (2*numFacingDirections),
				walkCycleStart + std::abs(unit.drawWalkStep)%walkCycleLength,
				unit.xy.first,
				unit.xy.second);
		if (NULL != spritesheet)
			spritesheet->render(renderer,
				( (unit.drawFacingAngle+90+360)*2*numFacingDirections/360) % (2*numFacingDirections),
				walkCycleStart + std::abs(unit.drawWalkStep)%walkCycleLength,
				unit.xy.first,
				unit.xy.second);
	} else{
		if (NULL != shadowsheet)
			shadowsheet->render(renderer,
				( (unit.drawFacingAngle+90+360)*2*numFacingDirections/360) % (2*numFacingDirections),
				attackCycleStart + std::abs(unit.drawWalkStep)%attackCycleLength,
				unit.xy.first,
				unit.xy.second);		
		if (NULL != spritesheet)
			spritesheet->render(renderer,
				( (unit.drawFacingAngle+90+360)*2*numFacingDirections/360) % (2*numFacingDirections),
				attackCycleStart + std::abs(unit.drawWalkStep)%attackCycleLength,
				unit.xy.first,
				unit.xy.second);

	}
}
