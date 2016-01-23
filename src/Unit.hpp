#pragma once

#include <deque>
#include <vector>
#include <memory>

#include "typedefs.hpp"
#include "Weapon.hpp"

class Team;
class UnitTemplate;
class Command;
class UnitState;
class Game;

typedef int UnitID;

enum QueueSetting{
	QUEUE_OVERWRITE,
	QUEUE_FRONT,
	QUEUE_BACK
};

class Unit
{
public:
	//Unit();
	Unit(Game&, UnitID, TeamID, UnitTemplateID);
	UnitTemplate& getUnitTemplate();
	int update(); //returns 1 if should be destroyed, 0 otherwise
	void handleCommand(Command, QueueSetting);
	void move(Coordinate);
	void damage(int);

	const TeamID teamID;
	const UnitID unitID;
	const UnitTemplateID unitTemplateID;

	Coordinate xy;
	int z;
	int hp;
	
private:
	void move_towards(const Coordinate c);

	Game& game;

	std::deque<UnitState*> StateQueue_;
	std::vector<Weapon> weapons_;
};
