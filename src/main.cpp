#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Game.hpp"
#include "Unit.hpp"
#include "Logging.hpp"
#include "Drawer.hpp"

const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 480;

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The surface contained by the window
SDL_Renderer* gRenderer = NULL;

//The image we will load and show on the screen
SDL_Surface* gHelloWorld = NULL;

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
		UnitTemplate p = UnitTemplate(std::ifstream("../conf/units/marine1"), gRenderer);

		Team& t = g.getTeam(tID);

		t.unitTemplates.emplace((UnitTemplateID)3, p);

		//Unit& a = g.getUnit(g.createUnit(tID, (UnitTemplateID)3, Coordinate(0, 0)));
		//Unit& b = g.getUnit(g.createUnit(tID, (UnitTemplateID)3, Coordinate(0, 300)));
		//Unit& c = g.getUnit(g.createUnit(tID, (UnitTemplateID)3, Coordinate(300, 300)));
		Unit& d = g.getUnit(g.createUnit(tID, (UnitTemplateID)3, Coordinate(0, 0)));

//		debugLog(a);
//		a.attack(a);
//		debugLog(a);


		debugLog(" Done testing.");
		
		//Spritesheet a = Spritesheet(sdl_suface, 40, 36, 2, 2, 3, 3);
		
		for (int i=0; i<30; i++){
			d.drawFacingAngle +=1;
			d.drawFacingAngle %= 18;
			draw_all(g);
			SDL_Delay( 70 );
		}

		std::pair<int, int> target(0, 0);
		for (int i=0; i<5; i++){
			target = std::pair<int,int>((target.first+233) % 50, (target.second + 66) % 50);
			for (int j=0; j<16; j++) {
				//a.move_towards(std::pair<int, int>(0, 300));
				//b.move_towards(std::pair<int, int>(300, 300));
				//c.move_towards(std::pair<int, int>(0, 0));
				d.move_towards(target);
				draw_all(g);
				//Wait a few seconds
				SDL_Delay( 70 );
			}
		}
	}

	//Free resources and close SDL
	cleanup_SDL();

	return 0;
}