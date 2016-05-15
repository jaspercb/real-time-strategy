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
			return std::make_shared<StateIdle>();
		}
		case CommandType::Gotocoord: {
			return std::make_shared<StateGoto>(command.targetCoord);
		}
		case CommandType::Attack: {
			return std::make_shared<StateAttack>(command.targetID);
		}
		case CommandType::Attackmove: {
			return std::make_shared<StateAttackMove>(command.targetCoord);
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
