#include "StateIdle.hpp"
#include "Unit.hpp"

StateExitCode StateIdle::update(Unit* unit){
	if (unit->animationState != ANIMSTATE_DYING){
		unit->animationState = ANIMSTATE_IDLE;
	}
	return STATE_EXIT_INCOMPLETE;
}
