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

WeaponTemplate::WeaponTemplate(std::ifstream is)
{
	std::string s;
	while (is>>s){
		if (s=="name"){
			getline(is, name_);
			debugLog("yo");
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
				throw "Error: WeaponTemplate created with invalid damage type: "+s;
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
	}
}

WeaponTemplate::WeaponTemplate(std::string s):
	WeaponTemplate(std::ifstream("../conf/weapons/"+s))
	{debugLog("swag");}


bool WeaponTemplate::canFire(Weapon& weapon) const
{
	return weapon.ticksUntilCanFire <= 0;
}

void WeaponTemplate::fire(Weapon& weapon, Unit& target)
{
	if (canFire(weapon) && (pythagoreanDistance(weapon.owner.xy, target.xy)<=range_))
	{
		target.damage(damage_);
		weapon.ticksUntilCanFire = reloadTime_;
	}
}

void WeaponTemplate::fire(Weapon& weapon, Coordinate& target)
{
	// if target in range
}
