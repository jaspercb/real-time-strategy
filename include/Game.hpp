
#pragma once

#include <vector>
#include <map>
#include <memory>

#include "typedefs.hpp"

#include "InhabitedGrid.hpp"
#include "Terrain.hpp"

class Team;
class Unit;
class InhabitedGrid;
class Command;

class Game {
public:
	Game();
	virtual ~Game();

	TeamID createTeam();
	Team* getTeam(TeamID i);

	UnitID createUnit(TeamID team, UnitTemplateID unitTemplateID, Coordinate pos);
	bool existsUnit(UnitID id) const;
	Unit* getUnit(UnitID id);
	void deleteUnit(UnitID id);

	void handleCommand(const Command&);
	void tick();

	bool teamsAreFriendly(TeamID, TeamID);
	bool unitsAreFriendly(UnitID a, UnitID b);

	InhabitedGrid inhabitedGrid;
	Terrain terrain;

	std::map<TeamID, Team*> teamsByID;
	std::map<UnitID, Unit*> unitsByID;

private:
	void resolveCollisions();
	UnitID smallestUnusedUnitID();
	TeamID smallestUnusedTeamID();

	TeamID smallestUnusedTeamID_;
	UnitID smallestUnusedUnitID_;
};
