#pragma once

#include <vector>
#include <map>
#include <memory>

#include "typedefs.hpp"
#include "InhabitedGrid.hpp"
#include "Team.hpp"

class Unit;
class InhabitedGrid;

class Game
{
public:
	Game();
	TeamID createTeam();
	UnitID createUnit(TeamID team, UnitTemplateID unitTemplateID, Coordinate pos);
	void deleteUnit(UnitID);
	Unit& getUnit(UnitID i);
	Team& getTeam(TeamID i);

	UnitID smallestUnusedUnitID();
	TeamID smallestUnusedTeamID();

	void tick();

	bool teamsAreFriendly(TeamID, TeamID);

	InhabitedGrid inhabitedGrid;
	std::map<UnitID, Unit> unitsByID;
	std::map<TeamID, Team> teamsByID;

private:
	TeamID smallestUnusedTeamID_;
	UnitID smallestUnusedUnitID_;
};
