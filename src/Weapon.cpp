#include "Weapon.hpp"

#include "WeaponTemplate.hpp"
#include "Unit.hpp"

Weapon::Weapon(std::shared_ptr<WeaponTemplate> _weaponTemplate, Unit* _owner): 
	weaponTemplate(_weaponTemplate),
	owner(_owner)
	{}

void Weapon::fire(Unit& target){
	weaponTemplate->fire(*this, target);
}

void Weapon::update(){
	if (ticksUntilCanFire>0)
	{
		ticksUntilCanFire--;
	}
}
