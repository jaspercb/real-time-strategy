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
#include "UserInterface.hpp"

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
	
	this->hp = unitTemplate.maxHP();
	this->es = unitTemplate.maxES();
	this->dimension = unitTemplate.dimension;

	for (auto it = unitTemplate.weaponTemplates.begin(); it!=unitTemplate.weaponTemplates.end(); it++){
		this->weapons_.push_back(Weapon(*it, *this));
	}

	this->idleState = std::shared_ptr<UnitState>( new StateIdle() );
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
stateQueue_(u.stateQueue_),
idleState(u.idleState)
{
	for(Weapon &w : u.weapons_) {
		weapons_.push_back(Weapon(w, *this));
	}

	u.weapons_.clear();
}

UnitTemplate& Unit::getUnitTemplate() const{
	return game.getTeam(teamID).unitTemplates.at(unitTemplateID);
}

UpdateStatus Unit::update()
{
	UnitTemplate &unitTemplate = getUnitTemplate();

	drawAnimationStep++;
	
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

			if (stateQueue_.empty() )
				this->idleState->update(*this);

			while (stateQueue_.size() && STATE_EXIT_COMPLETE == stateQueue_.front()->update(*this)) {
				stateQueue_.pop_front();
			}
	}
	return STATUS_OK;
}

void Unit::handleCommand(Command command)
{
	std::shared_ptr<UnitState> state;
	
	if (command.cmdtype == CMD_HALT){
		stateQueue_.clear();
	}
	else if (this->stateQueue_.empty()) {
		state = this->idleState->handleCommand(*this, command);
	}
	else {
		state = this->stateQueue_.front()->handleCommand(*this, command);
	}
	if (state != NULL) {
		switch (command.queueSetting) {
			case QUEUE_OVERWRITE: { // delete state queue and replace with just this command
				stateQueue_.clear();
				stateQueue_.push_front(state);
				break;
			}
			case QUEUE_BACK: { // append to queue, do after other states
				stateQueue_.push_back(state);
				break;
			}
			case QUEUE_FRONT: { // prepend to queue, but execute other states after
				stateQueue_.push_front(state);
				break;
			}
		}
	}
}

void Unit::move(const Coordinate c){
	// Unchecked, instant movement to a given point.
	const Coordinate oldcoord = this->xy;
	this->xy = c;
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

Distance Unit::getAttackRange() {
	/*
	int r = 0;
	for (auto it = weapons_.begin(); it!=weapons_.end(); it++) {
		r = std::max(r, it->weaponTemplate.range());
	}
	return r;
	*/

	if (this->weapons_.size() == 0) {
		return 0;
	}
	else {
		return this->weapons_.begin()->weaponTemplate.range();
	}
}

bool Unit::canAttack(Unit& u) const {
	if (this->game.unitsAreFriendly(u.unitID,this->unitID) || u.animationState == ANIMSTATE_DYING) {
		return false;
	}
	for (auto &w : this->weapons_) {
		if (w.canAttack(u) )
			return true;
	}
	return false;
}

void Unit::move_towards(const Coordinate c) {
	if (this->getUnitTemplate().speed() == 0) {
		return;
	}
	// Speed-limited movement, with collision handling
	if (animationState!=ANIMSTATE_DYING) {
		Distance dx = c.first - xy.first;
		Distance dy = c.second - xy.second;
		Distance dr = pythagoreanDistance(xy, c);
		Distance spd = this->getUnitTemplate().speed();

		if (pythagoreanDistance(xy, c)<=spd) {
			if (this->game.inhabitedGrid.unitOKToMoveTo(*this, c)) {
				this->move(c);
			}
		}
		else {
			int factor = 1.4;
			Coordinate target = Coordinate( xy.first + factor*spd*dx/dr , xy.second + factor*spd*dy/dr);
			for (int i=0; i<8; i++) {
				target = Coordinate( this->xy.first + (target.first - this->xy.first)/factor, this->xy.second + (target.second - this->xy.second)/factor);
				if (this->game.inhabitedGrid.unitOKToMoveTo(*this, target)) {
					this->move(target);
					this->animationState = ANIMSTATE_WALKING;
					this->drawFacingAngle = (180/M_PI) * std::atan2(dy, dx);
					//drawFacingAngle = (18+((int)(std::atan2(dy, dx)*(9/M_PI)) + 4 ))%18;
					//debugLog("setting drawFacingAngle to:");
					//debugLog((int)drawFacingAngle);
					break;
				}
			}
		}
	}
}

void Unit::attack(Unit& target){
	// Starts or continues the unit's attacking state.
	// If the attacking animation has completed, fires all weapons.
	UnitTemplate& unitTemplate = this->getUnitTemplate();
	int ticksUntilCanFire = this->weapons_[0].ticksUntilCanFire;
	int mainWeaponAnimationLength = unitTemplate.drawer->attackCycleLength;


	switch (this->animationState){
		case ANIMSTATE_DYING:{
			this->attackTargetID = 0;
			return;
		}
		case ANIMSTATE_ATTACKING: {
			if (target.unitID == this->attackTargetID) { // to ensure a player can't start targeting one unit, then instantly switch to another
				//debugLog(this->drawAnimationStep+1);
				if (this->drawAnimationStep < unitTemplate.drawer->attackCycleLength) {
					this->drawFacingAngle = (180/M_PI) * std::atan2(target.xy.second-this->xy.second, target.xy.first-this->xy.first); // turn to face target
				}
				else if (this->drawAnimationStep >= unitTemplate.drawer->attackCycleLength) {
					this->drawFacingAngle = (180/M_PI) * std::atan2(target.xy.second-this->xy.second, target.xy.first-this->xy.first); // turn to face target
					for (auto it = weapons_.begin(); it!=weapons_.end(); it++) {
						it->fire(target);
					}
					this->drawAnimationStep = mainWeaponAnimationLength - ticksUntilCanFire;
				}
				else {
					this->drawAnimationStep = 0;
					this->attackTargetID = 0;
				}
				return;
			}
		}
		default: {
			if (!game.teamsAreFriendly(teamID, target.teamID) && pythagoreanDistance(this->xy, target.xy) <= this->getAttackRange() ) {
				this->animationState = ANIMSTATE_ATTACKING;
				this->drawAnimationStep = mainWeaponAnimationLength - ticksUntilCanFire;
				this->attackTargetID = target.unitID;
			}
		}
	}
}

void Unit::draw(SDL_Renderer* renderer, UserInterface* ui) {
	int alpha = ui->game.inhabitedGrid.getCoordVisibility(this->xy, ui->teamID);
	this->getUnitTemplate().drawer->draw(renderer, *this, ui, alpha);
}



std::vector<Coordinate> Unit::getStateWaypoints() {
	std::vector<Coordinate> ret;
	for (auto &i : this->stateQueue_)
		for (auto &j : i->getStateWaypoints() )
			ret.push_back(j);
	
	return ret;
}
