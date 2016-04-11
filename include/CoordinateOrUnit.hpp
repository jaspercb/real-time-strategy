#pragma once

#include "typedefs.hpp"

class Unit; // forward declaration

class CoordinateOrUnit {
public:
	CoordinateOrUnit(UnitID);

	CoordinateOrUnit &operator=(Unit unit);
	CoordinateOrUnit &operator=(Coordinate pos);

	bool isUnit() const;
	bool isCoordinate() const;

	Unit* getUnit() const;
	Coordinate getCoordinate() const;

private:
	bool _isUnit;
	union {
		UnitID unitID;
		Coordinate coordinate;
	} data;
};
