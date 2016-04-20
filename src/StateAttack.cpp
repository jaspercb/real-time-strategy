#include "StateAttack.hpp"
#include "Unit.hpp"
#include "InhabitedGrid.hpp"
#include "Game.hpp"
#include "Logging.hpp"
#include "CoordinateOrUnit.hpp"
#include "globals.hpp"

StateAttack::StateAttack(UnitID targ):
	targetID(targ)
	{}

StateExitCode StateAttack::update(Unit* unit) {
	// if we're not in attacking range of the target, move towards the target
	// otherwise do nothing
	try{
		Unit* target = unit->game->getUnit(targetID);
		if (target->animationState == AnimationState::Dying) {
			unit->animationState = AnimationState::Idle;
			return STATE_EXIT_COMPLETE;
		}
		else{
			if (pythagoreanDistance(unit->xy, target->xy) > unit->getAttackRange()) {
				unit->moveTowards(target);
				return STATE_EXIT_INCOMPLETE;
			}
			else{
				unit->attack(target);
				return STATE_EXIT_INCOMPLETE;
			}
		}
	}
	catch (const std::exception& e){
		return STATE_EXIT_COMPLETE;
	}
	return STATE_EXIT_COMPLETE;
}


std::vector<Coordinate> StateAttack::getStateWaypoints() {
	Unit* u = game->getUnit(this->targetID);
	return std::vector<Coordinate>{u->xy};
}
