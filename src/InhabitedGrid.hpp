#pragma once

#include <unordered_set>
#include <map>
#include <vector>

#include "typedefs.hpp"

class Unit; //forward declaration

int pythagoreanDistance(Coordinate a, Coordinate b);

class InhabitedGrid{
public:
	InhabitedGrid(int w, int h, int dw, int dh);
	void move(Unit* unit, Coordinate c);
	Coordinate getCellCoords(Coordinate c);
	std::vector<UnitID> getCell(Coordinate c);
private:
	const int cellsX;
	const int cellsY;
	const int cellWidth;
	const int cellHeight;
	std::map<std::pair<int, int>, std::unordered_set<UnitID> > grid;
};
