#pragma once

#include <map>
#include <memory>
#include <vector>

#include "UnitTemplate.hpp"
#include "typedefs.hpp"

class Game;

class Team
{
public:
	//Team();
	Team(Game& _game, TeamID id);

	// Creates and sets up a new unit, linked to the templateID from 

	Game& game;
	const TeamID teamID;

	std::map<UnitTemplateID, UnitTemplate> unitTemplates;

protected:
	std::vector<int> resources;
};
