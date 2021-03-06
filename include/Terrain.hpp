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

extern EnvironmentSpec TerrainPassable[TerrainType::num];

// Handles drawing terrain, and also pathfinding for some reason
class Terrain {
	friend UserInterface;
	public:
		Terrain(std::string filename="map-default");
		void render(SDL_Renderer* renderer, UserInterface* ui);
		void renderMinimap(SDL_Renderer* renderer, UserInterface* ui);
		void updateDrawTile(int x, int y);
		TerrainType::Enum getTerrainAt(int x, int y) const;

		Coordinate tileFromCoord(Coordinate) const;
		Coordinate coordFromTile(Coordinate) const;

		bool linePassable(Coordinate a, Coordinate b, EnvironmentSpec passable) const;

		void compressPath(Path& path, EnvironmentSpec passable) const;

		// Finds a set of passable tiles from start to end where all tiles in
		// the path are passable by EnvironmentSpec
		bool getPath(CoordinateOrUnit start, CoordinateOrUnit end,
		             Path& path, EnvironmentSpec travel);

		bool operator() (unsigned int x, unsigned int y) const;

		std::vector< std::vector< TileData> > tiles;
	private:
		int height, width;
		std::shared_ptr<Spritesheet> minimap;
		mutable EnvironmentSpec searchPassable; // we pass the global Terrain instance to JPS
};
