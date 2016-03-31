#include "Weapon.hpp"

#include "WeaponTemplate.hpp"
#include "Unit.hpp"
#include "Logging.hpp"

Weapon::Weapon(WeaponTemplate *_weaponTemplate, Unit*_owner):
	weaponTemplate(_weaponTemplate),
	owner(_owner),
	ticksUntilCanFire(0)
	{
		//debugLog("from inside Weapon::Weapon():");
		//debugLog(owner);
	}

bool Weapon::canFire() const{
	return weaponTemplate->canFire(this);
}

bool Weapon::canAttack(Unit* u) const{
	return weaponTemplate->canAttack(u);
}

void Weapon::fire(Unit* target){
	//debugLog("firing weapon");
	weaponTemplate->fire(this, target);
}

void Weapon::update(){
	if (ticksUntilCanFire>0)
	{
		ticksUntilCanFire--;
	}
}
