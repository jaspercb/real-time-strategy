#pragma once

#include <memory>

#include "typedefs.hpp"
#include "Unit.hpp" // for UpdateStatus
#include "ResourceManager.hpp"
#include "globals.hpp"

class SDL_Renderer;
class Spritesheet;
class UserInterface;

class Animation {
	// Alternate wrapper for a spritesheet that plays exactly one animation, exactly once
	// If you want to play this in a specific screen location.
	public:
		Animation(int frames, int ticksPerFrame, Coordinate coord, std::shared_ptr<Spritesheet> spritesheet);
		void draw(SDL_Renderer* renderer, UserInterface* ui);
		UpdateStatus tick();

		Coordinate objectiveCoord;
		Coordinate screenCoord;

	private:
		std::shared_ptr<Spritesheet> spritesheet;
		int frame;
		int numFrames;
		int ticksPerFrame;
		int ticksSinceLastFrameUpdate;
};

std::unique_ptr<Animation> newAnimation(std::string animationname, Coordinate coord, int ticksPerFrame);