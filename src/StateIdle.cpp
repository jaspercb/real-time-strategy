#include "StateIdle.hpp"
#include "Unit.hpp"

StateExitCode StateIdle::update(Unit* unit){
	if (unit->animationState != AnimationState::Dying){
		unit->animationState = AnimationState::Idle;
	}
	return STATE_EXIT_INCOMPLETE;
}
