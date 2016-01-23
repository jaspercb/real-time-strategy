#include "Game.hpp"
#include "Unit.hpp"
#include "Logging.hpp"

int main(){
	debugLog(" Testing...");
	Game g = Game();
	Team t = Team(g);
	
	WeaponTemplate wpntmpl = WeaponTemplate("TestWeapon", DMG_KINETIC, 10, 2, 5, 1, 1, GROUND_ONLY);
	std::vector<WeaponTemplate> tmplv;
	tmplv.push_back(wpntmpl);
	UnitTemplate p = UnitTemplate("TestUnit", 50, 1, 1, GROUND_ONLY, tmplv);
	

	
	debugLog(" Done testing.");
}
