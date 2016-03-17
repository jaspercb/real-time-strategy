#pragma once

#include <vector>
#include <fstream>
#include <string>

#include "EnvironmentSpec.hpp"
#include "WeaponTemplate.hpp"
#include "enums.hpp"

class Drawer;

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
	UnitTemplate(UnitTemplateID id, std::ifstream, TeamColor teamColor);
	UnitTemplate(std::string, TeamColor teamColor);
	inline int maxHP() const { return maxHP_;};
	inline int regHP() const {return regHP_;};
	inline int maxES() const {return maxES_;} ;
	inline int regES() const {return regES_;};
	inline int armor() const {return armor_;};
	inline int speed() const {return speed_;};
	inline int radius() const {return radius_;};
	inline int buildtime() const {return buildtime_;};
	float getDamageEffectivenessVsHP(DamageType) const;
	float getDamageEffectivenessVsES(DamageType) const;

	const UnitTemplateID unitTemplateID;
	
	std::string name;
	std::vector<WeaponTemplate> weaponTemplates;
	EnvironmentSpec dimension;
	Drawer* drawer;

	std::vector<UnitTemplateID> spawnables; // units this unit can spawn
	std::vector<UnitTemplateID> morphables; // units this unit can morph into

private:
	int maxHP_;
	int regHP_; // HP regen per game tick
	int maxES_;
	int regES_; // ES regen per game tick

	int armor_;
	int speed_;
	int radius_;
	int buildtime_;
};
