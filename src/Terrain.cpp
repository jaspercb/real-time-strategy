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
	this->drawTiles.resize(this->width);
	for (int i=0; i<this->width; i++) {
		this->tiles[i].resize(this->height);
		this->drawTiles[i].resize(this->height);
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
					tile = ROAD;
					break;
				default:
					tile = NONE;
					debugLog("in Terrain::Terrain(), encountered weird pixel value: ");
					debugLog(pixels[i*height + (height-j-1)]);
			}
			this->tiles[i][j] = tile;
		}
	}

	for (int i=0; i<this->width; i++) {
		this->drawTiles[i].resize(this->height);
		for (int j=0; j<this->height; j++) {
			this->updateDrawTile(i, j);
		}
	}

	SDL_FreeSurface(image);
}

TerrainType Terrain::getTerrainAt(int x, int y) {
	if (x<0 || y < 0 || x >= this->width || x>= this->height)
		return NONE;
	else
		return this->tiles[x][y];
}

void Terrain::updateDrawTile(int x, int y) {
	if (x<0 || y < 0 || x >= this->width || x>= this->height) {
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


	std::string resource;

	if (center == GRASS)
		resource = "tile-grass";
	
	else if (center == WATER) {
		if (n == WATER && w == WATER && s == WATER && e == WATER
		&& nw == WATER && ne == WATER && sw == WATER && se == WATER)
			resource = "tile-ocean";
		
		else if (n == WATER && e == WATER && w != WATER && s != WATER
		&& ne != WATER )
			resource = "tile-river-grass-bendNE";
		else if (n == WATER && e != WATER && w == WATER && s != WATER
		&& nw != WATER )
			resource = "tile-river-grass-bendNW";
		else if (n != WATER && e == WATER && w != WATER && s == WATER
		&& se != WATER )
			resource = "tile-river-grass-bendSE";
		else if (n != WATER && e != WATER && w == WATER && s == WATER
		&& sw != WATER )
			resource = "tile-river-grass-bendSW";

		else if (n == WATER && s == WATER && e != WATER && w != WATER)
			resource = "tile-river-grass-straightN";
		else if (n != WATER && s != WATER && e == WATER && w == WATER)
			resource = "tile-river-grass-straightE";

		else if (n == GRASS && w == GRASS)
			resource = "tile-ocean-grass-NW";
		else if (n == GRASS && e == GRASS)
			resource = "tile-ocean-grass-NE";
		else if (s == GRASS && w == GRASS)
			resource = "tile-ocean-grass-SW";
		else if (s == GRASS && e == GRASS)
			resource = "tile-ocean-grass-SE";
		else if (n == GRASS)
			resource = "tile-ocean-grass-N";
		else if (e == GRASS)
			resource = "tile-ocean-grass-E";
		else if (s == GRASS)
			resource = "tile-ocean-grass-S";
		else if (w == GRASS)
			resource = "tile-ocean-grass-W";
		else if (nw == GRASS)
			resource = "tile-ocean-grass-cornerNW";
		else if (ne == GRASS)
			resource = "tile-ocean-grass-cornerNE";
		else if (sw == GRASS)
			resource = "tile-ocean-grass-cornerSW";
		else if (se == GRASS)
			resource = "tile-ocean-grass-cornerSE";
		else{
			resource = "tile-ocean";
		}
	}

	else if (center == ROAD) {
		if (n == ROAD && s == ROAD && e == ROAD && w == ROAD
		&& nw == ROAD && ne == ROAD && sw == ROAD && se == ROAD)
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


	}

	this->drawTiles[x][y] = gResourceManager->get(resource);
}

void Terrain::render(SDL_Renderer* renderer, UserInterface* ui) {
	Coordinate screenCorner1(-100*ui->viewMagnification, -100*ui->viewMagnification);
	Coordinate screenCorner2(SCREEN_WIDTH+100*ui->viewMagnification, SCREEN_HEIGHT+100*ui->viewMagnification);
	for (int i=0; i<this->width; i++) {
		for (int j=0; j<this->height; j++) {
			if (this->drawTiles[i][j]) {
				Coordinate drawPos = ui->screenCoordinateFromObjective(Coordinate(64*PIXEL_WIDTH*(i-1), 64*PIXEL_WIDTH*(j-1) ));
				if (coordInRect(drawPos, screenCorner1, screenCorner2)) {
					int timeTileSeen = ui->game.inhabitedGrid.getTileVisibilityTime(std::make_pair(i, j), ui->teamID);
					int tileColorMod = std::min(255, 64+4*timeTileSeen);
					SDL_SetTextureColorMod(this->drawTiles[i][j]->sheet, tileColorMod, tileColorMod, tileColorMod);

					this->drawTiles[i][j]->render(renderer, 0, 0, drawPos.first, drawPos.second +  (200-this->drawTiles[i][j]->spriteH)*ui->viewMagnification/2, ui->viewMagnification);
	
					SDL_SetTextureColorMod(this->drawTiles[i][j]->sheet, 255, 255, 255); // resetting color key for safety reasons
				}
			}
		}
	}
	
	SDL_Color gridLineColor{0, 0, 0, 186.0*std::min(ui->viewMagnification*2, 1.0f)};
	for (int i=-1; i<this->width; i++) {
		renderLine(renderer, ui->screenCoordinateFromObjective(Coordinate(64*PIXEL_WIDTH*i, -64*PIXEL_WIDTH)), ui->screenCoordinateFromObjective(Coordinate(64*PIXEL_WIDTH*i, 64*PIXEL_WIDTH*(this->width-1))), gridLineColor );
	}
	for (int j=-1; j<this->height; j++) {
		renderLine(renderer, ui->screenCoordinateFromObjective(Coordinate(-64*PIXEL_WIDTH, 64*PIXEL_WIDTH*j)), ui->screenCoordinateFromObjective(Coordinate(64*PIXEL_WIDTH*(this->width-1), 64*PIXEL_WIDTH*j)), gridLineColor );
	}
}

void Terrain::renderMinimap(SDL_Renderer* renderer, UserInterface* ui) {
	SDL_Rect target;
	target.x = 10+40;
	target.y = 610+40;
	target.w = 280/sqrt(2);
	target.h = 280/sqrt(2);
	SDL_RenderCopyEx(renderer, this->minimap->sheet, NULL, &target, -45, NULL, SDL_FLIP_NONE);
}