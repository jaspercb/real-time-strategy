#pragma once

#include "UnitState.hpp"

class StateGotoCoordinate : public UnitState
{
	friend class StateAttackMove;

public:
	StateGotoCoordinate(Coordinate c);
	virtual StateExitCode update(Unit* unit);
	std::vector<Coordinate> getStateWaypoints();
protected:
	Coordinate targetCoord;
	int last5distances[5];
	Distance last5FramesDistance;
	//std::deque<Coordinate> path;
};
