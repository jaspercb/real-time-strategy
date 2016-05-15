#pragma once

#include <vector>
#include <string>
#include <memory>

#include "enums.hpp"

class SDL_Renderer;
class UserInterface;
class Spritesheet;

struct TileData {
	// Type enum
	TerrainType::Enum terraintype;
	// Draw data
	std::shared_ptr<Spritesheet> spritesheet;
	int bottomX;
	int bottomY;
	int topX;
	int topY;
};

// Handles drawing terrain, and also pathfinding for some reason

class Terrain {
	friend UserInterface;
	public:
		Terrain(std::string filename="map-default");
		void render(SDL_Renderer* renderer, UserInterface* ui);
		void renderMinimap(SDL_Renderer* renderer, UserInterface* ui);
		void updateDrawTile(int x, int y);
		TerrainType::Enum getTerrainAt(int x, int y);

		std::vector< std::vector< TileData> > tiles;

	private:
		int height, width;
		std::shared_ptr<Spritesheet> minimap;
};
