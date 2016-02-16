#include "Team.hpp"

#include "Game.hpp"
#include "Unit.hpp"
#include "enums.hpp"

Team::Team(Game& _game, TeamID tID):
game(_game),
teamID(tID),
smallestUnusedUnitTemplateID(0)
{
	//for testing
	//unitTemplates.emplace(1, new UnitTemplate("testUnit", 100, 20, 20, GROUND_ONLY, WEAPONTEMPLATEVECTOR_GOHERE);
};


void Team::loadUnitTemplate( std::string filename ){
	unitTemplates.emplace( this->smallestUnusedUnitTemplateID++, UnitTemplate(std::ifstream(filename), (TeamColor)teamID) );
};
