#pragma once

#include <string>
#include <iostream>
#include <ostream>

class Unit;
class Weapon;

namespace Logging {
	template<typename T> inline void writeToStream(const T& s, std::ostream& stream) {
		stream<<s<<std::endl;
	}

	void writeToStream(const Unit* const u, std::ostream& stream);
	void writeToStream(const Weapon& w, std::ostream& stream);

	template<typename T> inline void log(const T& s) {
		std::cerr<<"Log: ";
		Logging::writeToStream(s, std::cerr);
	}

	template<typename T> inline void warning(const T& s) {
		std::cerr<<"Warning: ";
		Logging::writeToStream(s, std::cout);
	}

	template<typename T> inline void error(const T& s) {
		std::cerr<<"Error: ";
		Logging::writeToStream(s, std::cerr);
		throw;
	}
}
