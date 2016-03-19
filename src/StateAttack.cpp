#include "StateAttack.hpp"
#include "Unit.hpp"
#include "InhabitedGrid.hpp"
#include "Game.hpp"
#include "Logging.hpp"
#include "globals.hpp"

StateAttack::StateAttack(UnitID targ):
	targetID(targ)
	{}

StateExitCode StateAttack::update(Unit& unit) {
	// if we're not in attacking range of the target, move towards the target
	// otherwise do nothing
	try{
		Unit& target = unit.game.getUnit(targetID);
		if (target.animationState == ANIMSTATE_DYING) {
			unit.animationState = ANIMSTATE_IDLE;
			return STATE_EXIT_COMPLETE;
		}
		else{
			if (pythagoreanDistance(unit.xy, target.xy) > unit.getAttackRange()) {
				unit.move_towards(target.xy);
				return STATE_EXIT_INCOMPLETE;
			}
			else{
				unit.attack(target);
				return STATE_EXIT_INCOMPLETE;
			}
		}
	}
	catch (const std::exception& e){
		return STATE_EXIT_COMPLETE;
	}
}


std::vector<Coordinate> StateAttack::getStateWaypoints() {
	Unit& u = game->getUnit(this->targetID);
	return std::vector<Coordinate>{u.xy};
}
