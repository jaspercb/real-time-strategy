#include "UnitState.hpp"

#include "Unit.hpp"
#include "Command.hpp"
#include "Event.hpp"

#include "StateAttack.hpp"
#include "StateGotoCoordinate.hpp"
#include "StateIdle.hpp"


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
	return 0;
}

UnitState* UnitState::handleEvent(Unit& unit, Event event)
{
	return 0;
}
