#ifndef OUTLINE_H
#define OUTLINE_H

#include <deque>
#include <map>
#include <vector>
#include <string>
#include <set>
#include <iostream>

//forward declarations of everything
class Command;
class UnitTemplate;
class Unit;
class UnitState;
class WeaponTemplate;
class Team;
class Game;

typedef int TeamID;
typedef int UnitID;
typedef int TemplateID;
typedef std::pair<int, int> Coordinate;

enum CommandType{
	CMD_MOVE,
	CMD_AMOVE,
	CMD_TARGET,
};

class Command
{
public:
	Command(CommandType cmdtype_)
		: cmdtype(cmdtype_) {}
	CommandType cmdtype;
	std::vector<UnitID> commanded;

};

class EnvironmentSpec
{
public:
	EnvironmentSpec(bool _ground, bool _sea, bool _submerged, bool _air):
		ground(_ground),
		sea(_sea),
		submerged(_submerged),
		air(_air)
		{}
	bool ground, sea, submerged, air;
};

enum EventType{
	EVENT_DAMAGED,
	EVENT_DESTROYED,
	EVENT_TARGETED
};

class Event
{
public:
	EventType type_;
	UnitID subject;
	UnitID by;
};


class Weapon
{
public:
	Weapon(WeaponTemplate* weaponTemplate, Unit* _owner): 
		weaponTemplate_(weaponTemplate),
		owner(_owner) {
		}

	void fire();
	int ticksSinceFired(){return ticksSinceFired_;};
	virtual void update(){
		ticksSinceFired_++;
	}
	const WeaponTemplate* weaponTemplate_;
	const Unit* owner;

private:
	int ticksSinceFired_;
};

enum DamageType{
	DMG_ELECTRO,	// good against shields, shit against armor
	DMG_THERMAL,	// mediocre against shields and armor
	DMG_KINETIC,    // bad against shields, pretty good against armor
	DMG_EXPLOSIVE	// shit against shields, great against armor
};

class WeaponTemplate
{
public:
	WeaponTemplate();
	WeaponTemplate(std::string _name,
			DamageType _damageType,
			int _damage,
			int _reloadTime,
			int _range,
			int _weaponVelocity,
			int _aoeRadius,
			EnvironmentSpec dimensions) :
		name_(_name),
		damageType_(_damageType),
		damage_(_damage),
		reloadTime_(_reloadTime),
		range_(_range),
		weaponVelocity_(_weaponVelocity),
		aoeRadius_(_aoeRadius),
		dimensions_(dimensions)
		{};

	virtual std::string name(){return name_;};
	virtual DamageType damageType(){return damageType_;};
	virtual int damage(){return damage_;};
	virtual int reloadTime(){return reloadTime_;}; //
	virtual int range(){return range_;};
	virtual int weaponVelocity(){return weaponVelocity_;};
	virtual bool canFire(Weapon &weapon);
	virtual void fire(Weapon& weapon, Unit& target);
	virtual void fire(Weapon& weapon, Coordinate& target);
private:
	std::string name_;
	DamageType damageType_;
	bool targetGround_;
	int damage_;
	int reloadTime_;
	int range_;
	int weaponVelocity_;
	int aoeRadius_;
	EnvironmentSpec dimensions_;
};

bool WeaponTemplate::canFire(Weapon &weapon){
	return weapon.ticksSinceFired() >= reloadTime();
}

void WeaponTemplate::fire(Weapon& weapon, Unit& target){
	// if target in range
}

void WeaponTemplate::fire(Weapon& weapon, Coordinate& target){
	// if target in range
}


class UnitTemplate
{
public:
	UnitTemplate(std::string _name,
			int _maxHP,
			int _speed,
			int _radius,
			EnvironmentSpec _canTravelOn,
			std::vector<WeaponTemplate*> _weaponTemplates
			) :
		name_(_name),
		maxHP_(_maxHP),
		speed_(_speed),
		radius_(_radius),
		canTravelOn_(_canTravelOn),
		weaponTemplates_(_weaponTemplates) {std::cout<<"creating unit"<<std::endl;}
	std::string name();
	int maxHP();
	int speed();
	int radius();
	std::vector<WeaponTemplate*> weaponTemplates();
private:
	std::string name_;
	int maxHP_;
	int speed_;
	int radius_;
	EnvironmentSpec canTravelOn_;
	std::vector<WeaponTemplate*> weaponTemplates_;
};

int UnitTemplate::maxHP(){
	return maxHP_;
}

int UnitTemplate::speed(){
	return speed_;
}

int UnitTemplate::radius(){
	return radius_;
}

std::vector<WeaponTemplate*> UnitTemplate::weaponTemplates(){
	return weaponTemplates_;
}


class Team
{
public:
	Team(Game* _game):
		game(_game),
		id(1)
		//id(_game->getTeamID())
		{};

	const Game* game;
	const TeamID id;

private:
	std::map<TemplateID, UnitTemplate*> templates_;
};

class UnitState
{
public:
	virtual ~UnitState() {}
	virtual void enter(Unit& unit) {}
	virtual UnitState* handleCommand(Unit& unit, Command command) {return 0;}
	virtual UnitState* handleEvent(Unit& unit, Event event) {return 0;}
	virtual int update(Unit& unit) {} // returns 1 if should be removed, 0 otherwise
};


class StateIdle : public UnitState
{
public:
	int update(Unit& unit); // returns 1 if should be removed, 0 otherwise
};

