#include "Unit.hpp"

#include <cmath>

#include "typedefs.hpp"

#include "Team.hpp"
#include "UnitTemplate.hpp"
#include "Command.hpp"
#include "Weapon.hpp"
#include "WeaponTemplate.hpp"
#include "UnitState.hpp"
#include "Game.hpp"
#include "Logging.hpp"
#include "Command.hpp"
#include "StateIdle.hpp"
#include "UserInterface.hpp"
#include "Builder.hpp"
#include "CoordinateOrUnit.hpp"

Unit::Unit(Game *g, UnitID uID, TeamID tID, UnitTemplateID utID, Coordinate pos):
game(g),
teamID(tID),
unitID(uID),
unitTemplateID(utID),
xy(pos),
animationState(AnimationState::Idle),
drawAnimationStep(0),
drawFacingAngle(0),

attackTargetID(0) {
	UnitTemplate* unitTemplate = getUnitTemplate();

	this->builder = new Builder(this, unitTemplate->buildslots());	
	this->hp = unitTemplate->maxHP();
	this->es = unitTemplate->maxES();
	this->dimension = unitTemplate->dimension;

	for (auto it = unitTemplate->weaponTemplates.begin(); it!=unitTemplate->weaponTemplates.end(); it++) {
		this->weapons_.push_back(Weapon(&(*it), this));
	}

	this->idleState = std::shared_ptr<UnitState>( new StateIdle() );
}

Unit::~Unit() {
	delete builder;
}

UnitTemplate* Unit::getUnitTemplate() const {
	return game->getTeam(teamID)->unitTemplates.at(unitTemplateID);
}

void Unit::tick() {
	UnitTemplate*unitTemplate = getUnitTemplate();
	
	drawAnimationStep++;

	if (!this->isDead()) {
		this->builder->tick();

		this->hp = std::min(unitTemplate->maxHP(), hp+unitTemplate->regHP());
		this->es = std::min(unitTemplate->maxES(), es+unitTemplate->regES());

		for (auto &weapon : this->weapons_) {
			weapon.update();
		}

		if (stateQueue_.empty() )
			this->idleState->update(this);

		while (stateQueue_.size() && STATE_EXIT_COMPLETE == stateQueue_.front()->update(this)) {
			stateQueue_.pop_front();
		}
	}
	else if (this->animationState != AnimationState::Dying) {
		this->animationState = AnimationState::Dying;
		this->drawAnimationStep = 0;
	}
}

void Unit::handleCommand(Command& command) {
	auto& state = command.stateptr;

	if (command.cmdtype == CommandType::Halt) {
		stateQueue_.clear();
	}
	if (state == nullptr) {
		if (this->stateQueue_.empty()) {
			state = this->idleState->handleCommand(this, command);
		}
		else {
			state = this->stateQueue_.front()->handleCommand(this, command);
		}
	}
	if (state != nullptr) {
		switch (command.queueSetting) {
			case QueueSetting::Overwrite: { // delete state queue and replace with just this command
				stateQueue_.clear();
				stateQueue_.push_front(state);
				break;
			}
			case QueueSetting::Back: { // append to queue, do after other states
				stateQueue_.push_back(state);
				break;
			}
			case QueueSetting::Front: { // prepend to queue, but execute other states after
				stateQueue_.push_front(state);
				break;
			}
		}
	}
}

void Unit::move(const Coordinate& c) {
	// Unchecked, instant movement to a given point.
	const Coordinate oldcoord = this->xy;
	this->xy = c;
	game->inhabitedGrid.updatePos(this, oldcoord);
}

void Unit::damage(const int quant, const DamageType::Enum dmgtype, Unit* attackedBy) {
	UnitTemplate* utmpl = getUnitTemplate();
	if (es>0)
		es -= quant*utmpl->getDamageEffectivenessVsES(dmgtype);
	else{
		int dmg = quant*utmpl->getDamageEffectivenessVsHP(dmgtype);
		dmg = std::max(1, dmg-utmpl->armor());
		hp -= dmg;
	}
	lastAttackingTeamID = attackedBy->unitID;
	lastAttackingUnitID = attackedBy->teamID;
}

Distance Unit::getAttackRange() {
	if (this->weapons_.size() == 0) {
		return 0;
	}
	else {
		return this->weapons_.begin()->weaponTemplate->range();
	}
}

bool Unit::canAttack(Unit* u) const {
	if (this->game->unitsAreFriendly(u->unitID,this->unitID) || u->animationState == AnimationState::Dying) {
		return false;
	}
	for (auto &w : this->weapons_) {
		if (w.canAttack(u) )
			return true;
	}
	return false;
}

