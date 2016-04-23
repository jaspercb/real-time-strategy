#include "UnitState.hpp"

#include "Unit.hpp"
#include "Command.hpp"
#include "Event.hpp"

#include "StateAttack.hpp"
#include "StateGoto.hpp"
#include "StateIdle.hpp"
#include "StateAttackMove.hpp"
#include "Logging.hpp"

std::shared_ptr<UnitState> UnitState::handleCommand(Unit* unit, Command command) {
	switch (command.cmdtype){
		case CommandType::Idle: {
			return std::shared_ptr<UnitState>(new StateIdle());
		}
		case CommandType::Gotocoord: {
			return std::shared_ptr<UnitState>(new StateGoto(command.targetCoord));
		}
		case CommandType::Attack: {
			return std::shared_ptr<UnitState>(new StateAttack(command.targetID));
		}
		case CommandType::Attackmove: {
			return std::shared_ptr<UnitState>(new StateAttackMove(command.targetCoord));
		}
		case CommandType::Build: {
			unit->startBuilding(command.unitTemplateID);
		}
		default: {
			return NULL;
		}
	}
}

std::shared_ptr<UnitState> UnitState::handleEvent(Unit* unit, Event event) {
	return NULL;
}

StateExitCode UnitState::update(Unit* unit) {
	return STATE_EXIT_INCOMPLETE;
}

std::vector<Coordinate> UnitState::getStateWaypoints() {
	return std::vector<Coordinate>();
}
