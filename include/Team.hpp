#pragma once

#include <map>
#include <memory>
#include <vector>

#include "typedefs.hpp"
#include "UnitTemplate.hpp"

class Game;

class Team
{
public:
	//Team();
	Team(Game& _game, TeamID id);

	// Creates and sets up a new unit, linked to the templateID from 

	void loadUnitTemplate( std::string s );

	void onUnitBirth( Unit& unit );
	void onUnitDeath( Unit& unit );

	Game& game;
	const TeamID teamID;

	std::map<UnitTemplateID, UnitTemplate> unitTemplates;

protected:
	std::vector<int> resources;
	std::map<UnitTemplateID, int> activeUnitTemplateCount;
};
