#include "InhabitedGrid.hpp"

#include "Unit.hpp"

InhabitedGrid::InhabitedGrid(int w, int h, int dw, int dh):
		cellsX(dw),
		cellsY(dh),
		cellWidth(w),
		cellHeight(h)
		{}

std::pair<int, int> InhabitedGrid::getCellCoords(int x, int y){
	return std::pair<int, int>(x/cellWidth, y/cellWidth);
}

void InhabitedGrid::move(Unit* unit, int x, int y, int z){
	std::pair<int, int> oldpos, newpos;
	oldpos = getCellCoords(unit->x, unit->y);
	newpos = getCellCoords(x, y);
	unit->x = x;
	unit->y = y;
	unit->z = z;
	if (oldpos==newpos){
		return;
	}
	else{
		grid[oldpos].erase(unit->id);
		grid[oldpos].erase(unit->id);
	}

}

