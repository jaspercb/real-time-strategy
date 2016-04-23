#pragma once

#include "typedefs.hpp"
#include "enums.hpp"

#include <set>
#include <memory>

class UnitState;

class Command {
	public:
		Command();
		Command(CommandType::Enum cmdtype_);
		CommandType::Enum cmdtype;
		QueueSetting::Enum queueSetting;
		std::set<UnitID> commanded;
		UnitID targetID;
		Coordinate targetCoord;
		UnitTemplateID unitTemplateID;

		std::shared_ptr<UnitState> stateptr;
};
