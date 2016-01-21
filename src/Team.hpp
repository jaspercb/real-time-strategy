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
	Team(std::shared_ptr<Game> _game):
		game(_game),
		id(1)
		//id(_game->getTeamID())
		{};

	const std::shared_ptr<Game> game;
	const TeamID id;

private:
	std::map<UnitTemplateID, UnitTemplate*> templates_;
};
