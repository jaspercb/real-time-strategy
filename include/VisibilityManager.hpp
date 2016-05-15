#pragma once

#include "typedefs.hpp"

class Game;
class Unit;

class VisibilityManager {
public:
	VisibilityManager(Game* game, int w, int h, int numTeams);
	~VisibilityManager();

	void startTrackingVisibility(const Unit* unit);

	void incrementTileVisibility(const Coordinate center, const TeamID team);
	void decrementTileVisibility(const Coordinate center, const TeamID team);

	bool coordIsVisibleToTeam(const Coordinate location, const TeamID team) const;
	bool tileIsVisibleToTeam(const Coordinate tile, const TeamID team) const;
	bool unitIsVisibleToTeam(const Unit* unit, const TeamID team) const;
		
	int getCoordVisibility(const Coordinate tile, const TeamID team) const;
	int getTileVisibility(const Coordinate tile, const TeamID team) const;

	void updatePos(const Unit *unit, Coordinate oldcoord);

	void tick();

private:
	static const int visibilityRadius = 5; // the visibility radius for units
	Game* game;
	const int w, h;
	const int tileWidth;
	int numTeams;

	uint16_t* visibilityGrid;
	uint16_t* visibilityTimeGrid;

	Coordinate getTileCoords(Coordinate c) const; // tiles correspond to visibility
	int getTileIndex(Coordinate tile, int team) const;
};
