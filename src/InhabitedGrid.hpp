#pragma once

#include <unordered_set>
#include <map>
#include <vector>

#include "typedefs.hpp"

class Unit; //forward declaration
class Game;

int pythagoreanDistance(Coordinate a, Coordinate b);
bool coordInRect(Coordinate a, Coordinate b, Coordinate c);
bool coordInCircle(Coordinate a, Coordinate c, int r);

class InhabitedGrid{
public:
	InhabitedGrid(Game* game, int w, int h, int dw, int dh);

	const std::unordered_set<UnitID> get(Coordinate c);

	void emplace(const Unit &unit);
	void erase(const Unit &unit);
	void eraseWithHint(const Unit &unit, const Coordinate oldcoord);
	void updatePos(const Unit &unit, Coordinate oldcoord);

	Game* game;
private:
	std::vector<UnitID> unitsInRectangle(Coordinate a, Coordinate b);
	std::vector<UnitID> unitsInCircle(Coordinate c, int range);
	std::unordered_set<UnitID> getCell(Coordinate c);
	Coordinate getCellCoords(Coordinate c);
	const int cellsX;
	const int cellsY;
	const int cellWidth;
	const int cellHeight;
	std::map<std::pair<int, int>, std::unordered_set<UnitID> > grid;
};
