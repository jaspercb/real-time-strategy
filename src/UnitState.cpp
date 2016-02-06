#include "UnitState.hpp"

#include "Unit.hpp"
#include "Command.hpp"
#include "Event.hpp"

#include "StateAttack.hpp"
#include "StateGotoCoordinate.hpp"
#include "StateIdle.hpp"
#include "Logging.hpp"

UnitState* UnitState::handleCommand(Unit& unit, Command command)
{
	switch (command.cmdtype){
		case CMD_IDLE: {
			return new StateIdle();
		}
		case CMD_GOTOCOORD: {
			return new StateGotoCoordinate(command.targetCoord);
		}
		case CMD_ATTACK: {
			return new StateAttack(command.targetID);
		}
	}
	return NULL;
}

UnitState* UnitState::handleEvent(Unit& unit, Event event)
{
	return NULL;
}

StateExitCode UnitState::update(Unit& unit) {
	return STATE_EXIT_INCOMPLETE;
}