#pragma once

#include <vector>
#include <string>
#include <memory>

class SDL_Renderer;
class UserInterface;
class Spritesheet;

enum TerrainType {
	NONE,
	GRASS,
	WATER,
	ROAD
};

class Terrain {
	public:
		Terrain(std::string filename="map-default");
		void render(SDL_Renderer* renderer, UserInterface* ui);
		void renderMinimap(SDL_Renderer* renderer, UserInterface* ui);
		void updateDrawTile(int x, int y);
		TerrainType getTerrainAt(int x, int y);

		std::vector< std::vector< TerrainType> > tiles;

	private:
		int height, width;
		std::vector< std::vector< std::shared_ptr<Spritesheet> > > drawTiles;
		std::shared_ptr<Spritesheet> minimap;
};
