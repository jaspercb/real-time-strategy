#include "Team.hpp"

#include "Game.hpp"
#include "Unit.hpp"
#include "Logging.hpp"
#include "enums.hpp"
#include "WeaponTemplate.hpp"

Team::Team(Game& _game, TeamID tID):
game(_game),
teamID(tID),
teamColor((TeamColor::Enum)tID)
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
	unitTemplates.emplace( filename, new UnitTemplate(filename, std::ifstream(filename), this));
	this->activeUnitTemplateCount[filename];
}

UnitTemplate* Team::getUnitTemplate(UnitTemplateID unitTemplateID) const {
	return unitTemplates.at(unitTemplateID);
}

bool Team::canBuild( UnitTemplateID unitTemplateID ) const {
	return getUnitTemplate(unitTemplateID)->isBuildable(this);
}

void Team::onUnitBirth ( Unit* unit ) {
	this->activeUnitTemplateCount[unit->unitTemplateID]++;
}

void Team::onUnitDeath ( Unit* unit ) {
	this->activeUnitTemplateCount[unit->unitTemplateID]--;
}

int Team::countActiveUnits ( UnitTemplateID unitTemplateID) {
	return this->activeUnitTemplateCount[unitTemplateID];
}
