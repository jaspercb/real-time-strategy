#include <cmath>

#include "InhabitedGrid.hpp"
#include "Unit.hpp"
#include "Game.hpp"
#include "Logging.hpp"

int pythagoreanDistance(Coordinate a, Coordinate b) {
	return (int)pow(pow(a.first-b.first, 2) + pow(a.second-b.second, 2), 0.5);
}

bool coordInRect(Coordinate a, Coordinate b, Coordinate c) {
	// returns whether a is in the rectangle formed by b and c
	int bx = std::min(a.first, b.first);
	int by = std::min(a.second, b.second);
	int cx = std::max(a.first, b.first);
	int cy = std::max(a.second, b.second);
	return (bx<=a.first) && (a.first<=cx) && (by<=a.second) && (a.second<=cy);
}

bool coordInCircle(Coordinate a, Coordinate c, int r) {
	// returns whether a is in the circle with center c and radius r
	return pythagoreanDistance(a, c)<=r;
}

bool unitInCircle(Unit& u, Coordinate c, int r) {
	// returns whether a unit's circle overlaps with the circle with center c and radius r
	return pythagoreanDistance(u.xy, c) <= (r+u.getUnitTemplate().radius());
}


InhabitedGrid::InhabitedGrid(Game* game, int w, int h):
	game(game),
	cellWidth(w),
	cellHeight(h),
	emptyUnitIDset(new std::unordered_set<UnitID>)
	{
		for (int x=0; x<w; x++){
			for (int y=0; y<h; y++){

			}
		}
	}

Coordinate InhabitedGrid::getCellCoords(Coordinate p) {
	return Coordinate(p.first/cellWidth, p.second/cellWidth);
}

const std::shared_ptr<std::unordered_set<UnitID> > &InhabitedGrid::unitsInCell(Coordinate c) {
	if (grid.count(c)){
		return grid[c];
	}
	return this->emptyUnitIDset;
}

void InhabitedGrid::emplace(const Unit &unit) {
	Coordinate pos = getCellCoords(unit.xy);
	if (grid.count(pos)){
	}
	else{
		grid.emplace(pos, std::shared_ptr<std::unordered_set<UnitID> > (new std::unordered_set<UnitID>()));
	}
	grid[pos]->emplace(unit.unitID);
}

void InhabitedGrid::erase(const Unit &unit) {
	Coordinate pos = getCellCoords(unit.xy);
	grid[pos]->erase(unit.unitID);
	if (grid[pos]->size()==0){
		this->grid.erase(pos);
	}
}

void InhabitedGrid::eraseWithHint(const Unit &unit, const Coordinate oldcoord) {
	Coordinate oldpos = getCellCoords(oldcoord);
	grid[oldpos]->erase(unit.unitID);
	if (grid[oldpos]->size()==0){
		this->grid.erase(oldpos);
	}
}

void InhabitedGrid::updatePos(const Unit &unit, Coordinate oldcoord) {
	Coordinate oldpos, newpos;
	oldpos = getCellCoords(oldcoord);
	newpos = getCellCoords(unit.xy);
	if (oldpos==newpos){
		return;
	}
	else{
		this->eraseWithHint(unit, oldcoord);
		this->emplace(unit);
	}
}

std::vector<UnitID> InhabitedGrid::unitsInRectangle(Coordinate a, Coordinate b) {
	std::vector<UnitID> ret;

	Coordinate ga = getCellCoords(a);
	Coordinate gb = getCellCoords(b);
	int startX = std::min(ga.first, gb.first);
	int endX = std::max(ga.first, gb.first);
	int startY = std::min(ga.second, gb.second);
	int endY = std::max(ga.second, gb.second);

	for (int x=startX; x<=endX; x++){
		for (int y=startY; y<=endY; y++){
			const auto unitSubset = unitsInCell(Coordinate(x,y));
			for (auto it = unitSubset->begin(); it!=unitSubset->end(); it++){
				if (coordInRect(game->getUnit(*it).xy, a, b)){
					ret.push_back(*it);
				}
			}
		}
	}
	return ret;
}

std::vector<UnitID> InhabitedGrid::unitsInCircle(Coordinate c, int radius) {
	std::vector<UnitID> ret;

	Coordinate gc = getCellCoords(c);
	int startX = gc.first - 1 - cellWidth;
	int endX = gc.first + 1 + cellWidth;
	int startY = gc.second - 1 - cellHeight;
	int endY = gc.second + 1 + cellHeight;

	for (int x=startX; x<=endX; x++){
		for (int y=startY; y<=endY; y++){
			const auto unitSubset = unitsInCell(Coordinate(x,y));
			for (auto it = unitSubset->begin(); it!=unitSubset->end(); it++){
				if (coordInCircle(game->getUnit(*it).xy, c, radius)){
					ret.push_back(*it);
				}
			}
		}
	}
	return ret;
}




bool InhabitedGrid::unitOKToMoveTo(Unit &u, const Coordinate location) {
	std::vector<UnitID> ret;

	Coordinate gc = getCellCoords(location);
	int startX = gc.first - 1 - cellWidth;
	int endX = gc.first + 1 + cellWidth;
	int startY = gc.second - 1 - cellHeight;
	int endY = gc.second + 1 + cellHeight;

	const int radius = u.getUnitTemplate().radius();

	for (int x=startX; x<=endX; x++){
		for (int y=startY; y<=endY; y++){
			const auto unitSubset = unitsInCell(Coordinate(x,y));
			for (auto it = unitSubset->begin(); it!=unitSubset->end(); it++){
				Unit& other = game->getUnit(*it);
				if (u.unitID != other.unitID && unitInCircle(other, location, radius)) {
					return false;
				}
			}
		}
	}
	return true;
}