#include "WeaponTemplate.hpp"

#include "EnvironmentSpec.hpp"
#include "Weapon.hpp"
#include "InhabitedGrid.hpp"
#include "Unit.hpp"
#include "Game.hpp"
#include "Animation.hpp"
#include "UserInterface.hpp"
#include "Unitset.hpp"

#include "Logging.hpp"

WeaponTemplate::WeaponTemplate(std::string _name,
		DamageType::Enum _damageType,
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
			if (s=="electrical")
				damageType_ = DamageType::Electrical;
			else if (s=="thermal")
				damageType_ = DamageType::Thermal;
			else if (s=="kinetic")
				damageType_ = DamageType::Kinetic;
			else if (s=="explosive")
				damageType_ = DamageType::Explosive;
			else{
				Logging::error("Error: WeaponTemplate "+name_+" created with invalid damage type: "+s);
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
	{Logging::error("swag");}
*/

bool WeaponTemplate::canFire(const Weapon* weapon) const
{
	return weapon->ticksUntilCanFire <= 0;
}

bool WeaponTemplate::canAttack(const Unit* target) const{
	return dimensions_.overlaps(target->dimension);
} // returns whether the weapon is theoretically capable of hitting the target, IGNORING COOLDOWN

void WeaponTemplate::fire(Weapon* weapon, const CoordinateOrUnit target)
{
	if (!target.isValid()) {
		throw;
	};

	if (canFire(weapon) && pythagoreanDistance(weapon->owner->xy, target.getCoordinate()) <= range()) {
		if (aoeRadius()) {
			Unitset unitsInRadius = game->inhabitedGrid.unitsInCircle(target.getCoordinate(), aoeRadius());
			for (const UnitID targetID : unitsInRadius){
				Unit* potentialTarget = game->getUnit(targetID);
				if (!game->teamsAreFriendly(weapon->owner->teamID, potentialTarget->teamID) &&
				this->canAttack(potentialTarget) )
				{
					potentialTarget->damage(damage(), damageType_, weapon->owner);
				}
			}
		}
		else if (target.isUnit()) {
			target.getUnit()->damage(damage(), damageType_, weapon->owner);
		}

		this->playHitAnimation(gUserInterface, target.getCoordinate());
		weapon->ticksUntilCanFire = reloadTime();
	}
}

void WeaponTemplate::playHitAnimation(UserInterface* ui, const Coordinate& target) {
	ui->playAnimation(hitAnimation_, target, 3);
}
