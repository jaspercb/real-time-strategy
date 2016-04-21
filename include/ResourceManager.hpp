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
		
		std::shared_ptr<Spritesheet> load(std::string resourcename, TeamColor::Enum teamColor);
		
		std::shared_ptr<Spritesheet> get(std::string resourcename, TeamColor::Enum teamColor = TeamColor::Enum::Null);
		
		int getSpritesX(std::string resourcename);

		int getSpritesY(std::string resourcename);

		SDL_Surface* getRawSurface(std::string resourcename);
		// Given a resource address, checks if it's already loaded, loads it if necessary, and returns it.
		// If given a resource address that doesn't exist, throws.

	private:
		std::map< std::pair<std::string, TeamColor::Enum>, std::shared_ptr<Spritesheet> > resourceTable;
		std::unordered_map<std::string, ResourceData > resourceDataTable;
};
