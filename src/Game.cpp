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
	teamsByID.emplace(id, t);

	return id;
}

UnitID Game::createUnit(TeamID teamID, UnitTemplateID unitTemplateID, Coordinate pos){
	UnitID id = smallestUnusedUnitID();
	debugLog(id);
	Unit u = Unit(*this, id, teamID, unitTemplateID, pos);
	unitsByID.emplace(u.unitID, u);
	inhabitedGrid.emplace(u);

	return u.unitID;
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
		debugLog("Game::getUnit(TeamID) called with invalid TeamID: ");
		debugLog(i);
		debugLog(e.what());
	}
}

UnitID Game::smallestUnusedUnitID() {
	return smallestUnusedUnitID_++;
}

TeamID Game::smallestUnusedTeamID() {
	return smallestUnusedTeamID_++;
}
