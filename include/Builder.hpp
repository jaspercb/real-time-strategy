#pragma once

#include <vector>
#include <deque>

#include "BuildData.hpp"

class Unit;
class Game;
class UserInterface;

class Builder {
	friend UserInterface;
	// Handles all unit production and yadda yadda yadda
	public:
		Builder(Unit* parent, int slots);
		void startBuilding(UnitTemplateID, int time);
		void cancelBuilding();
		void tick();
	private:
		static int ticksUntilDone(const std::deque<BuildData>&);

		const UnitID parentID;
		Game& game;
		std::vector< std::deque<BuildData> > building;
};
