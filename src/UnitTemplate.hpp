#pragma once

#include <vector>
#include <fstream>
#include <string>

#include "EnvironmentSpec.hpp"
#include "WeaponTemplate.hpp"

class UnitTemplate
{
public:
	UnitTemplate();
	UnitTemplate(std::string _name,
			int _maxHP,
			int _maxES,
			int _speed,
			int _radius,
			EnvironmentSpec _dimension,
			std::vector<WeaponTemplate> _weaponTemplates);
	UnitTemplate(std::ifstream);
	UnitTemplate(std::string);
	int maxHP();
	int regHP(){return regHP_;}
	int maxES();
	int regES(){return regES_;}
	int armor();
	int speed();
	int radius();
	int buildtime();
	float getDamageEffectivenessVsHP(DamageType);
	float getDamageEffectivenessVsES(DamageType);

	std::string name;
	std::vector<WeaponTemplate> weaponTemplates;
	EnvironmentSpec dimension;
private:
	int maxHP_;
	int regHP_;
	int maxES_;
	int regES_;

	int armor_;
	int speed_;
	int radius_;
	int buildtime_;
};
