#include "Team.hpp"

#include "Game.hpp"
#include "Unit.hpp"
#include "Logging.hpp"
#include "enums.hpp"

Team::Team(Game& _game, TeamID tID):
game(_game),
teamID(tID)
{
	//for testing
	//unitTemplates.emplace(1, new UnitTemplate("testUnit", 100, 20, 20, GROUND_ONLY, WEAPONTEMPLATEVECTOR_GOHERE);
};


void Team::loadUnitTemplate ( std::string filename ){
	unitTemplates.emplace( filename, UnitTemplate(filename, std::ifstream(filename), (TeamColor)teamID) );
};

void Team::onUnitBirth ( Unit& unit ) {
	this->activeUnitTemplateCount[unit.unitTemplateID]++;
}

void Team::onUnitDeath ( Unit& unit ) {
	this->activeUnitTemplateCount[unit.unitTemplateID]--;
	debugLog(this->activeUnitTemplateCount[unit.unitTemplateID]);
}
