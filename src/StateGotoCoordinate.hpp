#include "UnitState.hpp"

class StateGotoCoordinate : public UnitState
{
public:
	StateGotoCoordinate(Coordinate c);
	int update(Unit& unit);
private:
	Coordinate target;
	//std::deque<Coordinate> path;
};
