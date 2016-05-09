#pragma once

#include "CoordinateOrUnit.hpp" // for auto-conversion

class WeaponTemplate;
class Unit;

class Weapon {
public:
	Weapon(WeaponTemplate *_weaponTemplate, Unit* _owner);
	bool canFire() const;
	bool canAttack(Unit* u) const;
	void fire(CoordinateOrUnit target);
	virtual void update();
	WeaponTemplate* weaponTemplate;
	Unit* owner;

	int ticksUntilCanFire;
};
