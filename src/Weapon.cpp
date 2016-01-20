#include "Weapon.hpp"

#include "WeaponTemplate.hpp"
#include "Unit.hpp"

Weapon::Weapon(WeaponTemplate* _weaponTemplate, Unit* _owner): 
	weaponTemplate(_weaponTemplate),
	owner(_owner)
	{}

void Weapon::fire(Unit& target){
	weaponTemplate->fire(*this, target);
}

int Weapon::ticksSinceFired(){
	return ticksSinceFired_;
}

void Weapon::update(){
	ticksSinceFired_++;
}
