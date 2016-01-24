#include "StateIdle.hpp"

StateExitCode StateIdle::update(Unit& unit){
	return STATE_EXIT_INCOMPLETE;
}
