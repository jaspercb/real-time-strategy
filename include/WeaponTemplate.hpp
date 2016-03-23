#pragma once

#include <string>
#include <fstream>

#include "EnvironmentSpec.hpp"
#include "typedefs.hpp"

class EnvironmentSpec;
class Weapon;
class Unit;
class UserInterface;

enum DamageType{
	DMG_ELECTRO,	// good against shields, shit against armor
	DMG_THERMAL,	// mediocre against shields and armor
	DMG_KINETIC,    // bad against shields, pretty good against armor
	DMG_EXPLOSIVE	// shit against shields, great against armor
};

enum ResourceType{
	RESOURCE_METAL,
	RESOURCE_CRYSTAL,
	RESOURCE_SUPPLY, // You get supply back when a unit is destroyed
	RESOURCE_NUM
};

class WeaponTemplate {
public:
	WeaponTemplate();
	WeaponTemplate(std::string _name,
			DamageType _damageType,
			int _damage,
			int _reloadTime,
			int _range,
			int _weaponVelocity,
			int _aoeRadius,
			EnvironmentSpec dimensions);
	WeaponTemplate(std::ifstream&);
	//WeaponTemplate(std::string);
	virtual std::string name(){return name_;};
	virtual DamageType damageType(){return damageType_;};
	virtual int damage() const {return damage_;};
	virtual int reloadTime() const {return reloadTime_;}; //
	virtual int range() const {return range_;};
	virtual int aoeRadius() const {return aoeRadius_;}
	virtual int weaponVelocity() const {return weaponVelocity_;};
	virtual bool canFire(const Weapon& weapon) const;
	virtual bool canAttack(const Unit& target) const; // returns whether the weapon is theoretically capable of hitting the target, IGNORING COOLDOWN
	virtual void fire(Weapon& weapon, Unit& target);
	virtual void fire(Weapon& weapon, Coordinate& target);
	virtual void playHitAnimation(UserInterface*, Coordinate& target);
protected:
	std::string name_;
	DamageType damageType_;
	bool targetGround_;
	int damage_;
	int reloadTime_;
	int range_;
	int weaponVelocity_;
	int aoeRadius_;
	EnvironmentSpec dimensions_;
	std::string hitAnimation_;
};
