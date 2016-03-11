#pragma once

#include <set>
#include <deque>

#include "typedefs.hpp"

class Unit;
class Game;
class Team;

class Builder {
	// Handles all unit production and yadda yadda yadda
	public:
		Builder(Unit* parent);
		const std::set<UnitTemplateID> getBuildables() const;
		void startBuilding(UnitTemplateID);
		void cancelBuilding();
		void tick();
	private:
		Unit* parent;
		Game& game;
		Team& team;
		std::set<UnitTemplateID> buildables;
		std::deque<std::pair<UnitTemplateID, int> > building;
};
