#pragma once

#include <vector>
#include <map>

#include "typedefs.hpp"

class Unit;
class InhabitedGrid;

class Game
{
public:
	Game();
	Unit* getUnit(UnitID i);
	UnitID smallestUnusedUnitID();

	InhabitedGrid* inhabited;
	const std::vector<Unit*> units;
	const std::map<UnitID, Unit*> unitsByID;

private:
	UnitID smallestUnusedUnitID_;
};
