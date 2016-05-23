#pragma once

#include <vector>
#include <string>
#include <memory>

#include "typedefs.hpp"
#include "enums.hpp"
#include "CoordinateOrUnit.hpp"
#include "EnvironmentSpec.hpp"

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

EnvironmentSpec TerrainPassability[TerrainType::num] = {
	// format is ground, sea, submerged, air
	EnvironmentSpec(false,false,false,false), // Invalid,
	EnvironmentSpec(false,false,false,false), // Any,
	EnvironmentSpec(false,false,false,false), // NotWater,
	EnvironmentSpec(true ,false,false,true ), // Grass,
	EnvironmentSpec(false,true ,true ,true ), // Water,
	EnvironmentSpec(true ,false,false,true ), // Road,
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

		// Finds a set of passable tiles from start to end where all tiles in
		// the path are passable by EnvironmentSpec
		bool getPath(CoordinateOrUnit start, CoordinateOrUnit end,
		             Path& path, EnvironmentSpec travel);

		bool operator() (unsigned int x, unsigned int y) const;
	private:
		int height, width;
		std::shared_ptr<Spritesheet> minimap;
		EnvironmentSpec searchPassable_; // we pass the global Terrain instance to JPS
};
