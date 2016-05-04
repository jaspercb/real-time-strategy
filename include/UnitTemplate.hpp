#pragma once

#include <vector>
#include <fstream>
#include <string>
#include <memory>

#include "enums.hpp"
#include "globals.hpp"

#include "EnvironmentSpec.hpp"
#include "Drawer.hpp"

class Team;
class WeaponTemplate;
class Spritesheet;

class UnitTemplate {
public:
	UnitTemplate();
	UnitTemplate(UnitTemplateID id, std::ifstream, Team* team);
	UnitTemplate(std::string, Team* team);
	inline int maxHP() const { return maxHP_;};
	inline int regHP() const {return regHP_;};
	inline int maxES() const {return maxES_;} ;
	inline int regES() const {return regES_;};
	inline int armor() const {return armor_;};
	inline int speed() const {return speed_;};
	inline int radius() const {return radius_;};
	inline int buildtime() const {return buildtime_;};
	inline int buildslots() const {return buildslots_;};
	inline bool isSelectable() const {return selectable_;};
	float getDamageEffectivenessVsHP(DamageType::Enum) const;
	float getDamageEffectivenessVsES(DamageType::Enum) const;

	inline bool isBuildable(const Team* const team) const {return true;} ; // This would be where unit prereqs go (ex. "Armory must be built")

	void renderIcon(const int x, const int y, Uint8 colormod) const;

	const UnitTemplateID unitTemplateID;
	
	std::string name;
	std::vector<WeaponTemplate> weaponTemplates;
	EnvironmentSpec dimension;
	Drawer drawer;
	std::shared_ptr<Spritesheet> icon;

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
	
	int buildslots_;

	bool selectable_;
};
