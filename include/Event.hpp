#pragma once

#include "typedefs.hpp"

enum EventType{
	EVENT_DAMAGED,
	EVENT_DESTROYED,
	EVENT_TARGETED
};

class Event
{
public:
	EventType type_;
	UnitID subject;
	UnitID by;
};
