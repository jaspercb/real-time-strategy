#include "Game.hpp"
#include "Unit.hpp"
#include "Logging.hpp"

int main(){
	debugLog(" Testing...");
	Game g = Game();
	TeamID tID = g.createTeam();

	WeaponTemplate wpntmpl = WeaponTemplate("TestWeapon", DMG_KINETIC, 10, 2, 5, 1, 1, GROUND_ONLY);
	std::vector<WeaponTemplate> tmplv;
	tmplv.push_back(wpntmpl);
	UnitTemplate p = UnitTemplate("TestUnit", 50, 1, 1, GROUND_ONLY, tmplv);
	
	Team& t = g.getTeam(tID);

	t.unitTemplates.emplace((UnitTemplateID)3, p);

	g.createUnit(tID, (UnitTemplateID)3, Coordinate(0, 0));
	//g.createUnit(tID, (UnitTemplateID)3, Coordinate(0, 0));
	
	Unit& a = g.getUnit(0);
	//Unit& b = g.getUnit(1);

	a.attack(a);
	debugLog(a);
	a.attack(a);
	debugLog(a);
	a.attack(a);
	debugLog(a);

	debugLog(" Done testing.");
}