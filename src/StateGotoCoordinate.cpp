#include "StateGotoCoordinate.hpp"
#include "Unit.hpp"
#include "InhabitedGrid.hpp"
#include "Logging.hpp"
#include "UnitTemplate.hpp"

StateGotoCoordinate::StateGotoCoordinate(Coordinate targ):
	target(targ),
	last5FramesDistance(0)
	{
		for (int i=0; i<5; i++){
			this->last5distances[i] = 0;
		}
	}

StateExitCode StateGotoCoordinate::update(Unit& unit){
	// if we're not at the target, move towards the target
	// otherwise we're done
	if (pythagoreanDistanceLessThan(unit.xy, target, 0)) {
		unit.animationState = ANIMSTATE_IDLE;
		return STATE_EXIT_COMPLETE;
	}
	else{
		Coordinate oldC = unit.xy;
		unit.move_towards(target);
		last5FramesDistance -= last5distances[0];
		for (int i=0; i<4; i++){
			this->last5distances[i]=last5distances[i+1];
		}
		last5distances[4] = pythagoreanDistance(oldC, unit.xy);
		last5FramesDistance += last5distances[4];

		if (pythagoreanDistanceLessThan(unit.xy, this->target, 15000) && last5FramesDistance < 600){
			unit.animationState = ANIMSTATE_IDLE;
			return STATE_EXIT_COMPLETE;
		}

		return STATE_EXIT_INCOMPLETE;
	}
}
