#include "WeaponTemplate.hpp"
#include "EnvironmentSpec.hpp"
#include "Weapon.hpp"
#include "InhabitedGrid.hpp"
#include "Unit.hpp"

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

bool WeaponTemplate::canFire(Weapon& weapon) const
{
	return weapon.ticksUntilCanFire <= 0;
}

void WeaponTemplate::fire(Weapon& weapon, Unit& target) const
{
	debugLog("xy:");
	debugLog(weapon.owner.xy.first);
	debugLog(weapon.owner.xy.second);
	
	debugLog("pythDistance:");
	debugLog(pythagoreanDistance(weapon.owner.xy, target.xy));
	
	if (pythagoreanDistance(weapon.owner.xy, target.xy)<=range_)
	{
		debugLog("hi");
		target.damage(damage_);
		weapon.ticksUntilCanFire = reloadTime_;
	}
}

void WeaponTemplate::fire(Weapon& weapon, Coordinate& target) const
{
	// if target in range
}
