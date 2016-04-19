#include "Animation.hpp"

#include "Spritesheet.hpp"
#include "UserInterface.hpp"

std::unique_ptr<Animation> Animation::create(std::string animationname, Coordinate coord, int ticksPerFrame) {
	return std::unique_ptr<Animation> (new Animation(gResourceManager->getSpritesX(animationname)*gResourceManager->getSpritesY(animationname), ticksPerFrame, coord, gResourceManager->get(animationname) ));
}

Animation::Animation(int numFrames, int ticksPerFrame, Coordinate coord, std::shared_ptr<Spritesheet> spritesheet) :
	objectiveCoord(coord),
	frame(0),
	numFrames(numFrames),
	ticksPerFrame(ticksPerFrame),
	ticksSinceLastFrameUpdate(0),
	spritesheet(spritesheet)
{

}

void Animation::draw(SDL_Renderer* renderer, UserInterface* ui){
	this->screenCoord = ui->screenCoordinateFromObjective(this->objectiveCoord);

	this->spritesheet->render(renderer, frame%spritesheet->spritesX, frame/spritesheet->spritesX, this->screenCoord.x, this->screenCoord.y, ui->viewMagnification);
}

UpdateStatus::Enum Animation::tick(){
	this->ticksSinceLastFrameUpdate++;
	if (this->ticksSinceLastFrameUpdate >= this->ticksPerFrame){
		this->ticksSinceLastFrameUpdate = 0;
		this->frame++;
	}
	if (this->frame >= this->numFrames){
		return UpdateStatus::Remove;
	}

	return UpdateStatus::OK;
}
