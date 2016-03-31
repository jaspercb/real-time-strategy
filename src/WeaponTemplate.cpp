#include "WeaponTemplate.hpp"
#include "EnvironmentSpec.hpp"
#include "Weapon.hpp"
#include "InhabitedGrid.hpp"
#include "Unit.hpp"
#include "Game.hpp"
#include "Animation.hpp"
#include "UserInterface.hpp"

#include "Logging.hpp"

WeaponTemplate::WeaponTemplate(std::string _name,
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

WeaponTemplate::WeaponTemplate(std::ifstream &is)
{
	damage_ = 0;
	reloadTime_ = 0;
	range_ = 0;
	weaponVelocity_ = 0;
	aoeRadius_ = 0;
	dimensions_ = EnvironmentSpec(0, 0, 0, 0);

	std::string s;
	while (is>>s){
		if (s=="name"){
			char c;
			is.get(c); //to strip out space
			getline(is, name_);
		}
		else if (s=="#"){
			getline(is, s); // this is a comment. we ignore comments.
		}
		else if (s=="damageType"){
			is>>s;
			if (s=="DMG_ELECTRO")
				damageType_ = DMG_ELECTRO;
			else if (s=="DMG_THERMAL")
				damageType_ = DMG_THERMAL;
			else if (s=="DMG_KINETIC")
				damageType_ = DMG_KINETIC;
			else if (s=="DMG_EXPLOSIVE")
				damageType_ = DMG_EXPLOSIVE;
			else{
				debugLog("Error: WeaponTemplate "+name_+" created with invalid damage type: "+s);
				throw;
			}
		}
		else if (s=="damage")
			is>>damage_;
		else if (s=="reloadTime")
			is>>reloadTime_;
		else if (s=="range")
			is>>range_;
		else if (s=="weaponVelocity")
			is>>weaponVelocity_;
		else if (s=="aoeRadius")
			is>>aoeRadius_;
		else if (s=="dimensions"){
			bool a, b, c, d;
			is>>a>>b>>c>>d;
			dimensions_ = EnvironmentSpec(a, b, c, d);
		}
		else if (s=="hitAnimation")
			is>>hitAnimation_;
		else if (s=="}"){
			return;
		}
	}
}

/*
WeaponTemplate::WeaponTemplate(std::string s):
	WeaponTemplate(std::ifstream("../conf/weapons/"+s))
	{debugLog("swag");}
*/

bool WeaponTemplate::canFire(const Weapon* weapon) const
{
	return weapon->ticksUntilCanFire <= 0;
}

bool WeaponTemplate::canAttack(const Unit* target) const{
	return dimensions_.overlaps(target->dimension);
} // returns whether the weapon is theoretically capable of hitting the target, IGNORING COOLDOWN

void WeaponTemplate::fire(Weapon* weapon, Unit* target)
{
	if (canFire(weapon) &&
	pythagoreanDistance(weapon->owner->xy, target->xy)<=range() &&
	this->canAttack(target) )
	{
		if (aoeRadius()){
			std::vector<UnitID> p = target->game->inhabitedGrid.unitsInCircle(target->xy, aoeRadius());
			for (UnitID &targetID : p){
				Unit* potentialTarget = target->game->getUnit(targetID);
				if (!target->game->teamsAreFriendly(weapon->owner->teamID, potentialTarget->teamID) &&
				this->canAttack(potentialTarget) )
				{
					potentialTarget->damage(damage(), damageType_, weapon->owner);
				}
			}
		}
		else{
			target->damage(damage(), damageType_, weapon->owner);
		}

		this->playHitAnimation(gUserInterface, target->xy);
		weapon->ticksUntilCanFire = reloadTime();
	}
}

void WeaponTemplate::fire(Weapon* weapon, const Coordinate& target)
{
	if (canFire(weapon) &&
	(pythagoreanDistance(weapon->owner->xy, target)<=range()) &&
	(aoeRadius() > 0) ) //only works for AOE weapons
	{
		if (aoeRadius()) {
			std::vector<UnitID> p = weapon->owner->game->inhabitedGrid.unitsInCircle(target, aoeRadius());
			for (UnitID &targetID : p){
				Unit* potentialTarget = weapon->owner->game->getUnit(targetID);
				if (!weapon->owner->game->teamsAreFriendly(weapon->owner->teamID, potentialTarget->teamID) &&
				this->canAttack(potentialTarget) )
				{
					potentialTarget->damage(damage(), damageType_, weapon->owner);
				}
			}
		}
		this->playHitAnimation(gUserInterface, target);
		weapon->ticksUntilCanFire = reloadTime();
	}
}

void WeaponTemplate::playHitAnimation(UserInterface* ui, const Coordinate& target) {
	ui->playAnimation(hitAnimation_, target, 3);
}