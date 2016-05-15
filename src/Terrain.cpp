#include "Terrain.hpp"
#include "ResourceManager.hpp"
#include "Spritesheet.hpp"
#include "globals.hpp"
#include "typedefs.hpp"
#include "UserInterface.hpp"
#include "Logging.hpp"
#include "sdlTools.hpp"
#include "InhabitedGrid.hpp"
#include "Game.hpp"

#include <SDL2/SDL.h>

Terrain::Terrain(std::string mapName) {
	SDL_Surface* image = gResourceManager->getRawSurface(mapName);
	this->minimap = gResourceManager->get(mapName);

	Uint32* pixels = (Uint32*)image->pixels;

	this->width = image->w;
	this->height = image->h;

	this->tiles.resize(this->width);
	for (int i=0; i<this->width; i++) {
		this->tiles[i].resize(this->height);
		for (int j=0; j<this->height; j++) {
			
			TerrainType tile = NONE;
			switch (pixels[i*height + (height-j-1)]) {
				case -65536:
					tile = WATER;
					break;
				case -16711936:
					tile = GRASS;
					break;
				case -8355712:
					tile = GRASS; // tile = ROAD;
					break;
				default:
					tile = NONE;
					Logging::error("in Terrain::Terrain(), encountered weird pixel value: ");
					Logging::error(pixels[i*height + (height-j-1)]);
			}
			this->tiles[i][j].terraintype = tile;
		}
	}

	for (int i=0; i<this->width; i++) {
		for (int j=0; j<this->height; j++) {
			this->updateDrawTile(i, j);
		}
	}

	SDL_FreeSurface(image);
}

TerrainType Terrain::getTerrainAt(int x, int y) {
	if (x<0 || y < 0 || x >= this->width || y>= this->height)
		return NONE;
	else
		return this->tiles[x][y].terraintype;
}

