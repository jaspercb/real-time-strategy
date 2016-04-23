#include "Command.hpp"

Command::Command() :
	stateptr(nullptr)
	{}

Command::Command(CommandType::Enum cmdtype_):
	cmdtype(cmdtype_),
	stateptr(nullptr)
	{}
