#pragma once
#include <memory>

class WeaponTemplate;
class Unit;

class Weapon
{
public:
	Weapon(WeaponTemplate &_weaponTemplate, Unit &_owner);
	Weapon(Weapon &w, Unit &u);
	bool canFire() const;
	bool canAttack(Unit& u) const;
	void fire(Unit& target);
	virtual void update();
	WeaponTemplate& weaponTemplate;
	Unit& owner;

	int ticksUntilCanFire;
};
