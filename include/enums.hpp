#pragma once

#include "SDL2/SDL.h"

namespace AnimationState {
	enum Enum {
		Idle,
		Walking,
		Attacking,
		Dying
	};
};

namespace UpdateStatus {
	enum Enum {
		OK,
		Remove
	};
};

namespace TeamColor {
	enum Enum {
		Null, // for when we don't want to recolor
		Pink,
		Red,
		Blue,
		Green,
		Purple,
		Orange,
		Brown,
		Yellow,
		Teal,
		Pale,
		Iris,
		Olive,
		num
	};
};
