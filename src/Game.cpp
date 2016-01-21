#include "Game.hpp"

#include "Unit.hpp"
#include "InhabitedGrid.hpp"

Game::Game():
	smallestUnusedUnitID_(0),
	inhabitedGrid(new InhabitedGrid(10, 10, 1, 1))
	{};

std::shared_ptr<Unit> Game::getUnit(UnitID i)
{
	return units[i];
}

UnitID Game::smallestUnusedUnitID(){
	return smallestUnusedUnitID_++;
}
