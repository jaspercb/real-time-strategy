#pragma once

#include <string>
#include <vector>
#include <memory>

#include "EnvironmentSpec.hpp"

class WeaponTemplate;

class UnitTemplate
{
public:
	UnitTemplate(std::string _name,
			int _maxHP,
			int _speed,
			int _radius,
			EnvironmentSpec _canTravelOn,
			std::vector<std::shared_ptr<WeaponTemplate> > _weaponTemplates);
	std::string name();
	int maxHP();
	int speed();
	int radius();
	int buildtime();
	std::vector<std::shared_ptr<WeaponTemplate> > weaponTemplates();
private:
	std::string name_;
	int maxHP_;
	int speed_;
	int radius_;
	int buildtime_;
	EnvironmentSpec canTravelOn_;
	std::vector<std::shared_ptr<WeaponTemplate> > weaponTemplates_;
};
