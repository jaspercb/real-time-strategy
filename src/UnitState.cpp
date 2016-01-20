#include "UnitState.hpp"

#include "Unit.hpp"
#include "Command.hpp"
#include "Event.hpp"

UnitState* UnitState::handleCommand(Unit& unit, Command command)
{
	return 0;
}

UnitState* UnitState::handleEvent(Unit& unit, Event event)
{
	return 0;
}

int StateIdle::update(Unit& unit){
	// if we've been attacked, chase
	// otherwise do nothing

	return 0;
}


int StateTargeting::update(Unit& unit){
	// if we've lose sight of the target, return 1
	// otherwise path to the target
	return 0;
}
