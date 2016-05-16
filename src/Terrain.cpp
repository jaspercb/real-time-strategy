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

#define SAME_OR_ANY(a, b) ( (a==TerrainType::Any) or (a==TerrainType::NotWater and b!=TerrainType::Water) or (a==b) )

#define MATCH_TERRAIN(a, b, c, d, e_, f, g, h, i) ( SAME_OR_ANY(a, nw) and SAME_OR_ANY(b, n) and SAME_OR_ANY(c, ne) and  SAME_OR_ANY(d, w) and SAME_OR_ANY(e_, center) and SAME_OR_ANY(f, e) and SAME_OR_ANY(g, sw) and SAME_OR_ANY(h, s) and SAME_OR_ANY(i, se))
// center, nw, n, ne, e, se, s, sw, w
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
			
			TerrainType::Enum tile = TerrainType::Invalid;
			switch (pixels[i*height + (height-j-1)]) {
				case -65536:
					tile = TerrainType::Water;
					break;
				case -16711936:
					tile = TerrainType::Grass;
					break;
				case -8355712:
					tile = TerrainType::Grass; // tile = TerrainType::Road;
					break;
				default:
					tile = TerrainType::Invalid;
					Logging::error("in Terrain::Terrain(), encountered weird pixel value: "+pixels[i*height + (height-j-1)]);
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

TerrainType::Enum Terrain::getTerrainAt(int x, int y) {
	if (x<0 || y < 0 || x >= this->width || y>= this->height)
		return TerrainType::Invalid;
	else
		return this->tiles[x][y].terraintype;
}

void Terrain::updateDrawTile(int x, int y) {
	using namespace TerrainType;

	if (x<0 || y < 0 || x >= this->width || y>= this->height) {
		throw;
	}

	Enum nw, n, ne, w, center, e, sw, s, se;
	nw = this->getTerrainAt(x-1, y+1);
	n = this->getTerrainAt(x-1, y);
	ne = this->getTerrainAt(x-1, y-1);
	w = this->getTerrainAt(x, y+1);
	center = this->getTerrainAt(x, y);
	e = this->getTerrainAt(x, y-1);
	sw = this->getTerrainAt(x+1, y+1);
	s = this->getTerrainAt(x+1, y);
	se = this->getTerrainAt(x+1, y-1);

	nw = (nw!=Invalid) ? nw : center;
	n = (n!=Invalid) ? n : center;
	ne = (ne!=Invalid) ? ne : center;
	w = (w!=Invalid) ? w : center;
	e = (e!=Invalid) ? e : center;
	sw = (sw!=Invalid) ? sw : center;
	s = (s!=Invalid) ? s : center;
	se = (se!=Invalid) ? se : center;

	int bottomX, bottomY, topX, topY;
	bottomX = -1; // if bottomX is still -1 by the end of this program, it shouldn't be drawn
	bottomY = -1;
	topX = -1; // same
	topY = -1;
	if MATCH_TERRAIN(Any, Any,      Any,
	                 Any, NotWater, Any,
	                 Any, Any,      Any) // (center == Grass)
		bottomX = 2+(x+y)%2, bottomY = 0;
	else if MATCH_TERRAIN(Water, Water, Water,
	                 Water, Water, Water,
	                 Water, Water, Water)
		bottomX = 5, bottomY = 8;
	else if MATCH_TERRAIN(Water, Water, NotWater,
	                 Water, Water, Water,
	                 Water, Water, Water) //if (ne != Water)
		topX = 6, topY = 9;
	else if MATCH_TERRAIN(Water, Water, Water,
	                      Water, Water, Water,
	                      Water, Water, NotWater) // (se != Water)
		topX = 7, topY = 9;
	else if MATCH_TERRAIN(NotWater, Water, Water,
	                      Water,    Water, Water,
	                      Water,    Water, Water) // (nw != Water)
		topX = 8, topY = 9;
	else if MATCH_TERRAIN(Water,    Water, Water,
	                      Water,    Water, Water,
	                      NotWater, Water, Water) //(sw != Water)
		topX = 9, topY = 9;
	//else if MATCH_TERRAIN(Water, Water, NotWater,
	//                       Water, NotWater, Water, NotWater, Water, NotWater))
	//	topX=2, topY=10;

	else if MATCH_TERRAIN(Any,      Water,    NotWater,
	                      NotWater, Water,    Water,
	                      Any,      NotWater, Any)
		topX = 0, topY = 9;
	else if MATCH_TERRAIN(NotWater, Water,    Any,
	                      Water,    Water,    NotWater,
	                      Any,      NotWater, Any)
		topX=4, topY=9;
	else if MATCH_TERRAIN(Any,      NotWater, Any,
	                      NotWater, Water,    Water,
	                      Any,      Water,    NotWater)
		topX=3, topY=9;
	else if MATCH_TERRAIN(Any,      NotWater, Any,
	                      Water,    Water,    NotWater,
	                      NotWater, Water,    Any)
		topX=5, topY=9;


	else if MATCH_TERRAIN(Any,      Water, Any,
	                      NotWater, Water, NotWater,
	                      Any,      Water, Any)
		topX=1, topY=9;
	else if MATCH_TERRAIN(Any, NotWater, Any,
	                      Water, Water, Water,
	                      Any, NotWater, Any)
		topX=2, topY=9;

	else if MATCH_TERRAIN(NotWater, NotWater, Any,
	                      NotWater, Water,    Any,
	                      Any,      Any,      Any)
		topX=3, topY=8; //resource = "tile-ocean-grass-NW";
	else if MATCH_TERRAIN(Any, NotWater, NotWater,
	                      Any, Water,    NotWater,
	                      Any, Any,      Any)
		topX=9, topY=8; //resource = "tile-ocean-grass-NE";
	else if MATCH_TERRAIN(Any,      Any,      Any,
	                      NotWater, Water,    Any,
	                      NotWater, NotWater, Any)
		topX=0, topY=8; //resource = "tile-ocean-grass-SW";
	else if MATCH_TERRAIN(Any, Any,      Any,
	                      Any, Water,    NotWater,
	                      Any, NotWater, NotWater)
		topX=6, topY=8; //resource = "tile-ocean-grass-SE";

	else if MATCH_TERRAIN(Any, NotWater, Any,
	                      Any, Water,    Any,
	                      Any, Any,      Any)
		topX=7, topY=8;//resource = "tile-ocean-grass-N";
	else if MATCH_TERRAIN(Any, Any,   Any,
	                      Any, Water, NotWater,
	                      Any, Any,   Any)
		topX=8, topY=8;//resource = "tile-ocean-grass-E";
	else if MATCH_TERRAIN(Any, Any,      Any,
	                      Any, Water,    Any,
	                      Any, NotWater, Any)
		topX=2, topY=8;//resource = "tile-ocean-grass-S";
	else if MATCH_TERRAIN(Any,      Any,   Any,
	                      NotWater, Water, Any,
	                      Any,      Any,   Any)
		topX=1, topY=8;//resource = "tile-ocean-grass-W";
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

		else if (n == ROAD && s == ROAD && e == TerrainType::Water && w == TerrainType::Water)
			resource = "tile-road-water-straightN";
		else if (n == TerrainType::Water && s == TerrainType::Water && e == ROAD && w == ROAD)
			resource = "tile-road-water-straightE";

		else if (n == ROAD && s == ROAD)
			resource = "tile-road-grass-straightN";
		else if (e == ROAD && w == ROAD)
			resource = "tile-road-grass-straightE";

		else
			resource = "tile-road";
	*/

	if (bottomX==-1 && bottomY == -1) {
		bottomX=1;
		bottomY=0;
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

				this->tiles[i][j].spritesheet->resetColorMod();
			}
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

#undef SAME_OR_ANY
#undef MATCH_TERRAIN