void Unit::moveTowards(const CoordinateOrUnit dest) {
	if (this->getUnitTemplate()->speed() == 0
		or !dest.isValid())
		return;

	Coordinate c = dest.getCoordinate();

	// Speed-limited movement, with "collision handling"
	if (animationState!=AnimationState::Dying) {
		Coordinate dr = c-xy;
		Distance spd = this->getUnitTemplate()->speed();

		if (pythagoreanDistance(xy, c)<=spd) {
			if (this->game->inhabitedGrid.unitOKToMoveTo(this, c)) {
				this->move(c);
			}
		}
		else {
			dr.setLength(spd);
			Coordinate target = xy + dr;
			for (int i=0; i<8; i++) {
				if (this->game->inhabitedGrid.unitOKToMoveTo(this, target)) {
					this->move(target);
					this->animationState = AnimationState::Walking;
					
					int target_angle = (180/M_PI) * std::atan2(dr.y, dr.x);
					int dTheta = target_angle-this->drawFacingAngle;
					if (std::abs(dTheta)%360 < 10 || 120 < dTheta || -120 > dTheta ) {
						this->drawFacingAngle = target_angle;
					}
					else if ( dTheta < 0) {
						this->drawFacingAngle = (this->drawFacingAngle-12)%360;
					}
					else {
						this->drawFacingAngle = (this->drawFacingAngle+12)%360;
					}
					break;
				}
			}
		}
	}
}

/*!
 * Starts or continues the unit's attacking state. 
 * If the attacking animation has completed, fires all off-cooldown weapons.
 */
void Unit::attack(Unit* target) {
	UnitTemplate* unitTemplate = this->getUnitTemplate();
	int ticksUntilCanFire = this->weapons_[0].ticksUntilCanFire;
	int mainWeaponAnimationLength = unitTemplate->drawer.attackCycleLength;


	switch (this->animationState) {
		case AnimationState::Dying:{
			this->attackTargetID = 0;
			return;
		}
		case AnimationState::Attacking: {
			if (target->unitID == this->attackTargetID) { // to ensure a player can't start targeting one unit, then instantly switch to another
				//Logging::error(this->drawAnimationStep+1);
				if (this->drawAnimationStep < unitTemplate->drawer.attackCycleLength) {
					this->drawFacingAngle = (180/M_PI) * std::atan2(target->xy.y-this->xy.y, target->xy.x-this->xy.x); // turn to face target
				}
				else if (this->drawAnimationStep >= unitTemplate->drawer.attackCycleLength) {
					this->drawFacingAngle = (180/M_PI) * std::atan2(target->xy.y-this->xy.y, target->xy.x-this->xy.x); // turn to face target
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
			if (!game->teamsAreFriendly(teamID, target->teamID) && pythagoreanDistance(this->xy, target->xy) <= this->getAttackRange() ) {
				this->animationState = AnimationState::Attacking;
				this->drawAnimationStep = mainWeaponAnimationLength - ticksUntilCanFire;
				this->attackTargetID = target->unitID;
			}
		}
	}
}

void Unit::startBuilding(UnitTemplateID unitTemplateID) {
	if (!game->getTeam(teamID)->canBuild(unitTemplateID)) {
		Logging::error("Error: Unit::tryToBuild called, but UnitTemplate::isBuildable=false. uTemplateID: "+unitTemplateID);
		return;
	}
	if (builder) {
		for (auto& i : this->getUnitTemplate()->spawnables) {
			if (i == unitTemplateID) {
				builder->startBuilding(unitTemplateID, game->getTeam(this->teamID)->unitTemplates.at(unitTemplateID)->buildtime() );
				return;
			}
		}
		Logging::error(("Error: Unit::tryToBuild called on invalid pair"));
		Logging::error(("    Unit #" + std::to_string(this->unitID) + (", an instance of \"" + this->getUnitTemplate()->unitTemplateID) + "\""));
		Logging::error(("    attempted to build an instance of \"" + unitTemplateID + "\""));
	}
	else
		Logging::error("Unit::tryToBuild called on unit with no building component!");
}

void Unit::draw(SDL_Renderer* renderer, UserInterface* ui) {
	int alpha = ui->game->inhabitedGrid.getCoordVisibility(this->xy, ui->teamID);
	this->getUnitTemplate()->drawer.draw(renderer, this, ui, alpha);
}

bool Unit::isDead() const {
	return this->hp <= 0 || this->animationState == AnimationState::Dying;
}

bool Unit::shouldDelete() const {
	return (this->animationState == AnimationState::Dying) && (this->drawAnimationStep > 20);
}

std::vector<Coordinate> Unit::getStateWaypoints() {
	std::vector<Coordinate> ret;
	for (auto &i : this->stateQueue_)
		for (auto &j : i->getStateWaypoints() )
			ret.push_back(j);
	
	return ret;
}
