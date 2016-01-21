#pragma once

#include <map>
#include <memory>

#include "Game.hpp"

typedef int TeamID;
typedef int UnitTemplateID;

class Game;
class UnitTemplate;


class Team
{
public:
	Team(std::shared_ptr<Game> _game);

	std::shared_ptr<Unit> createUnit(UnitTemplateID templateID);
	// Creates and sets up a new unit, linked to the templateID from 

	const std::shared_ptr<Game> game;
	const TeamID id;

private:
	std::map<UnitTemplateID, UnitTemplate*> templates_;
	std::map<UnitTemplateID, std::shared_ptr<UnitTemplate> > unitTemplates;
};
