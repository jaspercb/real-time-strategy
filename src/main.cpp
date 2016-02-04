#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Game.hpp"
#include "Unit.hpp"
#include "Logging.hpp"
#include "Drawer.hpp"
#include "globals.hpp"

const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 480;
const int FRAMERATE = 20;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

bool init() {
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Create window
		gWindow = SDL_CreateWindow( "I totally know how SDL works", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Get window surface
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
		}
	}

	SDL_SetRenderDrawColor(gRenderer, 128, 128, 128, 255);

	return success;
}

void cleanup_SDL()
{
	SDL_DestroyWindow( gWindow );
	SDL_Quit();
}

void draw_all(Game &g){
	SDL_RenderClear(gRenderer);

	for (auto &u : g.unitsByID){
		u.second.draw(gRenderer);
	}
	SDL_RenderPresent( gRenderer );
}

int main(){
	debugLog(" Testing...");

	//Start up SDL and create window
	if( !init() ) {
		printf( "Failed to initialize!\n" );
	}
	else {
		Game g = Game();
		TeamID tID = g.createTeam();
		Team& t = g.getTeam(tID);
		t.loadUnitTemplate("../conf/units/broodling");
		UnitTemplate& p = t.unitTemplates.begin()->second;

		//Unit& a = g.getUnit(g.createUnit(tID, (UnitTemplateID)3, Coordinate(0, 0)));
		//Unit& b = g.getUnit(g.createUnit(tID, (UnitTemplateID)3, Coordinate(0, 300)));
		//Unit& c = g.getUnit(g.createUnit(tID, (UnitTemplateID)3, Coordinate(300, 300)));
		Unit& a = g.getUnit(g.createUnit(tID, (UnitTemplateID)3, Coordinate(0, 0)));
		Unit& b = g.getUnit(g.createUnit(tID, (UnitTemplateID)3, Coordinate(100, 100)));

		std::pair<int, int> target1(0, 0), target2(0, 0);
		for (int i=0; i<5; i++){
			target1 = std::pair<int,int>((target1.first+233) % 150, (target1.second + 66) % 150);
			target2 = std::pair<int,int>((target2.first+273) % 200, (target2.second + 133) % 150);
			for (int j=0; j<16; j++) {
				//a.move_towards(std::pair<int, int>(0, 300));
				//b.move_towards(std::pair<int, int>(300, 300));
				//c.move_towards(std::pair<int, int>(0, 0));
				g.getUnit(0).move_towards(target1);
				//g.getUnit(1).move_towards(target2);
				g.tick();
				draw_all(g);
				b.hp-=5;
				// framerate
				SDL_Delay( 1000/FRAMERATE );
			}
		}
	}

	//Free resources and close SDL
	cleanup_SDL();

	debugLog(" Done testing.");
	
	return 0;
}