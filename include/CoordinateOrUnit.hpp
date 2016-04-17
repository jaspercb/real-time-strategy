#pragma once

#include "typedefs.hpp"

class Unit; // forward declaration

class CoordinateOrUnit {
public:
	CoordinateOrUnit();
	CoordinateOrUnit(const Coordinate c);
	CoordinateOrUnit(const UnitID uid);
	CoordinateOrUnit(const Unit &u);
	CoordinateOrUnit(const Unit *u);

	CoordinateOrUnit(const CoordinateOrUnit& c); // copy constructor

	CoordinateOrUnit &operator=(Unit unit);
	CoordinateOrUnit &operator=(Unit *unit);
	CoordinateOrUnit &operator=(Coordinate pos);

	bool isValid() const;

	bool isUnit() const;
	bool isCoordinate() const;

	Unit* getUnit() const;
	Coordinate getCoordinate() const;

private:
	bool _isUnit;
	union U {
		U() : coordinate(0,0) {};
		UnitID unitID;
		Coordinate coordinate;
	} data;
};
