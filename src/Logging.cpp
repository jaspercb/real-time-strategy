#include "Logging.hpp"
#include "Unit.hpp"
#include "UnitTemplate.hpp"

void debugLog(std::string s){
	std::cout<<s<<std::endl;
}

void debugLog(int s){
	std::cout<<s<<std::endl;
}

void debugLog(Unit& u){
	std::cout<<"Unit Log:"<<std::endl;
	std::cout<<"  Position: "<<u.xy.first<<","<<u.xy.second<<std::endl;
	std::cout<<"  HP: "<<u.hp<<"/"<<u.getUnitTemplate().maxHP()<<std::endl;
}