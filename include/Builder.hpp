#pragma once

#include <vector>
#include <deque>

#include "typedefs.hpp"

class Unit;
class Game;

class Builder {
	// Handles all unit production and yadda yadda yadda
	public:
		Builder(Unit* parent, int slots);
		void startBuilding(UnitTemplateID, int time);
		void cancelBuilding();
		void tick();
	private:
		static int ticksUntilDone(const std::deque<std::pair<UnitTemplateID, int> >&);

		const UnitID parentID;
		Game& game;
		std::vector< std::deque<std::pair<UnitTemplateID, int> > > building;
};
