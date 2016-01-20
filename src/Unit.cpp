#include "Unit.hpp"

#include <cmath>

#include "Team.hpp"
#include "UnitTemplate.hpp"
#include "Command.hpp"
#include "Weapon.hpp"
#include "UnitState.hpp"
#include "Coordinate.hpp"

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

void Unit::move(int nx, int ny, int nz){
	//game->grid->move(this, nx, ny, nz);
}


int distance(Coordinate& one, Coordinate& two){
	return (int) pow( pow(one.first-two.first, 2.0) + pow(one.second-two.second, 2.0), 0.5);
}

int coordinate(Unit& unit, Coordinate& coord){
	return (int) pow( pow(unit.y-coord.second, 2.0) + pow(unit.x-coord.first, 2.0), 0.5);
}

int distance(Unit& unit1, Unit& unit2){
	return (int) pow( pow(unit1.y-unit2.y, 2.0) + pow(unit1.x-unit2.x, 2.0), 0.5);
}
