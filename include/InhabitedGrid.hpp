#pragma once

#include <set>
#include <map>
#include <vector>
#include <memory>

#include "typedefs.hpp"
#include "Unitset.hpp"

class Unit; //forward declaration
class Game;

Distance pythagoreanDistance(Coordinate a, Coordinate b);
bool pythagoreanDistanceLessThan(Coordinate a, Coordinate b, Distance r);
bool pythagoreanDistanceLessEqThan(Coordinate a, Coordinate b, Distance r);
bool coordInRect(Coordinate a, Coordinate b, Coordinate c);
bool coordInCircle(Coordinate a, Coordinate c, int r);

bool unitInCircle(Unit* u, Coordinate c, int r);
bool unitInRectangle(Unit* u, Coordinate b, Coordinate c);

class InhabitedGrid {
	// Handles collision and visibility.
	// Unit collision currently assumes all units have a diameter of less than 192 pixels.
	public:
		InhabitedGrid();
		InhabitedGrid(Game* game);
		
		~InhabitedGrid();

		const std::shared_ptr<Unitset> &unitsInCell(Coordinate c) const;
		Unitset unitsInRectangle(Coordinate a, Coordinate b) const;
		Unitset unitsInCircle(Coordinate c, Distance radius) const;
		Unitset unitsCollidingWith(Unit* u) const;

		bool unitOKToMoveTo(Unit*, const Coordinate);

		void emplace(const Unit *unit);
		void erase(const Unit *unit);
		void eraseWithHint(const Unit *unit, const Coordinate oldcoord);
		void updatePos(const Unit *unit, Coordinate oldcoord);

		Game* game;
	private:
		Coordinate getCellCoords(Coordinate c) const; // cells correspond to the collision grid
		const int cellWidth;
		const std::shared_ptr<Unitset> emptyUnitIDset;
		std::map<Coordinate, std::shared_ptr<Unitset> > grid;

		//std::map<std::pair<Coordinate, TeamID>, int> visibilityGrid; // For each visible tile and team, contains a count of how many units on that team can see that tile
		//std::map<std::pair<Coordinate, TeamID>, int> visibilityTimeGrid; // For each visible tile and team, contains a count of how long that tile has been visible to units on that team
};
