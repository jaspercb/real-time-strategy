#pragma once

#include <vector>
#include <map>
#include <memory>

#include "typedefs.hpp"
#include "InhabitedGrid.hpp"

class Unit;
class InhabitedGrid;

class Game
{
public:
	Game();
	std::shared_ptr<Unit> getUnit(UnitID i);
	UnitID smallestUnusedUnitID();

	std::shared_ptr<InhabitedGrid> inhabitedGrid;
	const std::vector<std::shared_ptr<Unit> > units;
	const std::map<UnitID, std::shared_ptr<Unit> > unitsByID;

private:
	UnitID smallestUnusedUnitID_;
};
