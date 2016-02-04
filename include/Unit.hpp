#pragma once

#include <SDL2/SDL.h>
#include <deque>
#include <vector>
#include <memory>

#include "typedefs.hpp"
#include "Weapon.hpp"
#include "WeaponTemplate.hpp"
#include "EnvironmentSpec.hpp"
#include "Drawer.hpp"
#include "Command.hpp"

class Team;
class UnitTemplate;
class Command;
class UnitState;
class Game;

typedef int UnitID;

enum AnimationState{
	ANIMSTATE_IDLE,
	ANIMSTATE_WALKING,
	ANIMSTATE_ATTACKING,
	ANIMSTATE_DYING
};

enum UpdateStatus{
	STATUS_OK,
	STATUS_REMOVE
};

class Unit
{
public:

	//Unit();
	Unit(Game&, UnitID, TeamID, UnitTemplateID, Coordinate);
	Unit(Unit &&u);
	UnitTemplate& getUnitTemplate();
	UpdateStatus update(); //returns 1 if should be destroyed, 0 otherwise
	void handleCommand(Command);
	void move(const Coordinate);
	void damage(const int, const DamageType);
	int getAttackRange(); // returns the range of the 
	void move_towards(const Coordinate c);
	void attack(Unit& target);

	void draw(SDL_Renderer*);

	const TeamID teamID;
	const UnitID unitID;
	const UnitTemplateID unitTemplateID;

	Coordinate xy;
	EnvironmentSpec dimension;
	int hp;
	int es;
	int dim;

	AnimationState animationState;
	int drawAnimationStep, drawFacingAngle;
	
	Game& game;
	std::vector<Weapon> weapons_;
private:
	std::deque<UnitState*> StateQueue_;
};
