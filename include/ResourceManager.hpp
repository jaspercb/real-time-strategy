#pragma once

#include <map>
#include <unordered_map>
#include <memory>

#include "ResourceData.hpp"
#include "enums.hpp"

class Spritesheet; // forward declaration

class ResourceManager {
	public:
		ResourceManager();
		
		ResourceData loadResourceData(std::ifstream& is);
		
		std::shared_ptr<Spritesheet> load(std::string resourcename, TeamColor teamColor);
		
		std::shared_ptr<Spritesheet> get(std::string resourcename, TeamColor teamColor);
		// Given a resource address, checks if it's already loaded, loads it if necessary, and returns it.
		// If given a resource address that doesn't exist, throws.

	private:
		std::map< std::pair<std::string, TeamColor>, std::shared_ptr<Spritesheet> > resourceTable;
		std::unordered_map<std::string, ResourceData > resourceDataTable;
};