void Terrain::updateDrawTile(int x, int y) {
	if (x<0 || y < 0 || x >= this->width || y>= this->height) {
		throw;
	}

	TerrainType nw, n, ne, w, center, e, sw, s, se;
	nw = this->getTerrainAt(x-1, y+1);
	n = this->getTerrainAt(x-1, y);
	ne = this->getTerrainAt(x-1, y-1);
	w = this->getTerrainAt(x, y+1);
	center = this->getTerrainAt(x, y);
	e = this->getTerrainAt(x, y-1);
	sw = this->getTerrainAt(x+1, y+1);
	s = this->getTerrainAt(x+1, y);
	se = this->getTerrainAt(x+1, y-1);

	nw = (nw!=NONE) ? nw : center;
	n = (n!=NONE) ? n : center;
	ne = (ne!=NONE) ? ne : center;
	w = (w!=NONE) ? w : center;
	e = (e!=NONE) ? e : center;
	sw = (sw!=NONE) ? sw : center;
	s = (s!=NONE) ? s : center;
	se = (se!=NONE) ? se : center;

	int bottomX, bottomY, topX, topY;
	bottomX = -1; // if bottomX is still -1 by the end of this program, it shouldn't be drawn
	bottomY = -1;
	topX = -1; // same
	topY = -1;
	if (center == GRASS) {
		bottomX = 2+(x+y)%2, bottomY = 0;
	}

	else if (center == WATER) {
		int surroundingWaterCount = (int)(n==WATER) + (int)(w==WATER) + (int)(s==WATER) + (int)(e==WATER) + (int)(nw==WATER) + (int)(ne==WATER) + (int)(sw==WATER) + (int)(se==WATER);
		
		if (surroundingWaterCount == 8)
			bottomX = 5, bottomY = 8;
		else {
			bottomX = 0, bottomY = 2;

			if (surroundingWaterCount == 7) {
				if (ne != WATER)
					topX = 6, topY = 9;
				else if (se != WATER)
					topX = 7, topY = 9;
				else if (nw != WATER)
					topX = 8, topY = 9;
				else if (sw != WATER)
					topX = 9, topY = 9;
			}

			else if (surroundingWaterCount == 4 && n != WATER && w != WATER && s != WATER && e != WATER) 
				topX=2, topY=10;

			else if (n == WATER && e == WATER && w != WATER && s != WATER
			&& ne != WATER )
				topX = 0, topY = 9;
			else if (n == WATER && e != WATER && w == WATER && s != WATER
			&& nw != WATER )
				topX=4, topY=9;
			else if (n != WATER && e == WATER && w != WATER && s == WATER
			&& se != WATER )
				topX=3, topY=9;
			else if (n != WATER && e != WATER && w == WATER && s == WATER
			&& sw != WATER )
				topX=5, topY=9;

			else if (n == WATER && s == WATER && e != WATER && w != WATER)
				topX=1, topY=9;
			else if (n != WATER && s != WATER && e == WATER && w == WATER)
				topX=2, topY=9;

			else if (n != WATER && nw != WATER && w != WATER)
				topX=3, topY=8; //resource = "tile-ocean-grass-NW";
			else if (n != WATER && ne != WATER && e != WATER)
				topX=9, topY=8; //resource = "tile-ocean-grass-NE";
			else if (s != WATER && sw != WATER && w != WATER)
				topX=0, topY=8; //resource = "tile-ocean-grass-SW";
			else if (s != WATER && se != WATER && e != WATER)
				topX=6, topY=8; //resource = "tile-ocean-grass-SE";

			else if (n != WATER)
				topX=7, topY=8;//resource = "tile-ocean-grass-N";
			else if (e != WATER)
				topX=8, topY=8;//resource = "tile-ocean-grass-E";
			else if (s != WATER)
				topX=2, topY=8;//resource = "tile-ocean-grass-S";
			else if (w != WATER)
				topX=1, topY=8;//resource = "tile-ocean-grass-W";

		}
		/*
	}

	else if (center == ROAD) {
		if (n == ROAD && s == ROAD && e == ROAD && w == ROAD
		&& ( (int(nw == ROAD) + (int)(ne == ROAD) + (int)(sw == ROAD) + (int)(se == ROAD)) >=3 ) )
			resource = "tile-road";

		else if (n == ROAD && s == ROAD && e == ROAD && w == ROAD
		&& nw == GRASS && ne == GRASS && sw == GRASS && se == GRASS)
			resource = "tile-road-grass-4waycross";
		
		else if (n == GRASS && s == ROAD && e == ROAD && w == ROAD
		&& sw == GRASS && se == GRASS)
			resource = "tile-road-grass-3crossN";
		else if (n == ROAD && s == GRASS && e == ROAD && w == ROAD
		&& nw == GRASS && ne == GRASS)
			resource = "tile-road-grass-3crossS";
		else if (n == ROAD && s == ROAD && e == GRASS && w == ROAD
		&& nw == GRASS && sw == GRASS)
			resource = "tile-road-grass-3crossE";
		else if (n == ROAD && s == ROAD && e == ROAD && w == GRASS
		&& ne == GRASS && se == GRASS)
			resource = "tile-road-grass-3crossW";

		else if (n == ROAD && s == ROAD && e == ROAD && w == ROAD
		&& nw == ROAD && ne == GRASS && sw == ROAD && se == GRASS)
			resource = "tile-road-grass-2cornerE";
		else if (n == ROAD && s == ROAD && e == ROAD && w == ROAD
		&& nw == GRASS && ne == ROAD && sw == GRASS && se == ROAD)
			resource = "tile-road-grass-2cornerW";
		else if (n == ROAD && s == ROAD && e == ROAD && w == ROAD
		&& nw == GRASS && ne == GRASS && sw == ROAD && se == ROAD)
			resource = "tile-road-grass-2cornerN";
		else if (n == ROAD && s == ROAD && e == ROAD && w == ROAD
		&& nw == ROAD && ne == ROAD && sw == GRASS && se == GRASS)
			resource = "tile-road-grass-2cornerS";
		
		else if (n != ROAD && e != ROAD && s == ROAD && w == ROAD && sw == ROAD)
			resource = "tile-road-grass-cornerNE";
		else if (n != ROAD && e == ROAD && s == ROAD && w != ROAD && se == ROAD)
			resource = "tile-road-grass-cornerNW";
		else if (n == ROAD && e != ROAD && s != ROAD && w == ROAD && nw == ROAD)
			resource = "tile-road-grass-cornerSE";
		else if (n == ROAD && e == ROAD && s != ROAD && w != ROAD && ne == ROAD)
			resource = "tile-road-grass-cornerSW";

		else if (n != ROAD && w == ROAD && s == ROAD && e == ROAD)
			resource = "tile-road-grassN";
		else if (n == ROAD && w != ROAD && s == ROAD && e == ROAD)
			resource = "tile-road-grassW";
		else if (n == ROAD && w == ROAD && s != ROAD && e == ROAD)
			resource = "tile-road-grassS";
		else if (n == ROAD && w == ROAD && s == ROAD && e != ROAD)
			resource = "tile-road-grassE";
		
		else if (n != ROAD && s != ROAD && e != ROAD && w == ROAD)
			resource = "tile-road-grass-endE";
		else if (n != ROAD && s != ROAD && e == ROAD && w != ROAD)
			resource = "tile-road-grass-endW";
		else if (n == ROAD && s != ROAD && e != ROAD && w != ROAD)
			resource = "tile-road-grass-endN";
		else if (n != ROAD && s == ROAD && e != ROAD && w != ROAD)
			resource = "tile-road-grass-endS";

		else if (n == ROAD && e == ROAD && s != ROAD && w != ROAD)
			resource = "tile-road-grass-turnNE";
		else if (n == ROAD && e != ROAD && s != ROAD && w == ROAD)
			resource = "tile-road-grass-turnNW";
		else if (n != ROAD && e == ROAD && s == ROAD && w != ROAD)
			resource = "tile-road-grass-turnSE";
		else if (n != ROAD && e != ROAD && s == ROAD && w == ROAD)
			resource = "tile-road-grass-turnSW";

		else if (n == ROAD && s == ROAD && e == WATER && w == WATER)
			resource = "tile-road-water-straightN";
		else if (n == WATER && s == WATER && e == ROAD && w == ROAD)
			resource = "tile-road-water-straightE";

		else if (n == ROAD && s == ROAD)
			resource = "tile-road-grass-straightN";
		else if (e == ROAD && w == ROAD)
			resource = "tile-road-grass-straightE";

		else
			resource = "tile-road";
	*/
	}

	this->tiles[x][y].spritesheet = gResourceManager->get("new-landscape-tiles");
	this->tiles[x][y].bottomX = bottomX;
	this->tiles[x][y].bottomY = bottomY;
	this->tiles[x][y].topX = topX;
	this->tiles[x][y].topY = topY;
}

