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

enum UpdateStatus{
	STATUS_OK,
	STATUS_DEAD
};

enum QueueSetting{
	QUEUE_OVERWRITE,
	QUEUE_FRONT,
	QUEUE_BACK
};

class Unit
{
public:

	//Unit();
	Unit(Game&, UnitID, TeamID, UnitTemplateID, Coordinate);
	UnitTemplate& getUnitTemplate();
	int update(); //returns 1 if should be destroyed, 0 otherwise
	void handleCommand(Command, QueueSetting);
	void move(const Coordinate);
	void damage(const int);
	int getAttackRange(); // returns the range of the 
	void move_towards(const Coordinate c);
	void attack(Unit& target);

	const TeamID teamID;
	const UnitID unitID;
	const UnitTemplateID unitTemplateID;

	Coordinate xy;
	int z;
	int hp;
	
	Game& game;
	std::vector<Weapon> weapons_;
private:


	std::deque<UnitState*> StateQueue_;
};
