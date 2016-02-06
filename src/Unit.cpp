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
#include "StateIdle.hpp"

Unit::Unit(Game &g, UnitID uID, TeamID tID, UnitTemplateID utID, Coordinate pos):
game(g),
unitID(uID),
teamID(tID),
unitTemplateID(utID),
xy(pos),
animationState(ANIMSTATE_IDLE),
drawAnimationStep(0),
drawFacingAngle(0),
attackTargetID(0)
{
	UnitTemplate& unitTemplate = getUnitTemplate();
	
	hp = unitTemplate.maxHP();
	es = unitTemplate.maxES();
	dimension = unitTemplate.dimension;

	for (auto it = unitTemplate.weaponTemplates.begin(); it!=unitTemplate.weaponTemplates.end(); it++){
		weapons_.push_back(Weapon(*it, *this));
	}

	StateQueue_.push_back(new StateIdle());
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
animationState(u.animationState),
drawAnimationStep(u.drawAnimationStep),
drawFacingAngle(u.drawFacingAngle),
attackTargetID(u.attackTargetID),
StateQueue_(u.StateQueue_)
{
	for(Weapon &w : u.weapons_) {
		weapons_.push_back(Weapon(w, *this));
	}

	u.weapons_.clear();
}

UnitTemplate& Unit::getUnitTemplate(){
	return game.getTeam(teamID).unitTemplates.at(unitTemplateID);
}

UpdateStatus Unit::update()
{
	UnitTemplate &unitTemplate = getUnitTemplate();

	drawAnimationStep++;
	
	if (StateQueue_.empty()) {
		StateQueue_.push_back(new StateIdle());
	}
	switch (animationState){
		case (ANIMSTATE_DYING): {
			if (drawAnimationStep>20){
				return STATUS_REMOVE;
			}
			return STATUS_OK;
		}

		case (ANIMSTATE_ATTACKING): {

		}


		//case (ANIMSTATE_WALKING):


		default:
			if (hp<=0) {
				if (unitTemplate.drawer->deathCycleLength == 0)
					return STATUS_REMOVE;
				animationState = ANIMSTATE_DYING;
				drawAnimationStep = 0;
				return STATUS_OK;
			}

			this->hp = std::min(unitTemplate.maxHP(), hp+unitTemplate.regHP());
			this->es = std::min(unitTemplate.maxES(), es+unitTemplate.regES());

			for (auto i = this->weapons_.begin(); i != this->weapons_.end(); i++)
				i->update();

			if (StateQueue_.size()) {
				StateExitCode stateComplete = StateQueue_.front()->update(*this);
				if (stateComplete == STATE_EXIT_COMPLETE) {
					UnitState* p = StateQueue_.front();
					StateQueue_.pop_front();
					// delete p; // Uncommenting this causes a segfault when a movement ends. We definitely have a memory leak.
				}
			}
			if (StateQueue_.empty()) {
				StateQueue_.push_back(new StateIdle());
			}
	}
	return STATUS_OK;
}

void Unit::handleCommand(Command command)
{
	if (StateQueue_.empty()){
		StateQueue_.push_front(new StateIdle());
	}

	UnitState* state = StateQueue_.front()->handleCommand(*this, command);
	if (state != NULL) {
		switch (command.queueSetting) {
			case QUEUE_OVERWRITE: { // delete state queue and replace with just this command
				for (auto it = StateQueue_.begin(); it != StateQueue_.end(); it++) {
					delete *it;
				}
				StateQueue_.clear();
				StateQueue_.push_front(state);
			}
			case QUEUE_BACK: { // append to queue, do after other states
				StateQueue_.push_back(state);
			}
			case QUEUE_FRONT: { // prepend to queue, but execute other states after
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

void Unit::damage(const int quant, const DamageType dmgtype, Unit& attackedBy) {
	UnitTemplate& utmpl = getUnitTemplate();
	if (es>0)
		es -= quant*utmpl.getDamageEffectivenessVsES(dmgtype);
	else{
		int dmg = quant*utmpl.getDamageEffectivenessVsHP(dmgtype);
		dmg = std::max(1, dmg-utmpl.armor());
		hp -= dmg;
	}
	lastAttackingTeamID = attackedBy.unitID;
	lastAttackingUnitID = attackedBy.teamID;
}

int Unit::getAttackRange(){
	/*
	int r = 0;
	for (auto it = weapons_.begin(); it!=weapons_.end(); it++) {
		r = std::max(r, it->weaponTemplate.range());
	}
	return r;
	*/

	return weapons_.begin()->weaponTemplate.range();
}

void Unit::move_towards(const Coordinate c){
	if (animationState!=ANIMSTATE_DYING){
		int dx = c.first - xy.first;
		int dy = c.second - xy.second;
		int dr = std::pow(dx*dx + dy*dy, 0.5);
		int spd = getUnitTemplate().speed();
		if (pythagoreanDistance(xy, c)<=spd) {
			move(c);
			animationState = ANIMSTATE_IDLE;
			//drawAnimationStep = 0;
		}
		else {
			move(Coordinate( xy.first + spd*dx/dr , xy.second + spd*dy/dr) );
			this->animationState = ANIMSTATE_WALKING;
			this->drawFacingAngle = (180/M_PI) * std::atan2(dy, dx);
			//drawFacingAngle = (18+((int)(std::atan2(dy, dx)*(9/M_PI)) + 4 ))%18;
			//debugLog("setting drawFacingAngle to:");
			//debugLog((int)drawFacingAngle);
		}
	}
}

void Unit::attack(Unit& target){
	// Starts or continues the unit's attacking state.
	// If the attacking animation has completed, fires all weapons.

	UnitTemplate& unitTemplate = this->getUnitTemplate();

	switch (this->animationState){
		case ANIMSTATE_DYING:{
			this->drawAnimationStep = 0;
			this->attackTargetID = 0;
			return;
		}
		case ANIMSTATE_ATTACKING: {
			if (target.unitID == this->attackTargetID){
				if (this->drawAnimationStep+1 < unitTemplate.drawer->attackCycleLength){
					this->drawAnimationStep++;
					this->drawFacingAngle = (180/M_PI) * std::atan2(target.xy.second-this->xy.second, target.xy.first-this->xy.first); // turn to face target
				}
				else if (this->drawAnimationStep+1 == unitTemplate.drawer->attackCycleLength) {
					this->drawFacingAngle = (180/M_PI) * std::atan2(target.xy.second-this->xy.second, target.xy.first-this->xy.first); // turn to face target
					for (auto it = weapons_.begin(); it!=weapons_.end(); it++) {
						it->fire(target);
					}
				}
				else{
					this->drawAnimationStep = 0;
					this->attackTargetID = 0;
				}
				return;
			}
		}
		default: {
			if (!game.teamsAreFriendly(teamID, target.teamID) && pythagoreanDistance(this->xy, target.xy) <= this->getAttackRange() ) {
				this->animationState = ANIMSTATE_ATTACKING;
				this->drawAnimationStep = 0;
				this->attackTargetID = target.unitID;
			}
		}
	}
}

void Unit::draw(SDL_Renderer* renderer){
	getUnitTemplate().drawer->draw(renderer, *this);
}
