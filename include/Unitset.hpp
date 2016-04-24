#pragma once

#include <set>

#include "typedefs.hpp"

class Command;
class UnitTemplate;

class Unitset {
public:
	Unitset();
	Unitset(const Unitset&);

	void handleCommand(Command c);
	void startBuilding(UnitTemplateID unitTemplateID);

	// UI stuff
	const UnitTemplate* getWHATTOCALLIT();
private:
	std::set<UnitID> set;
};
