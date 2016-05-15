#include "VisibilityManager.hpp"

#include "globals.hpp" // for PIXEL_WIDTH
#include "Unit.hpp"
#include "InhabitedGrid.hpp" // for pythagorean distance functions and so on

VisibilityManager::VisibilityManager(Game* game, int w, int h, int numTeams):
game(game),
w(w),
h(h),
tileWidth(PIXEL_WIDTH * 32),
numTeams(numTeams) {
	visibilityGrid = new uint16_t[w*h*numTeams];
	memset(visibilityGrid, 0, w*h*numTeams);
	visibilityTimeGrid = new uint16_t[w*h*numTeams];
	memset(visibilityTimeGrid, 0, w*h*numTeams);
}


VisibilityManager::~VisibilityManager() {
	delete[] visibilityGrid;
	delete[] visibilityTimeGrid;
}

void VisibilityManager::startTrackingVisibility(const Unit *unit) {
	// must be called ONCE
	this->incrementTileVisibility( this->getTileCoords(unit->xy), unit->teamID );
}

void VisibilityManager::incrementTileVisibility(const Coordinate center, const TeamID team) {
	for (int i = -visibilityRadius ; i<=visibilityRadius ; i++ ) {
		for (int j = -visibilityRadius ; j<=visibilityRadius ; j++ ) {
			Coordinate coord(center.x+i, center.y+j);
			if (pythagoreanDistanceLessThan(center, coord, VisibilityManager::visibilityRadius)) {
				int k = getTileIndex(coord, team);
				if (k>0) // if on board
					this->visibilityGrid[k]++;
			}
		}
	}
}

void VisibilityManager::decrementTileVisibility(const Coordinate center, const TeamID team) {
	for (int i = -visibilityRadius ; i<=visibilityRadius ; i++ ) {
		for (int j = -visibilityRadius ; j<=visibilityRadius ; j++ ) {
			Coordinate coord(center.x+i, center.y+j);
			if (pythagoreanDistanceLessThan(center, coord, VisibilityManager::visibilityRadius)) {
				int k = getTileIndex(coord, team);
				if (k>0) // if on board
					this->visibilityGrid[k]--;
			}
		}
	}
}

bool VisibilityManager::coordIsVisibleToTeam(const Coordinate location, const TeamID team) const {
	Coordinate tile = this->getTileCoords(location);
	int k = getTileIndex(tile, team);
	if (k<0) return false;
	else return (bool)this->visibilityTimeGrid[k];
}

bool VisibilityManager::tileIsVisibleToTeam(const Coordinate tile, const TeamID team) const {
	int k = getTileIndex(tile, team);
	if (k<0) return false;
	else return (bool)this->visibilityTimeGrid[team*w*h + tile.x*h + tile.y];
}

bool VisibilityManager::unitIsVisibleToTeam(const Unit* unit, const TeamID team) const {
	return  unit->teamID == team || this->coordIsVisibleToTeam(unit->xy, team);
}

int VisibilityManager::getTileVisibility(const Coordinate tile, const TeamID team) const {
	int k = getTileIndex(tile, team);
	int a = k<0 ? 0 : this->visibilityTimeGrid[team*w*h + tile.x*h + tile.y];
	return std::min(255, 64+12*a);
}

int VisibilityManager::getCoordVisibility(const Coordinate coord, const TeamID team) const {
	Coordinate tile = this->getTileCoords(coord);
	int k = getTileIndex(tile, team);
	int a = k<0 ?  0 : this->visibilityTimeGrid[k];
	return std::min(255, 16*a);
	//return std::min(255, 16*this->visibilityTimeGrid.at(std::make_pair(this->getTileCoords(coord), team)) );
}

void VisibilityManager::updatePos(const Unit *unit, Coordinate oldcoord) {
	Coordinate oldtile = this->getTileCoords(oldcoord);
	Coordinate newtile = this->getTileCoords(unit->xy);
	
	if (oldtile != newtile) {
		this->decrementTileVisibility(oldtile, unit->teamID);
		this->incrementTileVisibility(newtile, unit->teamID);
	}
}

void VisibilityManager::tick() {
	for (int i=0; i<w*h*numTeams; i++) {
		if (this->visibilityGrid[i]) {
			this->visibilityTimeGrid[i] = std::min(this->visibilityTimeGrid[i]+1, 16);
		}
		else {
			this->visibilityTimeGrid[i] -= this->visibilityTimeGrid[i] ? 1 : 0;
		}
	}
}

Coordinate VisibilityManager::getTileCoords(Coordinate c) const {
	return Coordinate((c.x)/tileWidth, (c.y)/tileWidth); // hackish way to make rendered tiles line up with these internal cell coordinates. I know, I know.
}

int VisibilityManager::getTileIndex(Coordinate tile, int team) const {
	int x = tile.x;
	int y = tile.y;
	if ( 0<= team && team<numTeams && 0<=x && x<w && 0<=y && y<h)
		return team*h*w + x*h + y;
	return -1;
}
