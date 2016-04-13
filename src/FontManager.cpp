#include "FontManager.hpp"

#include <boost/algorithm/string.hpp>

#include "globals.hpp"
#include "Logging.hpp"

FontManager::FontManager() {
	if (TTF_Init() != 0){
		debugLog(SDL_GetError());
		SDL_Quit();
	}

	this->font = TTF_OpenFont("../resources/ClearSans-Regular.ttf", 16);
	
	if (NULL == this->font)
		debugLog((std::string)"Error in FontManager.cpp: "+SDL_GetError());

}

FontManager::~FontManager() {
	TTF_CloseFont(this->font);
	this->font = NULL; // just to be safe
}

int FontManager::renderLine(std::string line, Coordinate pos, SDL_Color color) {
	// Returns the height of the line of text, in pixels
	SDL_Surface* surfaceMessage = TTF_RenderText_Blended(this->font, line.c_str(), color);

	SDL_Texture* Message = SDL_CreateTextureFromSurface(gRenderer, surfaceMessage);

	SDL_Rect Message_rect; //create a rect
	Message_rect.x = pos.x;  //controls the rect's x coordinate 
	Message_rect.y = pos.y; // controls the rect's y coordinte
	SDL_QueryTexture(Message, NULL, NULL, &Message_rect.w, &Message_rect.h);
	
	SDL_RenderCopy(gRenderer, Message, NULL, &Message_rect);

	SDL_FreeSurface(surfaceMessage);
	SDL_DestroyTexture(Message);

	return Message_rect.h * 0.9;
}

void FontManager::renderVector(std::vector<std::string> lines, Coordinate pos, SDL_Color color) {
	int dy = 0;
	for (std::string line : lines) {
		dy += this->renderLine(line, {pos.x, pos.y+dy}, color);
	}
}

void FontManager::renderMultipleLines(std::string text, Coordinate pos, SDL_Color color) {
	std::vector<std::string> lines;
	boost::split(lines, text, boost::is_any_of("\n\r"), boost::token_compress_on);

	this->renderVector(lines, pos, color);
}
