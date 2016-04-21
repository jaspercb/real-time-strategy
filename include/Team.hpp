#pragma once

#include <map>
#include <memory>
#include <vector>

#include "typedefs.hpp"
#include "UnitTemplate.hpp"

class Game;

class Team {
public:
	//Team();
	Team(Game& _game, TeamID id);
	virtual ~Team();
	// Creates and sets up a new unit, linked to the templateID from 

	void loadUnitTemplate( std::string s );

	bool canBuild( UnitTemplateID unitTemplateID ) const;

	void onUnitBirth( Unit* unit );
	void onUnitDeath( Unit* unit );
	int countActiveUnits( UnitTemplateID );

	Game& game;
	const TeamID teamID;

	std::map<UnitTemplateID, UnitTemplate*> unitTemplates;

	const TeamColor::Enum teamColor;

protected:
	std::vector<int> resources;
	std::map<UnitTemplateID, int> activeUnitTemplateCount;
};
