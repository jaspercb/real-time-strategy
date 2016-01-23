#include "UnitTemplate.hpp"

#include "WeaponTemplate.hpp"
#include "EnvironmentSpec.hpp"

#include "Logging.hpp"

UnitTemplate::UnitTemplate(std::string _name, int _maxHP, int _speed, int _radius,
	EnvironmentSpec _canTravelOn, std::vector<WeaponTemplate> _weaponTemplates ) :
		name(_name),
		maxHP_(_maxHP),
		speed_(_speed),
		radius_(_radius),
		canTravelOn_(_canTravelOn),
		weaponTemplates(_weaponTemplates) {debugLog("Creating UnitTemplate, name="+_name);}

int UnitTemplate::maxHP(){
	return maxHP_;
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
