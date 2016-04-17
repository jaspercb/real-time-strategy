#include "CoordinateOrUnit.hpp"

#include "Game.hpp"
#include "Unit.hpp"

#include "globals.hpp"

CoordinateOrUnit::CoordinateOrUnit() {
	this->_isUnit = false;
	this->data.coordinate = {0,0};
}

CoordinateOrUnit::CoordinateOrUnit(const Coordinate c) {
	*this = c;
}

CoordinateOrUnit::CoordinateOrUnit(const UnitID uid) {
	this->_isUnit = true;
	this->data.unitID = uid;
}

CoordinateOrUnit::CoordinateOrUnit(const Unit &u) {
	*this = u;
}

CoordinateOrUnit::CoordinateOrUnit(const Unit *u) {
	this->_isUnit = true;
	this->data.unitID = u->unitID;
}

CoordinateOrUnit::CoordinateOrUnit(const CoordinateOrUnit& c) {
	_isUnit = c._isUnit;
	data.coordinate = c.data.coordinate;
	data.unitID = c.data.unitID;
}

CoordinateOrUnit& CoordinateOrUnit::operator=(Unit unit) {
	this->_isUnit = true;
	this->data.unitID = unit.unitID;
	return *this;
}

CoordinateOrUnit& CoordinateOrUnit::operator=(Unit *unit) {
	this->_isUnit = true;
	this->data.unitID = unit->unitID;
	return *this;
}

CoordinateOrUnit& CoordinateOrUnit::operator=(Coordinate coordinate) {
	this->_isUnit = false;
	this->data.coordinate = coordinate;
	return *this;
}

bool CoordinateOrUnit::isValid() const {
	return isCoordinate() || game->existsUnit(this->data.unitID);
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
