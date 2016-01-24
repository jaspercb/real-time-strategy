#include "Game.hpp"

#include "Unit.hpp"
#include "InhabitedGrid.hpp"

Game::Game():
	smallestUnusedUnitID_(0),
	inhabitedGrid(InhabitedGrid(this, 100, 100, 10, 10))
	{
		//inhabitedGrid = InhabitedGrid(this, 100, 100, 10, 10);
	};

UnitID Game::createUnit(TeamID teamID, UnitTemplateID unitTemplateID, Coordinate pos){
	Unit u = Unit(*this, smallestUnusedUnitID(), teamID, unitTemplateID, pos);
	inhabitedGrid.emplace(u);

	return u.unitID;
}

Unit& Game::getUnit(UnitID i)
{
	return unitsByID.at(i);
}

UnitID Game::smallestUnusedUnitID() {
	return smallestUnusedUnitID_++;
}
