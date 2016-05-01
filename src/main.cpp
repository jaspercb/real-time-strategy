#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "globals.hpp"

#include "Game.hpp"

#include "Animation.hpp"
#include "UserInterface.hpp"
#include "Unit.hpp"
#include "Logging.hpp"
#include "Drawer.hpp"
#include "ResourceManager.hpp"
#include "FontManager.hpp"
#include "Team.hpp"

const int SCREEN_WIDTH = 1600;
const int SCREEN_HEIGHT = 900;
const int GAME_TICKS_PER_SECOND = 20; // simulation framerate
const int UI_FRAMES_PER_GAMETICK = 3; // UI frames per simulation frame

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
ResourceManager* gResourceManager = NULL;
FontManager* gFontManager = NULL;
UserInterface* gUserInterface = NULL;

Game* game = NULL;

bool init() {
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		success = false;
	}
	else {
		//Create window
		gWindow = SDL_CreateWindow( "real time strategy prototype", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL ) {
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
			success = false;
		}
		else {
			//Get window surface
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
		}
	}

	gResourceManager = new ResourceManager();
	gFontManager = new FontManager();

	game = new Game();

	return success;
}

void cleanup_SDL() {
	delete gResourceManager;
	delete gFontManager;
	delete gUserInterface;
	delete game;

	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow( gWindow );
	SDL_Quit();
}

int main() {
	Logging::log("Testing...");

	//Start up SDL and create window
	if( !init() ) {
		printf( "Failed to initialize!\n" );
	}
	else {
		TeamID tID1 = game->createTeam();
		TeamID tID2 = game->createTeam();

		gUserInterface = new UserInterface(game, tID1);

		Team* t1 = game->getTeam(tID1);
		Team* t2 = game->getTeam(tID2);

		t1->loadUnitTemplate("../conf/units/arcology");
		t1->loadUnitTemplate("../conf/units/bomber");

		t2->loadUnitTemplate("../conf/units/ground-dummy");

		game->createUnit(tID1, "../conf/units/arcology", Coordinate(2*32*PIXEL_WIDTH, 2*32*PIXEL_WIDTH));

		for (int i=5; i<15; i++) {
			for (int j=5; j<15; j++) {
				game->createUnit(tID1, "../conf/units/bomber", Coordinate(2500*i, 2500*j));
			}
		}


		for (int i=5; i<15; i++) {
			for (int j=5; j<15; j++) {
				game->createUnit(tID2, "../conf/units/ground-dummy", Coordinate(2500*16 + 2500*i, 2500*16 + 2500*j));
			}
		}

		bool exit;

		while (!exit) {
			for (int j=0; j<UI_FRAMES_PER_GAMETICK; j++) {
				exit=gUserInterface->handleInputEvents(); // returns 1 if quit command processed

				gUserInterface->tick();
				gUserInterface->renderAll( gRenderer );
				SDL_Delay( 1000/ (GAME_TICKS_PER_SECOND*UI_FRAMES_PER_GAMETICK) );
			}
			game->tick();
		}
	}

	//Free resources and close SDL
	cleanup_SDL();

	Logging::log("Done testing");
	
	return 0;
}
