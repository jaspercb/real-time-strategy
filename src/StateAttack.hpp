#include "UnitState.hpp"
#include "typedefs.hpp"

class StateAttack : public UnitState
{
public:
	StateAttack(UnitID);
	int update(Unit& unit);
private:
	UnitID targetID;
	//std::deque<Coordinate> path;
};
