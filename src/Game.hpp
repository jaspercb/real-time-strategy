#pragma once

#include <vector>
#include <map>
#include <memory>

#include "typedefs.hpp"
#include "InhabitedGrid.hpp"

class Team; // forward declaration

class Unit;
class InhabitedGrid;

class Game
{
public:
	Game();
	std::shared_ptr<Unit> getUnit(UnitID i);
	UnitID smallestUnusedUnitID();

	std::shared_ptr<InhabitedGrid> inhabitedGrid;
	std::vector<std::shared_ptr<Unit> > units; // keeps track of all active units
	std::map<UnitID, std::shared_ptr<Unit> > unitsByID;

	std::vector<std::shared_ptr<Team> > teams;

private:
	UnitID smallestUnusedUnitID_;
};
