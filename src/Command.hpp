#pragma once

#include "UnitID.hpp"

#include <vector>

class Unit;

enum CommandType{
	CMD_MOVE,
	CMD_AMOVE,
	CMD_TARGET,
};

class Command
{
public:
	Command(CommandType cmdtype_);
	CommandType cmdtype;
	std::vector<UnitID> commanded;

};
