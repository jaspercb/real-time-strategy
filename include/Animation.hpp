#pragma once

#include <memory>
#include "typedefs.hpp"
#include "Unit.hpp" // for UpdateStatus

class SDL_Renderer;
class Spritesheet;
class UserInterface;


class Animation {
	// Alternate wrapper for a spritesheet that plays exactly one animation, exactly once
	// If you want to play this in a specific screen location.
	public:
		Animation(int frames, int ticksPerFrame, std::shared_ptr<Spritesheet> spritesheet);
		void draw(SDL_Renderer* renderer, UserInterface* ui);
		UpdateStatus update();

		Coordinate objectiveCoord;
		Coordinate screenCoord;
		bool usingObjectiveCoord; // if true, objectiveCoord will be used to determine where should be used to determine draw location.

	private:
		std::shared_ptr<Spritesheet> spritesheet;
		int frame;
		int numFrames;
		int ticksPerFrame;
		int ticksSinceLastFrameUpdate;
};
