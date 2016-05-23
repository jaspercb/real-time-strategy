#pragma once

#include "UnitState.hpp"

#include "CoordinateOrUnit.hpp"

class StateGoto : public UnitState
{
	friend class StateAttackMove;

public:
	StateGoto(CoordinateOrUnit c);
	StateGoto(Coordinate c);
	StateGoto(const Unit& u);
	virtual void enter(Unit* unit);
	virtual StateExitCode update(Unit* unit);
	Path getStateWaypoints();
protected:
	CoordinateOrUnit target;
};
