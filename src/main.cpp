#include "Game.hpp"
#include "Unit.hpp"
#include "Logging.hpp"

int main(){
	debugLog(" Testing...");
	Game g = Game();
	TeamID tID = g.createTeam();

	//WeaponTemplate wpntmpl = WeaponTemplate("TestWeapon", DMG_KINETIC, 10, 2, 5, 1, 1, GROUND_ONLY);
	//std::vector<WeaponTemplate> tmplv;
	//tmplv.push_back(wpntmpl);
	//UnitTemplate p = UnitTemplate("TestUnit", 50, 1, 1, GROUND_ONLY, tmplv);
	UnitTemplate p = UnitTemplate(std::ifstream("../conf/units/marine1"));

	Team& t = g.getTeam(tID);

	t.unitTemplates.emplace((UnitTemplateID)3, p);

	g.createUnit(tID, (UnitTemplateID)3, Coordinate(0, 0));
	
	Unit& a = g.getUnit(0);

	debugLog(a);
	a.attack(a);
	debugLog(a);

	debugLog(" Done testing.");
}