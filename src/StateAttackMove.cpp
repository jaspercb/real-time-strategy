#include "StateAttackMove.hpp"
#include "StateAttack.hpp"
#include "Unit.hpp"
#include "InhabitedGrid.hpp"
#include "Game.hpp"
#include "Logging.hpp"

StateAttackMove::StateAttackMove(Coordinate destination):
	gotostate(StateGotoCoordinate(destination)),
	atkstate(StateAttack(0)),
	attacking(false)
	{}

StateExitCode StateAttackMove::update(Unit& unit) {
	// If we're already targeting someone, behave like we're attacking them
	// If we're in attacking range of an enemy, behave like we're attacking them
	// Otherwise, behave like we're walking
	// otherwise do nothing
	
	if (!attacking){ // if we're not targeting anyone
		this->updateTarget(unit);
	}

	if (this->attacking){
		debugLog("attackstate");
		StateExitCode ret = this->atkstate.update(unit);
		if (ret == STATE_EXIT_COMPLETE){
			this->attacking = false;
		}
		return STATE_EXIT_INCOMPLETE;
	}
	else {
		debugLog("walkstate");
		return this->gotostate.update(unit);
	}
}

bool StateAttackMove::updateTarget(Unit& unit) {
	auto potentialIDs = unit.game.inhabitedGrid.unitsInCircle(unit.xy, unit.getAttackRange());
	for (auto &i : potentialIDs){
		Unit& potentialUnit = unit.game.getUnit(i);
		if (unit.canAttack(potentialUnit)){
			this->atkstate.targetID = i;
			this->attacking = true;
			return true;
		}
	}
	this->attacking = false;
	return false;
}
