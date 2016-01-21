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

void InhabitedGrid::emplace(const Unit &unit)
{
	Coordinate pos = getCellCoords(unit.xy);
	if (grid.count(pos)){
	}
	else{
		grid.emplace(pos, std::unordered_set<UnitID>());
	}
	grid[pos].emplace(unit.id);
}

void InhabitedGrid::erase(const Unit &unit)
{
	Coordinate pos = getCellCoords(unit.xy);
	grid[pos].erase(unit.id);
}

void InhabitedGrid::updatePos(const Unit &unit, Coordinate oldcoord)
{
	Coordinate oldpos, newpos;
	oldpos = getCellCoords(oldcoord);
	newpos = getCellCoords(unit.xy);
	if (oldpos==newpos){
		return;
	}
	else{
		erase(unit);
		emplace(unit);
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
