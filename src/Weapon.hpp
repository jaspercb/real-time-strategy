#pragma once

class WeaponTemplate;
class Unit;

class Weapon
{
public:
	Weapon(WeaponTemplate* _weaponTemplate, Unit* _owner);

	void fire(Unit& target);
	int ticksSinceFired();
	virtual void update();
	const WeaponTemplate* weaponTemplate;
	const Unit* owner;

private:
	int ticksSinceFired_;
};
