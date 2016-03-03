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
	public:
		InhabitedGrid(Game* game);

		const std::shared_ptr<std::set<UnitID> > &unitsInCell(Coordinate c);
		std::vector<UnitID> unitsInRectangle(Coordinate a, Coordinate b);
		std::vector<UnitID> unitsInCircle(Coordinate c, Distance radius);
		std::vector<UnitID> unitsCollidingWith(Unit& u);

		bool unitOKToMoveTo(Unit&, const Coordinate);
		void incrementTileVisibility(const Coordinate location, const TeamID team);
		void decrementTileVisibility(const Coordinate location, const TeamID team);
		int coordIsVisibleToTeam(const Coordinate location, const TeamID team);
		int tileIsVisibleToTeam(const Coordinate tile, const TeamID team);
		int unitIsVisibleToTeam(const Unit& unit, const TeamID team);

		void emplace(const Unit &unit);
		void erase(const Unit &unit);
		void eraseWithHint(const Unit &unit, const Coordinate oldcoord);
		void updatePos(const Unit &unit, Coordinate oldcoord);

		Game* game;
	private:
		static const int visibilityRadius = 5; // the visibility radius for units

		Coordinate getCellCoords(Coordinate c);
		const int cellWidth;
		const int cellHeight;
		const std::shared_ptr<std::set<UnitID>> emptyUnitIDset;
		std::map<Coordinate, std::shared_ptr<std::set<UnitID> > > grid;
		std::map<std::pair<Coordinate, TeamID>, int> visibilityGrid; // For each visible tile and team, contains a count of how many units on that team can see that tile
};
