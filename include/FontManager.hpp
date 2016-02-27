#pragma once

#include <string>
#include <vector>
#include <SDL2/SDL_ttf.h>


class FontManager {
	public:
		FontManager();
		int renderLine(std::string s, int x, int y, SDL_Color color = {255, 255, 255, 255} );
		void renderVector(std::vector<std::string> s, int x, int y, SDL_Color color = {255, 255, 255, 255} );
		void renderMultipleLines(std::string s, int x, int y, SDL_Color color = {255, 255, 255, 255} );

	private:
		TTF_Font* font;

};
