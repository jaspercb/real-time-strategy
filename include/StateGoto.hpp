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
	virtual StateExitCode update(Unit* unit);
	std::vector<Coordinate> getStateWaypoints();
protected:
	CoordinateOrUnit target;
	int last5distances[5];
	Distance last5FramesDistance;
	//std::deque<Coordinate> path;
};
