#pragma once

class EnvironmentSpec {
public:
	EnvironmentSpec():
		ground(false),
		sea(false),
		submerged(false),
		air(false)
		{}
	EnvironmentSpec(bool _ground, bool _sea, bool _submerged, bool _air):
		ground(_ground),
		sea(_sea),
		submerged(_submerged),
		air(_air)
		{}

	bool ground, sea, submerged, air;

	EnvironmentSpec operator&(const EnvironmentSpec other) const {
		return EnvironmentSpec(ground && other.ground, sea && other.sea, submerged && other.submerged, air && other.air);
	}
	EnvironmentSpec operator|(const EnvironmentSpec other) const {
		return EnvironmentSpec(ground || other.ground, sea || other.sea, submerged || other.submerged, air || other.air);		
	}
	operator bool() const{
		return ground||sea||submerged||air;
	}
};


const EnvironmentSpec GROUND_ONLY(1, 0, 0, 0);
const EnvironmentSpec SEA_ONLY(0, 1, 0, 0);
const EnvironmentSpec SUBMERSIBLE_SEA(0, 1, 1, 0);
const EnvironmentSpec AIR_ONLY(0, 0, 0, 1);
