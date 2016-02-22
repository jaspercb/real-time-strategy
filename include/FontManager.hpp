#pragma once

#include <string>
#include <SDL2/SDL_ttf.h>


class FontManager {
	public:
		FontManager();
		void renderText(std::string s, int x, int y);

	private:
		TTF_Font* font;

};