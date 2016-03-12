#pragma once

#include "typedefs.hpp"

#include <vector>

class Unit;

enum CommandType {
	CMD_IDLE, 
	CMD_GOTOCOORD,
	CMD_ATTACK,
	CMD_ATTACKMOVE,
	CMD_HALT,
	CMD_BUILD
};

enum QueueSetting {
	QUEUE_OVERWRITE,
	QUEUE_FRONT,
	QUEUE_BACK
};

class Command {
	public:
		Command();
		Command(CommandType cmdtype_);
		CommandType cmdtype;
		QueueSetting queueSetting;
		std::vector<UnitID> commanded;
		UnitID targetID;
		Coordinate targetCoord;
		UnitTemplateID unitTemplateID;
};
