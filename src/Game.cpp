#include "Game.hpp"

#include "Unit.hpp"
#include "InhabitedGrid.hpp"

#include "Logging.hpp"

Game::Game():
	smallestUnusedUnitID_(1),
	smallestUnusedTeamID_(2),
	inhabitedGrid(InhabitedGrid(this, 64, 64, 10)) // Currently hard coded
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
	this->inhabitedGrid.startTrackingVisibility(this->getUnit(id));

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
	for (auto &u : this->unitsByID)
		u.second.tick();

	for (const auto &u : this->unitsByID) {
		if (u.second.shouldDelete()) {
			toDelete.push_back(u.first);
		}
	}

	for (auto &u : toDelete)
		this->deleteUnit(u);


	this->resolveCollisions();
	this->inhabitedGrid.tick();
}

void Game::handleCommand(const Command& cmd){
	for (const UnitID &unitID : cmd.commanded){
		this->getUnit(unitID).handleCommand(cmd);
	}
}

void Game::resolveCollisions() {
	// Resolves any soft collisions between units.
	for (auto &id_unit_pair : this->unitsByID){
		UnitID id = id_unit_pair.first;
		Unit& unit = id_unit_pair.second;
		for ( auto &otherid : this->inhabitedGrid.unitsCollidingWith(unit) ) {
			if (unit.unitID == otherid) {
				continue;
			}

			Unit& other = this->getUnit(otherid);

			if (unit.xy == other.xy) {					// Deterministic location offset keeps game in lockstep.
				other.move_towards( other.xy + Coordinate(
							( (other.unitID+other.xy.x) % 2 == 0) ?  5 :
																	-5 ,
						( (other.unitID+other.xy.y) % 3 == 0) ?  5 :
						( (other.unitID+other.xy.y) % 3 == 1) ?  0 :
																-5 ) );
			}
			else if (unit.animationState != ANIMSTATE_DYING && (other.animationState == ANIMSTATE_IDLE || other.animationState == ANIMSTATE_ATTACKING) ) { // 
				Coordinate c = other.xy - unit.xy;
				c.setLength(other.getUnitTemplate().radius() + unit.getUnitTemplate().radius() - c.length());
				other.move_towards( other.xy + c );
			}
/*			else if ( (unit.animationState != ANIMSTATE_IDLE && other.animationState != ANIMSTATE_WALKING) ) {
				Distance dx = other.xy.x - unit.xy.x;
				Distance dy = other.xy.second - unit.xy.second;
				dx = dx ? ((Distance)10000000/dx) : 20000*((unit.xy.x+unit.unitID)%2? 1 : -1);
				dy = dy ? ((Distance)10000000/dy) : 20000*((unit.xy.second+unit.unitID)%2? 1 : -1);
				other.move_towards(Coordinate(other.xy.x+dx, other.xy.second+dy));
			}
*/
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

bool Game::unitsAreFriendly(UnitID a, UnitID b) {
	return this->teamsAreFriendly(this->getUnit(a).teamID, this->getUnit(b).teamID);
}

