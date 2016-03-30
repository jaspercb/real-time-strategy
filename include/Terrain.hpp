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

struct TileData {
	TerrainType terraintype;
	std::shared_ptr<Spritesheet> spritesheet;
	int bottomX;
	int bottomY;
	int topX;
	int topY;
};

class Terrain {
	friend UserInterface;
	public:
		Terrain(std::string filename="map-default");
		void render(SDL_Renderer* renderer, UserInterface* ui);
		void renderMinimap(SDL_Renderer* renderer, UserInterface* ui);
		void updateDrawTile(int x, int y);
		TerrainType getTerrainAt(int x, int y);

		std::vector< std::vector< TileData> > tiles;

	private:
		int height, width;
		std::shared_ptr<Spritesheet> minimap;
};
