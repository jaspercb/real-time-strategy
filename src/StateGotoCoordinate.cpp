#include "StateGotoCoordinate.hpp"
#include "Unit.hpp"
#include "InhabitedGrid.hpp"

StateGotoCoordinate::StateGotoCoordinate(Coordinate targ):
	target(targ)
	{}

StateExitCode StateGotoCoordinate::update(Unit& unit){
	// if we're not at the target, move towards the target
	// otherwise we're done
	if (pythagoreanDistance(unit.xy, target) > 0){
		unit.move_towards(target);
		return STATE_EXIT_INCOMPLETE;
	}
	else{
		return STATE_EXIT_COMPLETE;
	}
}
