#include "FontManager.hpp"

#include "globals.hpp"
#include "Logging.hpp"

#include <SDL2/SDL.h>

FontManager::FontManager() {
	if (TTF_Init() != 0){
		debugLog(SDL_GetError());
		SDL_Quit();
	}

	this->font = TTF_OpenFont("../resources/ClearSans-Regular.ttf", 24);
	
	if (NULL == this->font)
		debugLog((std::string)"Error in FontManager.cpp: "+SDL_GetError());

}

void FontManager::renderText(std::string text, int x, int y) {

	SDL_Color Black = {255, 255, 255, 255};

	SDL_Surface* surfaceMessage = TTF_RenderText_Blended(this->font, text.c_str(), Black);

	SDL_Texture* Message = SDL_CreateTextureFromSurface(gRenderer, surfaceMessage);

	SDL_Rect Message_rect; //create a rect
	Message_rect.x = x;  //controls the rect's x coordinate 
	Message_rect.y = y; // controls the rect's y coordinte
	Message_rect.w = 10*text.length(); // controls the width of the rect
	Message_rect.h = 24; // controls the height of the rect

	SDL_RenderCopy(gRenderer, Message, NULL, &Message_rect);

	SDL_FreeSurface(surfaceMessage);
	SDL_DestroyTexture(Message);
}
