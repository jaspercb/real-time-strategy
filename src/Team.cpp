#include "Team.hpp"

#include "UnitTemplate.hpp"
#include "Game.hpp"

Team::Team(Game& _game):
game(_game), id(1)
//id(_game->getTeamID())
{
	//for testing
	//unitTemplates.emplace(1, new UnitTemplate("testUnit", 100, 20, 20, GROUND_ONLY, WEAPONTEMPLATEVECTOR_GOHERE);
};