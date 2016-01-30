#include "Drawer.hpp"
#include "Spritesheet.hpp"

Drawer::Drawer(Spritesheet* sp):
	spritesheet(sp)
{
}

void Drawer::draw(SDL_Renderer* renderer, Unit& unit /*, Coordinate cameraposition */){
	// Draws the unit to the given surface.
	spritesheet->render(renderer, unit.drawFacingAngle, unit.drawWalkStep, unit.xy.first, unit.xy.second);
}
