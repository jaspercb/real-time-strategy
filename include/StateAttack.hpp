#pragma once

#include "UnitState.hpp"
#include "typedefs.hpp"

class StateAttack : public UnitState
{
	friend class StateAttackMove;
public:
	StateAttack(UnitID);
	StateExitCode update(Unit* unit);
	Path getStateWaypoints();
private:
	UnitID targetID;
	//std::deque<Coordinate> path;
};
