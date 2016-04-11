#include "CoordinateOrUnit.hpp"

#include "Game.hpp"
#include "Unit.hpp"

#include "globals.hpp"

CoordinateOrUnit& CoordinateOrUnit::operator=(Unit unit) {
	this->_isUnit = true;
	this->data.unitID = unit.unitID;
	return *this;
}

CoordinateOrUnit& CoordinateOrUnit::operator=(Coordinate coordinate) {
	this->_isUnit = false;
	this->data.coordinate = coordinate;
	return *this;
}

bool CoordinateOrUnit::isUnit() const {
	return this->_isUnit;
}

bool CoordinateOrUnit::isCoordinate() const {
	return !this->isUnit();
}

Unit* CoordinateOrUnit::getUnit() const {
	if (this->isUnit())
		return game->getUnit(this->data.unitID);
	else
		throw;
}

Coordinate CoordinateOrUnit::getCoordinate() const {
	if (isUnit())
		return this->getUnit()->xy;
	else
		return this->data.coordinate;
}
