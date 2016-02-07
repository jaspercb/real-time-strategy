#pragma once

#include <deque>
#include <memory>
#include "typedefs.hpp"

class Unit; //forward declarations
class Command;
class Event;
class UnitState;

enum StateExitCode{
	STATE_EXIT_INCOMPLETE, // signals that the state is incomplete, and should be continued
	STATE_EXIT_COMPLETE // signals that the state is complete, and the unit should switch to the next queued state
};

class UnitState
{
public:
	virtual ~UnitState() {}
	virtual void enter(Unit& unit) {}
	virtual std::shared_ptr<UnitState> handleCommand(Unit& unit, Command command);
	virtual std::shared_ptr<UnitState> handleEvent(Unit& unit, Event event);
	virtual StateExitCode update(Unit& unit); // returns STATE_EXIT_COMPLETE if should be removed, STATE_EXIT_INCOMPLETE otherwise
};
