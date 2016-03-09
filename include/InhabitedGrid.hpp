#pragma once

#include <set>
#include <map>
#include <vector>
#include <memory>

#include "typedefs.hpp"

class Unit; //forward declaration
class Game;

Distance pythagoreanDistance(Coordinate a, Coordinate b);
bool pythagoreanDistanceLessThan(Coordinate a, Coordinate b, Distance r);
bool pythagoreanDistanceLessEqThan(Coordinate a, Coordinate b, Distance r);
bool coordInRect(Coordinate a, Coordinate b, Coordinate c);
bool coordInCircle(Coordinate a, Coordinate c, int r);

bool unitInCircle(Unit& u, Coordinate c, int r);
bool unitInRectangle(Unit& u, Coordinate b, Coordinate c);

class InhabitedGrid {
	// Handles collision and visibility.
	// Unit collision currently assumes all units have a diameter of less than 192 pixels.
	public:
		InhabitedGrid(Game* game);

		const std::shared_ptr<std::set<UnitID> > &unitsInCell(Coordinate c) const;
		std::vector<UnitID> unitsInRectangle(Coordinate a, Coordinate b) const;
		std::vector<UnitID> unitsInCircle(Coordinate c, Distance radius) const;
		std::vector<UnitID> unitsCollidingWith(Unit& u) const;

		bool unitOKToMoveTo(Unit&, const Coordinate);
		
		void startTrackingVisibility(const Unit&);

		void incrementTileVisibility(const Coordinate center, const TeamID team);
		void decrementTileVisibility(const Coordinate center, const TeamID team);
		
		bool coordIsVisibleToTeam(const Coordinate location, const TeamID team) const;
		bool tileIsVisibleToTeam(const Coordinate tile, const TeamID team) const;
		bool unitIsVisibleToTeam(const Unit& unit, const TeamID team) const;
		
		int getCoordVisibility(const Coordinate tile, const TeamID team) const;
		int getTileVisibility(const Coordinate tile, const TeamID team) const;

		void emplace(const Unit &unit);
		void erase(const Unit &unit);
		void eraseWithHint(const Unit &unit, const Coordinate oldcoord);
		void updatePos(const Unit &unit, Coordinate oldcoord);

		void tick();

		Game* game;
	private:
		static const int visibilityRadius = 5; // the visibility radius for units

		Coordinate getTileCoords(Coordinate c) const; // tiles correspond to visibility
		Coordinate getCellCoords(Coordinate c) const; // cells correspond to the collision grid
		const int cellWidth;
		const int tileWidth;
		const std::shared_ptr<std::set<UnitID>> emptyUnitIDset;
		std::map<Coordinate, std::shared_ptr<std::set<UnitID> > > grid;
		std::map<std::pair<Coordinate, TeamID>, int> visibilityGrid; // For each visible tile and team, contains a count of how many units on that team can see that tile
		std::map<std::pair<Coordinate, TeamID>, int> visibilityTimeGrid; // For each visible tile and team, contains a count of how long that tile has been visible to units on that team
};
