#include "Weapon.hpp"

#include "WeaponTemplate.hpp"
#include "Unit.hpp"
#include "Logging.hpp"

Weapon::Weapon(WeaponTemplate &_weaponTemplate, Unit &_owner):
	weaponTemplate(_weaponTemplate),
	owner(_owner),
	ticksUntilCanFire(0)
	{
	}

bool Weapon::canFire(){
	return weaponTemplate.canFire(*this);
}

void Weapon::fire(Unit& target){
	//debugLog("firing weapon");
	weaponTemplate.fire(*this, target);
}

void Weapon::update(){
	if (ticksUntilCanFire>0)
	{
		ticksUntilCanFire--;
	}
}
