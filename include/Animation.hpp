/*! 
 * \class Animation

 * \brief Alternate wrapper for a spritesheet that plays exactly one animation, exactly once.

 * Contains information about a single animation (speed, duration, the number
 * of frames, the source image, etc.)
 */

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
	public:
		static std::unique_ptr<Animation> create(std::string animationname, Coordinate coord, int ticksPerFrame);
		void draw(SDL_Renderer* renderer, UserInterface* ui);
		UpdateStatus::Enum tick();

		Coordinate objectiveCoord;
		Coordinate screenCoord;

	private:
		Animation(int frames, int ticksPerFrame, Coordinate coord, std::shared_ptr<Spritesheet> spritesheet);
		int frame;
		int numFrames;
		int ticksPerFrame;
		int ticksSinceLastFrameUpdate;
		std::shared_ptr<Spritesheet> spritesheet;
};
