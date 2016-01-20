#include "Game.hpp"

#include "Unit.hpp"
#include "InhabitedGrid.hpp"

Game::Game():
	smallestUnusedUnitID_(1),
	inhabited(new InhabitedGrid(10, 10, 1, 1))
	{};

Unit* Game::getUnit(UnitID i)
{
	return units[i];
}

UnitID Game::smallestUnusedUnitID(){
	return smallestUnusedUnitID_++;
}
