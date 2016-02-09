#include "Game.hpp"

#include "Unit.hpp"
#include "InhabitedGrid.hpp"

#include "Logging.hpp"

Game::Game():
	inhabitedGrid(InhabitedGrid(this, 25000, 25000)),
	smallestUnusedUnitID_(1),
	smallestUnusedTeamID_(1)
	{
		//inhabitedGrid = InhabitedGrid(this, 100, 100, 10, 10);
	}

TeamID Game::createTeam() {
	TeamID id = smallestUnusedTeamID();
	Team t = Team(*this, id);
	this->teamsByID.emplace(id, t);

	return id;
}

UnitID Game::createUnit(TeamID teamID, UnitTemplateID unitTemplateID, Coordinate pos) {
	UnitID id = this->smallestUnusedUnitID();
	this->unitsByID.emplace(id, Unit(*this, id, teamID, unitTemplateID, pos));
	this->inhabitedGrid.emplace(this->getUnit(id));

	return id;
}

void Game::deleteUnit(UnitID id) {
	Unit& u = this->getUnit(id);
	this->inhabitedGrid.erase(u);
	this->unitsByID.erase(id);
}

Unit& Game::getUnit(UnitID i) {
	return unitsByID.at(i);
}

Team& Game::getTeam(TeamID i) {
	try{
		return teamsByID.at(i);
	}
	catch (std::out_of_range e){
		debugLog("Game::getTeam(TeamID) called with invalid TeamID: ");
		debugLog(i);
		debugLog(e.what());
	}
}

void Game::tick() {
	std::vector<UnitID> toDelete;
	for (auto u = this->unitsByID.begin(); u!=this->unitsByID.end(); u++){
		if (u->second.update() == STATUS_REMOVE){
			toDelete.push_back(u->first);
		}
	}
	for (auto u = toDelete.begin(); u!=toDelete.end(); u++){
		this->deleteUnit(*u);
	}

	this->resolveCollisions();
}

void Game::resolveCollisions() {
	// Resolves 
	for (auto &id_unit_pair : this->unitsByID){
		UnitID id = id_unit_pair.first;
		Unit& unit = id_unit_pair.second;
		for ( auto &otherid : this->inhabitedGrid.unitsCollidingWith(unit) ) {
			Unit& other = this->getUnit(otherid);
			Distance dx = other.xy.first - unit.xy.first;
			Distance dy = other.xy.second - unit.xy.second;
			dx = dx ? ((Distance)30000/dx) : 0;
			dy = dy ? ((Distance)30000/dy) : 0;
			other.move_towards(Coordinate(other.xy.first+dx, other.xy.second+dy));
		}

	}
}

UnitID Game::smallestUnusedUnitID() {
	return smallestUnusedUnitID_++;
}

TeamID Game::smallestUnusedTeamID() {
	return smallestUnusedTeamID_++;
}

bool Game::teamsAreFriendly(TeamID a, TeamID b) {
	return a==b;
}