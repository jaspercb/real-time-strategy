#include "Builder.hpp"

#include "Unit.hpp"
#include "Game.hpp"

Builder::Builder(Unit* parent):
	parent(parent),
	game(parent->game),
	team(parent->game.getTeam(parent->teamID)) {

}

const std::set<UnitTemplateID> Builder::getBuildables() const {
	return buildables;
}

void Builder::startBuilding(UnitTemplateID prodID) {
	if (this->buildables.count(prodID))
		this->building.push_back({prodID, 1});
}

void Builder::cancelBuilding() {
	if (this->buildables.size())
		this->building.pop_front();
}

void Builder::tick() {
	if (not this->buildables.empty()) {
		if (this->building.front().second <= 0) {
			this->game.createUnit(this->parent->teamID, this->building.front().first, this->parent->xy);
			this->building.pop_front();
		}
		else {
			this->building.front().second--;
		}
	}
}
