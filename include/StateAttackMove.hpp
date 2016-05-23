#pragma once

#include "StateGoto.hpp"
#include "StateAttack.hpp"

class StateAttackMove : public UnitState
{
public:
	StateAttackMove(Coordinate c);
	StateExitCode update(Unit* unit);
	Path getStateWaypoints();
protected:
	bool updateTarget(Unit* unit); // returns True if a valid target is found within range
	StateGoto gotostate;
	StateAttack atkstate;
private:
	bool attacking;
};
