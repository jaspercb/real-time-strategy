#include "FontManager.hpp"

#include "globals.hpp"
#include "Logging.hpp"

#include <SDL2/SDL.h>

FontManager::FontManager() {
	if (TTF_Init() != 0){
		debugLog(SDL_GetError());
		SDL_Quit();
	}

	std::string fontName = "../resources/ClearSans-Regular.ttf";

	this->font = TTF_OpenFont(fontName.c_str(), 24);
	
	if (NULL == this->font)
		debugLog((std::string)"Error in FontManager.cpp: "+SDL_GetError());

}

void FontManager::renderText(std::string text, int x, int y) {

	SDL_Color White = {255, 255, 255};  // this is the color in rgb format, maxing out all would give you the color white, and it will be your text's color

	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(this->font, text.c_str(), White); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first

	SDL_Texture* Message = SDL_CreateTextureFromSurface(gRenderer, surfaceMessage); //now you can convert it into a texture

	SDL_Rect Message_rect; //create a rect
	Message_rect.x = x;  //controls the rect's x coordinate 
	Message_rect.y = y; // controls the rect's y coordinte
	Message_rect.w = 100; // controls the width of the rect
	Message_rect.h = 100; // controls the height of the rect

	SDL_RenderCopy(gRenderer, Message, NULL, &Message_rect);
}
