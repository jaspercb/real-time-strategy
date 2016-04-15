#pragma once

#include "UnitState.hpp"

#include "CoordinateOrUnit.hpp"

class StateGotoCoordinate : public UnitState
{
	friend class StateAttackMove;

public:
	StateGotoCoordinate(CoordinateOrUnit c);
	StateGotoCoordinate(Coordinate c);
	StateGotoCoordinate(const Unit& u);
	virtual StateExitCode update(Unit* unit);
	std::vector<Coordinate> getStateWaypoints();
protected:
	CoordinateOrUnit target;
	int last5distances[5];
	Distance last5FramesDistance;
	//std::deque<Coordinate> path;
};
