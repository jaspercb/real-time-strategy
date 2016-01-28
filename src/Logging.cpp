#include "Logging.hpp"
#include "Unit.hpp"
#include "UnitTemplate.hpp"
#include "Weapon.hpp"

void debugLog(std::string s){
	std::cout<<s<<std::endl;
}

void debugLog(int s){
	std::cout<<s<<std::endl;
}

void debugLog(Unit& u){
	std::cout<<"LOGGING UNIT>> unitTemplate.name: "<<u.getUnitTemplate().name<<std::endl;
	std::cout<<"  unitID: "<<u.unitID;
	std::cout<<"  teamID: "<<u.teamID<<std::endl;
	
	std::cout<<"  xy: "<<u.xy.first<<","<<u.xy.second;
	std::cout<<"  dim: "<<u.dimension.ground
	<<u.dimension.sea
	<<u.dimension.submerged
	<<u.dimension.air <<std::endl;

	std::cout<<"  hp: "<<u.hp<<"/"<<u.getUnitTemplate().maxHP();
	std::cout<<"  es: "<<u.es<<"/"<<u.getUnitTemplate().maxES()<<std::endl;
}

void debugLog(Weapon& w){
	std::cout<<"Weapon Log>>"<<std::endl;
	std::cout<<"  ticksUntilCanFire: "<<w.ticksUntilCanFire<<std::endl;
	debugLog(w.owner);
}
