#include "UnitTemplate.hpp"

#include "WeaponTemplate.hpp"
#include "EnvironmentSpec.hpp"

#include "Logging.hpp"

UnitTemplate::UnitTemplate(std::string _name, int _maxHP, int _maxES, int _speed, int _radius,
	EnvironmentSpec _canTravelOn, std::vector<WeaponTemplate> _weaponTemplates ) :
		name(_name),
		maxHP_(_maxHP),
		maxES_(_maxES),
		speed_(_speed),
		radius_(_radius),
		canTravelOn(_canTravelOn),
		weaponTemplates(_weaponTemplates) {debugLog("Creating UnitTemplate, name="+_name);}

UnitTemplate::UnitTemplate(std::ifstream is){
	std::string s;
	while (is>>s){
		if (s=="name")
			getline(is, name);
		else if (s=="maxHP")
			is>>maxHP_;
		else if (s=="maxES")
			is>>maxES_;
		else if (s=="speed")
			is>>speed_;
		else if (s=="radius")
			is>>radius_;
		else if (s=="canTravelOn"){
			int a, b, c, d;
			is>>a>>b>>c>>d;
			canTravelOn = EnvironmentSpec(a, b, c, d);
		}
		else if (s=="weapon"){
			std::string wepname;
			is>>wepname;
			weaponTemplates.push_back(WeaponTemplate(wepname));
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
