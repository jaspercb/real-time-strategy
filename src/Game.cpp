#include "Game.hpp"

#include "Unit.hpp"
#include "InhabitedGrid.hpp"

#include "Logging.hpp"

Game::Game():
	inhabitedGrid(InhabitedGrid(this)),
	smallestUnusedUnitID_(1),
	smallestUnusedTeamID_(2)
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
			Unit& other = this->getUnit(otherid);
			if (unit.unitID == other.unitID) {
				continue;
			}

			else if (unit.xy == other.xy) {
				other.move_towards( Coordinate(
					other.xy.first + (
						( (other.unitID+other.xy.first) % 2 == 0) ? 5 :
																	-5 ),
					other.xy.second + (
						( (other.unitID+other.xy.second) % 3 == 0) ? 5 :
						( (other.unitID+other.xy.second) % 3 == 1) ? 0 :
																	-5 ) ) );
			}
			else if (unit.animationState != ANIMSTATE_DYING && (other.animationState == ANIMSTATE_IDLE || other.animationState == ANIMSTATE_ATTACKING) ) { // 
				Distance dx = other.xy.first - unit.xy.first;
				Distance dy = other.xy.second - unit.xy.second;
				//dx = dx ? 10000/dx : 0;
				//dy = dy ? 10000/dy : 1;
				dx/=3; // scale down for smoother movement
				dy/=3;
				other.move_towards( Coordinate(other.xy.first+dx, other.xy.second+dy) );
			}
/*			else if ( (unit.animationState != ANIMSTATE_IDLE && other.animationState != ANIMSTATE_WALKING) ) {
				Distance dx = other.xy.first - unit.xy.first;
				Distance dy = other.xy.second - unit.xy.second;
				dx = dx ? ((Distance)10000000/dx) : 20000*((unit.xy.first+unit.unitID)%2? 1 : -1);
				dy = dy ? ((Distance)10000000/dy) : 20000*((unit.xy.second+unit.unitID)%2? 1 : -1);
				other.move_towards(Coordinate(other.xy.first+dx, other.xy.second+dy));
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

