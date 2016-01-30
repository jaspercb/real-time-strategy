#include "Unit.hpp"

#include <cmath>

#include "Team.hpp"
#include "UnitTemplate.hpp"
#include "Command.hpp"
#include "Weapon.hpp"
#include "UnitState.hpp"
#include "typedefs.hpp"
#include "Game.hpp"
#include "Logging.hpp"
#include "Command.hpp"

Unit::Unit(Game &g, UnitID uID, TeamID tID, UnitTemplateID utID, Coordinate pos):
game(g),
unitID(uID),
teamID(tID),
unitTemplateID(utID),
xy(pos),
drawWalkStep(0),
drawFacingAngle(0)
{
	UnitTemplate& unitTemplate = getUnitTemplate();
	
	hp = unitTemplate.maxHP();
	es = unitTemplate.maxES();
	dimension = unitTemplate.dimension;

	for (auto it = unitTemplate.weaponTemplates.begin(); it!=unitTemplate.weaponTemplates.end(); it++){
		weapons_.push_back(Weapon(*it, *this));
	}
}

Unit::Unit(Unit &&u) : 
teamID(u.teamID),
unitID(u.unitID),
unitTemplateID(u.unitTemplateID),
xy(u.xy),
dimension(u.dimension),
hp(u.hp),
es(u.es),
game(u.game),
drawWalkStep(u.drawWalkStep),
drawFacingAngle(u.drawFacingAngle)
{
	for(Weapon &w : u.weapons_) {
		weapons_.push_back(Weapon(w, *this));
	}

	u.weapons_.clear();
}

UnitTemplate& Unit::getUnitTemplate(){
	return game.getTeam(teamID).unitTemplates.at(unitTemplateID);
}

int Unit::update()
{
	UnitTemplate &unitTemplate = getUnitTemplate();

	//'if unit is dead' logic should probably go here

	hp = std::min(unitTemplate.maxHP(), hp+unitTemplate.regHP());
	es = std::min(unitTemplate.maxES(), es+unitTemplate.regES());


	for (auto i = weapons_.begin(); i!=weapons_.end(); i++)
	{
		i->update();
	}

	if (StateQueue_.size())
	{
		StateExitCode stateComplete = StateQueue_.front()->update(*this);
		if (stateComplete == STATE_EXIT_COMPLETE)
		{
			delete StateQueue_.front();
			StateQueue_.pop_front();
		}
	}
	return 0;
}

void Unit::handleCommand(Command command)
{
	UnitState* state = StateQueue_.front()->handleCommand(*this, command);
	if (state != NULL) {
		switch (command.queueSetting){
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

void Unit::damage(const int quant, const DamageType dmgtype) {
	UnitTemplate& utmpl = getUnitTemplate();
	if (es>0)
		es -= quant*utmpl.getDamageEffectivenessVsES(dmgtype);
	else{
		int dmg = quant*utmpl.getDamageEffectivenessVsHP(dmgtype);
		dmg = std::max(1, dmg-utmpl.armor());
		hp -= dmg;
	}
}

int Unit::getAttackRange(){
	int r = 0;
	for (auto it = weapons_.begin(); it!=weapons_.end(); it++) {
		r = std::max(r, it->weaponTemplate.range());
	}
	return r;
}

void Unit::move_towards(const Coordinate c){
	int dx = c.first - xy.first;
	int dy = c.second - xy.second;
	int dr = std::pow(dx*dx + dy*dy, 0.5);
	int spd = getUnitTemplate().speed();
	if (pythagoreanDistance(xy, c)<=spd) {
		move(c);
		drawWalkStep = 0;
	}
	else {
		move(Coordinate( xy.first + spd*dx/dr , xy.second + spd*dy/dr) );
		drawWalkStep = (drawWalkStep+1)%9;
		drawFacingAngle = (18+((int)(std::atan2(dy, dx)*(9/M_PI)) + 4 ))%18;
		debugLog("setting drawFacingAngle to:");
		debugLog((int)drawFacingAngle);
	}
}

void Unit::attack(Unit& target){
	// Fires all weapons that are
	// a) off cooldown
	// b) in range
	// c) capable of hitting the target's dimension

	if (!game.teamsAreFriendly(teamID, target.teamID)) {
		for (auto it = weapons_.begin(); it!=weapons_.end(); it++) {
			it->fire(target);
		}
	}
}

void Unit::draw(SDL_Renderer* renderer){
	getUnitTemplate().drawer->draw(renderer, *this);
}
