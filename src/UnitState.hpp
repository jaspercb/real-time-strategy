#pragma once

#include <deque>
#include "typedefs.hpp"

class Unit; //forward declarations
class Command;
class Event;

class UnitState
{
public:
	virtual ~UnitState() {}
	virtual void enter(Unit& unit) {}
	virtual UnitState* handleCommand(Unit& unit, Command command);
	virtual UnitState* handleEvent(Unit& unit, Event event);
	virtual int update(Unit& unit) {} // returns 1 if should be removed, 0 otherwise
};


class StateIdle : public UnitState
{
public:
	int update(Unit& unit); // returns 1 if should be removed, 0 otherwise
};

class StateGoto : public UnitState
{
public:
	int update(Unit& unit);
private:
	Coordinate target;
	std::deque<Coordinate> path;
};

class StateTargeting : public UnitState
{
public:
	Unit* target;
	int update(Unit& unit);
};
