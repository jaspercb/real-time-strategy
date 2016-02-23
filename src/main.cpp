#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "globals.hpp"

#include "Game.hpp"
#include "UserInterface.hpp"
#include "Unit.hpp"
#include "Logging.hpp"
#include "Drawer.hpp"
#include "ResourceManager.hpp"
#include "FontManager.hpp"

const int SCREEN_WIDTH = 1600;
const int SCREEN_HEIGHT = 900;
const int FRAMERATE = 20;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
ResourceManager* gResourceManager = NULL;
FontManager* gFontManager = NULL;

bool init() {
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Create window
		gWindow = SDL_CreateWindow( "I totally know how SDL works", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL ) {
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
			success = false;
		}
		else {
			//Get window surface
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
		}
	}

	gResourceManager = new ResourceManager();
	gFontManager = new FontManager();

	return success;
}

void cleanup_SDL() {
	delete gResourceManager;
	delete gFontManager;

	SDL_DestroyWindow( gWindow );
	SDL_Quit();
}

int main() {
	debugLog(" Testing...");

	//Start up SDL and create window
	if( !init() ) {
		printf( "Failed to initialize!\n" );
	}
	else {
		Game g = Game();

		TeamID tID1 = g.createTeam();
		TeamID tID2 = g.createTeam();

		UserInterface userInterface = UserInterface(g, tID1);

		Team& t1 = g.getTeam(tID1);
		Team& t2 = g.getTeam(tID2);
		
		t1.loadUnitTemplate("../conf/units/townhall");
		t1.loadUnitTemplate("../conf/units/mutalisk");

		t2.loadUnitTemplate("../conf/units/marine");
		
		//UnitTemplate& p1 = t1.unitTemplates.begin()->second;
		//UnitTemplate& p2 = t2.unitTemplates.begin()->second;

		g.createUnit(tID1, "../conf/units/townhall", Coordinate(-5000, -5000));
		
		for (int i=5; i<15; i++) {
			for (int j=5; j<15; j++) {
				//g.createUnit(tID2, (UnitTemplateID)3, Coordinate(50*i, 50*j));
				g.createUnit(tID1, "../conf/units/mutalisk", Coordinate(2500*i, 2500*j));
			}
		}
		

		for (int i=5; i<15; i++) {
			for (int j=5; j<15; j++) {
				//g.createUnit(tID2, (UnitTemplateID)3, Coordinate(50*i, 50*j));
				g.createUnit(tID2, "../conf/units/marine", Coordinate(2500*16 + 2500*i, 2500*16 + 2500*j));
			}
		}

		//Command cmd1(CMD_GOTOCOORD);
		//cmd1.targetID = b;

		//cmd1.queueSetting=QUEUE_OVERWRITE;

		//g.getUnit(a).handleCommand(cmd1);

		//Command cmd2(CMD_GOTOCOORD);
		//cmd2.targetCoord = Coordinate(300, 300);
		//cmd2.queueSetting=QUEUE_OVERWRITE;

		//g.getUnit(b).handleCommand(cmd2);


		for (int i=0; i<2000; i++) {

			if (userInterface.handleInputEvents() ) { // returns 1 if should switch
				break;
			}

			g.tick();
			
			userInterface.tick();

			userInterface.renderAll( gRenderer );
			
			SDL_Delay( 1000/FRAMERATE );
		}
	}

	//Free resources and close SDL
	cleanup_SDL();

	debugLog(" Done testing.");
	
	return 0;
}