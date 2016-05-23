#include "StateGoto.hpp"

#include "Game.hpp"
#include "Terrain.hpp"
#include "InhabitedGrid.hpp"
#include "Logging.hpp"
#include "Unit.hpp"

#include "globals.hpp"

StateGoto::StateGoto(CoordinateOrUnit targ):
	target(targ)
	{}

StateGoto::StateGoto(Coordinate c): StateGoto(CoordinateOrUnit(c)) {};
StateGoto::StateGoto(const Unit& u): StateGoto(CoordinateOrUnit(u)) {};

void StateGoto::enter(Unit* unit) {
	game->terrain.getPath(unit->xy, target, unit->cachedPath, unit->dimension);
}

StateExitCode StateGoto::update(Unit* unit){
	// if we're not at the target, move towards the target
	// otherwise we're done
	Coordinate targetCoord = unit->cachedPath.front();
	if (pythagoreanDistanceLessThan(unit->xy, targetCoord, 600)) {
		unit->cachedPath.pop_front();
	} else {
		unit->moveTowards(targetCoord);
	}

	if (unit->cachedPath.empty()) {
		unit->animationState = AnimationState::Idle;
		return STATE_EXIT_COMPLETE;
	} else {
		return STATE_EXIT_INCOMPLETE;
	}
}

Path StateGoto::getStateWaypoints() {
	if (target.isValid())
		return Path{this->target.getCoordinate()};
	else
		return Path();
}