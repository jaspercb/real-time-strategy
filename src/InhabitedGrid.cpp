#include "InhabitedGrid.hpp"
#include "Unit.hpp"
#include <cmath>

int pythagoreanDistance(Coordinate a, Coordinate b)
{
	return pow(pow(a.first-b.first, 2) + pow(a.second-b.second, 2), 0.5);
}

InhabitedGrid::InhabitedGrid(int w, int h, int dw, int dh):
		cellsX(dw),
		cellsY(dh),
		cellWidth(w),
		cellHeight(h)
		{
			for (int x=0; x<w; x++){
				for (int y=0; y<h; y++){

				}
			}
		}

Coordinate InhabitedGrid::getCellCoords(Coordinate p)
{
	return std::pair<int, int>(p.first/cellWidth, p.second/cellWidth);
}

void InhabitedGrid::move(Unit* unit, Coordinate c)
{
	std::pair<int, int> oldpos, newpos;
	oldpos = getCellCoords(unit->xy);
	newpos = getCellCoords(c);
	unit->xy = c;
	if (oldpos==newpos){
		return;
	}
	else{
		grid[oldpos].erase(unit->id);
		grid[newpos].emplace(unit->id);
	}

}

std::vector<UnitID> InhabitedGrid::unitsInRectangle(Coordinate a, Coordinate b)
{
	std::vector<UnitID> ret;

	Coordinate ga = getCellCoords(a);
	Coordinate gb = getCellCoords(b);

	return ret;
}

std::vector<UnitID> InhabitedGrid::unitsInCircle(Coordinate c, int range)
{
	std::vector<UnitID> ret;
	Coordinate gCoordCenter = getCellCoords(c);

	return ret;
}
