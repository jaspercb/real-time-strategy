#pragma once

#include <vector>
#include <map>
#include <memory>

#include "typedefs.hpp"

#include "InhabitedGrid.hpp"
#include "VisibilityManager.hpp"
#include "Terrain.hpp"

class Team;
class Unit;
class InhabitedGrid;
class Command;
class UnitTemplate;

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

	UnitTemplate* getUnitTemplate(TeamID, UnitTemplateID);

	void handleCommand(Command&);
	void onMove(Unit* unit, Coordinate oldcoord);
	void tick();

	bool teamsAreFriendly(TeamID, TeamID);
	bool unitsAreFriendly(UnitID a, UnitID b);

	InhabitedGrid inhabitedGrid;
	VisibilityManager visibilityManager;
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
