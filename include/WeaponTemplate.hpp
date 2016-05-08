#pragma once

#include <string>
#include <fstream>

#include "EnvironmentSpec.hpp"
#include "CoordinateOrUnit.hpp" // for UnitID->CoordinateOrUnit conversion

#include "typedefs.hpp"
#include "enums.hpp"

class EnvironmentSpec;
class Weapon;
class Unit;
class UserInterface;

class WeaponTemplate {
public:
	WeaponTemplate();
	WeaponTemplate(std::string _name,
			DamageType::Enum _damageType,
			int _damage,
			int _reloadTime,
			int _range,
			int _weaponVelocity,
			int _aoeRadius,
			EnvironmentSpec dimensions);
	WeaponTemplate(std::ifstream&);
	//WeaponTemplate(std::string);
	virtual std::string name(){return name_;};
	virtual DamageType::Enum damageType(){return damageType_;};
	virtual int damage() const {return damage_;};
	virtual int reloadTime() const {return reloadTime_;}; //
	virtual int range() const {return range_;};
	virtual int aoeRadius() const {return aoeRadius_;}
	virtual int weaponVelocity() const {return weaponVelocity_;};
	virtual bool canFire(const Weapon* weapon) const;
	virtual bool canAttack(const Unit* target) const; // returns whether the weapon is theoretically capable of hitting the target, IGNORING COOLDOWN
	virtual void fire(Weapon* weapon, const CoordinateOrUnit target);
	virtual void playHitAnimation(UserInterface*, const Coordinate& target);
protected:
	std::string name_;
	DamageType::Enum damageType_;
	bool targetGround_;
	int damage_;
	int reloadTime_;
	int range_;
	int weaponVelocity_;
	int aoeRadius_;
	EnvironmentSpec dimensions_;
	std::string hitAnimation_;
};