int StateIdle::update(Unit& unit){
	// if we've been attacked, chase
	// otherwise do nothing

	return 0;
}

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

int StateTargeting::update(Unit& unit){
	// if we've lose sight of the target, return 1
	// otherwise path to the target
	return 0;
}

enum QueueSetting{
	QUEUE_OVERWRITE,
	QUEUE_FRONT,
	QUEUE_BACK
};

class Unit
{
public:
	Unit(Team* team_, UnitTemplate*);
	int update(); //returns 1 if should be destroyed, 0 otherwise
	void handleCommand(Command command, QueueSetting qSetting);
	void move(int x, int y, int z);

	const Team* team;
	const UnitID id;
	int x, y, z;

private:
	int hp;
	UnitTemplate* unitTemplate_;
	std::deque<UnitState*> StateQueue_;
	std::vector<Weapon> weapons_;
};

Unit::Unit(Team* team_, UnitTemplate* unitTemplate):
unitTemplate_(unitTemplate),
id(1) //FIX THIS SHIT YO
{
	hp = unitTemplate->maxHP();

	weapons_ = std::vector<Weapon>();

	for (std::vector<WeaponTemplate*>::iterator it = unitTemplate->weaponTemplates().begin(); it!=unitTemplate->weaponTemplates().end(); it++){
		weapons_.push_back(Weapon(*it, this));
	}
}

int Unit::update()
{
	for (std::vector<Weapon>::iterator i = weapons_.begin(); i!=weapons_.end(); i++)
	{
		i->update();
	}

	if (StateQueue_.size())
	{
		int stateComplete = StateQueue_.front()->update(*this);
		if (stateComplete)
		{
			delete StateQueue_.front();
			StateQueue_.pop_front();
		}
	}


	return 0;
}

void Unit::handleCommand(Command command, QueueSetting qSetting)
{
	UnitState* state = StateQueue_.front()->handleCommand(*this, command);
	if (state != NULL) {
		switch (qSetting){
			case QUEUE_OVERWRITE: // delete state queue and replace with just this command
			{
				for (std::deque<UnitState*>::const_iterator it = StateQueue_.begin(); it != StateQueue_.end(); it++)
				{
					delete *it;
				}
				StateQueue_.clear();
				StateQueue_.push_front(state);
			}
			case QUEUE_BACK: // append to queue, do after other states
			{
				StateQueue_.push_back(state);
			}
			case QUEUE_FRONT: // prepend to queue, but execute other states after
			{
				StateQueue_.push_front(state);
			}
		}
	}
}

class InhabitedGrid{
public:
	InhabitedGrid(int w, int h, int dw, int dh):
		cellsX(dw),
		cellsY(dh),
		cellWidth(w),
		cellHeight(h)
		{}
	void move(Unit* unit, int x, int y, int z);
	std::pair<int, int> getCellCoord(int x, int y);
	std::set<UnitID> getCell(int x, int y);
private:
	const int cellsX;
	const int cellsY;
	const int cellWidth;
	const int cellHeight;
	std::map<std::pair<int, int>, std::set<UnitID> > grid;
};

std::pair<int, int> InhabitedGrid::getCellCoord(int x, int y){
	return std::pair<int, int>(x/cellWidth, y/cellWidth);
}

void Unit::move(int nx, int ny, int nz){
	//game->grid->move(this, nx, ny, nz);
}

void InhabitedGrid::move(Unit* unit, int x, int y, int z){
	std::pair<int, int> oldpos, newpos;
	oldpos = getCellCoord(unit->x, unit->y);
	newpos = getCellCoord(x, y);
	unit->x = x;
	unit->y = y;
	unit->z = z;
	if (oldpos==newpos){
		return;
	}
	else{
		grid[oldpos].erase(unit->id);
		grid[oldpos].erase(unit->id);
	}

}


class Game
{
public:
	Game():
		smallestUnusedUnitID(1),
		inhabited(InhabitedGrid(10, 10, 1, 1))
		{};
	Unit* getUnit(UnitID i)
	{
		return units[i];
	}
	UnitID getUnitID(){
		return smallestUnusedUnitID++;
	}
private:
	std::vector<Unit*> units;
	std::map<UnitID, Unit*> unitsByID;
	UnitID smallestUnusedUnitID;
	InhabitedGrid inhabited;
};

const EnvironmentSpec GROUND_ONLY(1, 0, 0, 0);
const EnvironmentSpec AIR_ONLY(0, 0, 0, 1);
const EnvironmentSpec SEA_ONLY(0, 1, 0, 0);
const EnvironmentSpec SUBMERSIBLE_SEA(0, 1, 1, 0);


int main(){
	WeaponTemplate* testwpn = new WeaponTemplate("testWeapon", DMG_KINETIC, 50, 3, 40, 30, 0, GROUND_ONLY);
	std::vector<WeaponTemplate*> v;
	v.push_back(testwpn);
	v.push_back(testwpn);
	v.push_back(testwpn);
	Game* testgame = new Game();
	Team* testteam = new Team(testgame);
	UnitTemplate* testunittemplate = new UnitTemplate("testUnit", 100, 20, 20, GROUND_ONLY, v);
	Unit* testunit = new Unit(testteam, testunittemplate);
	//std::cout<<testunit->weaponTemplates().size()<<std::endl;
	delete testunit;
	delete testunittemplate;
	delete testwpn;
	delete testteam;
	delete testgame;
	return 0;
}

#endif
