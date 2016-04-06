#pragma once

#include <string>
#include <iostream>

class Unit;
class Weapon;

template<typename T> inline void debugLog(const T s) {
	std::cout<<s<<std::endl;
}

void debugLog(Unit* u);
void debugLog(Weapon& w);
