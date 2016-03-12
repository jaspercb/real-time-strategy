#include "UnitState.hpp"

#include "Unit.hpp"
#include "Command.hpp"
#include "Event.hpp"

#include "StateAttack.hpp"
#include "StateGotoCoordinate.hpp"
#include "StateIdle.hpp"
#include "StateAttackMove.hpp"
#include "Logging.hpp"

std::shared_ptr<UnitState> UnitState::handleCommand(Unit& unit, Command command)
{
	switch (command.cmdtype){
		case CMD_IDLE: {
			return std::shared_ptr<UnitState>(new StateIdle());
		}
		case CMD_GOTOCOORD: {
			return std::shared_ptr<UnitState>(new StateGotoCoordinate(command.targetCoord));
		}
		case CMD_ATTACK: {
			return std::shared_ptr<UnitState>(new StateAttack(command.targetID));
		}
		case CMD_ATTACKMOVE: {
			return std::shared_ptr<UnitState>(new StateAttackMove(command.targetCoord));
		}
		case CMD_BUILD: {
			unit.tryToBuild(command.unitTemplateID);
		}
	}
	return NULL;
}

std::shared_ptr<UnitState> UnitState::handleEvent(Unit& unit, Event event)
{
	return NULL;
}

StateExitCode UnitState::update(Unit& unit) {
	return STATE_EXIT_INCOMPLETE;
}

std::vector<Coordinate> UnitState::getStateWaypoints() {
	return std::vector<Coordinate>();
}
