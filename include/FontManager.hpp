#pragma once

#include <string>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "typedefs.hpp"

class FontManager {
	public:
		FontManager();
		~FontManager();
		int renderLine(std::string s, Coordinate pos, SDL_Color color = {255, 255, 255, 255} );
		void renderVector(std::vector<std::string> s, Coordinate pos, SDL_Color color = {255, 255, 255, 255} );
		void renderMultipleLines(std::string s, Coordinate pos, SDL_Color color = {255, 255, 255, 255} );

	private:
		TTF_Font* font;

};
