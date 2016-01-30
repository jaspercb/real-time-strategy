#include <SDL2/SDL.h>

#include "Game.hpp"
#include "Unit.hpp"
#include "Logging.hpp"
#include "Drawer.hpp"

const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 480;

//The window we'll be rendering to
SDL_Window* gWindow = NULL;
	
//The surface contained by the window
SDL_Surface* gScreen = NULL;

//The image we will load and show on the screen
SDL_Surface* gHelloWorld = NULL;

bool init()
{
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
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Get window surface
			gScreen = SDL_GetWindowSurface( gWindow );
		}
	}

	return success;
}

void cleanup_SDL()
{
	SDL_DestroyWindow( gWindow );
	SDL_Quit();
}


int main(){
	debugLog(" Testing...");
	Game g = Game();
	TeamID tID = g.createTeam();

	//WeaponTemplate wpntmpl = WeaponTemplate("TestWeapon", DMG_KINETIC, 10, 2, 5, 1, 1, GROUND_ONLY);
	//std::vector<WeaponTemplate> tmplv;
	//tmplv.push_back(wpntmpl);
	//UnitTemplate p = UnitTemplate("TestUnit", 50, 1, 1, GROUND_ONLY, tmplv);
	UnitTemplate p = UnitTemplate(std::ifstream("../conf/units/marine1"));

	Team& t = g.getTeam(tID);

	t.unitTemplates.emplace((UnitTemplateID)3, p);

	g.createUnit(tID, (UnitTemplateID)3, Coordinate(0, 0));
	
	Unit& a = g.getUnit(0);

	debugLog(a);
	a.attack(a);
	debugLog(a);

	debugLog(" Done testing.");
	
		//Spritesheet a = Spritesheet(sdl_suface, 40, 36, 2, 2, 3, 3);
	
	//Start up SDL and create window
	if( !init() ) {
		printf( "Failed to initialize!\n" );
	}
	else {
		Spritesheet k = Spritesheet("../resources/terran-ghost.bmp", 40, 36, 2, 2, 3, 3);
		Drawer b = Drawer(&k);
		for (int i=0; i<32; i++){
			SDL_FillRect(gScreen, NULL, 0x000000);
			b.draw(gScreen, a);
			
			//Update the surface
			SDL_UpdateWindowSurface( gWindow );

			//Wait two seconds
			SDL_Delay( 80 );

		}
	}

	//Free resources and close SDL
	cleanup_SDL();

	return 0;
}