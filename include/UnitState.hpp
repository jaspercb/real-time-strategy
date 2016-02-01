#pragma once

#include <deque>
#include "typedefs.hpp"

class Unit; //forward declarations
class Command;
class Event;


enum StateExitCode{
	STATE_EXIT_INCOMPLETE, // signals that the state is incomplete, and should be continued
	STATE_EXIT_COMPLETE // signals that the state is complete, and the unit should switch to the next queued state
};


class UnitState
{
public:
	virtual ~UnitState() {}
	virtual void enter(Unit& unit) {}
	virtual UnitState* handleCommand(Unit& unit, Command command);
	virtual UnitState* handleEvent(Unit& unit, Event event);
	virtual StateExitCode update(Unit& unit) {} // returns 1 if should be removed, 0 otherwise
};
