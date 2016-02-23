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
#include "UnitState.hpp"

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
	void damage(const int, const DamageType, Unit& attackedBy);
	Distance getAttackRange(); // returns the range of the first weapon
	bool canAttack(Unit& u);
	void move_towards(const Coordinate c);
	void attack(Unit& target);

	void draw(SDL_Renderer*, UserInterface*);

	// std::vector<Coordinate> getWaypoints(); // returns an ordered list of the coordinate targets of the unit's state queue

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
	
	UnitID attackTargetID;
	Game& game;
	std::vector<Weapon> weapons_;
private:
	std::deque<std::shared_ptr<UnitState> > stateQueue_;
	TeamID lastAttackingTeamID;
	UnitID lastAttackingUnitID;
	std::shared_ptr<UnitState> idleState;
};
