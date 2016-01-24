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
	UnitID createUnit(TeamID team, UnitTemplateID unitTemplateID, Coordinate pos);
	Unit& getUnit(UnitID i);
	UnitID smallestUnusedUnitID();

	InhabitedGrid inhabitedGrid;
	std::map<UnitID, Unit> unitsByID;
	std::map<TeamID, Team> teams;

private:
	UnitID smallestUnusedUnitID_;
};
