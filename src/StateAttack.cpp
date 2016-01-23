#include "StateAttack.hpp"
#include "Unit.hpp"
#include "InhabitedGrid.hpp"
#include "Game.hpp"

StateAttack::StateAttack(UnitID targ):
	targetID(targ)
	{}

int StateAttack::update(Unit& unit){
	// if we're not in attacking range of the target, move towards the target
	// otherwise do nothing

	try{
		Unit& target = unit.game.getUnit(targetID);
		if (pythagoreanDistance(unit.xy, target.xy) > unit.getAttackRange()) {
			unit.move_towards(target.xy);
			return STATE_EXIT_INCOMPLETE;
		}
		else{
			unit.attack(target);
		}
	}
	catch (int e){
		return STATE_EXIT_COMPLETE;
	}
}
