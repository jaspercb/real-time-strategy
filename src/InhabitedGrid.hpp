#pragma once

#include <unordered_set>
#include <map>
#include <vector>

#include "typedefs.hpp"

class Unit; //forward declaration

class InhabitedGrid{
public:
	InhabitedGrid(int w, int h, int dw, int dh);
	void move(Unit* unit, int x, int y, int z);
	std::pair<int, int> getCellCoords(int x, int y);
	std::vector<UnitID> getCell(int x, int y);
private:
	const int cellsX;
	const int cellsY;
	const int cellWidth;
	const int cellHeight;
	std::map<std::pair<int, int>, std::unordered_set<UnitID> > grid;
};
