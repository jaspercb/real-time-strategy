#include "UnitTemplate.hpp"

#include "WeaponTemplate.hpp"
#include "EnvironmentSpec.hpp"

#include "Logging.hpp"

UnitTemplate::UnitTemplate(std::string _name, int _maxHP, int _maxES, int _speed, int _radius,
	EnvironmentSpec _dimension, std::vector<WeaponTemplate> _weaponTemplates ) :
		name(_name),
		maxHP_(_maxHP),
		maxES_(_maxES),
		speed_(_speed),
		radius_(_radius),
		dimension(_dimension),
		weaponTemplates(_weaponTemplates) {debugLog("Creating UnitTemplate, name="+_name);}

UnitTemplate::UnitTemplate(std::ifstream is){
	maxHP_ = 0;
	regHP_ = 0;
	maxES_ = 0;
	regES_ = 0;
	armor_ = 0;
	speed_ = 0;
	radius_ = 0;
	dimension = EnvironmentSpec(0, 0, 0, 0);

	std::string s;

	while (is>>s){
		if (s=="name")
			getline(is, name);
		else if (s=="maxHP")
			is>>maxHP_;
		else if (s=="regHP")
			is>>regHP_;
		else if (s=="maxES")
			is>>maxES_;
		else if (s=="regES")
			is>>regES_;
		else if (s=="armor")
			is>>armor_;
		else if (s=="speed")
			is>>speed_;
		else if (s=="radius")
			is>>radius_;
		else if (s=="dimension"){
			int a, b, c, d;
			is>>a>>b>>c>>d;
			dimension = EnvironmentSpec(a, b, c, d);
		}
		else if (s=="weapon"){
			is>>s; // to get rid of the trailing {
			weaponTemplates.push_back(WeaponTemplate(is));
		}
		else if (s=="weapon{"){
			weaponTemplates.push_back(WeaponTemplate(is));
		}
		else if (s=="}"){
			return;
		}
	}
}

UnitTemplate::UnitTemplate(std::string s):
	UnitTemplate(std::ifstream("../conf/units/"+s))
	{}


int UnitTemplate::maxHP(){
	return maxHP_;
}

int UnitTemplate::maxES(){
	return maxES_;
}

int UnitTemplate::armor(){
	return armor_;
}

int UnitTemplate::speed(){
	return speed_;
}

int UnitTemplate::radius(){
	return radius_;
}

int UnitTemplate::buildtime(){
	return buildtime_;
}

float UnitTemplate::getDamageEffectivenessVsHP(DamageType dmgtype){
	switch (dmgtype){
		case DMG_ELECTRO:
			return 0.5;
		case DMG_THERMAL:
			return 0.75;
		case DMG_KINETIC:
			return 1.25;
		case DMG_EXPLOSIVE:
			return 1.5;
	}
}

float UnitTemplate::getDamageEffectivenessVsES(DamageType dmgtype){
	switch (dmgtype){
		case DMG_ELECTRO:
			return 1.5;
		case DMG_THERMAL:
			return 1.25;
		case DMG_KINETIC:
			return 0.75;
		case DMG_EXPLOSIVE:
			return 0.5;
	}
}
