#include "Game.hpp"

#include "Unit.hpp"
#include "InhabitedGrid.hpp"

#include "Logging.hpp"

Game::Game():
	smallestUnusedUnitID_(0),
	smallestUnusedTeamID_(0),
	inhabitedGrid(InhabitedGrid(this, 100, 100, 10, 10))
	{
		//inhabitedGrid = InhabitedGrid(this, 100, 100, 10, 10);
	}

TeamID Game::createTeam(){
	TeamID id = smallestUnusedTeamID();
	Team t = Team(*this, id);
	this->teamsByID.emplace(id, t);

	return id;
}

UnitID Game::createUnit(TeamID teamID, UnitTemplateID unitTemplateID, Coordinate pos){
	UnitID id = smallestUnusedUnitID();
	unitsByID.emplace(id, Unit(*this, id, teamID, unitTemplateID, pos));
	inhabitedGrid.emplace(getUnit(id));

	return id;
}

void Game::deleteUnit(UnitID id){
	Unit& u = this->getUnit(id);
	this->inhabitedGrid.erase(u);
	this->unitsByID.erase(id);
}

Unit& Game::getUnit(UnitID i)
{
	try{
		return unitsByID.at(i);
	}
	catch (std::out_of_range e){
		debugLog("Game::getUnit(UnitID) called with invalid UnitID: ");
		debugLog(i);
		debugLog(e.what());
	}
}

Team& Game::getTeam(TeamID i)
{
	try{
		return teamsByID.at(i);
	}
	catch (std::out_of_range e){
		debugLog("Game::getTeam(TeamID) called with invalid TeamID: ");
		debugLog(i);
		debugLog(e.what());
	}
}

void Game::tick(){
	std::vector<UnitID> toDelete;
	for (auto u = this->unitsByID.begin(); u!=this->unitsByID.end(); u++){
		if (u->second.update() == STATUS_REMOVE){
			toDelete.push_back(u->first);
		}
	}
	for (auto u = toDelete.begin(); u!=toDelete.end(); u++){
		this->deleteUnit(*u);
	}
}

UnitID Game::smallestUnusedUnitID() {
	return smallestUnusedUnitID_++;
}

TeamID Game::smallestUnusedTeamID() {
	return smallestUnusedTeamID_++;
}

bool Game::teamsAreFriendly(TeamID a, TeamID b){
	return a==b;
}