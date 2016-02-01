#pragma once

#include "UnitState.hpp"
#include "typedefs.hpp"

class StateAttack : public UnitState
{
public:
	StateAttack(UnitID);
	StateExitCode update(Unit& unit);
private:
	UnitID targetID;
	//std::deque<Coordinate> path;
};
