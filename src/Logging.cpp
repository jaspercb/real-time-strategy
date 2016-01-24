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
	std::cout<<"Unit Log:"<<std::endl;
	std::cout<<"  unitID: "<<u.unitID<<std::endl;
	std::cout<<"  teamID: "<<u.teamID<<std::endl;
	std::cout<<"  xy: "<<u.xy.first<<","<<u.xy.second<<std::endl;
	std::cout<<"  hp: "<<u.hp<<std::endl;
}

void debugLog(Weapon& w){
	std::cout<<"Weapon Log:"<<std::endl;
	std::cout<<"  ticksUntilCanFire: "<<w.ticksUntilCanFire<<std::endl;
	debugLog(w.owner);
}
