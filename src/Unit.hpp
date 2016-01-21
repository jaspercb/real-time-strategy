#pragma once

#include <deque>
#include <vector>
#include <memory>

#include "Coordinate.hpp"

class Team;
class UnitTemplate;
class Weapon;
class Command;
class UnitState;

typedef int UnitID;

enum QueueSetting{
	QUEUE_OVERWRITE,
	QUEUE_FRONT,
	QUEUE_BACK
};

class Unit
{
public:
	Unit(std::shared_ptr<Team> team_, std::shared_ptr<UnitTemplate>);
	int update(); //returns 1 if should be destroyed, 0 otherwise
	void handleCommand(Command command, QueueSetting qSetting);
	void move(Coordinate c);

	const std::shared_ptr<Team> team;
	const UnitID id;
	Coordinate xy;
	int z;
	int hp;
	
private:
	const std::shared_ptr<UnitTemplate> unitTemplate_;
	std::deque<UnitState*> StateQueue_;
	std::vector<Weapon> weapons_;
};
