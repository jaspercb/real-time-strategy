#include "ResourceManager.hpp"
#include "Logging.hpp"
#include "Spritesheet.hpp"

#include "globals.hpp"

#include <fstream>

ResourceManager::ResourceManager(){
	std::ifstream is("../conf/resourcedata");
	
	std::string name;
	while (is>>name){
		this->resourceDataTable.emplace(name, this->loadResourceData(is));
	}
}

ResourceData ResourceManager::loadResourceData(std::ifstream& is) {
	// Loads a single piece of resource data - NOT THE SPRITESHEET.


	ResourceData ret = ResourceData();
	
	std::string s;
	while (is >> s){
		if (s=="{")
			continue;

		else if (s=="filepath") {
			is>>ret.filepath;
		}

		else if (s=="size") {
			is>>ret.spriteW;
			is>>ret.spriteH;
		}

		else if (s=="sheetsize") {
			is>>ret.spritesX;
			is>>ret.spritesY;
		}

		else if (s=="offset") {
			is>>ret.offsetX;
			is>>ret.offsetY;
		}

		else if (s=="gap") {
			is>>ret.gapX;
			is>>ret.gapY;
		}

		else if (s=="isShadow"){
			is>>ret.isShadow;
		}

		else if (s=="shouldApplyTeamColor"){
			is>>ret.shouldApplyTeamColor;
		}

		else if (s=="}")
			break;

		else
			debugLog("ResourceManager::loadResourceData passed weird argument: "+s);
	}

	return ret;
}

SDL_Surface* ResourceManager::getRawSurface(std::string resourcename) {
	return loadSurface(this->resourceDataTable[resourcename].filepath);
}

std::shared_ptr<Spritesheet> ResourceManager::load(std::string resourcename, TeamColor teamColor) {
	return std::shared_ptr<Spritesheet>( new Spritesheet(gRenderer, this->resourceDataTable[resourcename], teamColor) );
}

std::shared_ptr<Spritesheet> ResourceManager::get(std::string resourcename, TeamColor teamColor) {

	std::pair<std::string, TeamColor> key(resourcename,
								this->resourceDataTable[resourcename].shouldApplyTeamColor ? teamColor : COLOR_NULL);

	auto iter = this->resourceTable.find(key);

	if (iter == this->resourceTable.end() ) {
		this->resourceTable[key] = this->load(resourcename, teamColor);
	}
	return this->resourceTable[key];
}



int ResourceManager::getSpritesX(std::string resourcename) {
	return this->resourceDataTable[resourcename].spritesX;
}

int ResourceManager::getSpritesY(std::string resourcename) {
	return this->resourceDataTable[resourcename].spritesY;
}