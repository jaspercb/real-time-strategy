#include "Builder.hpp"

#include "Unit.hpp"
#include "Game.hpp"
#include "Logging.hpp"

Builder::Builder(Unit* parent):
	parentID(parent->unitID),
	game(parent->game) {
}

const std::set<UnitTemplateID> Builder::getBuildables() const {
	return buildables;
}

bool Builder::canBuild(UnitTemplateID) const {
	return true;
}

void Builder::startBuilding(UnitTemplateID prodID) {
	//if (this->buildables.count(prodID))
	debugLog("building a thing");
	this->building.push_back({prodID, 1});
}

void Builder::cancelBuilding() {
	if (this->buildables.size())
		this->building.pop_front();
}

void Builder::tick() {
	if (not this->building.empty()) {
		if (this->building.front().second <= 0) {
			Unit& parent = game.getUnit(parentID);
			UnitID id = this->game.createUnit(parent.teamID, this->building.front().first, parent.xy);
			this->game.getUnit(id).stateQueue_ = parent.stateQueue_; // copy by value
			this->building.pop_front();
		}
		else {
			this->building.front().second--;
		}
	}
}
