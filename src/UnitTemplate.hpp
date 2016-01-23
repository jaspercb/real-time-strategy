#pragma once

#include <string>
#include <vector>
#include <memory>

#include "EnvironmentSpec.hpp"
#include "WeaponTemplate.hpp"

class UnitTemplate
{
public:
	UnitTemplate();
	UnitTemplate(std::string _name,
			int _maxHP,
			int _speed,
			int _radius,
			EnvironmentSpec _canTravelOn,
			std::vector<WeaponTemplate> _weaponTemplates);
	int maxHP();
	int speed();
	int radius();
	int buildtime();

	std::string name;
	std::vector<WeaponTemplate> weaponTemplates;
private:
	int maxHP_;
	int speed_;
	int radius_;
	int buildtime_;
	EnvironmentSpec canTravelOn_;
};
