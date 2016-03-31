#pragma once

#include <string>
#include <iostream>

class Unit;
class Weapon;

void debugLog(std::string s);
void debugLog(int s);
void debugLog(Unit* u);
void debugLog(Weapon& w);
