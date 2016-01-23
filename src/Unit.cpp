#include "Unit.hpp"

#include <cmath>

#include "Team.hpp"
#include "UnitTemplate.hpp"
#include "Command.hpp"
#include "Weapon.hpp"
#include "UnitState.hpp"
#include "typedefs.hpp"
#include "Game.hpp"

Unit::Unit(Game &g, UnitID uID, TeamID tID, UnitTemplateID utID):
game(g),
unitID(uID),
teamID(tID),
unitTemplateID(utID)
{
	UnitTemplate& unitTemplate = getUnitTemplate();
	
	hp = unitTemplate.maxHP();

	weapons_ = std::vector<Weapon>();

	for (auto it = unitTemplate.weaponTemplates.begin(); it!=unitTemplate.weaponTemplates.end(); it++){
		weapons_.push_back(Weapon(*it, *this));
	}
}

UnitTemplate& Unit::getUnitTemplate(){
	return game.teams.at(teamID).unitTemplates.at(unitTemplateID);
}

int Unit::update()
{
	for (auto i = weapons_.begin(); i!=weapons_.end(); i++)
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

void Unit::move(const Coordinate c){
	const Coordinate oldcoord = xy;
	xy = c;
	game.inhabitedGrid.updatePos(*this, oldcoord);
}

void Unit::damage(const int quant){
	hp -= quant;
}

int Unit::getAttackRange(){
	int r = 0;
	for (auto it = weapons_.begin(); it!=weapons_.end(); it++){
		r = std::max(r, it->weaponTemplate.range());
	}
	return r;
}

void Unit::move_towards(const Coordinate c){
	int dx = c.first - xy.first;
	int dy = c.second - xy.second;
	int dr = std::pow(dx*dx + dy*dy, 0.5);
	int spd = getUnitTemplate().speed();
	move(Coordinate( xy.first + spd*dx/dr , xy.second + spd*dy/dr) ); 
}

void Unit::attack(Unit& target){
	// Fires all weapons that are
	// a) off cooldown
	// b) in range
	// c) capable of hitting the target's dimension

	for (auto it = weapons_.begin(); it!=weapons_.end(); it++){
		it->fire(target);
	}
}