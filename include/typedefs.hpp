#pragma once
#include <utility>
#include <string>
#include <stdint.h>
#include <array>

typedef int64_t Distance;
const Distance MAX_DISTANCE = 906744073709551615;

typedef std::pair<Distance, Distance> Coordinate;
typedef std::array< std::array< float, 3>, 3> Matrix3;

typedef std::string UnitTemplateID;
typedef int UnitID;
typedef int TeamID;
