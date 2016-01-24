#pragma once

#include "UnitState.hpp"

class StateGotoCoordinate : public UnitState
{
public:
	StateGotoCoordinate(Coordinate c);
	StateExitCode update(Unit& unit);
private:
	Coordinate target;
	//std::deque<Coordinate> path;
};
