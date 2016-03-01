#include "Terrain.hpp"
#include "ResourceManager.hpp"
#include "Spritesheet.hpp"
#include "globals.hpp"
#include "typedefs.hpp"
#include "UserInterface.hpp"

#include <SDL2/SDL.h>

Terrain::Terrain(std::string mapName) {
	std::shared_ptr<Spritesheet> image = gResourceManager->get(mapName);
	this->width = image->spriteW;
	this->height = image->spriteH;

	this->tiles.resize(this->width);
	this->drawTiles.resize(this->width);
	for (int i=0; i<this->width; i++) {
		this->tiles[i].resize(this->height);
		this->drawTiles[i].resize(this->height);
		for (int j=0; j<this->height; j++) {
			TerrainType tile;
			if (true)
				tile = GRASS;
			else
				tile = WATER;
			this->tiles[i][j] = tile;
		}
	}

	for (int i=5; i<10; i++){
		for (int j=5; j<10; j++){
			this->tiles[i][j]=WATER;
		}
	}

	for (int i=0; i<this->width; i++) {
		this->drawTiles[i].resize(this->height);
		for (int j=0; j<this->height; j++) {
			this->updateDrawTile(i, j);
		}
	}

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
		if (n == GRASS && w == GRASS)
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

	this->drawTiles[x][y] = gResourceManager->get(resource);
}

void Terrain::render(SDL_Renderer* renderer, UserInterface* ui) {
	for (int i=0; i<this->width; i++) {
		for (int j=0; j<this->height; j++) {
			if (this->drawTiles[i][j]) {
				Coordinate drawPos = ui->screenCoordinateFromObjective(Coordinate(64*PIXEL_WIDTH*i, 64*PIXEL_WIDTH*j ));
				this->drawTiles[i][j]->render(renderer, 0, 0, drawPos.first, drawPos.second +  (83-this->drawTiles[i][j]->spriteH)*ui->viewMagnification/2, ui->viewMagnification);
			}
		}
	}
}
