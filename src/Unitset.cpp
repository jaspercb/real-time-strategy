#include "Unitset.hpp"

#include "Unit.hpp"
#include "Game.hpp"
#include "globals.hpp"

Unitset::Unitset() {}

Unitset::Unitset(const Unitset& other) : set(other.set) {}

void Unitset::insert(const UnitID uID) {
	set.insert(uID);
}

void Unitset::insert(const Unitset& container) {
	set.insert(container.begin(), container.end());
}

template<typename T> void Unitset::insert(const T& start, const T& end) {
	set.insert(start, end);
}

void Unitset::removeInvalidUnits() const {
	// Normally making "const" methods that modify mutable members is a symptom
	// of bad design, but this is genuinely a useful abstraction. Honest.
	for (auto &unitID : set) {
		if (!game->existsUnit(unitID)) {
			set.erase(unitID);
		}
	}
}
