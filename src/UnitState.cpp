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
