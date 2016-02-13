#include <cmath>
#include <algorithm>

#include "InhabitedGrid.hpp"
#include "Unit.hpp"
#include "Game.hpp"
#include "Logging.hpp"
#include "globals.hpp"

Distance pythagoreanDistance(Coordinate a, Coordinate b) {
	return pow( (a.first-b.first)*(a.first-b.first) + (a.second-b.second)*(a.second-b.second), 0.5);
}

bool pythagoreanDistanceLessThan(Coordinate a, Coordinate b, Distance r) {
	return ((float)a.first - (float)b.first) * ((float)a.first - (float)b.first) + ((float)a.second - (float)b.second)*((float)a.second - (float)b.second) <= (float)r*(float)r;
}

bool coordInRect(Coordinate a, Coordinate b, Coordinate c) {
	// returns whether a is in the rectangle formed by b and c
	Distance bx = std::min(c.first, b.first);
	Distance by = std::min(c.second, b.second);
	Distance cx = std::max(c.first, b.first);
	Distance cy = std::max(c.second, b.second);
	return (bx<=a.first) && (a.first<=cx) && (by<=a.second) && (a.second<=cy);
}

bool coordInCircle(Coordinate a, Coordinate c, int r) {
	// returns whether a is in the circle with center c and radius r
	return pythagoreanDistanceLessThan(a, c, r);
}

bool unitInCircle(Unit& u, Coordinate c, int r) {
	// returns whether a unit's circle overlaps with the circle with center c and radius r
	return pythagoreanDistanceLessThan(u.xy, c, r+u.getUnitTemplate().radius());
}

InhabitedGrid::InhabitedGrid(Game* game, int w, int h):
	game(game),
	cellWidth(w),
	cellHeight(h),
	emptyUnitIDset(new std::set<UnitID>)
	{
	}

Coordinate InhabitedGrid::getCellCoords(Coordinate c) {
	// takes an objective coordinate, returns the cell coordinates
	return Coordinate(c.first/cellWidth, c.second/cellHeight);
}

const std::shared_ptr<std::set<UnitID> > &InhabitedGrid::unitsInCell(Coordinate c) {
	auto found = this->grid.find(c);
	if (found != this->grid.end()){
		return found->second;
	}
	return this->emptyUnitIDset;
}

void InhabitedGrid::emplace(const Unit &unit) {
	Coordinate pos = getCellCoords(unit.xy);
	if (grid.count(pos)){
	}
	else{
		grid.emplace(pos, std::shared_ptr<std::set<UnitID> > (new std::set<UnitID>()));
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
			const auto &unitSubset = unitsInCell(Coordinate(x,y));
			for (auto it = unitSubset->begin(); it!=unitSubset->end(); it++){
				if (coordInRect(game->getUnit(*it).xy, a, b)){
					ret.push_back(*it);
				}
			}
		}
	}
	return ret;
}

std::vector<UnitID> InhabitedGrid::unitsInCircle(Coordinate c, Distance radius) {
	// Returns units that clip the given circle, (center & radius provided)
	std::vector<UnitID> ret;

	Coordinate gc = getCellCoords(c);
	int startX = gc.first - 1 - radius/cellWidth;
	int endX = gc.first + 1 + radius/cellWidth;
	int startY = gc.second - 1 - radius/cellHeight;
	int endY = gc.second + 1 + radius/cellHeight;

	for (int x=startX; x<=endX; x++){
		for (int y=startY; y<=endY; y++){
			const auto &unitSubset = unitsInCell(Coordinate(x,y));
			for (auto it = unitSubset->begin(); it!=unitSubset->end(); it++){
				if (unitInCircle(game->getUnit(*it), c, radius)){
					ret.push_back(*it);
				}
			}
		}
	}
	return ret;
}

std::vector<UnitID> InhabitedGrid::unitsCollidingWith(Unit& u) {
	std::vector<UnitID> ret;
	for (auto &unitID : this->unitsInCircle(u.xy, u.getUnitTemplate().radius())) {
		Unit& other = this->game->getUnit(unitID);
		if (other.unitID != u.unitID && other.dimension.overlaps(u.dimension)) {
			ret.push_back(unitID);
		}
	}
	return ret;
}

bool InhabitedGrid::unitOKToMoveTo(Unit &u, const Coordinate location) {
	return true; // testing soft collisions
	Coordinate gc = getCellCoords(location);
	int startX = gc.first - 1;
	int endX = gc.first + 1;
	int startY = gc.second - 1;
	int endY = gc.second + 1;

	//const int radius = u.getUnitTemplate().radius() / 500;
	const int radius = 0;
	
	for (int x=startX; x<=endX; x++){
		for (int y=startY; y<=endY; y++){
			const auto &unitSubset = unitsInCell(Coordinate(x,y));
			for (auto it = unitSubset->begin(); it!=unitSubset->end(); it++){
				Unit& other = game->getUnit(*it);
				if (u.unitID != other.unitID && unitInCircle(other, location, radius) && u.dimension.overlaps(other.dimension)) {
					return false;
				}
			}
		}
	}
	return true;
}
