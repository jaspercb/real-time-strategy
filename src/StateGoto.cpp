#include "StateGoto.hpp"

#include "Unit.hpp"
#include "InhabitedGrid.hpp"
#include "Logging.hpp"

StateGoto::StateGoto(CoordinateOrUnit targ):
	target(targ),
	last5FramesDistance(100000)
	{
		for (int i=0; i<5; i++){
			this->last5distances[i] = 10000;
		}
	}

StateGoto::StateGoto(Coordinate c): StateGoto(CoordinateOrUnit(c)) {};
StateGoto::StateGoto(const Unit& u): StateGoto(CoordinateOrUnit(u)) {};

StateExitCode StateGoto::update(Unit* unit){
	// if we're not at the target, move towards the target
	// otherwise we're done
	if (!target.isValid()) {
		return STATE_EXIT_COMPLETE;
	}

	Coordinate targetCoord = target.getCoordinate();
	if (pythagoreanDistanceLessThan(unit->xy, targetCoord, 50)) {
		unit->animationState = AnimationState::Idle;
		return STATE_EXIT_COMPLETE;
	}
	else{
		Coordinate oldC = unit->xy;
		unit->moveTowards(targetCoord);
		last5FramesDistance -= last5distances[0];
		for (int i=0; i<4; i++){
			this->last5distances[i]=last5distances[i+1];
		}
		last5distances[4] = pythagoreanDistance(oldC, unit->xy);
		last5FramesDistance += last5distances[4];

		if (pythagoreanDistanceLessThan(unit->xy, targetCoord, 15000) && last5FramesDistance < 600){
			unit->animationState = AnimationState::Idle;
			return STATE_EXIT_COMPLETE;
		}

		return STATE_EXIT_INCOMPLETE;
	}
}

std::vector<Coordinate> StateGoto::getStateWaypoints() {
	if (target.isValid())
		return std::vector<Coordinate>{this->target.getCoordinate()};
	else
		return std::vector<Coordinate>();
}