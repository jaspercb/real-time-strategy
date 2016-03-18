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
#include "Builder.hpp"

class Team;
class UnitTemplate;
class Game;
class UserInterface;

enum AnimationState {
	ANIMSTATE_IDLE,
	ANIMSTATE_WALKING,
	ANIMSTATE_ATTACKING,
	ANIMSTATE_DYING
};

enum UpdateStatus {
	STATUS_OK,
	STATUS_REMOVE
};

class Unit {
public:
	friend Builder;
	friend UserInterface;

	//Unit();
	Unit(Game&, UnitID, TeamID, UnitTemplateID, Coordinate);
	Unit(Unit &&u);
	~Unit();
	UnitTemplate& getUnitTemplate() const;
	void tick();
	void handleCommand(Command);
	void move(const Coordinate);
	void damage(const int, const DamageType, Unit& attackedBy);
	Distance getAttackRange(); // returns the range of the first weapon
	bool canAttack(Unit& u) const;
	void move_towards(const Coordinate c);
	void attack(Unit& target);

	void startBuilding(UnitTemplateID unitTemplateID);

	void draw(SDL_Renderer*, UserInterface*);

	bool isDead() const;
	bool shouldDelete() const;

	std::vector<Coordinate> getStateWaypoints(); // returns an ordered list of the coordinate targets of the unit's state queue

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
private:
	std::vector<Weapon> weapons_;
	std::deque<std::shared_ptr<UnitState> > stateQueue_;
	TeamID lastAttackingTeamID;
	UnitID lastAttackingUnitID;
	std::shared_ptr<UnitState> idleState;

	Builder* builder;
	std::deque<std::pair<UnitTemplateID, int> > buildingQueue;
};
