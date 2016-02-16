#include "Animation.hpp"

#include "Spritesheet.hpp"
#include "UserInterface.hpp"

Animation::Animation(int numFrames, int ticksPerFrame, Coordinate coord, std::shared_ptr<Spritesheet> spritesheet) :
	numFrames(numFrames),
	ticksPerFrame(ticksPerFrame),
	frame(0),
	ticksSinceLastFrameUpdate(0),
	spritesheet(spritesheet),
	objectiveCoord(coord)
{

}

void Animation::draw(SDL_Renderer* renderer, UserInterface* ui){
	this->screenCoord = ui->screenCoordinateFromObjective(this->objectiveCoord);

	this->spritesheet->render(renderer, this->frame, 0, this->screenCoord.first, this->screenCoord.second, ui->viewMagnification);
}

UpdateStatus Animation::tick(){
	this->ticksSinceLastFrameUpdate++;
	if (this->ticksSinceLastFrameUpdate >= this->ticksPerFrame){
		this->ticksSinceLastFrameUpdate = 0;
		this->frame++;
	}
	if (this->frame >= this->numFrames){
		return STATUS_REMOVE;
	}

	return STATUS_OK;
}

std::unique_ptr<Animation> newAnimation(std::string animationname, Coordinate coord, int ticksPerFrame) {
	return std::unique_ptr<Animation> (new Animation(gResourceManager->getSpritesX(animationname), ticksPerFrame, coord, gResourceManager->get(animationname) ));
}
