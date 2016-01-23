#pragma once

#include <map>
#include <memory>
#include "UnitTemplate.hpp"
#include "typedefs.hpp"

class Game;

class Team
{
public:
	//Team();
	Team(Game& _game);

	Unit& createUnit(UnitTemplateID templateID);
	// Creates and sets up a new unit, linked to the templateID from 

	Game& game;
	const TeamID id;

	std::map<UnitTemplateID, UnitTemplate> unitTemplates;
};
