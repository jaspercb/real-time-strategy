#include "Unit.hpp"

#include <cmath>

#include "Team.hpp"
#include "UnitTemplate.hpp"
#include "Command.hpp"
#include "Weapon.hpp"
#include "UnitState.hpp"
#include "Coordinate.hpp"

Unit::Unit(std::shared_ptr<Team> team_, std::shared_ptr<UnitTemplate> unitTemplate):
unitTemplate_(unitTemplate),
id(1) //FIX THIS SHIT YO
{
	hp = unitTemplate->maxHP();

	weapons_ = std::vector<Weapon>();

	for (std::vector<std::shared_ptr<WeaponTemplate> >::iterator it = unitTemplate->weaponTemplates().begin(); it!=unitTemplate->weaponTemplates().end(); it++){
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

void Unit::move(Coordinate c){
	const Coordinate oldcoord = xy;
	xy = c;
	team->game->inhabitedGrid->updatePos(*this, oldcoord);
}

void Unit::damage(int quant){
	this->hp -= quant;
}
