#pragma once

#include "UnitState.hpp"

class StateGotoCoordinate : public UnitState
{
public:
	StateGotoCoordinate(Coordinate c);
	StateExitCode update(Unit& unit);
private:
	Coordinate target;
	int last5distances[5];
	Distance last5FramesDistance;
	//std::deque<Coordinate> path;
};