void Terrain::render(SDL_Renderer* renderer, UserInterface* ui) {
	Coordinate screenCorner1(-100*ui->viewMagnification, -100*ui->viewMagnification);
	Coordinate screenCorner2(SCREEN_WIDTH+100*ui->viewMagnification, SCREEN_HEIGHT+100*ui->viewMagnification);
	for (int i=0; i<this->width; i++) {
		for (int j=0; j<this->height; j++) {
			Coordinate drawPos = ui->screenCoordinateFromObjective(Coordinate(32*PIXEL_WIDTH*i, 32*PIXEL_WIDTH*j ));
			if (coordInRect(drawPos, screenCorner1, screenCorner2)) {
				int tileColorMod = ui->game->visibilityManager.getTileVisibility(Coordinate(i, j), ui->teamID);
				this->tiles[i][j].spritesheet->setColorMod(tileColorMod, tileColorMod, tileColorMod);

				if (this->tiles[i][j].bottomX != -1)
					this->tiles[i][j].spritesheet->render(renderer, tiles[i][j].bottomX, tiles[i][j].bottomY, drawPos.x, drawPos.y +  (64-this->tiles[i][j].spritesheet->spriteH)*ui->viewMagnification/2, ui->viewMagnification);
				
				if (this->tiles[i][j].topX != -1)
					this->tiles[i][j].spritesheet->render(renderer, tiles[i][j].topX, tiles[i][j].topY, drawPos.x, drawPos.y +  (64-this->tiles[i][j].spritesheet->spriteH)*ui->viewMagnification/2, ui->viewMagnification);

				this->tiles[i][j].spritesheet->resetColorMod();			}
		}
	}
	
	
	SDL_Color gridLineColor{0, 0, 0, (unsigned char) (186.0*std::min(ui->viewMagnification*2, 1.0f) )};
	for (int i=0; i<=this->width; i++) {
		renderLine(renderer, ui->screenCoordinateFromObjective(Coordinate(64*PIXEL_WIDTH*i, -64*PIXEL_WIDTH)), ui->screenCoordinateFromObjective(Coordinate(64*PIXEL_WIDTH*i, 64*PIXEL_WIDTH*(this->width))), gridLineColor );
	}
	for (int j=0; j<=this->height; j++) {
		renderLine(renderer, ui->screenCoordinateFromObjective(Coordinate(-64*PIXEL_WIDTH, 64*PIXEL_WIDTH*j)), ui->screenCoordinateFromObjective(Coordinate(64*PIXEL_WIDTH*(this->width), 64*PIXEL_WIDTH*j)), gridLineColor );
	}
}

void Terrain::renderMinimap(SDL_Renderer* renderer, UserInterface* ui) {
	SDL_Rect target;
	target.w = 280/sqrt(2);
	target.h = 280/sqrt(2);
	target.x = 150-target.w/2;
	target.y = 750-target.h/2;
	SDL_RenderCopyEx(renderer, this->minimap->sheet, NULL, &target, -45, NULL, SDL_FLIP_NONE);
}
