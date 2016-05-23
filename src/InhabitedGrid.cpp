#include <cmath>
#include <algorithm>

#include "InhabitedGrid.hpp"

#include "Unit.hpp"
#include "UnitTemplate.hpp"
#include "Game.hpp"
#include "Logging.hpp"
#include "globals.hpp"

Distance pythagoreanDistance(Coordinate a, Coordinate b) {
	return pow( (a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y), 0.5);
}

bool pythagoreanDistanceLessThan(Coordinate a, Coordinate b, Distance r) {
	return ((float)a.x - (float)b.x) * ((float)a.x - (float)b.x) + ((float)a.y - (float)b.y)*((float)a.y - (float)b.y) < (float)r*(float)r;
}

bool pythagoreanDistanceLessEqThan(Coordinate a, Coordinate b, Distance r) {
	return ((float)a.x - (float)b.x) * ((float)a.x - (float)b.x) + ((float)a.y - (float)b.y)*((float)a.y - (float)b.y) <= (float)r*(float)r;
}

bool coordInRect(Coordinate a, Coordinate b, Coordinate c) {
	// returns whether a is in the rectangle formed by b and c
	Distance bx = std::min(c.x, b.x);
	Distance by = std::min(c.y, b.y);
	Distance cx = std::max(c.x, b.x);
	Distance cy = std::max(c.y, b.y);
	return (bx<=a.x) && (a.x<=cx) && (by<=a.y) && (a.y<=cy);
}

bool coordInCircle(Coordinate a, Coordinate c, int r) {
	// returns whether a is in the circle with center c and radius r
	return pythagoreanDistanceLessThan(a, c, r);
}

bool unitInCircle(Unit *u, Coordinate c, int r) {
	// returns whether a unit's circle overlaps with the circle with center c and radius r
	return pythagoreanDistanceLessThan(u->xy, c, r+u->getUnitTemplate()->radius());
}

bool unitInRectangle(Unit* u, Coordinate b, Coordinate c) {
	//make rect bigger
	Distance radius = u->getUnitTemplate()->radius();
	if (b.x < c.x) {
		b.x -= radius;
		c.x += radius;
	}
	else {
		b.x += radius;
		c.x -= radius;
	}

	if (b.y < c.y) {
		b.y -= radius;
		c.y += radius;
	}
	else {
		b.y += radius;
		c.y -= radius;
	}


	// returns whether a unit's circle overlaps with the rectangle formed by a, b
	return coordInRect(u->xy, b, c);
}

InhabitedGrid::InhabitedGrid(Game* game):
	game(game),
	cellWidth(6 * PIXEL_WIDTH * 32),
	emptyUnitIDset(std::make_shared<Unitset>())
	{}

InhabitedGrid::~InhabitedGrid() {}

Coordinate InhabitedGrid::getCellCoords(Coordinate c) const { 
	return Coordinate(c.x/cellWidth, c.y/cellWidth);
}

const std::shared_ptr<Unitset> &InhabitedGrid::unitsInCell(Coordinate c) const {
	auto found = this->grid.find(c);
	if (found != this->grid.end()){
		return found->second;
	}
	return this->emptyUnitIDset;
}

void InhabitedGrid::emplace(const Unit *unit) {
	Coordinate pos = getCellCoords(unit->xy);
	if (grid.count(pos)){
	}
	else{
		grid.emplace(pos, std::shared_ptr<Unitset> (new Unitset()));
	}
	grid[pos]->insert(unit->unitID);
}

void InhabitedGrid::erase(const Unit *unit) {
	Coordinate pos = getCellCoords(unit->xy);
	grid[pos]->erase(unit->unitID);
	if (grid[pos]->size()==0){
		this->grid.erase(pos);
	}
	//this->decrementTileVisibility(unit->xy, unit->teamID);
}

void InhabitedGrid::eraseWithHint(const Unit *unit, const Coordinate oldcoord) {
	Coordinate oldpos = getCellCoords(oldcoord);
	grid[oldpos]->erase(unit->unitID);
	if (grid[oldpos]->size()==0){
		this->grid.erase(oldpos);
	}
	//->decrementTileVisibility(oldcoord, unit->teamID);
}

void InhabitedGrid::updatePos(const Unit *unit, Coordinate oldcoord) {
	Coordinate oldcell = this->getCellCoords(oldcoord);
	Coordinate newcell = this->getCellCoords(unit->xy);

	if (oldcell != newcell){
		this->eraseWithHint(unit, oldcoord);
		this->emplace(unit);
	}
}

Unitset InhabitedGrid::unitsInRectangle(Coordinate a, Coordinate b) const {
	Unitset ret;

	Coordinate ga = getCellCoords(a);
	Coordinate gb = getCellCoords(b);
	int startX = std::min(ga.x, gb.x);
	int endX = std::max(ga.x, gb.x);
	int startY = std::min(ga.y, gb.y);
	int endY = std::max(ga.y, gb.y);

	for (int x=startX; x<=endX; x++){
		for (int y=startY; y<=endY; y++){
			const auto &unitSubset = unitsInCell(Coordinate(x,y));
			for (auto it = unitSubset->begin(); it!=unitSubset->end(); it++){
				if (unitInRectangle(game->getUnit(*it), a, b)) {
					ret.insert(*it);
				}
			}
		}
	}
	return ret;
}

Unitset InhabitedGrid::unitsInCircle(Coordinate c, Distance radius) const {
	// Returns units that clip the given circle, (center & radius provided)
	Unitset ret;

	Coordinate gc = getCellCoords(c);
	int startX = gc.x - 1 - radius/cellWidth;
	int endX = gc.x + 1 + radius/cellWidth;
	int startY = gc.y - 1 - radius/cellWidth;
	int endY = gc.y + 1 + radius/cellWidth;

	for (int x=startX; x<=endX; x++){
		for (int y=startY; y<=endY; y++){
			const auto &unitSubset = unitsInCell(Coordinate(x,y));
			for (auto it = unitSubset->begin(); it!=unitSubset->end(); it++){
				if (unitInCircle(game->getUnit(*it), c, radius)){
					ret.insert(*it);
				}
			}
		}
	}
	return ret;
}

Unitset InhabitedGrid::unitsCollidingWith(Unit* u) const {
	Unitset ret;
	for (auto& unitID : this->unitsInCircle(u->xy, u->getUnitTemplate()->radius())) {
		Unit* other = this->game->getUnit(unitID);
		if (other->unitID != u->unitID && other->dimension&u->dimension) {
			ret.insert(unitID);
		}
	}
	return ret;
}

bool InhabitedGrid::unitOKToMoveTo(Unit*u, const Coordinate location) {
	return true; // testing soft collisions
	Coordinate gc = getCellCoords(location);
	int startX = gc.x - 1;
	int endX = gc.x + 1;
	int startY = gc.y - 1;
	int endY = gc.y + 1;

	//const int radius = u.getUnitTemplate()->radius() / 500;
	const int radius = 0;
	
	for (int x=startX; x<=endX; x++){
		for (int y=startY; y<=endY; y++){
			const auto& unitSubset = unitsInCell(Coordinate(x,y));
			for (auto it = unitSubset->begin(); it!=unitSubset->end(); it++){
				Unit* other = game->getUnit(*it);
				if (u->unitID != other->unitID && unitInCircle(other, location, radius) && u->dimension&other->dimension) {
					return false;
				}
			}
		}
	}
	return true;
}
