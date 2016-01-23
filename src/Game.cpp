#include "Game.hpp"

#include "Unit.hpp"
#include "InhabitedGrid.hpp"

Game::Game():
	smallestUnusedUnitID_(0),
	inhabitedGrid(InhabitedGrid(this, 100, 100, 10, 10))
	{
		//inhabitedGrid = InhabitedGrid(this, 100, 100, 10, 10);
	};

Unit& Game::getUnit(UnitID i)
{
	return unitsByID.at(i);
}

UnitID Game::smallestUnusedUnitID() {
	return smallestUnusedUnitID_++;
}
