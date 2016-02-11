#pragma once

#include "StateGotoCoordinate.hpp"
#include "StateAttack.hpp"

class StateAttackMove : public UnitState
{
public:
	StateAttackMove(Coordinate c);
	StateExitCode update(Unit& unit);
protected:
	bool updateTarget(Unit& unit); // returns True if a valid target is found within range
	StateGotoCoordinate gotostate;
	StateAttack atkstate;
private:
	bool attacking;
};
