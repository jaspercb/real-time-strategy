#include "Animation.hpp"

#include "Spritesheet.hpp"
#include "UserInterface.hpp"

Animation::Animation(int numFrames, int ticksPerFrame, std::shared_ptr<Spritesheet> spritesheet) :
	numFrames(numFrames),
	ticksPerFrame(ticksPerFrame),
	frame(0),
	ticksSinceLastFrameUpdate(0),
	spritesheet(spritesheet)
	
{

}

void Animation::draw(SDL_Renderer* renderer, UserInterface* ui){
	if (this->usingObjectiveCoord){
		this->screenCoord = ui->screenCoordinateFromObjective(this->objectiveCoord);
	}

	this->spritesheet->render(renderer, this->frame, 0, this->screenCoord.first, this->screenCoord.second, ui->viewMagnification);
}

UpdateStatus Animation::update(){
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