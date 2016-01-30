#include <SDL2/SDL.h>

#include "Drawer.hpp"
#include "Spritesheet.hpp"

Drawer::Drawer(Spritesheet* sp):
	spritesheet(sp)
{
}

Drawer::Drawer(std::ifstream& is, SDL_Renderer* renderer){
	int sw=0, sh=0, ox=0, oy=0, gx=0, gy=0;
	std::string filename;

	std::string s;

	while (is>>s){
		if (s=="#"){
			getline(is, s); // this is a comment. we ignore comments.
		}
		else if (s=="filename"){
			is>>filename;
		}
		else if (s=="size"){
			is>>sw;
			is>>sh;
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
			spritesheet = new Spritesheet(renderer, filename.c_str(), sw, sh, ox, oy, gx, gy);
			return;
		}
	}
}

void Drawer::draw(SDL_Renderer* renderer, Unit& unit /*, Coordinate cameraposition */){
	// Draws the unit to the given surface.
	spritesheet->render(renderer, unit.drawFacingAngle, unit.drawWalkStep, unit.xy.first, unit.xy.second);
}
