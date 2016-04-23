#include "Logging.hpp"
#include "Unit.hpp"
#include "UnitTemplate.hpp"
#include "Weapon.hpp"

namespace Logging {
	void writeToStream(const Unit* const u, std::ostream& ostream) {
		ostream<<"LOGGING UNIT>> unitTemplate.name: "<<u->getUnitTemplate()->name<<std::endl;
		ostream<<"  unitID: "<<u->unitID;
		ostream<<"  teamID: "<<u->teamID<<std::endl;
		
		ostream<<"  xy: "<<u->xy.x<<","<<u->xy.y;
		ostream<<"  dim: "<<u->dimension.ground
		<<u->dimension.sea
		<<u->dimension.submerged
		<<u->dimension.air <<std::endl;

		ostream<<"  hp: "<<u->hp<<"/"<<u->getUnitTemplate()->maxHP();
		ostream<<"  es: "<<u->es<<"/"<<u->getUnitTemplate()->maxES()<<std::endl;
	}

	void writeToStream(const Weapon& w, std::ostream& ostream) {
		ostream<<"Weapon Log>>"<<std::endl;
		ostream<<"  ticksUntilCanFire: "<<w.ticksUntilCanFire<<std::endl;
		Logging::writeToStream(w.owner, ostream);
	}
}
