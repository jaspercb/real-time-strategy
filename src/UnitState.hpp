#pragma once

#include <deque>
#include "typedefs.hpp"

class Unit; //forward declarations
class Command;
class Event;


enum StateExitCode{
	STATE_EXIT_INCOMPLETE,
	STATE_EXIT_COMPLETE
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


class StateIdle : public UnitState
{
public:
	StateExitCode update(Unit& unit); // returns 1 if should be removed, 0 otherwise
};

class StateTargeting : public UnitState
{
public:
	Unit* target;
	StateExitCode update(Unit& unit);
};
