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

Team::~Team() {
	for (auto &i : unitTemplates) {
		delete i.second;
	}
}

void Team::loadUnitTemplate ( std::string filename ) { // filename doubles as unit template ID
	unitTemplates.emplace( filename, new UnitTemplate(filename, std::ifstream(filename), (TeamColor)teamID));
	this->activeUnitTemplateCount[filename];
};

void Team::onUnitBirth ( Unit* unit ) {
	this->activeUnitTemplateCount[unit->unitTemplateID]++;
}

void Team::onUnitDeath ( Unit* unit ) {
	this->activeUnitTemplateCount[unit->unitTemplateID]--;
}

int Team::countActiveUnits ( UnitTemplateID unitTemplateID) {
	return this->activeUnitTemplateCount[unitTemplateID];
}
