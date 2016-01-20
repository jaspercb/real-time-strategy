#pragma once

#include <map>

typedef int TeamID;
typedef int UnitTemplateID;

class Game;
class UnitTemplate;


class Team
{
public:
	Team(Game* _game):
		game(_game),
		id(1)
		//id(_game->getTeamID())
		{};

	const Game* game;
	const TeamID id;

private:
	std::map<UnitTemplateID, UnitTemplate*> templates_;
};
