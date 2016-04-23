#include "Command.hpp"

#include "Unit.hpp"

Command::Command(){	
}

Command::Command(CommandType::Enum cmdtype_):
	cmdtype(cmdtype_)
	{}
