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
	return ((float)a.first - (float)b.first) * ((float)a.first - (float)b.first) + ((float)a.second - (float)b.second)*((float)a.second - (float)b.second) < (float)r*(float)r;
}

bool pythagoreanDistanceLessEqThan(Coordinate a, Coordinate b, Distance r) {
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

bool unitInRectangle(Unit& u, Coordinate b, Coordinate c) {
	//make rect bigger
	Distance radius = u.getUnitTemplate().radius();
	if (b.first < c.first) {
		b.first -= radius;
		c.first += radius;
	}
	else {
		b.first += radius;
		c.first -= radius;
	}

	if (b.second < c.second) {
		b.second -= radius;
		c.second += radius;
	}
	else {
		b.second += radius;
		c.second -= radius;
	}


	// returns whether a unit's circle overlaps with the rectangle formed by a, b
	return coordInRect(u.xy, b, c);
}

InhabitedGrid::InhabitedGrid(Game* game):
	game(game),
	cellWidth(PIXEL_WIDTH * 66),
	cellHeight(PIXEL_WIDTH * 66),
	emptyUnitIDset(std::make_shared<std::set<UnitID> >() )
	{

	}

Coordinate InhabitedGrid::getCellCoords(Coordinate c) {
	// takes an objective coordinate, returns the cell coordinates
	return Coordinate((c.first+cellWidth*0.2)/cellWidth, (c.second+cellHeight*0.25)/cellHeight); // hackish way to make rendered tiles line up with these internal cell coordinates. I know, I know.
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

	this->incrementTileVisibility(unit.xy, unit.teamID);
}

void InhabitedGrid::erase(const Unit &unit) {
	Coordinate pos = getCellCoords(unit.xy);
	grid[pos]->erase(unit.unitID);
	if (grid[pos]->size()==0){
		this->grid.erase(pos);
	}
	this->decrementTileVisibility(unit.xy, unit.teamID);
}

void InhabitedGrid::eraseWithHint(const Unit &unit, const Coordinate oldcoord) {
	Coordinate oldpos = getCellCoords(oldcoord);
	grid[oldpos]->erase(unit.unitID);
	if (grid[oldpos]->size()==0){
		this->grid.erase(oldpos);
	}
	this->decrementTileVisibility(oldcoord, unit.teamID);
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
				if (unitInRectangle(game->getUnit(*it), a, b)) {
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

void InhabitedGrid::incrementTileVisibility(const Coordinate location, const TeamID team) {
	Coordinate center = this->getCellCoords(location);
	auto key_pair = std::make_pair(center, team);

	for (int i = -visibilityRadius ; i<=visibilityRadius ; i++ ) {
		for (int j = -visibilityRadius ; j<=visibilityRadius ; j++ ) {
			Coordinate coord(center.first+i, center.second+j);
			if (pythagoreanDistanceLessThan(center, coord, InhabitedGrid::visibilityRadius)) {
				this->visibilityGrid[make_pair(coord, team)]++;
			}
		}
	}
}

void InhabitedGrid::decrementTileVisibility(const Coordinate location, const TeamID team) {
	Coordinate center = this->getCellCoords(location);
	auto key_pair = std::make_pair(center, team);

	for (int i = -visibilityRadius ; i<=visibilityRadius ; i++ ) {
		for (int j = -visibilityRadius ; j<=visibilityRadius ; j++ ) {
			Coordinate coord(center.first+i, center.second+j);
			if (pythagoreanDistanceLessThan(center, coord, InhabitedGrid::visibilityRadius)) {
				this->visibilityGrid[make_pair(coord, team)]--;
			}
		}
	}
}

int InhabitedGrid::coordIsVisibleToTeam(const Coordinate location, const TeamID team) {
	Coordinate coord = this->getCellCoords(location);
	auto p = this->visibilityGrid.find(std::make_pair(coord, team));
	return p==this->visibilityGrid.end() ? 0 : p->second;
}

int InhabitedGrid::tileIsVisibleToTeam(const Coordinate tile, const TeamID team) {
	auto p = this->visibilityGrid.find(std::make_pair(tile, team));
	return p==this->visibilityGrid.end() ? 0 : p->second;
}

int InhabitedGrid::unitIsVisibleToTeam(const Unit& unit, const TeamID team) {
	Coordinate tile = this->getCellCoords(unit.xy);
	auto p = this->visibilityGrid.find(std::make_pair(tile, team));
	return p==this->visibilityGrid.end() ? 0 : p->second;
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
