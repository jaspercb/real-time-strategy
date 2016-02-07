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
	return success;
}

void cleanup_SDL()
{
	SDL_DestroyWindow( gWindow );
	SDL_Quit();
}

void draw_all(Game &g){
	SDL_SetRenderDrawColor(gRenderer, 128, 128, 128, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(gRenderer);

	for (auto &u : g.unitsByID){
		u.second.draw(gRenderer);
	}
}

int main(){
	debugLog(" Testing...");

	//Start up SDL and create window
	if( !init() ) {
		printf( "Failed to initialize!\n" );
	}
	else {
		Game g = Game();
		TeamID tID1 = g.createTeam();
		TeamID tID2 = g.createTeam();

		Team& t1 = g.getTeam(tID1);
		Team& t2 = g.getTeam(tID2);
		
		t1.loadUnitTemplate("../conf/units/marine");
		t2.loadUnitTemplate("../conf/units/ultralisk");
		//UnitTemplate& p1 = t1.unitTemplates.begin()->second;
		//UnitTemplate& p2 = t2.unitTemplates.begin()->second;

		UnitID a,b;
		a = g.createUnit(tID1, (UnitTemplateID)3, Coordinate(100, 100));
		b = g.createUnit(tID2, (UnitTemplateID)3, Coordinate(150, 150));

		//Command cmd1(CMD_GOTOCOORD);
		//cmd1.targetID = b;

		//cmd1.queueSetting=QUEUE_OVERWRITE;

		//g.getUnit(a).handleCommand(cmd1);

		Command cmd2(CMD_GOTOCOORD);
		cmd2.targetCoord = Coordinate(300, 300);
		cmd2.queueSetting=QUEUE_OVERWRITE;

		g.getUnit(b).handleCommand(cmd2);


		bool quit = false;

		bool drawSelectionBox = false;
		Coordinate selectionBoxCorner1;
		Coordinate selectionBoxCorner2;
		std::vector<UnitID> unitsInSelectionBox;

		for (int i=0; i<200; i++){
			if (quit)
				break;

			SDL_Event event;
			std::cout<<selectionBoxCorner1.first<<","<<selectionBoxCorner1.second<<std::endl;
			std::cout<<selectionBoxCorner2.first<<","<<selectionBoxCorner2.second<<std::endl;
			std::cout<<unitsInSelectionBox.size()<<std::endl;
			std::cout<<std::endl;
			while(SDL_PollEvent(&event)) {
				if (event.type == SDL_QUIT) {
					quit = true;
					break;
				}
				else if (event.type == SDL_MOUSEBUTTONDOWN) {
					if (event.button.button == SDL_BUTTON_RIGHT) {
						Command movecmd;
						movecmd.targetCoord = Coordinate(event.button.x, event.button.y);
						movecmd.queueSetting = QUEUE_OVERWRITE;
						for (auto &i : unitsInSelectionBox) {
							std::cout<<"MOVE COMMAND"<<std::endl;
							g.getUnit(i).handleCommand(movecmd);
						}
					}
					else if (event.button.button=SDL_BUTTON_LEFT) {
						selectionBoxCorner1=Coordinate(event.button.x, event.button.y);
						drawSelectionBox = true;
					}
				}
				else if (event.type == SDL_MOUSEBUTTONUP) {
					if (event.button.button == SDL_BUTTON_LEFT) {
						drawSelectionBox = false;
					}
				}
				else if (event.type == SDL_MOUSEMOTION) {
					if (SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(1)) { // if left mouse button is being held down
						selectionBoxCorner2 = Coordinate(event.button.x, event.button.y); 

						unitsInSelectionBox.clear();
						unitsInSelectionBox = g.inhabitedGrid.unitsInRectangle(selectionBoxCorner1, selectionBoxCorner2);
					}
				}
			}

			g.tick();
			draw_all(g);
			if (drawSelectionBox){
				SDL_Rect selectionBox;
				selectionBox.x = selectionBoxCorner1.first;
				selectionBox.y = selectionBoxCorner1.second;
				selectionBox.w = selectionBoxCorner2.first - selectionBoxCorner1.first;
				selectionBox.h = selectionBoxCorner2.second - selectionBoxCorner1.second;
				SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
				if (!SDL_RenderDrawRect(gRenderer, &selectionBox))
					debugLog(SDL_GetError());
			}
			SDL_RenderPresent( gRenderer );
			SDL_Delay( 1000/FRAMERATE );
		}
	}

	//Free resources and close SDL
	cleanup_SDL();

	debugLog(" Done testing.");
	
	return 0;
}