#include "UnitState.hpp"

class StateIdle : public UnitState
{
public:
	StateExitCode update(Unit* unit);
};
