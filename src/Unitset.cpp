#include "Unitset.hpp"

#include "Unit.hpp"

Unitset::Unitset() {}

Unitset::Unitset(const Unitset& other) : set(other.set) {}

void Unitset::insert(const UnitID uID) {
	set.insert(uID);
}

template<typename T> void Unitset::insert(const T& start, const T& end) {
	set.insert(start, end);
}

//Unit::handleCommand