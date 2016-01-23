#include "StateGotoCoordinate.hpp"
#include "Unit.hpp"
#include "InhabitedGrid.hpp"

StateGotoCoordinate::StateGotoCoordinate(Coordinate targ):
	target(targ)
	{}

int StateGotoCoordinate::update(Unit& unit){
	// if we're not in attacking range of the target, move towards the target
	// otherwise do nothing
	if (pythagoreanDistance(unit.xy, target) > unit.getAttackRange()){
		unit.move_towards(target);
		return STATE_EXIT_INCOMPLETE;
	}
	else{
		return STATE_EXIT_COMPLETE;
	}
}
