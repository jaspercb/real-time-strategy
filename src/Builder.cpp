#include "Builder.hpp"

#include "Unit.hpp"
#include "Game.hpp"
#include "Logging.hpp"

Builder::Builder(Unit* parent, int slots):
	parentID(parent->unitID),
	game(parent->game)
{
	this->building.resize(slots);
}

void Builder::startBuilding(UnitTemplateID prodID, int time) {
	//if (this->buildables.count(prodID))
	if (this->building.size() == 0)
		return;
	debugLog( ("Building a \"" + prodID + '"' ));
	auto& min = this->building.at(0);
	int t = 99999999;
	for (auto &i : this->building)
		if (Builder::ticksUntilDone(i) < t) {
			t = Builder::ticksUntilDone(i);
			min = i;
		}
	min.push_back({prodID, time});
}

void Builder::cancelBuilding() {
	for (auto &i : this->building)
		i.clear();
}

void Builder::tick() {
	Unit& parent = game.getUnit(parentID);
	for (auto &q : this->building) {
		if (not q.empty()) {
			if (q.front().second <= 0) {
				UnitID id = this->game.createUnit(parent.teamID, q.front().first, parent.xy);
				this->game.getUnit(id).stateQueue_ = parent.stateQueue_; // copy by value
				q.pop_front();
			}
			else {
				q.front().second--;
			}
		}
	}
}

int Builder::ticksUntilDone(const std::deque<std::pair<UnitTemplateID, int> > &q) {
	int tot = 0;
	for (auto &i : q)
		tot += i.second;
	return tot;
}

