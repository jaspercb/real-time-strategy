#include "WeaponTemplate.hpp"

#include "EnvironmentSpec.hpp"
#include "Weapon.hpp"

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

bool WeaponTemplate::canFire(Weapon &weapon)
{
	return weapon.ticksSinceFired() >= reloadTime();
}

void WeaponTemplate::fire(Weapon& weapon, Unit& target) const
{
	// if target in range
}

void WeaponTemplate::fire(Weapon& weapon, Coordinate& target)
{
	// if target in range
}
