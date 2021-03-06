#pragma once

#include <set>

#include "typedefs.hpp"

class Command;
class UnitTemplate;

class Unitset {
public:
	Unitset();
	Unitset(const Unitset&);

	inline size_t size() { return set.size(); };
	inline bool empty() { return set.empty(); };
	inline void clear() { set.clear(); };
	inline void erase(UnitID u) { set.erase(u); };

	void removeInvalidUnits() const;

	void insert(const UnitID uID);
	void insert(const Unitset& container);
	template<typename T> void insert(const T& start, const T& end);

	void startBuilding(UnitTemplateID unitTemplateID); // builds exactly one

	// iterator stuff

	typedef std::set<UnitID>::iterator iterator;
	typedef std::set<UnitID>::const_iterator const_iterator;

	iterator begin() { removeInvalidUnits(); return set.begin(); };

	const_iterator begin() const { removeInvalidUnits(); return set.cbegin(); };

	iterator end() { return set.end(); }

	const_iterator end() const { return set.end(); }

	// UI stuff
	const UnitTemplate* getWHATTOCALLIT();
private:
	mutable std::set<UnitID> set;
};
