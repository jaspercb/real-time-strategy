#include "Weapon.hpp"

#include "WeaponTemplate.hpp"
#include "Unit.hpp"
#include "Logging.hpp"

Weapon::Weapon(WeaponTemplate *_weaponTemplate, Unit*_owner):
	weaponTemplate(_weaponTemplate),
	owner(_owner),
	ticksUntilCanFire(0)
	{
		//Logging::error("from inside Weapon::Weapon():");
		//Logging::error(owner);
	}

bool Weapon::canFire() const{
	return weaponTemplate->canFire(this);
}

bool Weapon::canAttack(Unit* u) const{
	return weaponTemplate->canAttack(u);
}

void Weapon::fire(CoordinateOrUnit target) {
	//Logging::error("firing weapon");
	weaponTemplate->fire(this, target);
}

void Weapon::update(){
	if (ticksUntilCanFire>0)
	{
		ticksUntilCanFire--;
	}
}
