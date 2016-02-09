#include "StateGotoCoordinate.hpp"
#include "Unit.hpp"
#include "InhabitedGrid.hpp"
#include "Logging.hpp"

StateGotoCoordinate::StateGotoCoordinate(Coordinate targ):
	target(targ)
	{}

StateExitCode StateGotoCoordinate::update(Unit& unit){
	// if we're not at the target, move towards the target
	// otherwise we're done
	if (pythagoreanDistanceLessThan(unit.xy, target, 0)) {
		unit.animationState = ANIMSTATE_IDLE;
		return STATE_EXIT_COMPLETE;
	}
	else{
		unit.move_towards(target);
		return STATE_EXIT_INCOMPLETE;
	}
}
