#pragma once

class EnvironmentSpec
{
public:
	EnvironmentSpec(bool _ground, bool _sea, bool _submerged, bool _air):
		ground(_ground),
		sea(_sea),
		submerged(_submerged),
		air(_air)
		{}
	bool ground, sea, submerged, air;
};

const EnvironmentSpec GROUND_ONLY(1, 0, 0, 0);
const EnvironmentSpec AIR_ONLY(0, 0, 0, 1);
const EnvironmentSpec SEA_ONLY(0, 1, 0, 0);
const EnvironmentSpec SUBMERSIBLE_SEA(0, 1, 1, 0);
